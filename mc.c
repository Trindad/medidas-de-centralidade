/**
 * Calcular distância geodésica para obter a centralidade de intermediação
 * Como compilar : gcc  geodesica.c -o main -g -Wall
 *                 ./main < Caminho/nome_arquivo > arquivo_saida
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
 
//numero de vértices do grafo
// #define V 37
// #define V 24
// #define V 19
// #define V 16
// #define V 32
// #define V 17
// #define V 15
// #define V 31
// #define V 56
// #define V 14
// #define V 33
// #define V 19
// #define V 33
// #define V 15
// #define V 26
// #define V 19
// #define V 14
// #define V 38
// #define V 21
// #define V 26
// #define V 17
// #define V 27
// #define V 10
// #define V 25
// #define V 20
// #define V 100
// #define V 12
#define V 5
#define LIMIT 2000

typedef struct _caminho
{
  int vertices[V];
  int nVertices;

} Caminho;

typedef struct _vertice
{
  Caminho *caminhos;
  int nCaminhos;
  int origem;
  int grau;

} Vertice;

int verticesAdjacentes[V];
int verticesAux[V];
int **caminhoMinimo;
double sumGrau;


void centralidadeProximidade();
void centralidadeEficiencia();



/**
 * Distância minima 
 * @param  distancia []
 * @param  sptSet    []
 * @return           distancia minima
 */
int distanciaMinima(int *distancia, int *sptSet, int *vetor,int origem)
{

  int min = INT_MAX, min_index,v;

  int count = 0;
 
  for (v = 0; v < V; v++) {

    if (sptSet[v] == 0 && distancia[v] < min)
    {
      if (distancia[v] == 0 && v != origem)
      {
        distancia[v] = INT_MAX;
        continue;
      }
      else
      {
        min = distancia[v]; 
        min_index = v; 
        
      }
    } 
  }
    
  for (v = 0; v < V; v++) {
    
    if (sptSet[v] == 0 && distancia[v] <= min)
    {
      min = distancia[v];
      verticesAdjacentes[count] = v;
      count++;   
    } 
  }

  for (v = 0; v < count; v++)
  {
    vetor[v] = min_index;
  }

  return count;
}

/**
 * Imprime a distância minima de um vertice até n vértices
 * @param distancia minima de cada vertice, e o numero de vértices
 */
void imprimeSolucao(int *distancia, int n)
{
   int i;

   for (i = 0; i < V; i++) {

      printf("%d \t\t %d\n", i, distancia[i]);
   }
}

/**
 * Adiona os vertices do caminho na estrutura Vertice  
 */
void adicionaVertice(Vertice vertices[],int **caminho,int numVertices,int nCaminho,int origem,int destino) {

  int i = 0,j = 0,auxiliar = 0;

  for (i = 0; i < nCaminho; i++)
  {
   
    if (caminho[i][numVertices-1] > -1)
    {
       vertices[origem].nCaminhos++;
       vertices[origem].caminhos[vertices[origem].nCaminhos-1].nVertices = 0;
    }
    for (j = 0; j <= numVertices; j++)
    {
      if (caminho[i][numVertices-j] > -1)
      {
        
        int posicao = vertices[origem].nCaminhos;

        auxiliar = vertices[origem].caminhos[posicao-1].nVertices++;
        int temp = caminho[i][numVertices-j]+0;
        // printf("posicao %d\n",posicao );
        vertices[origem].caminhos[posicao-1].vertices[auxiliar] = temp;
      } 
    }
  }

  return;
}

/**
 * Adiciona todos os caminhos minimos distintos na matriz caminho 
 * @param grafo com uma matriz adjacente, e o source
 */
