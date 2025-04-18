# to run anaglyph gnenerator


# with openCV
git clone https://github.com/InnovatorSol/Haze.git

git checkout 3DAnaglyph

cd Haze/script

cp anyxyz.mp4 to video.mp4

python3 playanaglyph.py

Install missing package with pip3 install xyz




# with ffmpeg

./video2sbs.sh bunny720p.mp4 

ffplay ./bunny720p.mp4.3dsbs-20-0.0400.mp4 -vf stereo3d=sbs2l:agmd

ffplay ./jigest3dWithMath.mp4 -vf stereo3d=sbs2l:arcd

ffmpeg -i Jigeesh_2D.mp4.3dsbs-20-0.0416.mp4 -vf stereo3d=sbs2l:agmd -c:a copy output.mp4
ffmpeg -i test5_3d.mp4 -vf stereo3d=sbs2l:arcd -c:a copy 3dMovie640.mp4



# 2d to 3d with AI

python3 ai2Dto3D.py --model ./export/deep3d_v1.0_1280x720_cuda.pt --video ./test.mp4 --out ./test3d.mp4 --inv

Install missing package with pip3 install xyz

# rgbd_to_anaglyph.py

python3 rgbd_to_anaglyph.py -i car.jpg -d car.png 