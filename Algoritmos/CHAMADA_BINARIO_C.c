#include <stdio.h>
#include <stdlib.h>


#define SHELLSCRIPT "\
#!/bin/sh \n\
python3 PREDICTION_BINARIO_FINAL.py \n\
"

char *chama_modelo(){

    FILE *p;
    char *classificacao;
    char c[1000];

    p = fopen("RESULTADO.txt","r");
    	if(p==NULL)
	{
		printf("Erro! Impossivel abrir o arquivo!\n");
		exit(-1);
	}

    fscanf(p, "%[^\n]", classificacao);
    printf("Data from the file:\n%s\n", classificacao);
    fclose(p);

    return classificacao;
}

int main(int argc, const char * argv[])
{
FILE *p;
char *classificacao;

    puts(SHELLSCRIPT);
    system(SHELLSCRIPT);    //it will run the script inside the c code. 
    classificacao = chama_modelo();
    return 0;
}
