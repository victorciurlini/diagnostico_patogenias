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
void* populate_mysql(char* path_database, char* file_name, char* data, char *classificacao);
void finish_with_error(MYSQL *con);
char* model_result();


#define SHELLSCRIPT "\
#!/bin/sh \n\
python3 predict_model.py \n\
"

int main(int argc, char **argv)
{
    /********Declarando variáveis*******/
    char ch, file_name[100];
    char path[20] = "../Photos/";
    char *path_database;
    char *data;
    char *nome_data;
    char *cmd;
    char *classificacao;
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
    FILE *p;
    clock_t start, stop;
    /***********************************/

    while(1){
        
        size = strlen(format)+strlen(folder)+1;
        cmd = malloc(size * sizeof(char));
        snprintf(cmd, size, format, folder);
        //printf("%s\n", cmd);

        status = system(cmd);
        //printf("status: %d\n", status);
        exitcode = WEXITSTATUS(status);

        //printf ("exit code: %d, exit status: %d\n", exitcode, status);
        //Verifica se o diretório está vazio, aguarda 5 segundos para verificar novamente
        if (exitcode == 1){
                sleep(2);
                printf("\nO diretório está vazio\n");
        }
        else{
            printf("\nexecuta fluxo\n");
            //Caso o repositório não estiver vazio, ele é aberto
            //Para receber o conteúdo
            directory = opendir(folder);

            //Ao abrir o repositório, ele lê todos os arquivos contidos
            while((entry=readdir(directory))){
                files++; // Incrimenta pelos arquivos
                char *filename = entry->d_name; //Recebe o nome dos arquivos
            //     //Loop para ignorar os repositórios "." e ".."
                for(i=0; filename[i]; i++){
                    if (filename[i] == '.'){
                        break;
                    }
                    else{
                        strcpy(file_name, filename); //Copia o nome da imagem para outra variável
                    }
                }
            }
                strcat(path, file_name); //Concatena o nome do arquivo com o path para próxima verificação
                printf("%s\n", path);
                compare = strcmp(path, "../Photos/"); // Verifica se o repositório é vazio
                if(compare != 0){
                    //Abre o arquivo
                    fp = fopen(path, "r");
                    //Move o arquivo para a pasta "database"
                    data = datetime();
                    nome_data = cleandata(data);
                    path_database = move_file(path, nome_data);

            //         //puts(SHELLSCRIPT);
                    system(SHELLSCRIPT);    //it will run the script inside the c code. 

                    classificacao = model_result();
                    fclose(fp);
                    populate_mysql(path_database, file_name, data, classificacao);
                }
                //reinicia a variável path
                strcpy(path, "../Photos/");
                strcpy(file_name, "");
                
            
            status = 0;
            free(cmd);
            closedir(directory);
            sleep(2);
        }
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
    novo_path = malloc(sizeof(char)*100);
    sprintf(novo_path, "%s%s%s", path_database, nome_data, ext);
    //printf("Movendo arquivo para: %s\n", novo_path);
    rename(path, novo_path);

    return novo_path;
}

//Adiciona as informações da planta no banco de dados
void* populate_mysql(char *path_database, char *file_name, char *data, char *classificacao){
    MYSQL *con = mysql_init(NULL);

    char *sql_command;

    sql_command = malloc(sizeof(char)*1000);

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
        printf("Salva na tabela: %s, %s, %s, %s\n",file_name, data, classificacao, path_database );
    sprintf(sql_command, "INSERT INTO dado_folha VALUES ('%s', '%s', '%s', '%s')", file_name, data, classificacao, path_database);

    if (mysql_query(con, sql_command)) {
        finish_with_error(con);
    }

    //exit(0);
    mysql_close(con);
}

//Levanta erro da conexão com o banco de dados
void finish_with_error(MYSQL *con){
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

//Retorna o resultado do modelo
char* model_result(){

    FILE *p;
    char *classifica;
    char c[1000];

    classifica = malloc(sizeof(char)*100);
    p = fopen("../resultado/RESULTADO.txt","r");
    	if(p==NULL)
	{
		printf("Erro! Impossivel abrir o arquivo!\n");
		exit(-1);
	}

    fscanf(p, "%[^\n]", classifica);
    fclose(p);

    return classifica;
}