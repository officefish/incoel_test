## 1.
git clone https://github.com/oatpp/oatpp
cd oatpp
mkdir build && cd build
cmake ..
make install

## 2.
g++ -std=c++17 -o server oatpp_web_server.cpp -I/usr/local/include -L/usr/local/lib -loatpp

## 3.
./server

## 4.
curl http://localhost:1616/log

## 5.
curl -X POST -F "file=@myTestFile.txt" http://localhost:1616/upload


