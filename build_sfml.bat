cmake -G"CodeBlocks - MinGW Makefiles" -Ssfml -Bsfml -DBUILD_SHARED_LIBS=FALSE -DSFML_USE_STATIC_STD_LIBS=TRUE -DCMAKE_BUILD_TYPE=Release
mingw32-make --directory=sfml
cmake -G"CodeBlocks - MinGW Makefiles" -Ssfml -Bsfml -DBUILD_SHARED_LIBS=FALSE -DSFML_USE_STATIC_STD_LIBS=TRUE -DCMAKE_BUILD_TYPE=Debug
mingw32-make --directory=sfml