int adicionaCaminhos(Vertice vertices[V],int **caminho,int adjacente,int origem,int destino) {

  // printf("origem %d destino %d\n",origem,destino );
  int temp,i = 0,j = 0,k = vertices[origem].nCaminhos,n = 0,verticesCaminho = 0,minimo = 0 ;

  for(temp = 1; temp  < vertices[origem].nCaminhos; temp++)
  { 

    int count = vertices[origem].caminhos[k-temp].nVertices;
    int auxiliar = count-1;

    if (vertices[origem].caminhos[k-temp].vertices[auxiliar] == adjacente && k-temp >= 0)
    {
      while( temp < vertices[origem].nCaminhos && k-temp >= 0) {
        
        verticesCaminho = vertices[origem].caminhos[k-temp].nVertices;
        minimo = caminhoMinimo[origem][destino];

        // printf("nVertices %d minimo %d k-temp %d\n",verticesCaminho,minimo-1,k-temp);

        if (verticesCaminho < minimo-1)
        {
          break;
        }

        if ( vertices[origem].caminhos[k-temp].vertices[auxiliar] == adjacente && verticesCaminho == minimo-1)
        {
          caminho[i][j++] = destino;

          for (n = auxiliar; n >= 0; n--)
          {
            caminho[i][j++] = vertices[origem].caminhos[k-temp].vertices[n];
            
          }
          i++;
          j = 0;
        }
        temp++;
      }
      break;
    }
  }
  // printf("\n");
  return i;
}
void inserirCaminhos(Vertice vertices[],int origem,int destino,int adjacente) {

  int i = 0,j = 0;

  int **caminho = (int**) calloc (V,sizeof(int));

  if (caminho == NULL)
  {
    exit(1);
  }

  
  for (i = 0; i < V; i++)
  {
    caminho[i] = (int*) calloc (V,sizeof(int));

    if (caminho == NULL)
    {
      exit(1);
    }
    for ( j = 0; j < V; j++)
    {
      caminho[i][j] = -1;
    }
  }

  if (caminhoMinimo[origem][destino] > 2)
  {

    int path = adicionaCaminhos(vertices,caminho,adjacente,origem,destino);

    adicionaVertice(vertices,caminho,caminhoMinimo[origem][destino],path,origem,destino);
  }
  else
  {
    if (caminhoMinimo[origem][destino] == 2)
    {
      
      caminho[0][0] = destino+0;
      caminho[0][1] = adjacente+0;
      adicionaVertice(vertices,caminho,2,caminhoMinimo[origem][destino],origem,destino);
    }
    else
    {
      caminho[0][0] = destino+0;
      adicionaVertice(vertices,caminho,1,caminhoMinimo[origem][destino],origem,destino);   
    }
   
  }

  for (i = 0; i < V; i++)
  {
    free(caminho[i]);
  }
  free(caminho);

  return;
}
/**
 * Calcula o menor caminho, de um vertice até outro  
 * @param grafo com uma matriz adjacente, e o source
 */
void algoritmoDijkstra(int **grafo, int src,Vertice *vertices)
{

  int *distancia = (int*) malloc (sizeof(int)*V);


  int *sptSet = (int*) malloc (sizeof(int)*V);

  if (distancia == NULL || sptSet == NULL)
  {
    exit(1);
  }     

  int i = 0,count = 0,v = 0,k = 0;


  for (i = 0; i < V; i++) {

    distancia[i] = INT_MAX;
    sptSet[i] = 0;
  }
 
    int link[V][2];

    for (i = 0; i < V; i++)
    {
      link[i][0] = -1;
      link[i][1] = -1;
    }

    distancia[src] = 0;

    for (count = 0; count < V; count++)
    {
      
      vertices[src].origem = src;

      for (k = 0; k < V; k++)
      {
        verticesAdjacentes[k] = -1;
      }

      int u[V];

      int nMin = distanciaMinima(distancia,sptSet,u,src);
      
      for (k = 0; k < nMin; k++)
      {
        sptSet[u[k]] = 1;
      }
       
      int incremento = 0;
      int destinoAtual = verticesAdjacentes[0],destinoAnterior = verticesAdjacentes[0];
      
      for (k = 0; k < nMin; k++)
      {
        
        int aux = -1;

        for (v = 0; v < V; v++) {

          if (v == src)
          {
            continue;
          }
          
          if (!sptSet[v] && grafo[verticesAdjacentes[k]][v] && distancia[verticesAdjacentes[k]] != INT_MAX && distancia[verticesAdjacentes[k]]+grafo[verticesAdjacentes[k]][v] <= distancia[v] && destinoAtual != v)
          {
            destinoAtual = v;

            aux = distancia[v];

            int temporario = distancia[verticesAdjacentes[k]];

            distancia[v] =  temporario + grafo[verticesAdjacentes[k]][v];
            caminhoMinimo[src][v] = caminhoMinimo[v][src] = distancia[v];
            
            if (caminhoMinimo[src][v] > 0)
            {
              int t,verificador = 0;

              for (t = 0; t < V; t++)
              {
                if (link[t][0] == v && link[t][1] == verticesAdjacentes[k])
                {
                  verificador = 1;
                  break;
                }
              }
              if (verificador == 0)
              {
                
                inserirCaminhos(vertices,src,v,verticesAdjacentes[k]);

                link[incremento][0] = v;
                link[incremento][1] = verticesAdjacentes[k];

                incremento++;
              }       
            }
            verticesAux[v] = destinoAtual;
          }
        }
      
        if (destinoAtual != destinoAnterior)
        {
          distancia[destinoAtual] = aux;
          destinoAnterior = destinoAtual;
        }
      }

      for (i = 0; i < V; i++)
      {
        if (verticesAux[i] > -1)
        {
          distancia[verticesAux[i]] = caminhoMinimo[src][verticesAux[i]];
        }
      }
      
      for (v = 0; v < V; v++)
      {
        verticesAux[v] = verticesAdjacentes[v];
        verticesAdjacentes[v] = -1;
      }
  
    }
    free(distancia);
    free(sptSet);

    return;
}

