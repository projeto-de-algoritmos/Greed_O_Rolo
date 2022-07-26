#!/bin/bash
sudo apt install git gcc g++ cmake libjsoncpp-dev uuid-dev openssl libssl-dev zlib1g-dev
git clone https://github.com/an-tao/drogon
cd drogon
git submodule update --init
mkdir build
cd build
cmake ..
make && sudo make install
echo "o rolo compressor está pronto!"
