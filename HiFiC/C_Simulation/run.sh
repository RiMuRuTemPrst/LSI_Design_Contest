#!/bin/bash
#!/bin/bash
#!/bin/bash
set -e

rm -rf build
mkdir build
cd build

cmake ..
make -j

echo "=== RUNNING C++ SIMULATION ==="

cd ..
./build/run
## Set quyền chạy:

## chmod +x run.sh


## Chạy:

## ./run.sh