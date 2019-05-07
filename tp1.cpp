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
grafo nao_orientado(vector <int> buffer, int n, int m, int *i)
{
	grafo g;
	g.v.resize(n);
	int aux = 0;
	for(int k=0;k<g.v.size();k++)
	{
		g.v[k].id = k+1; // em 0 está 1
	}
	while(aux<m)
	{
		g.v[buffer[*i]-1].adj.push_back(buffer[*i+1]);
		if(buffer[*i] != buffer[*i+1])
		{
			g.v[buffer[*i+1]-1].adj.push_back(buffer[*i]);
		}
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
		if(g.v[buffer[*i]-1].adj.size() >= g.v[buffer[*i+1]-1].adj.size())
		{
			g.v[buffer[*i]-1].adj.push_back(buffer[*i+1]);
		}
		else
		{
			g.v[buffer[*i+1]-1].adj.push_back(buffer[*i]);
		}
		
		*i=*i+2;
	}
	return g;
}
void dfs_visit(grafo *g, int i, int *tempo, componente *c){
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
	g->v[i].flag = 1; // cor cinza
	*tempo = *tempo + 1;
	g->v[i].tempo_d = *tempo;
	int index;
	for(int k=0;k<g->v[i].adj.size();k++)
	{
		index = g->v[i].adj[k] - 1;
		if(g->v[index].flag == 0)
		{
			g->v[index].componente = g->v[i].componente; // recebe o componente do pai
			c->quant_vertices = c->quant_vertices + 1;
			dfs_visit(g, index, tempo, c);
		}
		c->quant_arestas = c->quant_arestas + 1;
	}

	if(g->v[i].adj.size() <= c->min_degree)
	{
		c->min_degree = g->v[i].adj.size();
		if(c->min_degree == 1)
		{
			c->quant_um = c->quant_um + 1;
		}
	}
	if(g->v[i].adj.size()>c->max_degree)
	{
		c->max_degree = g->v[i].adj.size();
	}

	g->v[i].flag = 2; // cor preto
	*tempo = *tempo + 1;
	g->v[i].tempo_f = *tempo;
}
vector <componente> dfs(grafo *g){ // 0 false 1 true

	/*
		Método responsável por realizar fazer o método de busca em profundidade.
		A implementação foi realizada com base no código do livro Algoritmos do Cormen.

		Parâmetros:
			grafo g;
	*/
	int tempo = 0;
	vector <componente> comp_list;
	componente c;
	for(int i=0;i<g->v.size();i++)
	{
		c.quant_um = 0, c.componente = 0, c.quant_vertices = 1, c.quant_arestas = 0;
		c.max_degree = g->v[i].adj.size();
		c.min_degree = g->v[i].adj.size();
		if (g->v[i].flag == 0) // quer dizer que a cor é branca
		{ 	
			g->v[i].componente = g->v[i].componente + 1;
			dfs_visit(g, i, &tempo, &c);
			c.componente = g->v[i].componente; // id do componente
			c.root = i; // o root daquele componente, passando o index
			comp_list.push_back(c);
		}
	}
	g->num_componentes = comp_list.size();
	return comp_list;
}
int primeiro_tipo(componente *c)
{
	if(c->min_degree == 1 && c->max_degree == 2 && c->quant_um == 2 && c->quant_vertices == (c->quant_arestas/2)+1)
	{
		return 0;
	}
	return -1;
}
int segundo_tipo(componente *c)
{
	if(c->quant_vertices == (c->quant_arestas/2)+1)
	{
		return 0;
	}
	return -1;
}
int quarto_tipo(componente *c)
{	
	if (c->quant_arestas == c->quant_vertices*2)
	{
		return 0;
	}
	if (c->max_degree == c-> min_degree && c->max_degree == 2)
	{
		return 0;
	}
	return -1;
}
int dfs_bipartido(grafo *g, int root, int cor, int *quant_zeros, int *quant_um, int *grau_medio)
{
	/*
		Está função é responsável por identificar se um grafo é bipartido ou não.
		Para isso, ela associa cores aos vértices. 
		Caso ela consiga colocar vizinhos sempre com cores diferentes (0 ou 1),
		então o grafo é bipartido.

		Essa função é utilizada no terceiro método. Por meio das cores é possível
		definir se o vértice é bipartido e se ele satisfaz as requisições do quarto
		método.

		Esse algoritmo é uma variação do DFS.

		Parâmetros:
			grafo g;
			int root; que deve ser o index do vértice da componente
	*/
	int index, bipartido;
	g->v[root].cor = cor; // o primeiro vértice vai ter cor 0
	if(cor == 0)
	{
		*quant_zeros = *quant_zeros + 1;
	}
	else
	{
		*quant_um = *quant_um + 1;
		*grau_medio = *grau_medio + g->v[root].adj.size();
	}	
	for(int i = 0; i<g->v[root].adj.size();i++)
	{
		index = g->v[root].adj[i] - 1;
		if(g->v[index].cor == -1) // não foi visitado ainda
		{
			bipartido = dfs_bipartido(g, index, 1-cor, quant_zeros, quant_um, grau_medio);
		}
		else
		{
			if(g->v[root].cor == g->v[index].cor)
			{
				return -1;
			}
		}
	}
	return 0;
}
void bfs(grafo *g, int root)
{
	/*
		Método responsável por calcular a busca em largura no grafo.
		A implementação deste código é baseada na implementação disponível
		no livro de Algoritmos do Cormen.

		Parâmetros:
			grafo g; --> grafo que será calculado o bfs
			int root; --> index do vértice inicial para o calculo do bfs
	*/
	vector <int> q; // fila de prioridades utilizada na implementação
	int u, i;
	g->v[root].cor = 1; // inicializando a cor cinza do vértice
	g->v[root].distancia_pai = 0; // a distância dele para ele mesmo
	q.push_back(root); // estou adicionando o id do vértice na fila
	g->vertices_visitados.push_back(root);
	while(q.size()!=0)
	{
		u = q[0]; // retirando da fila o primeiro elemento
		q.erase(q.begin()); // retirando o elemento da fila
		for(i=0;i<g->v[u].adj.size();i++)
		{
			if(g->v[g->v[u].adj[i] - 1].cor==0) // se a cor do vértice for branco
			{
				g->vertices_visitados.push_back(g->v[u].adj[i] - 1);
				g->v[g->v[u].adj[i] - 1].cor = 1; // nova cor vai ser cinza
				g->v[g->v[u].adj[i] - 1].pai = g->v[u].id; // adicionando o id do par
				g->v[g->v[u].adj[i] - 1].distancia_pai = g->v[u].distancia_pai + 1;
				q.push_back(g->v[u].adj[i] - 1); // adicionando o vértice na lista
			}
		}
		g->v[u].cor = 2; // a cor do vértice é preto e ele já foi processado
	}
}
int terceiro_tipo(grafo *g, componente *c)
{
	int quant_zeros = 0, quant_um = 0, grau_medio = 0;
	int bipartido = dfs_bipartido(g, c->root, 0, &quant_zeros, &quant_um, &grau_medio);
	if(bipartido == 0)
	{
		if(grau_medio/quant_zeros == quant_um)
		{
			return 0;
		}
	}
	return -1;
}
void identifica_naves(grafo *g, int *t1, int *t2, int *t3, int *t4)
{
	vector <componente> comp_list = dfs(g);
	int k = 0, tempo = 0, quant_vertices;
	for(int i=0;i<comp_list.size();i++)
	{
		if(primeiro_tipo(&comp_list[i]) != -1)
		{
			*t1 = *t1 + 1;
		}
		else if(segundo_tipo(&comp_list[i]) != -1)
		{
			*t2 = *t2 + 1;
		}
		else if(terceiro_tipo(g, &comp_list[i]) != -1)
		{
			*t3 = *t3 + 1;
		}
		else if(quarto_tipo(&comp_list[i]) != -1)
		{
			*t4 = *t4 + 1;
		}
	}

}
void inicializa_vertices(grafo *g)
{
	for(int k=0;k<g->vertices_visitados.size();k++)
	{
		g->v[g->vertices_visitados[k]].cor = 0;
		g->v[g->vertices_visitados[k]].distancia_pai = -1;
		g->v[g->vertices_visitados[k]].pai = -1;
	}
	g->vertices_visitados.clear();
}
int calcula_tempo_vantagem(grafo *g, grafo *g_orien)
{
	for(int i=0;i<g->v.size();i++)
	{ // preciso inicializar a cor as mesmas estão setadas depois do DFS
		g->v[i].cor = 0;
	}
	int aux = 0, min = 0, flag_iteracao = 0, flag_entrada = 0, flag_bfs = 0, cont = 0, tempo_vantagem = 0, k;
	for(int i=0;i<g->v.size();i++)
	{ // tenho que verificar para todos os vértices
		if(flag_entrada == 1)
		{
			inicializa_vertices(g);	
		}
		flag_entrada = 0;
		tempo_vantagem = 0;
		if(g_orien->v[i].adj.size() > 0)
		{
			bfs(g, i);
			aux = i + g->vertices_visitados.size() - cont;
			cont = 0;
			flag_bfs = 1;
			while(i<aux)
			{
				if(flag_bfs == 1)
				{
					for(k=0;k<g_orien->v[i].adj.size();k++)
					{
						tempo_vantagem = g->v[g_orien->v[i].adj[k]-1].distancia_pai + tempo_vantagem;
					}
					if(flag_iteracao > 0 && tempo_vantagem > min)
					{
						i = aux;
						break;
					}
					inicializa_vertices(g);
				}
				i++;
				flag_bfs = 0;
				if(g_orien->v[i].adj.size()>0 && i-1 != g->v.size()-1)
				{
					bfs(g, i);
					flag_bfs = 1;
				}
			}
			i--;
			flag_entrada = 1;
		}
		else
		{
			cont++;
		}
		if(flag_entrada == 1 && flag_iteracao > 0)
		{
			if(tempo_vantagem == 0)
			{
				return 0;
			}
			else if(tempo_vantagem < min)
			{
				min = tempo_vantagem;
			}
		}
		else if(flag_iteracao == 0 && flag_entrada == 1)
		{
			min = tempo_vantagem;
			flag_iteracao = 1;
		}
	}
	cout << "Tempo Vantagem: " << min/2 << "\n";
	return min/2;

}
int main(int argc, char *argv[])
{
	if(argc != 5)
	{
		printf("Os parametros foram passados de forma incorreta\n");
		exit(1);
	}
	// o argv[4] possui o nome do arquivo de saída
	int inicio = clock();
	vector <int> buffer;
	leitura_arquivo(argv[2], &buffer);
	
	int n = buffer[0]; // n = de onde para onde deve ser feito o teleporte 
	int m = buffer[1]; // m = quantidade de teleportes
	int i=2; // começa de 2 pois já foram avaliados os dois primeiros vértices
	int t1 = 0, t2 = 0, t3 = 0, t4 = 0;
	
	grafo g = nao_orientado(buffer, n, m, &i);
	grafo g_orien = orientado(buffer, n, &i);
	buffer.clear();
	identifica_naves(&g, &t1, &t2, &t3, &t4);
	int tempo_vantagem = calcula_tempo_vantagem(&g, &g_orien);
	cout <<"Tempo gasto: " << (clock() - inicio)/CLOCKS_PER_SEC << "\n";
	// salvando o arquivo
	ofstream arquivo_saida;
	arquivo_saida.open(argv[4]);
	arquivo_saida << t1 << " " << t2 << " " << t3 << " " << t4 << "\n";
	arquivo_saida << tempo_vantagem;
	arquivo_saida.close();
}