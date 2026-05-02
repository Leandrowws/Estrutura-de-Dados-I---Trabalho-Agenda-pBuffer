#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_PESSOA (sizeof(char) * 50 + sizeof(int) + sizeof(char) * 50)
#define TAM_BASE   (sizeof(char) * 50 + sizeof(int) * 3) 

void *AdicionarPessoa( void *pBuffer ) {

    ((int *)pBuffer)[1]++;
    void *pBufferRealocado = realloc(pBuffer, TAM_BASE + (((int *)pBuffer)[1] * TAM_PESSOA));

    if ( pBufferRealocado == NULL ) {
        printf("Erro ao alocar memoria!\n");
        return pBuffer;
    }

    char *nome = (char *)pBufferRealocado + TAM_BASE + ((((int *)pBufferRealocado)[1] - 1) * TAM_PESSOA);
    printf("Digite o seu nome: ");
    fgets(nome,50,stdin);

    int *idade = (int *)((char *)pBufferRealocado + TAM_BASE + ((((int *)pBufferRealocado)[1] - 1) * TAM_PESSOA + sizeof(char) * 50));
    printf("Digite a sua idade: ");
    scanf(" %d", idade);
    getchar();

    char *email = (char *)pBufferRealocado + TAM_BASE + ((((int *)pBufferRealocado)[1] - 1) * TAM_PESSOA + sizeof(char) * 50 + sizeof(int));
    printf("Digite o seu email: ");
    fgets(email,50,stdin);
 
    return pBufferRealocado;
}

void ListarTodos(void *pBuffer) {

     ((int *)pBuffer)[2] = 0;

     if ( ((int *)pBuffer)[1] == 0 ) {
        printf("\nAgenda vazia!\n");
        return;
     }

     while( ((int *)pBuffer)[2] < ((int *)pBuffer)[1] ) {

        char *pessoa = (char *)pBuffer + TAM_BASE + (((int *)pBuffer)[2] * TAM_PESSOA);
        
        char *nome =  (char *)pessoa;
        int  *idade = (int *)(pessoa + sizeof(char)*50);
        char *email = (char *)pessoa + sizeof(char)*50 + sizeof(int);

        printf("\nPessoa %d --------------------------\n", ((int *)pBuffer)[2] + 1);
        printf("Nome: %s", nome);
        printf("Idade: %d\n", *idade);
        printf("Email: %s", email);

        ((int *)pBuffer)[2]++;
     }
}

void *BuscarPessoa(void *pBuffer) {

     ((int *)pBuffer)[2] = 0;
    
     if ( ((int *)pBuffer)[1] == 0 ) {
        printf("\nAgenda vazia!\n");
        return NULL;
     }

    printf("\nDigite o nome a ser buscado: ");
    char *busca = (char *)pBuffer + (sizeof(int)*3);
    fgets(busca,50,stdin);

     while ( ((int *)pBuffer)[2] < ((int *)pBuffer)[1]) {

        char *pessoa = (char *)pBuffer + TAM_BASE + (((int *)pBuffer)[2] * TAM_PESSOA);
        
        char *nome = (char *)pessoa;

        if ( strcmp(busca,nome)==0 ) {

            int  *idade = (int *)(pessoa + sizeof(char)*50);
            char *email = (char *)pessoa + sizeof(char)*50 + sizeof(int);

        printf("\nPessoa %d --------------------------\n", ((int *)pBuffer)[2] + 1);
        
        printf("Nome: %s", nome);
        printf("Idade: %d\n", *idade);
        printf("Email: %s", email);
        printf("--------------------------------------\n");
        return pessoa;
        }
        
        ((int *)pBuffer)[2]++;
     }
     printf("\nEssa pessoa nao foi encontrada!\n");
     return NULL;
}

void *RemoverPessoa(void *pBuffer) {

    void *remover = BuscarPessoa(pBuffer);

    if ( remover == NULL ) {
        return pBuffer;
    }

    ((int *)pBuffer)[1]--;

    if( ((int *)pBuffer)[2] == ((int *)pBuffer)[1]) {

        void *pBufferRealocado = realloc(pBuffer, TAM_BASE + (((int *)pBuffer)[1] * TAM_PESSOA));

        if ( pBufferRealocado == NULL ) {
            return pBuffer;
        }

        
        printf("A PESSOA ACIMA FOI REMOVIDA COM SUCESSO!\n");
        return pBufferRealocado;

    } else {

        void *destino = (char *)pBuffer + TAM_BASE + ((int *)pBuffer)[2] * TAM_PESSOA; 
        void *origem =  (char *)destino + TAM_PESSOA;

        memmove(destino, origem, (((int *)pBuffer)[1] - ((int *)pBuffer)[2]) * TAM_PESSOA);

        void *pBufferRealocado = realloc(pBuffer, TAM_BASE + (((int *)pBuffer)[1] * TAM_PESSOA));
        if ( pBufferRealocado == NULL ) {
            return pBuffer;
        }
        
        printf("A PESSOA ACIMA FOI REMOVIDA COM SUCESSO!\n");
        return pBufferRealocado;
    }
}

int main() {
     
    void *pBuffer = malloc( sizeof(int) * 3 + sizeof(char) * 50 );

    ((int *)pBuffer)[0] = 0;    //seletor de operação
    ((int *)pBuffer)[1] = 0;    //quantidade de pessoas
    ((int *)pBuffer)[2] = 0;    //usado para criar laços
                                //os proximos 50 bytes eu guardo para poder ter algum lugar para trabalhar com a funcao de busca/remover

    while( ((int *)pBuffer)[0] != 5 ) {

        printf("\n1 - Adicionar Pessoa (Nome, Idade, Email)\n2 - Remover Pessoa\n3 - Buscar Pessoa\n4 - Listar Todos\n5 - Sair\n\n");
        scanf("%d", &((int *)pBuffer)[0]);
        getchar();

        switch( ((int *)pBuffer)[0] ) {

            case 1:
                pBuffer = AdicionarPessoa(pBuffer);
                break;
            case 2:
                pBuffer = RemoverPessoa(pBuffer);
                break;
            case 3:
                BuscarPessoa(pBuffer);
                break;
            case 4:
                ListarTodos(pBuffer);
                break;
            case 5:
                printf("\nSaindo...");
                break;

        }
    }

 free(pBuffer);
 return 0;

}
