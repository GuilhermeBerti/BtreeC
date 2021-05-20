#include <stdio.h>
#include <stdlib.h>
#include "Btree.h"

/*
Grupo XXX
Integrantes:
Guilherme de Castro Berti - 11821BCC023
Kau� Lucas Silv�rio Oliveira - 11821bcc007
La�s Saloum Deghaide - 11821bcc001
*/

// obs: Pelo meus teste toda inser��o e o balanceamento p�s inser��o est� funcionando tudo ok , a remo��o est� funcionando ok mas pode ocorrer problemas que ainda n�o consegui resolver no balanceamento p�s inser��o.

#define M 4
struct NO{
    int info[M];
    int cont;
    struct NO *v[M+1];
};

Btree* cria_Btree(){
    Btree* btree = (Btree*) malloc(sizeof(Btree));
    if(btree != NULL)
        *btree = NULL;
    return btree;
}

int busca_Btree(Btree *btree, int valor){
    if(btree == NULL){
        return 0;
    }
    struct NO* atual = *btree;
    struct NO* ant = *btree;
    int i;
    while(atual != NULL){
        ant = atual;
        for(i=0;i<atual->cont;i++){
            if(valor == atual->info[i]){
                return 1;
            }
        }
        for(i=0;i<atual->cont;i++){
            if(valor <= atual->info[i]){
                atual = ant->v[i];
                break;
            }
        }
        if(atual == ant){
            atual = ant->v[atual->cont];
        }
    }
    return 0;
}

int insere_NO_Btree(Btree* btree,int valor){
    int i , j;
    for(i=0;i<(*btree)->cont;i++){
        if(valor <= (*btree)->info[i]){
            for(j=(*btree)->cont;j>i;j--){
                (*btree)->info[j] = (*btree)->info[j-1];
            }
            (*btree)->info[i] = valor;
            (*btree)->cont++;
            return 1;
        }
    }
    (*btree)->cont++;
    (*btree)->info[i] = valor;
    return 1;
}

struct NO* divisao_raiz_Btree(Btree *btree){
    int metade ;
    metade = ((M/2));
    int i;
    struct NO* novo1;
    novo1 = (struct NO*) malloc(sizeof(struct NO));
    novo1->cont = 0;
    for(i=0;i<=M;i++){
        novo1->v[i] = NULL;
    }
    for(i=0;i<metade;i++){
        insere_NO_Btree(&novo1,(*btree)->info[i]);
    }
    for(i=0;i<=metade;i++){
        novo1->v[i] = (*btree)->v[i];
    }
    struct NO* novo2;
    novo2 = (struct NO*) malloc(sizeof(struct NO));
    novo2->cont = 0;
    for(i=0;i<=M;i++){
        novo2->v[i] = NULL;
    }
    for(i=metade+1;i<(*btree)->cont;i++){
        insere_NO_Btree(&novo2,(*btree)->info[i]);
    }
    for(i=0;i<=metade;i++){
        novo2->v[i] = (*btree)->v[metade+i+1];
    }

    struct NO* novo3;
    novo3 = (struct NO*) malloc(sizeof(struct NO));
    for(i=0;i<=M;i++){
        novo3->v[i] = NULL;
    }
    novo3->cont = 0;
    insere_NO_Btree(&novo3,(*btree)->info[metade]);
    novo3->v[0] = novo1;
    novo3->v[1] = novo2;
    return novo3;
}


struct NO* divisao_NOfilho_Btree(Btree *btree){
    struct NO* filho = *btree;
    int metade = (M/2);
    int i,j;
    int vetor;
    for(i=0;i<M;i++){//procura do no filho
        if((*btree)->v[i]->cont == M){
            filho = (*btree)->v[i];
            j = i;
            break;
        }
    }
    //vetor = 1
    //filho = v[1]

    struct NO* novo1;
    novo1 = (struct NO*) malloc(sizeof(struct NO));
    novo1->cont = 0;
    for(i=0;i<=M;i++){
        novo1->v[i] = NULL;
    }
    for(i=0;i<metade;i++){
        insere_NO_Btree(&novo1,filho->info[i]);
    }
    for(i=0;i<=metade;i++){
        novo1->v[i] = filho->v[i];
    }
    // 3
    struct NO* novo2;
    novo2 = (struct NO*) malloc(sizeof(struct NO));
    novo2->cont = 0;
    for(i=0;i<=M;i++){
        novo2->v[i] = NULL;
    }
    for(i=metade+1;i<filho->cont;i++){
        insere_NO_Btree(&novo2,filho->info[i]);
    }
    for(i=0;i<=metade;i++){
        novo2->v[i] = filho->v[metade+i+1];
    }
    // 5
    insere_NO_Btree(btree,filho->info[metade]);
    for(i=M;i>j;i--){
        (*btree)->v[i] = (*btree)->v[i-1];
    }
    (*btree)->v[j] = novo1;
    (*btree)->v[j+1] = novo2;
    // 2 4
    return (btree);
}


