#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <conio.h>
#include <windows.h>

typedef struct no {
  int v;
  int bal;
  struct no *esq, *dir;
} No;

int altura(No* t) {
  if (t == NULL)
    return 0;
  int hesq = altura(t->esq);  
  int hdir = altura(t->dir);
  return hesq > hdir ? hesq + 1 : hdir + 1;
}

No* cria(int v, No* esq, No* dir) {
  No* n = (No*) malloc (sizeof(No));
  n->v = v;
  n->bal = altura(dir) - altura(esq);
  n->esq = esq;
  n->dir = dir;
  return n;
}

int testarAVL(No* t) {
  if (t == NULL)
    return 1;
  return abs(altura(t->dir) - altura(t->esq)) <= 1;
}

No* arv_buscar (No* r, int v){
 if (r == NULL)
    return NULL;
 else if (r->v > v)
    return arv_buscar (r->esq, v);
 else if (r->v < v)
    return arv_buscar (r->dir, v);
 else return r;
}

void exibir(No* r) {
  if (r != NULL) {
    exibir(r->esq);
    printf("%d(%d)", r->v, r->bal);
    exibir(r->dir);
  }
}

void exibirAVL(No* t) { 
  if (!testarAVL(t)) 
    printf("Não possui registro.\n");
  exibir(t); 
  printf("\n");
}

void LL(No** r) {
  No* b = *r;
  No* a = b->esq;
  b->esq = a->dir;
  a->dir = b;
  a->bal = 0;
  b->bal = 0;
  *r = a;
}

void RR(No** r) {
  No* a = *r;
  No* b = a->dir;
  a->dir = b->esq;
  b->esq = a;
  a->bal = 0;
  b->bal = 0;
  *r = b;
}

void LR(No** r) {
  No *c = *r;
  No *a = c->esq;
  No *b = a->dir;
  c->esq = b->dir;
  a->dir = b->esq;
  b->esq = a;
  b->dir = c;
  switch(b->bal) {
  case -1:
    a->bal = 0;
    c->bal = 1;
    break;
  case 0:
    a->bal = 0;
    c->bal = 0;
    break;
  case +1:
    a->bal = -1;
    c->bal = 0;
    break;
  }
  b->bal = 0;
  *r = b;
}

void RL(No** r) {
  No *a = *r;
  No *c = a->dir;
  No *b = c->esq;
  c->esq = b->dir;
  a->dir = b->esq;
  b->esq = a;
  b->dir = c;
  switch(b->bal) {
  case -1:
    a->bal = 0;
    c->bal = 1;
    break;
  case 0:
    a->bal = 0;
    c->bal = 0;
    break;
  case +1:
    a->bal = -1;
    c->bal = 0;
    break;
  }
  b->bal = 0;
  *r = b;  
}

int aux_insere(No** t, int v, int *cresceu) {
  if (*t == NULL) {
    *t = cria(v, NULL, NULL);
    *cresceu = 1;
    return 1;
  }
  if (v == (*t)->v) 
    return 0;
  if (v < (*t)->v) {
    if (aux_insere(&(*t)->esq, v, cresceu)) {
      if (*cresceu) {
	switch ((*t)->bal) {
	case -1:
	  if ((*t)->esq->bal == -1)
	    LL(t);
	  else
	    LR(t);
	  *cresceu = 0;
	  break;
	case 0:
	  (*t)->bal = -1;
	  *cresceu = 1;
	  break;
	case +1:
	  (*t)->bal = 0;
	  *cresceu = 0;
	  break;
	}
      }
      return 1;
    }
    else
      return 0;
  }
  if (aux_insere(&(*t)->dir, v, cresceu)) {
      if (*cresceu) {
	switch ((*t)->bal) {
	case -1:
	  (*t)->bal = 0;
	  *cresceu = 0;
	  break;
	case 0:
	  (*t)->bal = +1;
	  *cresceu = 1;
	  break;
	case +1:
	  if ((*t)->dir->bal == +1)
	    RR(t);
	  else
	    RL(t);
	  *cresceu = 0;
	  break;
	}
      }
      return 1;
    }
    else
      return 0;
}

