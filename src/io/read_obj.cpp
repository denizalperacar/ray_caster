
#include <string>
#include <iterator>
#include <fstream>
#include <iostream>

#include "read_obj.h"
#include "../core/vertex.h"

namespace obj {

  void read_object_file(
      std::string filename, 
      vertex_container &vertex_container, 
      std::vector<volume> &volume_container
      ) {
    
    obj::vertex vert; 
    char key;
    std::ifstream ifile{filename};
    bool flag = false;
    double x[3];
    while (ifile.good()) {
      ifile >> key;
      if (key == 'v') {
        ifile >> vertex_container;
      }
    }
  }
}