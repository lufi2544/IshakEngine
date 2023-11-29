
premake5 gmake
make -j$(nproc)
cd ../Binaries/Tests/Debug
gdb IshakEngineTests
cd ../../../TestEnviroment
