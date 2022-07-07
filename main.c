#include <stdio.h>
#include <stdlib.h>

//PROCURE A PALAVRA "TODO" (de TO DO) PARA VER O QUE PRECISA MUDAR
//NAO ALTERE OS PONTOS SORTEADOS PARA OS BARCOS E TIROS
//Procure mexer o minimo possivel na main.
//basicamente voce precisa implementar as funcoes
//"adiciona_ponto" e "busca_ponto"
typedef struct ponto{
    int x;
    int y;
} Ponto;

typedef struct sno{
  Ponto p;
  int altura;
  struct sno * esq;
  struct sno * dir;
}no;

no * criar_no(int a, int b){
  no * novo = (no*) malloc(sizeof(no));
  novo->p.x=a;
  novo->p.y=b;
  novo->esq = NULL;
  novo->dir = NULL;
  novo->altura = 0;
  return novo;
}

int compara(Ponto p,int a, int b ){
    if(p.x > a || (p.x == a && p.y > b))
        return -1;
    else
        return 0;
}

int max(int a, int b){
  if(a > b) return a;
  return b;
}

int altura(no* oceano){
  if(oceano == NULL) return -1;
  return oceano->altura;
}

int fator_balanceamento(no * oceano){
  return altura(oceano->esq) - altura(oceano->dir);
}

no * rotaciona_direita(no * oceano){
  no * temp = oceano->esq;
  oceano->esq = temp->dir;
  temp->dir = oceano;
  
  oceano->altura = max(altura(oceano->esq), altura(oceano->dir)) + 1;
  temp->altura = max(altura(temp->esq), altura(temp->dir)) + 1;
  return temp;
}


no * rotaciona_esquerda(no * oceano){
  no * temp = oceano->dir;
  oceano->dir = temp->esq;
  temp->esq = oceano;
  
  oceano->altura = max(altura(oceano->esq), altura(oceano->dir)) + 1;
  temp->altura = max(altura(temp->esq), altura(temp->dir)) + 1;
  return temp;
}

no * rotaciona_esquerda_direita(no * oceano){
  oceano->esq = rotaciona_esquerda(oceano->esq);
  return rotaciona_direita(oceano);
}

no * rotaciona_direita_esquerda(no * oceano){
  oceano->dir = rotaciona_direita(oceano->dir);
  return rotaciona_esquerda(oceano);
}

no * adiciona_ponto(no * oceano, int a, int b){
  if(oceano == NULL) return criar_no(a,b);
  
  int comp =compara(oceano->p,a,b);
  
  if(comp==0){
    //inserir na direita
    oceano->dir = adiciona_ponto(oceano->dir, a,b);
    if(fator_balanceamento(oceano) == -2){
      //tive problemas
      if(compara(oceano->dir->p,a,b)==0) oceano = rotaciona_esquerda(oceano);
      else oceano = rotaciona_direita_esquerda(oceano);
    }
  }else if(comp==-1){
    //insere na esquerda
    oceano->esq = adiciona_ponto(oceano->esq, a,b);
    if(fator_balanceamento(oceano) == 2){
      //tive problemas
      if(compara(oceano->esq->p,a,b)==-1) oceano = rotaciona_direita(oceano);
      else oceano = rotaciona_esquerda_direita(oceano);
    }
  }
  
  oceano->altura = max(altura(oceano->esq), altura(oceano->dir)) + 1;
  //tratar casos iguais
  return oceano;
}

int busca_ponto (no * oceano , int a, int b) {
    if (oceano == NULL){ return 0;}
    else{
    if (oceano->p.x == a && oceano->p.y==b)
       return 1;
    int comp = compara(oceano->p, a,b);
    if (comp==-1)
       return busca_ponto (oceano->esq, a,b);
    else
       return busca_ponto (oceano->dir, a,b);
    }
}

no * libera_oceano(no * oceano) {
    if (oceano != NULL) {
        libera_oceano(oceano->esq); /* libera sae */
        libera_oceano(oceano->dir); /* libera sad */
        free(oceano); /* libera raiz */
    }
    return NULL;
}
int main(int argc, char * argv[]){
  int dimensao;
  int num_pontos;
  int num_tiros;
  
  //TODO: GUARDAR OS OCEANOS DE A E B, MUDE PARA A ESTRUTURA
  //DE DADOS QUE VOCE QUISER
  no * pontosJA = NULL;
  no * pontosJB = NULL;
  
  
  scanf("%d %d %d", &dimensao, &num_pontos, &num_tiros);
  printf("Campo %d x %d\nCada jogador coloca %d barcos\n\n", dimensao, dimensao, num_pontos);
  
  //Definindo uma semente para gerar os mesmos pontos
  srand(dimensao);

  //Jogador A gera pontos para colocar os barcos
  printf("Jogador A coloca barcos:\n");
  for(int i = 0; i < num_pontos; i++){
    int x, y;
    //sorteia um ponto que ja nao tenha um barco
    do{
      x = rand() % dimensao;
      y = rand() % dimensao;
    }while(busca_ponto(pontosJA, x, y) != 0); //TODO: Procura no oceano A se x, y ja tem barco
  
    if(i<5) printf("(%d, %d)\n", x, y);
    
    pontosJA = adiciona_ponto(pontosJA, x, y); //TODO: Adiciona x, y no oceano A
  }
  
  
  //Jogador 2 gera pontos, com a estrategia (duvidosa) de colocar os pontos 
  //em mais ou menos em ordem crescente.
  printf("...\n\nJogador B coloca barcos:\n");
  for(int i = 0; i < num_pontos; i++){
    int x, y;
    //sorteia um ponto que ja nao tenha um barco
    do{
      x = i % dimensao;
      y = rand() % dimensao;
    }while(busca_ponto(pontosJB, x, y) != 0); //TODO: Procura no oceano B se x, y ja tem barco
      
    if(i<5) printf("(%d, %d)\n", x, y);
    
    pontosJB = adiciona_ponto(pontosJB, x, y); //TODO: Adiciona x, y no oceano B
  }
  
  printf("...\n\nCada jogador vai dar %d tiros\n", num_tiros);
  
  
  //Jogador 1 ataca pontos (aleatorios)
  int acertosJA = 0;
  printf("\nJogador A atira:\n");
  for(int i = 0; i < num_tiros; i++){
    int x = rand() % dimensao;
    int y = rand() % dimensao;
    if(i<5) printf("(%d, %d)\n", x, y);
    
    //Jogador A ataca os pontos de B
    if(busca_ponto(pontosJB, x, y) == 1) acertosJA++; //TODO: Procura no oceano B se x, y tem barco
  }
  
  //Jogador B ataca pontos (aleatorios)
  int acertosJB = 0;
  printf("...\n\nJogador B atira:\n");
  for(int i = 0; i < num_tiros; i++){
    int x = rand() % dimensao;
    int y = rand() % dimensao;
    if(i<5) printf("(%d, %d)\n", x, y);

    //Jogador B ataca os pontos de A
    if(busca_ponto(pontosJA, x, y) == 1) acertosJB++;  //TODO: Procura no oceano A se x, y ja tem barco
  }
  
  printf("...\n\nResultado: Jogador A acertou %d e Jogador B %d\n", acertosJA, acertosJB);
  
  libera_oceano(pontosJA); libera_oceano(pontosJB); //TODO: Liberar memorias
  return 0;
}

