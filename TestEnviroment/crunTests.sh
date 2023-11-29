#Cleans the Project and run tests, is a rebuild

premake clean
premake5 gmake
make clean
make -j$(nproc)
cd ../Binaries/Tests/Debug
./IshakEngineTests
cd ../../../TestEnviroment