/**
 *Encontra o numero de caminhos que contem o vértice de destino
 */

int encontraCaminhos(Vertice *vertices,int origem,int destino,int **caminhos) {


  int i = 0,k = 0,nCaminhos = 0;

  for (i = 0; i < V; i++)
  {
    caminhos[i] = (int*) calloc (V,sizeof(int));

    if (caminhos[i] == NULL)
    {
      exit(1);
    }
    for (k = 0; k < V; k++)
    {
      caminhos[i][k] = -1;
    }
  }

  for (i = 0; i < vertices[origem].nCaminhos; i++)
  {
    if (vertices[origem].caminhos[i].vertices[vertices[origem].caminhos[i].nVertices-1] == destino)
    {
      for (k = 0; k < vertices[origem].caminhos[i].nVertices; k++)
      {
        caminhos[nCaminhos][k] = vertices[origem].caminhos[i].vertices[k];
      }
      nCaminhos++;
    }
  }

  return nCaminhos;
}

/**
 *Busca vertice em um ou mais caminhos 
 *parametros de entrada: caminhos e o vertice a ser buscado e o numero de caminhos minimos a ser buscado
 *retorna o numero de vezes que o vertice foi encontrado
 */
int buscaVertice(int **caminho,int vertice,int nCaminhos) {

  int count = 0,i = 0,j = 0;

  for (i = 0; i < nCaminhos; i++)
  {
    for (j = 0; j < V-1; j++)
    {
      if (vertice == caminho[i][j] && caminho[i][j] > -1)
      {
        count++;
      }
    }
  }

  return count;
}

/**
 * Calcula a distância geodésica para cada nó do grafo
 * @param geodesica [V] e o vertice que esta sendo calculado a geodesica.
 * @return vetor com a geodesica 
 */
double calculaGeodesica(Vertice vertices[],int vertice) {

  int i = 0,j = 0,k = 0;
  double cb = 0.0;

  for ( i = 0; i < V-1; i++)
  {
    for ( j = i+1; j < V; j++)
    {
      if (j != vertice && i != j && i != vertice)
      {
        int **caminhos = (int**) malloc (sizeof(int*)*V);

        if (caminhos == NULL)
        {
          exit(1);
        }

        double nCaminhos = encontraCaminhos(vertices,i,j,caminhos);

        double nVertice = buscaVertice(caminhos,vertice,nCaminhos);

        // printf("u %d v %d nCaminho %f nVertice %f\n",i,j,nCaminhos,nVertice );
        cb = cb+(nVertice/nCaminhos);
        
        for (k = 0 ; k < V ; k++) free(caminhos[k]);
        free(caminhos);
      }
    }
  }
  printf("cb = %f\n",cb );
  return cb;
}

/**
 * Calcula a centralidade de intermediação do grafo
 * @param geodesica [V] -- vetor com as geodésicas de cada nó
 */
void centralidadeIntermediacao(Vertice vertices[]) {

  int v = 0,verticeCentral = 0;
  double cb = -1.0;
  double some = 0.0;
  double temporario = 0;

  for (v = 0;v < V;v++)
  {
    double temp =  calculaGeodesica(vertices,v);
    temporario = temporario + temp*vertices[v].grau;
    some = temp+some;
    if (cb < temp)
    {
      cb = temp;
      verticeCentral = v;
    }
  }
  double aux = 0.0;
  printf("centralidade  de intermediação indica o vertice v%d\n",verticeCentral );
  printf("%f %d\n",cb,verticeCentral );
  aux = some/V;
  printf("Media da centralidade de intermediação %f\n", aux);

  double ponderada = 0;


  ponderada = (temporario)/sumGrau;


  printf("Media da centralidade de intermediação ponderada = %f\n",ponderada);
  double aux1 = ((V*V)-3*V+2)/2;
  printf("%f < %f\n",aux,aux1 );
  return;
}

void grauVertices(int **A,Vertice *vertices) {

  sumGrau = 0;
  if (A == NULL || V < 1)
  {
    exit(1);
  }

  int i = 0,j,grau = 0;

  for (i = 0; i < V; i++)
  {
    grau = 0;

    for (j = 0; j < V; j++)
    {
      if (A[i][j] == 1)
      {
        // printf("AQUI\n");
        grau++;
      } 
    }
    vertices[i].grau = grau;
    sumGrau = sumGrau + grau;
    printf("grau(%d) = %d\n",i,grau );
  }
  double media = sumGrau/V;
  printf("Grau médio : %f\n",media ); 
}




