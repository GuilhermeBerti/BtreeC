#include <stdio.h>
#include <stdlib.h>
#include "Btree.h"


int main(){

    Btree* arv = cria_Btree();

    int i;
    for(i=0; i < 4; i++)
        insere_Btree(arv,i);
    preOrdem_Btree(arv);
    system("pause");
    return 0;
}

