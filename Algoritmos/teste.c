#include<stdio.h>
#include<time.h>
#include<stdlib.h>


char* datetime();
char* cleandata(char* data);


int main(void){
    char* data = datetime();
    int i = 0;
    printf("retorno na main %s\n", data);

    cleandata(data);


    return 0;
}

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
    printf("retorno na função %s\n", timeString);

    return timeString;
}

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
    printf("%s\n", data);


}