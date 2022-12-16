#ifndef VEOLUME_H
#define VEOLUME_H

#include <vector>

namespace obj {

  class face;

  class volume {
    private:
      double attun_{1.0}; // atteunation of air
      std::vector<face> vol_;
    public:
      // [TODO] add the constructors
  };
}

#endif