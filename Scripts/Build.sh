cmake -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ -D CMAKE_BUILD_TYPE=Debug -B ./../Build/Debug -S ./..
cd ./../Build/Debug
make
cd ./../../Scripts

cmake -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ -D CMAKE_BUILD_TYPE=Release -B ./../Build/Release -S ./..
cd ./../Build/Release
make
cd ./../../Scripts