int balanceamento_Btree(Btree* btree,int valor){
    int i;
    struct NO* atual = (*btree);
    struct NO* ant = (*btree);
    int altura=0;
    while((atual != NULL) && (atual->cont != M)){ //acha o N� cheio
        ant = atual;
        for(i=0;i<atual->cont;i++){
            if(valor <= ant->info[i]){
                atual = ant->v[i];
                altura++;
                break;
            }
        }
        if(atual == ant){
            atual = ant->v[atual->cont];
            altura++;
        }
    }
    if(atual == NULL){
        return 1;
    }
    if(altura != 0){
        struct NO* ant2 = (*btree);
        atual = (*btree);
        int guarda;
        for(i=0;i<(altura-1);i++){
            ant2 = atual;
            for(i=0;i<atual->cont;i++){
                if(valor <= ant2->info[i]){
                    atual = ant2->v[i];
                    guarda = i;
                    break;
                }
            }
            if(atual == ant2){
                guarda = i;
                atual = ant2->v[atual->cont];
            }
        }
        if(altura == 1){
            btree = divisao_NOfilho_Btree(&ant);
            balanceamento_Btree(btree, valor);
        }
        else{
            btree = divisao_NOfilho_Btree(&ant);
            balanceamento_Btree(btree, valor);
        }
    }
    else{
        (*btree) = divisao_raiz_Btree(btree);
        return 1;
    }
}

int insere_Btree(Btree* btree,int valor){
    if(btree == NULL){
        return 0;
    }
    int i,j;
    if(((*btree) == NULL)){
        struct NO* novo;
        novo = (struct NO*) malloc(sizeof(struct NO));
        novo->info[0] = valor;
        for(i=0;i<=M;i++){
            novo->v[i] = NULL;
        }
        novo->cont = 1;
        (*btree) = novo;
        return 1;
    }
    struct NO* atual = *btree;
    struct NO* ant = *btree;
    while(atual != NULL){ //procura no folha
        ant = atual;
        for(i=0;i<atual->cont;i++){
            if(valor <= atual->info[i]){
                atual = ant->v[i];
                break;
            }
        }
        if(atual == ant){
            atual = ant->v[atual->cont];
        }
    }
    //apos achar n� vazio insere no n� anterior(folha);
    for(i=0;i<=M;i++){
        ant->v[i] = NULL;
    }
    insere_NO_Btree(&(ant),valor);
    balanceamento_Btree(btree,valor);
    return 1;
}

void preOrdem_Btree(Btree *btree){
    if(btree == NULL){
        return ;
    }
    if(*btree != NULL){
        int i=0;
        for(i=0;i < (*btree)->cont;i++){
            printf("pre: %d %d\n",(*btree)->info[i],(*btree)->cont);
            preOrdem_Btree(&((*btree)->v[i]));
        }
        preOrdem_Btree(&((*btree)->v[(*btree)->cont]));
    }else
        printf("pre: fim\n");
}




int remove_Btree(Btree* btree,int valor){
    printf("\n%d",valor);
    int i;
    struct NO* atual = *btree;
    struct NO* ant = *btree;
    int achou = 0;
    while(atual != NULL && achou == 0){ //procura no folha
        for(i=0;i<atual->cont;i++){
            if(atual->info[i]==valor){
                achou = 1;
            }
        }
        ant = atual;
        for(i=0;i<atual->cont;i++){
            if(valor <= atual->info[i]){
                atual = ant->v[i];
                break;
            }
        }
        if(atual == ant){
            atual = ant->v[atual->cont];
        }
    }
    if(atual == NULL){
        remocao_folha1_Btree(&ant,valor);
    }
    else{
        remocao_interno_Btree(btree,valor);
    }
}


int remocao_folha1_Btree(Btree* btree,int valor){ //remove de um n� folha
    int i;
    struct NO* atual = *btree;
    struct NO* ant = *btree;
    int achou = 0;
    while(atual != NULL && achou == 0){ //procura no folha
        for(i=0;i<atual->cont;i++){
            if(atual->info[i]==valor){
                achou = 1;
            }
        }
        ant = atual;
        for(i=0;i<atual->cont;i++){
            if(valor <= atual->info[i]){
                atual = ant->v[i];
                break;
            }
        }
        if(atual == ant){
            atual = ant->v[atual->cont];
        }
    }
    i = 0;
    while(ant->info[i] != valor){
        i++;
    }
    for(i;i<ant->cont;i++){
        ant->info[i] = ant->info[i+1];
    }
    ant->info[ant->cont] = NULL;
    ant->cont--;
    return 1;
    balanceamento_remocao_Btree(btree,valor);
}


