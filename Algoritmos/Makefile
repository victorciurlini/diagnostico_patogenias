all: createdb.out create_populate.out
createdb.out: createdb.c
	c99 createdb.c -o createdb.out  `mysql_config --cflags --libs`
create_populate.out: create_populate.c
	c99 create_populate.c -o create_populate.out  `mysql_config --cflags --libs`

clean:
	rm -rf *out *.o *.bin *.txt 