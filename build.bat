mkdir bin
cd sfml
cmake -G"MinGW Makefiles" -S./ -DBUILD_SHARED_LIBS=TRUE -DSFML_USE_STATIC_STD_LIBS=FALSE
REM cmake -G"MinGW Makefiles" -S./ -DBUILD_SHARED_LIBS=FALSE -DSFML_USE_STATIC_STD_LIBS=TRUE
make
cd ..
make
copy /Y .\sfml\extlibs\bin\x86\openal32.dll .\ 
copy /Y .\sfml\lib\sfml-audio-2.dll .\ 
copy /Y .\sfml\lib\sfml-graphics-2.dll .\ 
copy /Y .\sfml\lib\sfml-network-2.dll .\ 
copy /Y .\sfml\lib\sfml-system-2.dll .\ 
copy /Y .\sfml\lib\sfml-window-2.dll .\ 
