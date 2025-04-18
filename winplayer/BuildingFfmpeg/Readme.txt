# Steps to generate ffmpeg binaries on windows:=
1. Downaload repo form here - https://github.com/m-ab-s/media-autobuild_suite.git
2. Run with minimal options selecting shared ffmpeg binaries, See attached files for referencce.
3. Replace 'https://git.ffmpeg.org/ffmpeg.git' with 'https://git.ffmpeg.org/ffmpeg.git#tag=release/3.3' in 'media-suite_compile.sh'. See attached files for referencce.
4. In first run it will show some configuration flags, correct them.
5. During compilation, it will show compilation error for libfdk, replace attached file with problematic file and rerun.
6. Now build will compete and ffmpeg binaries will be placed in 'bin-video' subfolder.

enable x264 , fdkaac cuda and nvenc at media-autobuild_suite

dubleclick  media-autobuild_suite to start the build process


# With command line 

https://trac.ffmpeg.org/wiki/CompilationGuide/MinGW

install MSYS2 from ​https://msys2.github.io/.

Then run the mingw32_shell.bat or mingw64_shell.bat command in the MSYS2 installation home. Be careful not to run the MSYS2 Shell as this will result in FFmpeg's configure saying building an MSYS binary is not recommended and exiting.

Now install requisite packages:

pacman -S make

pacman -S diffutils

pacman -S yasm


https://stackoverflow.com/questions/54164545/how-to-compile-ffmpeg-for-windows-statically-with-msys2-environment

git bash
cd /d/media-autobuild_suite/msys64

 ./msys2_shell.cmd -mingw64


msys2_shell.cmd -mingw64


cd /d/media-autobuild_suite/build/ffmpeg-git

git chechout release/4.4

./configure --pkg-config-flags= --enable-shared  --enable-gpl --disable-w32threads --disable-autodetect

for dll 

./configure --pkg-config-flags= --enable-shared  --enable-gpl --disable-w32threads --disable-autodetect
