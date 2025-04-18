@echo off >nul 2>&1
rem() { "$@"; }
rem test -f nul && rm nul
rem false --help > /dev/null 2>&1 && false /mingw64/bin/cpp "$@" || /mingw64/bin/cpp "$@"
rem exit $?
 E:/fb_goal/msys64/mingw64/bin/cpp.exe %*
