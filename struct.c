#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct{
    char address[32];
}inner;

typedef struct{
    int id;
    inner* innerStruct;
}outer;

int main()
{
    int i = 0;
    outer* outerArray;
    outer* outerReference;
    /* inner* innerReference; */ /* unused */

    /* create 20 outer structs */
    outerArray = malloc(20 * sizeof(*outerArray));

    /* for each outer struct, dynamically allocate 10 inner structs */

    for(i = 0; i < 10; i++)
    {
        outerReference = outerArray + i;
        outerReference->innerStruct = malloc(10 * sizeof(*outerReference->innerStruct));
    }

    printf("id: '%d'\n", outerArray[1].id);
    printf("address: '%s'\n", outerArray[0].innerStruct[0].address);
    printf("address: '%s'\n", outerArray[3].innerStruct[4].address);
    outerArray[1].id = 1;
    strcpy(outerArray[0].innerStruct[0].address, "<satu>");
    strcpy(outerArray[3].innerStruct[4].address, "<xxx>");
    printf("id: '%d'\n", outerArray[1].id);
    printf("address: '%s'\n", outerArray[0].innerStruct[0].address);
    printf("address: '%s'\n", outerArray[3].innerStruct[4].address);

    return 0;
}
