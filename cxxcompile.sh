reset;
cd "$(dirname "$0")";
g++ -g -o main src/main.cpp src/io/read_obj.cpp src/core/vertex.cpp src/core/face.cpp src/core/volume.cpp;
