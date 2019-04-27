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
		vector <int> adj_index; // vai adicionar o index na lista de vértices
		int id; //identificador do vértice
		int flag = 0; // vai representar a cor no dfs e no bfs
		int tempo_d = -1; // dfs
		int tempo_f = -1; // dfs
		int componente = 0; // componente do vértice
		int cor = -1; // parâmetro para ajudar identificar se o grafo é bipartido
		int distancia_pai = -1; // se for menos um, sei que o vértice não é atingível pelo root
		int pai = -1; // id do pai do vértice
		int index = 0; // index do vértice
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
int verifica_valor(grafo *g, int valor){
	/*
		Método responsável por verificar se um vértice u, está presente
		no grafo.

		Parâmetros:
			grafo g
			int valor; é o id do valor que deve ser verificado se está na lista

	*/
	int *pos = g->id_vertices.data();
	for(int i=g->id_vertices.size();i>0;i--){
		if(*pos == valor)
		{
			return i;
		}
		*pos++;
	}
	return -1;
}
void adiciona_vertice(grafo *g, int valor, int *k)
{	
	g->id_vertices.push_back(valor);
	g->v[*k].id = valor;
	*k=*k+1;

}
void adiciona_lista_adj(grafo *g, int valor, int index, int index_aux)
{
	/*
		valor - quem eu vou adicionar na lista de adjacência
		index - o index onde serão armazenados os valores
		index_aux - index de quem eu vou adicionar
	*/
	g->v[index].adj.push_back(valor);
	g->v[index].adj_index.push_back(index_aux);
}
grafo montagem_grafo_n_orientado(vector <int> buffer, int m, int *i)
{
	grafo g;
	g.v.resize(m);
	int index = 0, index_aux = 0, k=0, aux=0;
	while(aux<m)
	{
		index = verifica_valor(&g, buffer[*i]);
		index_aux = verifica_valor(&g, buffer[*i+1]);
		if(index == -1 && index_aux==-1)
		{ // caso nenhum dos dois vértices tenham sido alocados
			adiciona_vertice(&g, buffer[*i], &k);
			if(buffer[*i] != buffer[*i+1])
			{
				adiciona_vertice(&g, buffer[*i+1], &k);
				adiciona_lista_adj(&g, buffer[*i], k-2, k-1);
				adiciona_lista_adj(&g, buffer[*i+1], k-1, k-2);
				aux=aux+2;
			}
			else
			{
				adiciona_lista_adj(&g, buffer[*i], k-1, k-1);
				aux++;
			}
		}
		else if(index == -1)
		{ // caso apenas um não tenha sido alocado
			adiciona_vertice(&g, buffer[*i], &k);
			adiciona_lista_adj(&g, buffer[*i+1], k-1, index_aux);
			adiciona_lista_adj(&g, buffer[*i], index_aux, k-1);
			aux++;
		}
		else if(index_aux == -1)
		{ // caso apenas um não tenha sido alocado
			adiciona_vertice(&g, buffer[*i+1], &k);
			adiciona_lista_adj(&g, buffer[*i+1], index, k-1);
			adiciona_lista_adj(&g, buffer[*i], k-1, index);
			aux++;
		}
		else
		{ // caso os dois tenham sido alocados
			adiciona_lista_adj(&g, buffer[*i], index_aux, index);
			adiciona_lista_adj(&g, buffer[*i+1], index, index_aux);
		}
		*i=*i+2;
	}
	return g;
}
grafo montagem_grafo_orientado(vector <int> buffer, int n, int *i)
{
	grafo g;
	g.v.resize(n);
	int cont = *i, index = 0, index_aux = 0, k=0;
	while(*i<buffer.size())
	{
		index = verifica_valor(&g, buffer[*i]);
		index_aux = verifica_valor(&g, buffer[*i+1]);
		if (index == -1 && index_aux == -1)
		{
			adiciona_vertice(&g, buffer[*i], &k);
			if(buffer[*i] != buffer[*i+1])
			{
				adiciona_vertice(&g, buffer[*i+1], &k);
				adiciona_lista_adj(&g, buffer[*i+1], k-2, k-1);
			}
			else
			{
				adiciona_lista_adj(&g, buffer[*i+1], k-1, k-1);
			}
		}
		else if(index == -1)
		{
			adiciona_vertice(&g, buffer[*i], &k);
			adiciona_lista_adj(&g, buffer[*i+1], k-1, index_aux);
		}
		else if(index_aux == -1)
		{
			adiciona_vertice(&g, buffer[*i+1], &k);
			adiciona_lista_adj(&g, buffer[*i+1], index, k-1);
		}
		else
		{
			adiciona_lista_adj(&g, buffer[*i+1], index, index_aux);
		}
		cont=cont+2;
		*i = *i+2;
	}
	return g;
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
	while(aux<m)
	{
		g.v[buffer[*i]-1].adj.push_back(buffer[*i+1]);
		g.v[buffer[*i+1]-1].adj.push_back(buffer[*i]);
		*i=*i+2;
		aux++;
	}
	*i=*i-2;
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
int main()
{
	int inicio = clock();
	vector <int> buffer;
	leitura_arquivo("10.in", &buffer);
	int n = buffer[0]; // n = de onde para onde deve ser feito o teleporte 
	int m = buffer[1]; // m = quantidade de teleportes
	int i=2;
	grafo g = nao_orientado(buffer, n, &i);
	grafo g_orien = orientado(buffer, n, &i);
	//grafo g_orien = montagem_grafo_orientado(buffer, n, &i);
	buffer.clear();
	cout << (clock() - inicio)/CLOCKS_PER_SEC << "\n";

}