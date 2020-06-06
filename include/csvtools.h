#ifndef CSVTOOLS_H
#define CSVTOOLS_H
    #include <vector>
    #include <fstream>
    #include "csv.h"
    void load(std::string&, std::vector<double>&, std::vector<double>&);
    void save(std::string&, std::vector<double>&, std::vector<double>&);
#endif
