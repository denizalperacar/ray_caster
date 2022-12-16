#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <vector>

namespace obj {
  class vertex {
    private:
      double x_, y_, z_;
    
    public:
      vertex() = default;
      vertex(double x, double y, double z):x_{x}, y_{y}, z_{z} {}
      vertex(const vertex &v);
      vertex &operator=(vertex &obj) = default;
      vertex &operator= (vertex &&obj); 
      ~vertex() = default;

      friend std::ostream& operator<<(std::ostream &os, vertex const &obj); 
      friend std::ifstream& operator>>(std::ifstream &os, vertex &obj); 

  };

  class vertex_container {
    private:
      std::vector<vertex> vertices_;

    public:
      friend std::ifstream& operator>>(
        std::ifstream &os, 
        vertex_container &vert
      );
      friend std::ostream& operator<<(
        std::ostream &os, 
        vertex_container &vert);
  };
}

#endif