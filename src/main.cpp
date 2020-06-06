#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

#include "csvtools.h"
#include "numtools.h"
#include "smooth.h"

int main(int argc, char **argv)
{

  if (argc < 2 || argc > 3)
    std::abort();

  // Get fname from user input
  std::string ifname(argv[1]);
  std::cout << "# Loading from                ::   " << ifname << std::endl;
  std::string ofname(argv[2]);
  std::cout << "# Saving to                   ::   " << ofname << std::endl;

#ifdef _OPENMP
  std::cout << "# Number of threads           ::   " << omp_get_max_threads() << std::endl;
#endif

  // Load unsmoothed frequency and magnitude
  std::vector<double> frqs;
  std::vector<double> mags;
  load(ifname, frqs, mags);
  std::cout << "# Frequency vector size       ::   " << frqs.size() << std::endl;
  std::cout << "# Magnitude vector size       ::   " << mags.size() << std::endl;

  /*
  // Debug -> View first 5 lines of file
  std::cout<<"Frequency, Magnitude"<<std::endl;
  for (int i=0; i<5; i++){
    std::cout<<frqs[i]<<", "<<mags[i]<<std::endl;
  }
*/

  // Define new logarithmically-spaced frequency vector
  double start = 0.1;
  double stop = 20;
  int n = 512;
  std::vector<double> sfrqs = geomspace(start, stop, n);
  std::cout << "# Smoothing frequency vector  ::   geomspace(start=" << start << ", "
            << "stop=" << stop << ", "
            << "n=" << n << ")" << std::endl;

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
  std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
  std::cout << "# Elapsed Time                ::   " << time_span.count() << " seconds" << std::endl;

  /*
  // Debug -> Check smags
  std::cout<<"Smoothed magnitude size     ::  "<<smags.size()<<std::endl;
  std::cout<<"Frequency, Magnitude"<<std::endl;
  for (int i=0; i<5; i++){
    std::cout<<sfrqs[i]<<", "<<smags[i]<<std::endl;
  }
*/

  // Write to file
  save(ofname, sfrqs, smags);
}
