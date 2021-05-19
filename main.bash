clang++ -std=c++17 -L$PWD/GLFW/lib main.cpp -pthread -ldl -lGL -lglfw3 -o main
chmod 744 $PWD/main
export LD_LIBRARY_PATH=$PWD/GLFW/lib
$PWD/main