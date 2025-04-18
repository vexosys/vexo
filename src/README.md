# RGBD converted to Anaglyph with Depth



## How to Run

apt install python3-plt

apt install matplotlib


User argument -i to specify rgb image and -d to specify depth image. Depth should be in mm.

python3 rgbd_to_anaglyph.py -i car.jpg -d car.png -opt



## For C++

cd src 

mkdir build

cd build

cmake ..

make clean 

make

./mediaplayer