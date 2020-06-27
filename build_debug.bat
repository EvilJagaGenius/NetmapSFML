mkdir bin
mkdir bin\Debug
mingw32-make Debug
copy /Y .\sfml\extlibs\bin\x86\openal32.dll .\bin\Debug\ 