int insere(No **t, int v) {
  int cresceu;
  return aux_insere(t, v, &cresceu);
}

int arv_vazia (No* a){
 return a==NULL;
}

No* arv_libera (No* a) {
 if (!arv_vazia(a)){
    arv_libera(a->esq);
    arv_libera(a->dir);
    free(a);
 }
 return NULL;
}

void escreverArquivo(int x) {
 FILE *arq;	
 int i = 0;
 for(i=1;i<=x;i++){	
    arq = fopen("relatorio.txt","a");
    int cpf = rand() % 1000000000;
    fprintf(arq,"Número= %i ",i);
    fprintf(arq,"CPF= %i \n",cpf);
    fclose(arq);
 }
}

void abrirArquivo(int numero){
   FILE *arq = fopen("relatorio.txt", "r");
   char caractere;
   int linha=1;
	while((caractere = fgetc(arq)) != EOF){
	if(linha==numero && caractere!='\n'){
		printf("%c",caractere);
	}
	if(caractere=='\n'){     		
		linha++;             
	}
}
if(linha<numero){
	printf("Sem registros no arquivo.");
}
fclose(arq);
}

void menu(){
int numero , i;
int opcao;
clock_t c2, c1; 
float tempo1, tempo2;
No* t = NULL;
No* aux = NULL;
while (opcao!=6) {
	system("cls");
	printf("1 - Inserir número árvore.\n");
	printf("2 - Inserir 8.000 números\n");
	printf("3 - Exibir árvore balanceada.\n");
	printf("4 - Buscar.\n");
	printf("5 - Liberar árvore e limpar registros.\n");
	printf("6 - Sair\n");
	scanf("%i", &opcao);
	system("cls");
		switch (opcao){
		case 1: {
			printf("Inserir numero.\n");
			scanf("%d",&numero);
			insere(&t,numero);
			break;
		}
		case 2: {
			int x = 8000;
			tempo1 = 0;
			c1 = clock();
			for(i=1;i<=x;i++){
				insere(&t,i);
			}
			c2 = clock();
			tempo1 = (c2 - c1)*CLOCKS_PER_SEC;
			printf("\n Tempo cadastro ARVORE: %f\n\n",tempo1);
			tempo1 = 0;
			c1 = clock();
			escreverArquivo(x);
			c2 = clock();
			tempo2 = (c2 - c1)*CLOCKS_PER_SEC;
			printf("\n Tempo cadastro ARQUIVO: %f\n",tempo2);
			getch();
			break;
		}
		case 3: {
			exibirAVL(t);
			getch();
			break;
		}
			case 4: {
			printf("Digite o numero que deseja buscar.");
			scanf("%d",&numero);
			tempo1 = 0;
			c1 = clock();
			aux = arv_buscar(t,numero);
			if(aux!= NULL){
				printf("Número= %d ",aux->v);
				printf("Indice= %d",aux->bal);
			}
			if(aux==NULL){
				printf("Sem registro.");
			}
			c2 = clock();
			tempo1 = (c2 - c1)*CLOCKS_PER_SEC;
			printf("\n Tempo de busca na Árvore: %f\n\n",tempo1);
			
			tempo2 = 0;
			c1 = clock();
			abrirArquivo(numero);
			c2 = clock();
			tempo2 = (c2 - c1)*CLOCKS_PER_SEC;
			printf("\n Tempo de busca no arquivo: %f",tempo2);
			getch();
			break;
		}
		case 5: {
			t = arv_libera(t);
			remove("relatorio.txt");
			printf("Apagando...");
			Sleep(1000);
			system("cls");
			printf("Árvore e registros limpos.");
			getch();
			break;
		}
		case 6:{
			break;
		}
		default: {
			printf("Opção Inválida!\n");
			getch();
				}
		}
}
system("pause");
}

int main() {
	setlocale(LC_ALL, "Portuguese");
	menu();
return 0;
}

