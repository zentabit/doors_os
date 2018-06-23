#include <stdio.h>

int strleng(char *str){
    int i = 0;
    while(*(str+i) != '\0'){
        i++;
    }
    return i;
}


int main(int argc, char const *argv[])
{
    printf("%d", strleng("Hello!"));
    return 0;
}
