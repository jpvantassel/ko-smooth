#include "smooth.h"

std::vector<double> smooth_konno_ohmachi(std::vector<double> &frqs, std::vector<double> &mags,
                                         std::vector<double> &sfrqs, float bandwidth)
{
    float n = 3;
    double upper_limit = pow(10, n / bandwidth);
    double lower_limit = pow(10, -n / bandwidth);

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
    for (int f_index = 0; f_index < sfrqs.size(); f_index++)
    {
        fc = sfrqs[f_index];
        if (fc < 1E-6)
        {
            // // Debug
            // std::cout << "fc < 1E-6" << std::endl;

            smags[f_index] = 0;
            continue;
        }

        sumproduct = 0;
        sumwindow = 0;
        window = 0;

        // Loop across true frequency vector
        for (int j = 0; j < frqs.size(); j++)
        {
            double f = frqs[j];
            double c_spectrum = mags[j];
            double f_on_fc = f / fc;
            if ((f < 1E-6) || (f_on_fc > upper_limit) || (f_on_fc < lower_limit))
                continue;
            else if (std::abs(f - fc) < 1E-6)
                window = 1;
            else
            {
                window = bandwidth * log10(f_on_fc);
                window = sin(window) / window;
                window *= window;
                window *= window;
            }
            sumproduct += window * c_spectrum;
            sumwindow += window;
        }

        if (sumwindow > 0)
            smags[f_index] = sumproduct / sumwindow;
        else
            smags[f_index] = 0;
    }

    return smags;
}
