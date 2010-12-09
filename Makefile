all: asciibar.o example.o
	$(CC) -o asciibar asciibar.o example.o

clean:
	rm -f asciibar.o example.o asciibar
