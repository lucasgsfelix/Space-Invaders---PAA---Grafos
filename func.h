#ifndef func_h
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class vertices{
	/*
		Classe vertices:
			Nessa classe são armazenados os vértices.
	*/
	public:
		vector <int> adj; // essa variável vai guardar a lista de adjacência do vértice
		int id; //identificador do vértice
		int flag = 0; // vai representar a cor no dfs e no bfs
		int tempo_d = -1; // dfs
		int tempo_f = -1; // dfs
		int componente = 0; // componente do vértice
		int cor = -1; // parâmetro para ajudar identificar se o grafo é bipartido
		int distancia_pai = -1; // se for menos um, sei que o vértice não é atingível pelo root
		int pai = -1; // id do pai do vértice
};

class grafo{
	/*
		Classe grafo:
			Responsável por definir a estrutura da classe grafo, e de como o grafo
			irá se comportar.
	*/
	public:
		vector <vertices> v; // guarda a os vértices
		vector <int> id_vertices; //guarda o id dos vértices
		int num_componentes; // a quantidade de componentes presentes no grafo
		vector <int> vertices_visitados;
};
class componente{
	public:
		int componente = 0;
		int min_degree = 0;
		int max_degree = 0;
		int quant_um = 0;
		int quant_vertices = 0;
		int quant_arestas = 0;
		int root = 0;
		int grau_medio = 0;
};
void leitura_arquivo(const char* nome_arquivo, vector <int> *buffer);
void imprime_lista_adj(grafo g);
grafo nao_orientado(vector <int> buffer, int n, int m, int *i);
grafo orientado(vector <int> buffer, int n, int *i);
void dfs_visit(grafo *g, int i, int *tempo, componente *c);
vector <componente> dfs(grafo *g);
int primeiro_tipo(componente *c);
int segundo_tipo(componente *c);
int quarto_tipo(componente *c);
int dfs_bipartido(grafo *g, int root, int cor, int *quant_zeros, int *quant_um, int *grau_medio);
void bfs(grafo *g, int root);
int terceiro_tipo(grafo *g, componente *c);
void identifica_naves(grafo *g, int *t1, int *t2, int *t3, int *t4);
void inicializa_vertices(grafo *g);
int calcula_tempo_vantagem(grafo *g, grafo *g_orien);
#endif
