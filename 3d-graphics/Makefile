build:
	gcc -Wall -std=c99 \
		-L/opt/homebrew/Cellar/sdl2/2.30.4/lib/ \
		-lSDL2 \
		-I/opt/homebrew/Cellar/sdl2/2.30.4/include/ \
		./src/*.c -o renderer
run:
	./renderer
# gcc -Wall -std=c99 -rpath /Library/Frameworks/ -F/Library/Frameworks -framework SDL2 ./src/*.c -o renderer