default: Mini_MIPS

Mini_MIPS: objs/main.o objs/iss.o 
	gcc objs/main.o objs/iss.o -o Mini_MIPS -Wall -Wextra -pedantic -lPython
	mv Mini_MIPS bin/Mini_MIPS

objs/main.o: src/main.c src/shared.h src/cache.h
	gcc -c src/main.c -Wall -Wextra -pedantic 
	mv main.o objs/main.o

objs/iss.o: src/iss.c src/iss.h src/shared.h src/cache.h
	gcc -c src/iss.c -Wall -Wextra -pedantic 
	mv iss.o objs/iss.o

clean:
	rm -rf *.o Mini_MIPS