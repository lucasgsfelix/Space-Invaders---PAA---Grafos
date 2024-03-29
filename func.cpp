#include "func.h"
using namespace std;

void leitura_arquivo(const char* nome_arquivo, vector <int> *buffer)
{
	/* 
		Função responsável por leitura em um arquivo e armazenamento no buffer.

		Parâmetros:
			char: nome_arquivo; nome do arquivo a ser lido
			vector <int> buffer; buffer onde serão armazenados os dados

	*/
	ifstream file;
	file.open(nome_arquivo);
	if(!file){
		cerr << "Não foi possível abrir o arquivo!";
		//exit(1);
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
	int j;
	for(int i=0;i<g.v.size();i++)
	{
		cout << "ID: " << g.v[i].id << " ADJ :" "\n";
		for(j=0;j<g.v[i].adj.size();j++)
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
	/*
		Função responsável pela modelagem do grafo não orientado G.

		Parâmetros:
			vector <int> buffer; que possui os dados que serão utilizados para a modelagem
			int n; possui o número de vértices, lido do arquivo
			int m; possui o número de teleportes que serão realizados
			int *i; variável 
	*/
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
	/*
		Modelagem do grafo orientado G'.

		Parâmetros:
			vector <int> buffer; possui os dados que serão utilizados 
			int n; indica as n linhas finais do artigo
			int i; variável iteradora que já se iniciará com valor maior que zero
	*/
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
	for(int k=0;k<g->v[i].adj.size();k++)
	{
		if(g->v[g->v[i].adj[k] - 1].flag == 0)
		{
			g->v[g->v[i].adj[k] - 1].componente = g->v[i].componente; // recebe o componente do pai
			c->quant_vertices = c->quant_vertices + 1;
			dfs_visit(g, g->v[i].adj[k] - 1, tempo, c);
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
	/*
		Método responsável pela identificação de naves do primeiro tipo;
		Parâmetros:
			componente c; componente que possui as informações da nave
	*/
	if(c->min_degree == 1 && c->max_degree == 2 && c->quant_um == 2 && c->quant_vertices == (c->quant_arestas/2)+1)
	{
		return 0;
	}
	return -1;
}
int segundo_tipo(componente *c)
{
	/*
		Método responsável pela identificação de naves do segundo tipo;
		Parâmetros:
			componente c; componente que possui as informações da nave
	*/
	if(c->quant_vertices == (c->quant_arestas/2)+1)
	{
		return 0;
	}
	return -1;
}
int quarto_tipo(componente *c)
{	
	/*
		Método responsável pela identificação de naves do quarto tipo;
		Parâmetros:
			componente c; componente que possui as informações da nave
	*/
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
		if(g->v[g->v[root].adj[i] - 1].cor == -1) // não foi visitado ainda
		{
			dfs_bipartido(g, g->v[root].adj[i] - 1, 1-cor, quant_zeros, quant_um, grau_medio);
		}
		else
		{
			if(g->v[root].cor == g->v[g->v[root].adj[i] - 1].cor)
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
	/*
		Método responsável pela identificação de naves do terceiro tipo;
		Parâmetros:
			grafo *g; possui o grafo G, o qual será iterado para verificação da componente bipartida
			componente c; componente que possui as informações da nave
	*/
	int quant_zeros = 0, quant_um = 0, grau_medio = 0;
	if(dfs_bipartido(g, c->root, 0, &quant_zeros, &quant_um, &grau_medio) == 0)
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
	/*
		Método responsável pela chamada dos métodos de identificação das naves.

		Parâmetros:
			grafo g; o qual possui o grafo G
			int t1, t2, t3, t4; os quais serão incrementadas quando for reconhecido uma nave de cada tipo

	*/
	vector <componente> comp_list = dfs(g);
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
	comp_list.clear();

}
void inicializa_vertices(grafo *g)
{
	/*
		Método responsável pela inicialização dos vértices, 
		evitando conflitos quando o vértice já foi utilizado

		Parâmetros:
			grafo g; que corresponde ao grafo G
	*/
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
	/*
		Método responsável por calcular o tempo de vantagem no grafo G, 
		dados os vértices do grafo G'.

		Parâmetros:
			grafo g; grafo G
			grafo g_orient; grafo G'

	*/
	int aux = 0, min = 0, flag_iteracao = 0, flag_entrada = 0;
	int flag_bfs = 0, cont = 0, tempo_vantagem = 0, k, i = 0;;
	for(i=0;i<g->v.size();i++)
	{ // preciso inicializar a cor as mesmas estão setadas depois do DFS
		g->v[i].cor = 0;
	}
	for(i=0;i<g->v.size();i++)
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
	//cout << "Tempo Vantagem: " << min/2 << "\n";
	return min/2;

}