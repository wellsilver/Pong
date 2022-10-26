@echo off
:: cringey bat file because apparently its a courtesy to have this

echo you need gcc and sdl2 dev
echo you also need sdl2 headers to be in gcc's headers
pause
mkdir out
:: compile (gcc)
gcc ./src/pong.c -lSDL2main -lSDL2 -o ./out/pong.exe
:: sdl2
copy "./SDL2.dll" "./out/SDL2.dll"
copy "./sdl2-config" "./out/sdl2-config"
:: images
cd out
mkdir data
cd ..
copy "./QUIT.bmp" "./out/data/QUIT.bmp"
copy "./START.bmp" "./out/data/START.bmp"
echo.
echo.
echo ./out/pong.exe
pause
