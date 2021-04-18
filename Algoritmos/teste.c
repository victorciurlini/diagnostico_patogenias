#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv)
{
    int num = 2;
    char *texto = "alooo galera de cowboy";
    char *insert = "INSERT INTO";
    char *size_string;
    size_string = malloc(sizeof(char)*100);
    //"INSERT INTO cars VALUES(1, 'AudiTT',23642)"
    sprintf(size_string, "E aqui eu posso até já colocar o texto %s %s", texto, insert);
    printf("%s", size_string);
  exit(0);
}