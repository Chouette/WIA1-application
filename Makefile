test: test.o files.o mot.o liste.o hashtable.o
	gcc -o test test.o files.o mot.o liste.o hashtable.o -lm
appli: app.o files.o mot.o liste.o hashtable.o
	gcc -o appli app.o files.o mot.o liste.o hashtable.o -lm


app.o: app.c files.h mot.h hashtable.h
	gcc -c app.c -o app.o

hashtable.o : hashtable.c hashtable.h 
	gcc -c hashtable.c -o hashtable.o

test.o: test.c files.h mot.h liste.h
	gcc -c test.c -o test.o

files.o: files.c files.h
	gcc -c files.c -o files.o

mot.o: mot.c mot.h
	gcc -c mot.c -o mot.o

liste.o: liste.c liste.h
	gcc -c liste.c -o liste.o

clean:
	rm *.o
	