int main()
{
  
  int n = 0, u = 0, v = 0,k = 0,i = 0, j = 0;

  scanf("%d\n", &n);

  int **grafo = (int**)malloc(sizeof(int*)*n);;

  if (grafo == NULL)
  {
    exit(1);
  }
  for (i = 0; i < V; i++) {

    grafo[i] = (int*) malloc (sizeof(int)*V);

    if (grafo[i] == NULL)
    {
      exit(1);
    }
    for (j = 0; j < V; j++) {
      grafo[i][j] = 0;
    }
  }
  
  while (!feof(stdin)) {

    scanf("%d %d\n", &u, &v);
    grafo[u-1][v-1] = 1;
    grafo[v-1][u-1] = 1;
  }


  /**
   * Inicializa matriz de caminhos minimos
   */
  Vertice vertices[V];

  for (i = 0; i < V; i++)
  {
    vertices[i].caminhos = (Caminho*) malloc (sizeof(Caminho)*LIMIT);

    if (vertices[i].caminhos == NULL)
    {
      exit(1);
    }
    vertices[i].grau = 0;
  }

  grauVertices(grafo,vertices);

  caminhoMinimo = (int**) malloc (sizeof(int*)*V);

  if (caminhoMinimo == NULL)
  {
    exit(1);
  }

  for (i = 0; i < V; i++)
  {
    caminhoMinimo[i] = (int*) malloc (sizeof(int)*V);

    if (caminhoMinimo[i] == NULL)
    {
      exit(1);
    }

    for (j = 0; j < V; j++)
    {
      caminhoMinimo[i][j] = 0;
    }
    vertices[i].nCaminhos = 0;
    vertices[i].origem = i;
  }

  /**
  * Obtem caminhos minimos do grafo
  */
 

  for (i = 0; i < V; i++)
  {
    // printf("\n\nVertice %d\n",i );
    algoritmoDijkstra(grafo,i,vertices);
  }

  for ( k = 0; k < V; k++)
  {
    printf("Vertice de origem %d nCaminhos %d\n",k,vertices[k].nCaminhos );
    for (i = 0; i < vertices[k].nCaminhos; i++)
    {
      for (j = 0; j < vertices[k].caminhos[i].nVertices; j++)
      {
        printf(" %d ",vertices[k].caminhos[i].vertices[j]);
      }
      printf("\n");
    }

  }
  
  
  printf("\nCentralidade de Intermediação\n");
  //centralidadeIntermediacao(vertices);

  printf("\nCentralidade de Eficiência\n");
  //centralidadeEficiencia();

  printf("\nCentralidade de Proximidade\n");
  centralidadeProximidade();

  
  for (i = 0 ; i < V ; i++) free(grafo[i]);
  free(grafo);
  
  for (i = 0 ; i < V ; i++) free(caminhoMinimo[i]);
  free(caminhoMinimo);

  for (i = 0 ; i < V ; i++) free(vertices[i].caminhos);

  return 0;
}



void centralidadeProximidade(){
  int distancias[V], i, j, min= 999;
  
  for(i=0;i<V;i++){
    for(j=0;j<V;j++){
      printf("%d ", caminhoMinimo[i][j]);
    }
    printf("\n");
  }

  for(i=0;i<V;i++){
    //seta todos os caminhos para o "maior" valor possivel
    distancias[i] = 0;
    for(j=0;j<V;j++){
      if(i==j) continue;

      if(caminhoMinimo[i][j] > distancias[i]){
        distancias[i] = caminhoMinimo[i][j];
        printf("Dist: %d, i: %d\n",distancias[i], i);
      }
    }
  }
printf("min: %d",min); 
  for(i=0;i<V;i++){
    if(distancias[i] < min){
      min = distancias[i];
    }
  }
  printf("\nA Centralidade de Proximidade da rede é: %d, e o(s) vértice(s) é(são):\n",min);
  for(i=0;i<V;i++){
    if(distancias[i] == min){
      printf("%d\n",i);
    }
  }

}

void centralidadeEficiencia(){

  int v, src;
  int saltos[V];

  for (v = 0; v < V; v++) {
    int maiorMenor = 0;
    for (src = 0; src < V; src++){
      if(v != src){
        if(caminhoMinimo[v][src] > maiorMenor){
          saltos[v] = caminhoMinimo[v][src];
        }
      } 
    }
  }

  int max=0, i;

  for (i = 0; i < V; i++) {
    if(saltos[i] > max){
      max = saltos[i];
    }
  }
 
  printf("\nCentralidade de Eficiencia da rede: %f \n\n Nó(s) central(is) pela medida de eficiencia :\n",((float)1/max));
  
  for (i = 0; i < V; i++){
    if(saltos[i] == max){
      printf("%d\n",saltos[i]);
    }

  }
  
}