int remocao_interno_Btree(Btree* btree,int valor){
    //preciso trocar pelo valor anterior a ele
    int i,j;
    struct NO* atual = *btree;
    struct NO* ant = *btree;
    int achou = 0;
    while(atual != NULL && achou == 0){ //procura no folha
        for(i=0;i<atual->cont;i++){
            if(atual->info[i]==valor){
                achou = 1;
            }
        }
        ant = atual;
        for(i=0;i<atual->cont;i++){
            if(valor <= atual->info[i]){
                atual = ant->v[i];
                break;
            }
        }
        if(atual == ant){
            atual = ant->v[atual->cont];
        }
    }
    int guarda;
    struct NO* atual2 = ant;
    for(i=0;i<atual2->cont;i++){
        if(atual2->info[i]==valor){
            guarda = i ;
            i = M;
        }
    }
    struct NO* ant2 = ant;
    ant2 = atual2->v[guarda];
    atual2 = ant2;
    while(atual2 != NULL){
        ant2 = atual2;
        atual2 = ant2->v[ant2->cont];
    }
    i = ant2->info[ant2->cont-1];
    remove_Btree(btree,i);
    for(j=ant->cont;j>guarda;j--){
        ant->info[j] = ant->info[j-1];
    }
    ant->cont++;
    ant->info[guarda] = i;
    balanceamento_remocao_Btree(btree,valor);
    return 1;
}

int balanceamento_remocao_Btree(Btree* btree,int valor){
    int i;
    struct NO* atual = (*btree);
    struct NO* ant = (*btree);
    int metade = (M/2 + 0.5);
    int guarda;
    while((atual != NULL) && (atual->cont > metade)){ //acha o N� cheio
        ant = atual;
        for(i=0;i<atual->cont;i++){
            if(valor <= ant->info[i]){
                atual = ant->v[i];
                guarda = i;
                break;
            }
        }
        if(atual == ant){
            atual = ant->v[i];
            guarda = i;
        }
    }
    struct NO* lado = ant;
    lado = ant->v[guarda+1];
    struct NO* ladoesq = ant;
    ladoesq = ant->v[guarda-1];
    if(atual == NULL){
        return 1;
    }
    //caso que o visinho da direita pode emprestar
    if(ant->cont > guarda){
        if(lado->cont > metade-1 && lado != NULL){
            i = 0;
            if(atual->cont != 0)
                atual->info[atual->cont-1] = ant->info[guarda];
            else
                atual->info[0] = ant->info[guarda];
            atual->cont++;
            ant->info[guarda] = lado->info[0];
            for(i=0;i<lado->cont;i++){
                lado->info[i] = lado->info[i+1];
            }
            lado->cont--;
            return 1;
        }
    }
    if(guarda > 1){
        if(ladoesq->cont > metade-1 && ladoesq != NULL){
            atual->info[0] = ant->info[guarda-1];
            atual->cont++;
            ant->info[guarda-1] = ladoesq->info[ladoesq->cont-1];
            ladoesq->cont--;
            return 1;
        }
    }
    if(ant->cont > guarda){
        if(lado->cont <= metade && lado != NULL){
            atual->info[atual->cont] = ant->info[guarda];
            ant->cont--;
            atual->cont++;
            for(i=guarda;i<ant->cont;i++){
                ant->info[i] = ant->info[i+1];
            }
            for(i=0;i<lado->cont;i++){
                atual->info[atual->cont+i] = lado->info[i];
            }
            atual->cont = atual->cont + lado->cont;
            if(ant->cont != 0){
                for(i=guarda;i<=ant->cont;i++){
                    ant->v[i] = ant->v[i+1];
                }
            }
            return 1;
        }
    }
    if(ladoesq->cont <= metade && ladoesq != NULL){
        for(i=atual->cont;i>0;i--){
            atual->info[i] = atual->info[i-1];
        }
        atual->info[0] = ant->info[guarda-1];
        atual->cont++;
        for(i=guarda;i<ant->cont;i++){
            ant->info[i] = ant->info[i+1];
        }
        ant->cont--;
        for(i=0;i<atual->cont;i++){
            ladoesq->info[ladoesq->cont+i] = atual->info[i];
        }
        ladoesq->cont = atual->cont + ladoesq->cont;
        for(i=guarda;i<=ant->cont;i++){
            ant->v[i] = ant->v[i+1];
        }
        return 1;
    }

}
