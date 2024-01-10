# This sh file will build the Engine

cd Source/Modules

make -j$(nproc)
cd ..
cd ..
