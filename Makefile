OBJS	= animation.o animator.o choiceinputbox.o databattle.o databattlepiece.o databattleplayer.o inputbox.o netmap_playable.o npc.o npcanim.o player.o program.o programaction.o programsector.o scene.o sceneentity.o scenelayer.o sectorinputbox.o textinputbox.o titlescreen.o toolbox.o uploadzone.o main.o
SOURCE	= animation.cpp animator.cpp choiceinputbox.cpp databattle.cpp databattlepiece.cpp databattleplayer.cpp inputbox.cpp netmap_playable.cpp npc.cpp npcanim.cpp player.cpp program.cpp programaction.cpp programsector.cpp scene.cpp sceneentity.cpp scenelayer.cpp sectorinputbox.cpp textinputbox.cpp titlescreen.cpp toolbox.cpp uploadzone.cpp main.cpp
HEADER	= animation.h animator.h choiceinputbox.h databattle.h databattlepiece.h databattleplayer.h inputbox.h netmap_playable.h npc.h npcanim.h player.h program.h programaction.h programsector.h scene.h sceneentity.h scenelayer.h sectorinputbox.h textinputbox.h titlescreen.h toolbox.h uploadzone.h

BIN	 = .
SRC	 = src

ifeq ($(OS),Windows_NT)
OUT	= $(BIN)/NetmapSFML.exe
OUT_DEBUG	 = $(BIN)/NetmapSFML_Debug.exe
else
OUT	= $(BIN)/NetmapSFML
OUT_DEBUG	 = $(BIN)/NetmapSFML_Debug
endif

CC	 = g++
FLAGS	 = -g -c -Wall -std=c++11 -DSFML_STATIC -Iinclude -Isfml/include
WIN32_EXAMPLE_FLAGS	 = -g -c -Wall -std=c++11 -DSFML_STATIC -Isfml/include
LFLAGS	 = -DSFML_STATIC -std=c++11
LIBS_INCLUDE	 = -Lsfml/lib -Lsfml/extlibs/libs-mingw/x86
LIBS	 = -lsfml-audio-s -lsfml-graphics-s -lsfml-window-s -lsfml-network-s -lsfml-system-s -lwinmm -lopengl32 -lgdi32 -lfreetype -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lws2_32
LIBS_DEBUG	 = -lsfml-audio-s-d -lsfml-graphics-s-d -lsfml-window-s-d -lsfml-network-s-d -lsfml-system-s-d -lwinmm -lopengl32 -lgdi32 -lfreetype -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lws2_32
WIN32_EXAMPLE_LIBS	 = -Lsfml/lib -Lsfml/extlibs/libs-mingw/x86 -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lfreetype 

# Release version (default): 
all: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(OUT) $(LIBS_INCLUDE) $(LIBS)

#Debug version: 
Debug: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(OUT_DEBUG) $(LIBS_INCLUDE) $(LIBS_DEBUG)

#An example program: 
Win32: Win32.o
	$(CC) $(LFLAGS) Win32.o -o sfml/examples/win32/Win32.exe $(LIBS_INCLUDE) $(LIBS)  

Win32.o: sfml/examples/win32/Win32.cpp
	$(CC) $(WIN32_EXAMPLE_FLAGS) sfml/examples/win32/Win32.cpp


animation.o: $(SRC)/animation.cpp
	$(CC) $(FLAGS) $(SRC)/animation.cpp 

animator.o: $(SRC)/animator.cpp
	$(CC) $(FLAGS) $(SRC)/animator.cpp 

choiceinputbox.o: $(SRC)/choiceinputbox.cpp
	$(CC) $(FLAGS) $(SRC)/choiceinputbox.cpp 

databattle.o: $(SRC)/databattle.cpp
	$(CC) $(FLAGS) $(SRC)/databattle.cpp 

#databattleeditor.o: $(SRC)/databattleeditor.cpp
#	$(CC) $(FLAGS) $(SRC)/databattleeditor.cpp 

databattlepiece.o: $(SRC)/databattlepiece.cpp
	$(CC) $(FLAGS) $(SRC)/databattlepiece.cpp 

databattleplayer.o: $(SRC)/databattleplayer.cpp
	$(CC) $(FLAGS) $(SRC)/databattleplayer.cpp 

#hud.o: $(SRC)/hud.cpp
#	$(CC) $(FLAGS) $(SRC)/hud.cpp 

inputbox.o: $(SRC)/inputbox.cpp
	$(CC) $(FLAGS) $(SRC)/inputbox.cpp 

netmap_playable.o: $(SRC)/netmap_playable.cpp
	$(CC) $(FLAGS) $(SRC)/netmap_playable.cpp 

npc.o: $(SRC)/npc.cpp
	$(CC) $(FLAGS) $(SRC)/npc.cpp 

npcanim.o: $(SRC)/npcanim.cpp
	$(CC) $(FLAGS) $(SRC)/npcanim.cpp 

player.o: $(SRC)/player.cpp
	$(CC) $(FLAGS) $(SRC)/player.cpp 

program.o: $(SRC)/program.cpp
	$(CC) $(FLAGS) $(SRC)/program.cpp 

programaction.o: $(SRC)/programaction.cpp
	$(CC) $(FLAGS) $(SRC)/programaction.cpp 

programsector.o: $(SRC)/programsector.cpp
	$(CC) $(FLAGS) $(SRC)/programsector.cpp 

scene.o: $(SRC)/scene.cpp
	$(CC) $(FLAGS) $(SRC)/scene.cpp 

sceneentity.o: $(SRC)/sceneentity.cpp
	$(CC) $(FLAGS) $(SRC)/sceneentity.cpp 

scenelayer.o: $(SRC)/scenelayer.cpp
	$(CC) $(FLAGS) $(SRC)/scenelayer.cpp 

sectorinputbox.o: $(SRC)/sectorinputbox.cpp
	$(CC) $(FLAGS) $(SRC)/sectorinputbox.cpp 

textinputbox.o: $(SRC)/textinputbox.cpp
	$(CC) $(FLAGS) $(SRC)/textinputbox.cpp 

titlescreen.o: $(SRC)/titlescreen.cpp
	$(CC) $(FLAGS) $(SRC)/titlescreen.cpp 

toolbox.o: $(SRC)/toolbox.cpp
	$(CC) $(FLAGS) $(SRC)/toolbox.cpp 

uploadzone.o: $(SRC)/uploadzone.cpp
	$(CC) $(FLAGS) $(SRC)/uploadzone.cpp 

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp 


clean:
	rm -f $(OBJS) $(OUT)
