#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


char* datetime();

int main()
{
    char ch, file_name[100];
    FILE *fp;
    char path[20] = "../Photos/";
    char ext[20] = ".txt";
    char full_path[200] = "";
    printf("Enter name of a file you wish to see\n");
    scanf("%s", file_name);

    strcat(file_name, ext);
    strcat(path, file_name);
    printf("Nome do arquivo: %s\n", file_name);
    printf("Caminho do arquivo: %s\n", path);
    fp = fopen(path, "r"); // read mode
//    printf("\n%s\n", file_name);
//    printf("%s", path);

    if (fp == NULL){
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }


    while((ch = fgetc(fp)) != EOF)
        printf("%c", ch);

    fclose(fp);
    return 0;
}

char* datetime(){
    time_t secs = time(0);
    char* timeString;
    timeString = malloc(sizeof(char)*100);

    /* convert to localtime */
    struct tm *local = localtime(&secs);
    int year = local-> tm_year + 1900;

    /* and set the string */
    sprintf(timeString, "%d/%02d/%02d %02d:%02d:%02d", year, local->tm_mon, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);
    printf("retorno na função %s\n", timeString);

    return timeString;
}