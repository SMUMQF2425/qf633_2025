#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *alloc_memory(void)
{
    char * str = malloc(100 * sizeof(char));
    str = strdup("Hello World");
    printf("Memory allocated...\n");
    return str;
}

void free_memory(char *ptr)
{
    printf("Freeing memory...\n");
    free(ptr);
}

char *str_concatenate(char *str1, char *str2)
{
    char *str;
    str = malloc(100 * sizeof(char));
    printf("Memory allocated...%s\n", str);
    memccpy(str, strcat(str1, str2), 0, 100);
    return str;
}


