#Cleans the Project and run tests, is a rebuild

premake5 clean
premake5 gmake
make clean
make -j$(nproc)

