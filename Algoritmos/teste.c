#include <stdio.h>
#include<time.h>
#include <unistd.h>

int main () {

/* local variable definition */
int a = 10;

/* while loop execution */
while(1) {
sleep(1);
printf("value of a: %d\n", a);
a++;

}

return 0;
}