chain_heal : main.o
	gcc -g -o chain_heal chain_heal.o ./libfdr/lib/libfdr.a  -lm
	rm *.o
	@echo Obje dosyalar basari ile temizlenmistir.
main.o : chain_heal.c
	gcc -g -I ./libfdr/inc -c chain_heal.c 