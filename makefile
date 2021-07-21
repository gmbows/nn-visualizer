net.exe: bin/Main.o bin/Neuron.o bin/Utility.o bin/Window.o
	g++  bin/Main.o bin/Neuron.o bin/Utility.o bin/Window.o -lpthread -I /include/SDL2 -L /lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -fpermissive -O3 -o net.exe

bin/Main.o: src/Main.cpp  
	g++  -c src/Main.cpp -lpthread -fpermissive -O3 -I /include/SDL2 -L /lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o bin/Main.o

bin/Neuron.o: src/Neuron.cpp src/Neuron.h  
	g++  -c src/Neuron.cpp -lpthread -fpermissive -O3 -I /include/SDL2 -L /lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o bin/Neuron.o

bin/Utility.o: src/Utility.cpp src/Utility.h  
	g++  -c src/Utility.cpp -lpthread -fpermissive -O3 -I /include/SDL2 -L /lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o bin/Utility.o

bin/Window.o: src/Window.cpp src/Window.h  src/Neuron.h src/Neuron.cpp  
	g++  -c src/Window.cpp -lpthread -fpermissive -O3 -I /include/SDL2 -L /lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o bin/Window.o

