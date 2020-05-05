#include <iostream>
#include <vector>
#include <cmath>
#include <math.h>
#include <chrono>

#include "csv.h"

void load(std::string &fname, std::vector<double> &frqs, std::vector<double> &mags){
  double frq;
  double mag;

  io::CSVReader<2> in (fname);
  in.read_header(io::ignore_extra_column, "frq", "mag");

  while (in.read_row(frq, mag)){
    frqs.emplace_back(frq);
    mags.emplace_back(mag);
  }
}

std::vector<double> geomspace(double start, double stop, int n){
  std::vector<double> logspaced;
  double logstart = log10(start);
  double logstop = log10(stop);
  double step = (logstop - logstart)/(n-1);
  for (int i=0; i<n; i++){
     logspaced.emplace_back(pow(10, (logstart + i*step)));
  }
  return logspaced;
}

std::vector<double> smooth_konno_ohmachi(std::vector<double> &frqs, std::vector<double> &mags, 
                          std::vector<double> &sfrqs, float bandwidth){
  float n=3;
  double upper_limit = pow(10, n/bandwidth);
  double lower_limit = pow(10, -n/bandwidth);

/*  
  // Debug
  std::cout<<"upper_limit, lower_limit "<<upper_limit<<", "<<lower_limit<<std::endl;
*/
  std::vector<double> smags(sfrqs.size());
  double sumproduct;
  double sumwindow;  
  double window;
  double fc;

  // Loop across the center frequencies 
  for (int f_index=0; f_index<sfrqs.size(); f_index++){
    fc = sfrqs[f_index];
    if (fc < 1E-6){

      // Debug
      std::cout<<"fc < 1E-6"<<std::endl;

      smags[f_index] = 0;
      continue;
    }
    
    sumproduct = 0;
    sumwindow = 0;
    window = 0;

    // Loop across true frequency vector
    for (int j=0; j<frqs.size(); j++){
      double f = frqs[j];
      double c_spectrum = mags[j];
      double f_on_fc = f/fc;
      if ((f < 1E-6) || (f_on_fc > upper_limit) || (f_on_fc < lower_limit))
        continue;
      else if (std::abs(f-fc) < 1E-6)
        window = 1;
      else{
        window = bandwidth * log10(f_on_fc);
        window = sin(window) / window;
        window *= window;
        window *= window;
      }
      sumproduct += window*c_spectrum;
      sumwindow += window;
    }
  
    if (sumwindow > 0)
      smags[f_index] = sumproduct / sumwindow;
    else
      smags[f_index] = 0;
    }
  
  return smags;
}

int main(int argc, char **argv) {

  if (argc != 2)
    std::abort();
  
  // Get fname from user input
  std::string fname(argv[1]);
  std::cout<<"# Loading file                ::   "<<fname<<std::endl;
  
  // Load unsmoothed frequency and magnitude
  std::vector<double> frqs;
  std::vector<double> mags;
  load(fname, frqs, mags);
  std::cout<<"# Frequency vector size       ::   "<<frqs.size()<<std::endl;
  std::cout<<"# Magnitude vector size       ::   "<<mags.size()<<std::endl;

/*
  // Debug -> View first 5 lines of file
  std::cout<<"Frequency, Magnitude"<<std::endl;
  for (int i=0; i<5; i++){
    std::cout<<frqs[i]<<", "<<mags[i]<<std::endl;
  }
*/
  
  // Define new logarithmically-spaced frequency vector
  double start=0.1;
  double stop=20;
  int n=512;  
  std::vector<double> sfrqs = geomspace(start, stop, n);
  std::cout<<"# Smoothing frequency vector  ::   geomspace(start="<<start<<", "<<
                                                        "stop="<<stop<<", "<<
                                                        "n="<<n<<")"<<std::endl;
/*
  // Debug -> Check sfrqs
  std::cout<<"# Smoothing frequency size    ::  "<<sfrqs.size()<<std::endl;
  std::cout<<"# Starting frequency          ::  "<<sfrqs.front()<<std::endl;
  std::cout<<"# Ending frequency            ::  "<<sfrqs.back()<<std::endl;
*/

  // Smooth
  float bandwidth = 40;
  
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  std::vector<double> smags = smooth_konno_ohmachi(frqs, mags, sfrqs, bandwidth);
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2-t1);
  std::cout<<"# Elapsed Time                ::   "<<time_span.count()<<" seconds"<<std::endl;
  std::cout<<time_span.count()<<std::endl; 
/*
  // Debug -> Check smags
  std::cout<<"Smoothed magnitude size     ::  "<<smags.size()<<std::endl;
  std::cout<<"Frequency, Magnitude"<<std::endl;
  for (int i=0; i<5; i++){
    std::cout<<sfrqs[i]<<", "<<smags[i]<<std::endl;
  }
*/

/*
  // Write to stdout
  std::cout<<"sfrqs,smags"<<std::endl;
  for (int i=0; i<smags.size(); i++){
    std::cout<<sfrqs[i]<<","<<smags[i]<<std::endl;
  }
*/
}
