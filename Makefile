cc = gcc
CFLAGS = -std=c11 -Werror -Wall
LDLIBS = -lm
OUTPUT = ps1

all : playfair
playfair : main.o bmp.o playfair.o 
	$(cc) main.o bmp.o playfair.o -o ps1 $(CFLAGS) $(LDLIBS)
main.o : main.c
	cppcheck --enable=performance,unusedFunction --error-exitcode=1 main.c
	$(cc) -c main.c $(CFLAGS) $(LDLIBS)
bmp.o : bmp.c
	cppcheck --enable=performance --error-exitcode=1 bmp.c
	$(cc) -c bmp.c $(CFLAGS) $(LDLIBS)
playfair.o : playfair.c
	cppcheck --enable=performance --error-exitcode=1 playfair.c
	$(cc) -c playfair.c $(CFLAGS) $(LDLIBS)
clean:
	rm *o
	rm $(OUTPUT)