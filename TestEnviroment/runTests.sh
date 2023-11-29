#Script for running the tests

premake5 gmake
make -j$(nproc)
cd ../Binaries/Tests/Debug
./IshakEngineTests
cd ../../../TestEnviroment
