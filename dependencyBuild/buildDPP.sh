cd DPP 
cmake .. -DCMAKE_INSTALL_PREFIX=../../lib
cmake -DBUILD_SHARED_LIBS=OFF ..
cmake -DBUILD_VOICE_SUPPORT=OFF ..
make -B ./build -j2
cd build
sudo make install
