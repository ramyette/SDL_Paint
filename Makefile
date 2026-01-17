debug:
	gcc main.c -o paint_debug.exe -I src/include -L src/lib -lmingw32 -lSDL2main -lSDL2
all:
	gcc main.c -o paint.exe -I src/include -L src/lib -lmingw32 -lSDL2main -lSDL2 -mwindows