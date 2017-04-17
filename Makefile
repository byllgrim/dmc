dmc: dmc.o
	cc -o dmc dmc.o

dmc.o: dmc.c
	cc -c dmc.c
