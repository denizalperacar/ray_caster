#ifndef READ_OBJ_H
#define READ_OBJ_H

#include <string>
#include <vector>

namespace obj {

  class vertex;
  class face;
  class volume;
  class vertex_container;

  void read_object_file(
      std::string filename, 
      vertex_container &vertex_container, 
      std::vector<volume> &volume_container
  );
  
}

#endif