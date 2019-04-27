#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>

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
};

void leitura_arquivo(const char* nome_arquivo, vector <int> *buffer)
{
	ifstream file;
	file.open(nome_arquivo);
	if(!file){
		cerr << "Não foi possível abrir o arquivo!";
		exit(1);
	}
	int valor;
	while(file>>valor)
	{
		buffer->push_back(valor);
	}
	file.close();
}
void imprime_lista_adj(grafo g)
{
	/*

		Responsável pela impressão do grafo.

		Parâmetros:
			grafo g;

	*/
	for(int i=0;i<g.v.size();i++)
	{
		cout << "ID: " << g.v[i].id << " ADJ :" "\n";
		for(int j=0;j<g.v[i].adj.size();j++)
		{
			cout << g.v[i].adj[j] << " ";
		}
		cout << "\n";
	}
	if(g.v.size()==0)
	{
		cout << "GRAFO VAZIO ! \n";
	}
}
grafo nao_orientado(vector <int> buffer, int m, int *i)
{
	grafo g;
	g.v.resize(m);
	int aux = 0;
	for(int k=0;k<g.v.size();k++)
	{
		g.v[k].id = k+1; // em 0 está 1
	}
	while(aux<m-1)
	{
		g.v[buffer[*i]-1].adj.push_back(buffer[*i+1]);
		g.v[buffer[*i+1]-1].adj.push_back(buffer[*i]);
		*i=*i+2;
		aux++;
	}
	return g;

}
grafo orientado(vector <int> buffer, int n, int *i)
{
	grafo g;
	g.v.resize(n);
	for(int k=0;k<n;k++)
	{
		g.v[k].id = k+1;
	}
	while(*i<buffer.size())
	{
		g.v[buffer[*i]-1].adj.push_back(buffer[*i+1]);
		*i=*i+2;
	}
	return g;
}
grafo dfs_visit(grafo g, int i, int *tempo, int *quant_vertices){
	/*
		Está função é responsável por realizar a operação principal do método de DFS.
		Está é uma função recursiva que irá analisar cada um dos vértices, assim como
		seus vizinhos.

		Parâmetros:
			grafo g;
			int i; que representa o index do vértice inicial (root)
			int *tempo; que é um ponteiro para o tempo calculado para cada um dos
			vértices por meio do dfs
	*/
	g.v[i].flag = 1; // cor cinza
	*tempo = *tempo + 1;
	g.v[i].tempo_d = *tempo;
	int index;
	for(int k=0;k<g.v[i].adj.size();k++){
		index = g.v[i].adj[k] - 1;
		if(g.v[index].flag == 0){ // cor branca
			g.v[index].componente = g.v[i].componente; // recebe o componente do pai
			*quant_vertices = *quant_vertices + 1;
			g = dfs_visit(g, index, tempo, quant_vertices);
		}
	}
	g.v[i].flag = 2; // cor preto
	*tempo = *tempo + 1;
	g.v[i].tempo_f = *tempo;
	return g;
}
grafo dfs(grafo g){ // 0 false 1 true

	/*
		Método responsável por realizar fazer o método de busca em profundidade.
		A implementação foi realizada com base no código do livro Algoritmos do Cormen.

		Parâmetros:
			grafo g;
	*/

	int tempo = 0, componentes = 0, quant_vertices = 0;
	for(int i=0;i<g.v.size();i++)
	{
		quant_vertices = 1;
		if (g.v[i].flag == 0 && g.v[i].adj.size()>0) // quer dizer que a cor é branca
		{ 	
			g.v[i].componente = componentes + 1;
			g = dfs_visit(g, i, &tempo, &quant_vertices);
			cout << quant_vertices << "\n";
			if(quant_vertices >= 5)
			{
				componentes++;
			}
		}
	}
	cout << componentes << "\n";
	g.num_componentes = componentes;
	return g;
}
void identifica_naves(grafo g, grafo g_orien)
{
	g = dfs(g);
	cout <<"Número de componentes "<< g.num_componentes << "\n";

}
int main()
{
	int inicio = clock();
	vector <int> buffer;
	leitura_arquivo("1.in", &buffer);
	int n = buffer[0]; // n = de onde para onde deve ser feito o teleporte 
	int m = buffer[1]; // m = quantidade de teleportes
	int i=2; // começa de 2 pois já foram avaliados os dois primeiros vértices
	grafo g = nao_orientado(buffer, n, &i);
	grafo g_orien = orientado(buffer, n, &i);
	buffer.clear();
	cout << (clock() - inicio)/CLOCKS_PER_SEC << "\n";
	identifica_naves(g, g_orien);

	cout << (clock() - inicio)/CLOCKS_PER_SEC << "\n";

}