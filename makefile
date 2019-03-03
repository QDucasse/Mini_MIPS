default: Mini_MIPS

Mini_MIPS: main.o iss.o 
	gcc main.o iss.o -o Mini_MIPS -Wall -Wextra -pedantic -lPython

main.o: src/main.c src/shared.h
	gcc -c src/main.c -Wall -Wextra -pedantic 

iss.o: src/iss.c src/iss.h src/shared.h
	gcc -c src/iss.c -Wall -Wextra -pedantic 

clean:
	rm -rf *.o Mini_MIPS