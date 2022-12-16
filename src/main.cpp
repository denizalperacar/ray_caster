
#include <string>

#include "./core/vertex.h"
#include "./core/face.h"
#include "./core/volume.h"
#include "./io/read_obj.h"

int main() {

  std::string filename = "data/data.obj";
  obj::vertex_container vertices;
  std::vector<obj::volume> volume_container;
  obj::read_object_file(filename, vertices, volume_container);
}