# Rebuilds all the project Premake and Make


cd Source/Modules

premake5 clean
premake5 gmake
make clean
make -j$(nproc)
cd ..
cd ..
