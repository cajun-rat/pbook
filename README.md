PBOOK
=====

Next steps
----------
Implement networkarpencryptor (and think of a better name for it)

Purpose
-------

My wife can't communicate with her sister without her communications being routinly monitored and (potentially) censored. PBOOK aims to fix this by providing a communication system that is encrypted end-to-end and verifibly so by being open source.


Building
--------
mkdir build
cd build
sudo apt-get install cmake libboost-system-dev libboost-dev protobuf-compiler libprotobuf-dev libboost-test-dev
cmake ..
make

