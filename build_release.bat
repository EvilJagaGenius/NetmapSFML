mkdir bin
mkdir bin\Release
mingw32-make
copy /Y .\sfml\extlibs\bin\x86\openal32.dll .\bin\Release\ 
