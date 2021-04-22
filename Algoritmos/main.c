#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <mysql/mysql.h>
#include <sys/wait.h>


// Inicializa as funções.
char* datetime();
char* cleandata(char* data);
char* move_file(char* path, char* data);
void* populate_mysql(MYSQL *con, char* path_database, char* file_name, char* data);
void finish_with_error(MYSQL *con);

int main(int argc, char **argv)
{
    /********Declarando variáveis*******/
    char ch, file_name[100];
    char path[20] = "../Photos/";
    char *path_database;
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
    MYSQL *con = mysql_init(NULL);
    /***********************************/

    size = strlen(format)+strlen(folder)+1;
    cmd = malloc(size * sizeof(char));

    snprintf(cmd, size, format, folder);
    //printf("executing: %s\n", cmd);

    status = system(cmd);
    exitcode = WEXITSTATUS(status);

    //printf ("exit code: %d, exit status: %d\n", exitcode, status);
    //Verifica se o diretório está vazio, aguarda 5 segundos para verificar novamente
    if (exitcode == 1){
            //sleep(5);
            printf("O diretório está vazio\n");
    }
    else{
        //Caso o repositório não estiver vazio, ele é aberto
        //Para receber o conteúdo
        directory = opendir(folder);

        //Ao abrir o repositório, ele lê todos os arquivos contidos
        while((entry=readdir(directory))){
            files++; // Incrimenta pelos arquivos
            char *filename = entry->d_name; //Recebe o nome dos arquivos
            
            //Loop para ignorar os repositórios "." e ".."
            for(i=0; filename[i]; i++){
                if (filename[i] == '.'){
                    break;
                }
                else{
                    strcpy(file_name, filename); //Copia o nome da imagem para outra variável
                }
            }
            strcat(path, file_name); //Concatena o nome do arquivo com o path para próxima verificação
            compare = strcmp(path, "../Photos/"); // Verifica se o repositório é vazio
            if(compare != 0){
                printf("compare: %d\n", compare);
                printf("file_name: %s\n", file_name);
                printf("path:   %s\n", path);
                printf("Nome do arquivo: %s\n", file_name);
                printf("Caminho do arquivo: %s\n", path);

                //Abre o arquivo
                fp = fopen(path, "r");
                printf("Arquivo encontrado!\n");

                //Move o arquivo para a pasta "database"
                path_database = move_file(path, nome_data);
                printf("path_database: %s\n", path_database);
                
                populate_mysql(con, path_database, file_name, data);
            }
            //reinicia a variável path
            strcpy(path, "../Photos/");
        }
        free(cmd);
        closedir(directory);
    }

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
// Função para limpar o datetime e transformar
// em nome para figura
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
char* move_file(char* path, char* nome_data){

    char* path_database = "../database/";
    char* ext = ".jpg";
    char* novo_path;

    sprintf(novo_path, "%s%s%s", path_database, nome_data, ext);
    printf("Movendo arquivo para: %s\n", novo_path);
    rename(path, novo_path);

    return novo_path;
}

void* populate_mysql(MYSQL *con, char* path_database, char* file_name, char* data){

    printf("MySQL client version: %s\n", mysql_get_client_info());
    
    printf("passou do mysql con ");
    char *sql_command;
    sql_command = malloc(sizeof(char)*100);
    if (con == NULL){
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

  if (mysql_real_connect(con, 
                        "localhost", 
                        "cliente", 
                        "Cliente@0001",
                        "DATABASE_PATOGENOS", 0, NULL, 0) == NULL){
        finish_with_error(con);
    }

    sprintf(sql_command, "INSERT INTO dado_folha VALUES ('%s', '%s', '%s')", file_name, data, path_database);

    if (mysql_query(con, sql_command)) {
        finish_with_error(con);
    }

    // if (mysql_query(con, "INSERT INTO cars VALUES(40,'Mercedes',57127)")) {
    //     finish_with_error(con);
    // }
    //mysql_close(con);
    exit(0);
}

void finish_with_error(MYSQL *con){
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}