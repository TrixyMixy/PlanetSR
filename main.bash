clang++ -std=c++17 main.cpp -pthread -ldl -lGL -lX11 -lglfw -o main.exec
chmod 744 $PWD/main.exec
$PWD/main.exec
