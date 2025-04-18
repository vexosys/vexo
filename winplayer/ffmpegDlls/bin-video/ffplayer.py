import os
from ctypes import *

#ldd  libfoo.so much do dependecny of SDL2 and ttf, check ld and Make sure ld libs are in last $(LINK.cc) $(program_OBJS) -o $(program_NAME) $(LDLIBS)

path_to_deps = "./"
os.environ['PATH'] = path_to_deps + os.pathsep + os.environ['PATH']
lib = CDLL("./ffmpegPlayer.dll")


lib.fmpPlayer_Start("dummy")
