#!/bin/bash

echo "running MJR_RECORDER PROJECT! ..."

# create all build file in 'mcu_rec_build'
cd build
cmake ..
make
# running built execution file.
./media-player
