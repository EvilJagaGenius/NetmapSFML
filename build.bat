mkdir bin
cd sfml
REM cmake -G"MinGW Makefiles" -S./ -DBUILD_SHARED_LIBS=TRUE -DSFML_USE_STATIC_STD_LIBS=FALSE
cmake -G"MinGW Makefiles" -S./ -DBUILD_SHARED_LIBS=FALSE -DSFML_USE_STATIC_STD_LIBS=TRUE -DCMAKE_BUILD_TYPE=Release
mingw32-make
cd ..
mingw32-make
