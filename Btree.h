typedef struct NO* Btree;
Btree* cria_Btree();

int insere_Btree(Btree* btree,int valor);
void preOrdem_Btree(Btree *btree);
int busca_Btree(Btree *btree, int valor);
int insere_NO_Btree(Btree* btree,int valor);
int balanceamento_Btree(Btree* btree,int valor);
struct NO* divisao_raiz_Btree(Btree *btree);
struct NO* divisao_NOfilho_Btree(Btree *btree);
int balanceamento_remocao_Btree(Btree* btree,int valor);
int remocao_folha1_Btree(Btree* btree,int valor);
int remocao_interno_Btree(Btree* btree,int valor);
int remove_Btree(Btree* btree,int valor);
