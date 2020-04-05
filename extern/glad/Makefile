LIB = libglad.a

$(LIB): glad.o
	ar rcs $@ $^

glad.o: glad.c
	gcc -O3 -c -o $@ $^ -I.. -I.
	
clean:
	rm -f *.o *.a $(LIB)