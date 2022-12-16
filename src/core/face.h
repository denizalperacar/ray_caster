#ifndef FACE_H
#define FACE_H

class vertex;

namespace obj {

  class face {
    private:
      vertex *v1_, *v2_, *v3_;
    public:
      face() = delete;
      face(vertex *v1, vertex *v2, vertex *v3): v1_{v1}, v2_{v2}, v3_{v3} {}
      face(face &obj);
      ~face() = default;
      face &operator=(face &obj);
      face &operator=(face &&obj);
  };

}

#endif