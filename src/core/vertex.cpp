#include <iostream>
#include <fstream>
#include "vertex.h"
 
namespace obj {
  
  vertex::vertex(const vertex &v) {
    x_ = v.x_;
    y_ = v.y_;
    z_ = v.z_;
  }

  std::ostream& operator<<(std::ostream &os, vertex const &obj) {
    os << obj.x_ << ", " << obj.y_ << ", " << obj.z_;
    return os;
  } 

  std::ifstream& operator>>(std::ifstream &os, vertex &obj) {
    os >> obj.x_ >> obj.y_ >> obj.z_;
    return os;
  } 

  std::ifstream& operator>>(
      std::ifstream &os, 
      vertex_container &vert
      ) {
      vertex v;
      os >> v;
      vert.vertices_.push_back(v);
    return os;
  }

  std::ostream& operator<<(
      std::ostream &os, 
      vertex_container &vert
      ) {
    for (int ind = 0; ind < static_cast<int>(vert.vertices_.size()); ind++) {
      os << ind << ", " << vert.vertices_[ind] << std::endl;
    }
    return os;
  }

}