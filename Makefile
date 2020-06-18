OBJS	= animation.o animator.o choiceinputbox.o databattle.o databattleeditor.o databattlepiece.o databattleplayer.o hud.o inputbox.o netmap_playable.o npc.o npcanim.o player.o program.o programaction.o programsector.o scene.o sceneentity.o scenelayer.o sectorinputbox.o textinputbox.o titlescreen.o toolbox.o uploadzone.o main.o
SOURCE	= animation.cpp animator.cpp choiceinputbox.cpp databattle.cpp databattleeditor.cpp databattlepiece.cpp databattleplayer.cpp hud.cpp inputbox.cpp netmap_playable.cpp npc.cpp npcanim.cpp player.cpp program.cpp programaction.cpp programsector.cpp scene.cpp sceneentity.cpp scenelayer.cpp sectorinputbox.cpp textinputbox.cpp titlescreen.cpp toolbox.cpp uploadzone.cpp main.cpp
HEADER	= animation.h animator.h choiceinputbox.h databattle.h databattleeditor.h databattlepiece.h databattleplayer.h hud.h inputbox.h netmap_playable.h npc.h npcanim.h player.h program.h programaction.h programsector.h scene.h sceneentity.h scenelayer.h sectorinputbox.h textinputbox.h titlescreen.h toolbox.h uploadzone.h

ifeq ($(OS),Windows_NT)
OUT	= NetmapSFML.exe
else
OUT	= NetmapSFML
endif

CC	 = g++
FLAGS	 = -I./include -I./sfml/include -g -c -Wall -std=c++11
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

animation.o: src\animation.cpp
	$(CC) $(FLAGS) src\animation.cpp 

animator.o: src\animator.cpp
	$(CC) $(FLAGS) src\animator.cpp 

choiceinputbox.o: src\choiceinputbox.cpp
	$(CC) $(FLAGS) src\choiceinputbox.cpp 

databattle.o: src\databattle.cpp
	$(CC) $(FLAGS) src\databattle.cpp 

databattleeditor.o: src\databattleeditor.cpp
	$(CC) $(FLAGS) src\databattleeditor.cpp 

databattlepiece.o: src\databattlepiece.cpp
	$(CC) $(FLAGS) src\databattlepiece.cpp 

databattleplayer.o: src\databattleplayer.cpp
	$(CC) $(FLAGS) src\databattleplayer.cpp 

hud.o: src\hud.cpp
	$(CC) $(FLAGS) src\hud.cpp 

inputbox.o: src\inputbox.cpp
	$(CC) $(FLAGS) src\inputbox.cpp 

netmap_playable.o: src\netmap_playable.cpp
	$(CC) $(FLAGS) src\netmap_playable.cpp 

npc.o: src\npc.cpp
	$(CC) $(FLAGS) src\npc.cpp 

npcanim.o: src\npcanim.cpp
	$(CC) $(FLAGS) src\npcanim.cpp 

player.o: src\player.cpp
	$(CC) $(FLAGS) src\player.cpp 

program.o: src\program.cpp
	$(CC) $(FLAGS) src\program.cpp 

programaction.o: src\programaction.cpp
	$(CC) $(FLAGS) src\programaction.cpp 

programsector.o: src\programsector.cpp
	$(CC) $(FLAGS) src\programsector.cpp 

scene.o: src\scene.cpp
	$(CC) $(FLAGS) src\scene.cpp 

sceneentity.o: src\sceneentity.cpp
	$(CC) $(FLAGS) src\sceneentity.cpp 

scenelayer.o: src\scenelayer.cpp
	$(CC) $(FLAGS) src\scenelayer.cpp 

sectorinputbox.o: src\sectorinputbox.cpp
	$(CC) $(FLAGS) src\sectorinputbox.cpp 

textinputbox.o: src\textinputbox.cpp
	$(CC) $(FLAGS) src\textinputbox.cpp 

titlescreen.o: src\titlescreen.cpp
	$(CC) $(FLAGS) src\titlescreen.cpp 

toolbox.o: src\toolbox.cpp
	$(CC) $(FLAGS) src\toolbox.cpp 

uploadzone.o: src\uploadzone.cpp
	$(CC) $(FLAGS) src\uploadzone.cpp 

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp 


clean:
	rm -f $(OBJS) $(OUT)
