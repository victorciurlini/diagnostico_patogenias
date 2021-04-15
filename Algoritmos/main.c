#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>

// Inicializa as funções.
char* datetime();
char* cleandata(char* data);
void* move_file(char* path, char* data);

int main()
{
    /********Declarando variáveis*******/
    char ch, file_name[100];
    char path[20] = "../Photos/";
    //char ext[20] = ".txt";
    char full_path[200] = "";
    char *data = datetime();
    char *nome_data = cleandata(data);
    char *cmd;
    char *folder = "../Photos";
    char *format="test $(ls -AU \"%s\" 2>/dev/null | head -1 | wc -l) -ne 0";
    int status, exitcode;
    int size;
    int files = 0;
    int i;
    int compare;
    struct dirent *entry;
    DIR *directory;
    FILE *fp;
    clock_t start, stop;

    /***********************************/

    size = strlen(format)+strlen(folder)+1;
    cmd = malloc(size * sizeof(char));

    snprintf(cmd, size, format, folder);
    //printf("executing: %s\n", cmd);

    status = system(cmd);
    //exitcode = WEXITSTATUS(status);

    //printf ("exit code: %d, exit status: %d\n", exitcode, status);

    //Verifica se o diretório está vazio, aguarda 5 segundos para verificar novamente
    if (exitcode == 1){
            sleep(5);
            printf("O diretório está vazio\n");
    }
    else
        directory = opendir(folder);

        while((entry=readdir(directory))){
            files++;
            char *filename = entry->d_name;
            for(i=0; filename[i]; i++){
                if (filename[i] == '.'){
                    break;
                }
                else{
                    strcpy(file_name, filename);
                }
            }
            strcat(path, file_name);
            compare = strcmp(path, "../Photos/");
            if(compare != 0){
                printf("compare: %d\n", compare);
                printf("file_name: %s\n", file_name);
                printf("path: %s\n", path);
                printf("Nome do arquivo: %s\n", file_name);
                printf("Caminho do arquivo: %s\n", path);
                fp = fopen(path, "r");

                if (fp == NULL){
                    perror("Arquivo não encontrado.\n");
                    exit(EXIT_FAILURE);
                }
                else
                    printf("Arquivo encontrado!\n");
                    move_file(path, nome_data);
            }
            strcpy(path, "../Photos/");
        }
        free(cmd);
        closedir(directory);
       
    return 0;
}


//Retorna o horário em que a imagem foi acessada
char* datetime(){
    time_t secs = time(0);
    char* timeString;
    timeString = malloc(sizeof(char)*100);

    /* convert to localtime */
    struct tm *local = localtime(&secs);

    int year = local-> tm_year + 1900;
    int mon = local-> tm_mon;
    int mday = local-> tm_mday;
    int hour = local-> tm_hour;
    int min = local-> tm_min;
    int sec = local-> tm_sec;

    /* and set the string */
    sprintf(timeString, "%d/%02d/%02d %02d:%02d:%02d", year, mon, mday, hour, min, sec);

    return timeString;
}

// Função para limpar o datetime e transformar em nome para
// figura
char* cleandata(char* data){
    int i,j;

    for(i=0; data[i]!='\0'; ++i){
        while (!((data[i]>='0'&&data[i]<='9') || (data[i]>='A'&&data[i]<='Z' || data[i]=='\0'))){
            for(j=i;data[j]!='\0';++j){
                data[j]=data[j+1];
            }
            data[j]='\0';
        }
    }
    return data;
}
//Move o arquivo encontrado na pasta Photos
void* move_file(char* path, char* nome_data){

    char* path_database = "../database/";
    char* ext = ".jpg";
    char* novo_path;

    sprintf(novo_path, "%s%s%s", path_database, nome_data, ext);
    printf("Movendo arquivo para: %s\n", novo_path);
    rename(path, novo_path);
}