#include "numtools.h"

std::vector<double> geomspace(double start, double stop, int n) {
  std::vector<double> logspaced;
  double logstart = log10(start);
  double logstop = log10(stop);
  double step = (logstop - logstart) / (n - 1);
  for (int i = 0; i < n; i++) {
    logspaced.emplace_back(pow(10, (logstart + i * step)));
  }
  return logspaced;
}
