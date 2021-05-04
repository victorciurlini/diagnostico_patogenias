all: main.out create_populate.out

main: main.c
	c99 main.c -o main.out  `mysql_config --cflags --libs`

create_populate: create_populate.c
	c99 create_populate.c -o create_populate.out  `mysql_config --cflags --libs`

testa_binario: CHAMADA_BINARIO_C.c
	gcc CHAMADA_BINARIO_C.c -o testa_binario.out

clean:
	rm -rf *out *.o *.bin *.txt 
