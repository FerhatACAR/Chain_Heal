CLASSDIR = /home/mrford/Documents/deneme/libfdr
INCLUDE = -I$(CLASSDIR)/inc
LIBS = $(CLASSDIR)/lib/libfdr.a
CC = gcc

proje : main.o
	$(CC) -g -o proje testEsas.o $(LIBS)  -lm
main.o : deneme.c
	$(CC) -g $(INCLUDE) -c testEsas.c 
clean:
	${LIBCDIR}rm *.o
	@echo Obje dosyalar basari ile temizlenmistir. 