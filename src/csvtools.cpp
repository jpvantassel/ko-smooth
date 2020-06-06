#include "csvtools.h"

void load(std::string &fname, std::vector<double> &frequencies, std::vector<double> &magnitudes)
{
  double frq;
  double mag;

  io::CSVReader<2> in(fname);
  in.read_header(io::ignore_extra_column, "frq", "mag");

  while (in.read_row(frq, mag))
  {
    frequencies.emplace_back(frq);
    magnitudes.emplace_back(mag);
  }
}

void save(std::string &fname, std::vector<double> &frequencies, std::vector<double> &magnitudes)
{
  std::ofstream fstream(fname);
  fstream << "sfrq,smag\n";

  for (int i = 0; i < frequencies.size(); i++)
    fstream << frequencies[i] << "," << magnitudes[i] << "\n";

  fstream.close();
}