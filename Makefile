CC = cc
CFLAGS = -Os -pedantic -s -static -std=c89 -Wall -Wextra

OBJ = dmc.o util.o #TODO

dmc: ${OBJ}
	cc ${CFLAGS} -o dmc ${OBJ}

.c.o:
	${CC} ${CFLAGS} -c $<

clean:
	rm -f dmc ${OBJ}
