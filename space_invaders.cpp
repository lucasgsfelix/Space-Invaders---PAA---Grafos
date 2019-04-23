#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

class vertices{
	/*
		Classe vertices:
			Nessa classe são armazenados os vértices.
	*/
	public:
		vector <vertices> adj; // essa variável vai guardar a lista de adjacência do vértice
		int id; //identificador do vértice
		int flag = 0; // vai representar a cor no dfs e no bfs
		int tempo_d = -1; // dfs
		int tempo_f = -1; // dfs
		int componente = 0; // vai identificar as componentes
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
			cout << g.v[i].adj[j].id << " ";
		}
		cout << "\n";
	}
	if(g.v.size()==0)
	{
		cout << "GRAFO VAZIO ! \n";
	}
}
int verifica_valor(grafo g, int valor){
	/*
		Método responsável por verificar se um vértice u, está presente
		no grafo.

		Parâmetros:
			grafo g
			int valor; é o id do valor que deve ser verificado se está na lista

	*/
	for(int i=0;i<g.id_vertices.size();i++){
		//cout << g.id_vertices[i] << " " << valor << "\n";
		if(valor == g.id_vertices[i]){
			return i;
		}
	}
	return -1;
}

int verifica_lista_adjacencia(vector <vertices> v, int valor){
	/*
		Método responsável por verificar se um vértice u, está presente
		na lista de adjacência do vértice v.

		Parâmetros:
			vector <vertices> v; lista do tipo vértices
			int valor; é o id do valor que deve ser verificado se está na lista

	*/

	for(int i=0;i<v.size();i++){
		if(valor == v[i].id){
			return i;
		}
	}
	return -1;

}
grafo adiciona_vertice(grafo g, int *i, int valor){
	/*
		Método responsável verifica o vértice que ainda não está na 
		lista de adjacência e adicionar o mesmo.
		Também adiciona o valor na lista de id_vertices.

		Parâmetros:

			grafo g;
			int i; index do valor a ser adicionado
			int valor; o valor que será adicionado
	*/
	int index = *i;
	g.id_vertices.push_back(valor); // adicionei na lista de vértices do grafo
	g.v[index].id = valor; // dei um id ao valor
	*i = *i + 1;
	
	return g;
}
grafo leitura_vertices(grafo g, int *i, int valor, int ant_valor){
	/*
		Método responsável pela leitura dos vértices e montagem da lista de adjacência.
		Como temos um grafo não direcionado, é necessário colocar adicionar o vértice v
		na lista de adjacência de vértice u, e u na lista de adjacência de v. Sendo u e v
		vértices presentes no grafo.

		Parâmetros:
			grafo g;
			int i; a posição em que irá ser armazenado o valor
			int valor; vértice u
			int ant_valor; vértice v
	*/

	int index_valor = verifica_valor(g, valor);
	int index_ant_valor = verifica_valor(g, ant_valor);
	int k = *i;
	if(index_valor == -1 && index_ant_valor == -1){
		if(valor != ant_valor){
			g = adiciona_vertice(g, i, valor);
			g = adiciona_vertice(g, i, ant_valor);
			k = *i;
			g.v[k-2].adj.push_back(g.v[k-1]); // estou adicionando o objeto do vértice vizinho
			g.v[k-1].adj.push_back(g.v[k-2]); // adicionei para um e para o outro
		}else{
			g = adiciona_vertice(g, i, valor);
			k = *i;
			g.v[k-1].adj.push_back(g.v[k-1]); // estou adicionando o objeto do vértice vizinho
		}
	}
	else if(index_valor == -1)
	{
		g = adiciona_vertice(g, i, valor);
		k = *i;
		g.v[index_ant_valor].adj.push_back(g.v[k-1]); // adicionei o novo vértice
		g.v[k-1].adj.push_back(g.v[index_ant_valor]);
	}
	else if(index_ant_valor == -1)
	{	

		g = adiciona_vertice(g, i, ant_valor);
		k = *i;
		g.v[index_valor].adj.push_back(g.v[k-1]); // adicionei o novo vértice
		g.v[k-1].adj.push_back(g.v[index_valor]);
	}
	else // aqui os dois já existem
	{
		if ((verifica_lista_adjacencia(g.v[index_ant_valor].adj, g.v[index_valor].id)) == -1){
			g.v[index_valor].adj.push_back(g.v[index_ant_valor]);
			g.v[index_ant_valor].adj.push_back(g.v[index_valor]);
		}
	}
	return g;
}

void leitura_arquivo(grafo *g_um, grafo *g_tele_um, int *n_vertices, int *m_tele, const char* nome_arquivo){
	/*
		Método responsável pela leitura do arquivo e identificação e chamada das funções
		para realização da montagem do grafo.

		Parâmetros:
			grafo g_um; é o grafo que possui as ligações dos postos de transferências
			grafo g_tele_um; é o grafo que possui os teletransportes que devem ser realizados
			pelos tripulantes da nave;
			int n_vértices; é o número de postos de combate que será lido do arquivo
			int m_tele; são os possível locais de teletranspote
			const char nome_arquivo; é o nome do arquivo que possui o grafo
	*/
	ifstream file;
	file.open(nome_arquivo);
	int n = *n_vertices;
	int m = *m_tele;
	int valor, ant_valor, flag = 0, i = 0, j = 0;
	grafo g = *g_um;
	grafo g_tele = *g_tele_um;

	if(!file){
		cerr << "Não foi possível abrir o arquivo!";
		exit(1);
	}

	while(file >> valor){
		if (flag == 0){
			n = valor; // número de postos de combate
			*n_vertices = n;
			g.v.resize(n);
			if(n < 10 || n> pow(10, 5)){
				cerr << "N em valores não permitidos!";
				exit(1);
			}
		}
		else if (flag == 1){
			m = valor; // quantidade total de teleportes possíveis
			*m_tele = m;
			g_tele.v.resize(n);
			if(m < 8 || m> pow(10, 6)){
				cerr << "M em valores não permitidos!";
				exit(1);
			}

		}
		else if(m + 1 != g_tele.id_vertices.size()){
			if(flag % 2 == 0){
				ant_valor = valor;
			}
			else if(flag % 2 == 1){
				g_tele = leitura_vertices(g_tele, &j, valor, ant_valor);
				*g_tele_um = g_tele;
			}
		}
		else{
			if(flag % 2 == 0){
				ant_valor = valor;
			}	
			else if(flag % 2 == 1){
				g = leitura_vertices(g, &i, valor, ant_valor);
				*g_um = g;
			}
		}
		flag++;
	}

	file.close();
}
grafo dfs_visit(grafo g, int i, int *tempo){
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
		index = verifica_valor(g, g.v[i].adj[k].id);
		if(g.v[index].flag == 0){ // cor branca
			g.v[index].componente = g.v[i].componente; // recebe o componente do pai
			g = dfs_visit(g, index, tempo);
		}
	}
	g.v[i].flag = 2; // cor preto
	*tempo = *tempo + 1;
	g.v[i].tempo_f = *tempo;
	return g;
}
int dfs_bipartido(grafo *g_aux)
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
	*/
	grafo g = *g_aux;
	int index, bipartido;
	g.v[0].cor = 0; // o primeiro vértice vai ter cor 0
	for(int i = 0; i<g.v.size();i++)
	{
		for(int k=0;k<g.v[i].adj.size();k++)
		{
			index = verifica_valor(g, g.v[i].adj[k].id);
			if(g.v[index].cor == -1)
			{ // quer dizer que o vértice ainda não foi visitado
				if(g.v[i].cor == 0)
				{	
					g.v[index].cor = 1;
				}
				else
				{
					g.v[index].cor = 0;
				}
			}
			else
			{ // quer dizer que ele já foi visitado
				if(g.v[i].cor == g.v[index].cor)
				{ 
				/*
				 	Quer dizer que ele e o vizinho possuem a mesma cor, 
				 	então o grafo não é bipartido.
				*/
					return -1;	
				}

			}
		}

	}
	*g_aux = g;
	return 0;
}
grafo dfs(grafo g){ // 0 false 1 true

	/*
		Método responsável por realizar fazer o método de busca em profundidade.
		A implementação foi realizada com base no código do livro Algoritmos do Cormen.

		Parâmetros:
			grafo g;
	*/

	int tempo = 0;
	int componentes = 0;
	for(int i=0;i<g.v.size();i++)
	{
		if (g.v[i].flag == 0 && g.v[i].id != 0)
		{ // quer dizer que a cor é branca
			componentes = componentes + 1;
			g.v[i].componente = componentes;
			g = dfs_visit(g, i, &tempo);
		}
	}
	g.num_componentes = componentes;
	return g;
}

grafo componente_grafo(grafo g, int i){
	/*
		Método utilizado para identificação dos componentes do grafo, e retornar
		subgrafos baseados em cada uma das componentes.

		Parâmetros:
			grafo g;
			int i;  i é a componente a qual quero gerar o subgrafo
	*/

	grafo g_temp;
	int index = 0;
	for(int k=0;k<g.v.size();k++){
			
		if(g.v[k].componente == i+1){
			g_temp.v.push_back(g.v[k]);
			g_temp.id_vertices.push_back(g.v[k].id);
			g_temp.v[index].flag = 0;
			index++;
		}
	}
	return g_temp;

}
int primeiro_tipo(grafo g){
	/*
		Método responsável por identificar naves do primeiro tipo.
		Naves do primeiro tipo possuem como característica o fato de que a partir
		de um vértice raíz é todos os outros vértices terão degree == 2, exceto o último.

		Além disso, o valor de tempo fechamento do vértice raíz (calculado por meio do dfs),
		é maior que todos os outros valores de fechamento presentes no cálculo.

		Parâmetros:
			grafo g;
	*/
	int root = -1, tempo = 0, maior = 0;
	for(int i = 0; i<g.v.size();i++){
		if(g.v[i].adj.size()>2){
			return -1;
		}
		else if(g.v[i].adj.size()==1){
			root = i; // será meu candidato a root, pois ele será o início ou o final
		}
	}
	if(root==-1){ // então ele não tem nenhum vértice de tamanho 1
		return root;
	}

	g = dfs_visit(g, root, &tempo);
	maior = g.v[root].tempo_f;
	for(int i=0;i<g.v.size();i++){
		if (maior < g.v[i].tempo_f){
			return -1;
		}
	}
	return 0;
}
int quarto_tipo(grafo g)
{
	/*
		Este método é responsável por identificar naves do quarto tipo.
		Os grafos que caracterizam o quarto tipo são grafos cíclicos, onde
		todos os vértices possuem o mesmo número de vizinhos.

		Parâmetros:
			grafo g; 
	*/
	int degree = g.v[0].adj.size();
	for(int i = 1; i<g.v.size();i++){ 
		if(g.v[i].adj.size()!=degree){
			return -1;
		}
	}
	return 0;
}
int terceiro_tipo(grafo g)
{ 	/* 
	   Neste caso, o tamanho da lista de adjacência tem que ser igual ao número
	   de vértices com cor oposta ao vértice avaliado. O grafo deve ser bipartido.

	   Desta maneira, se a cor do vértice é 1, ele deve estar conectado com todos os
	   vértices de cor 2.

	   Parâmetros: 
	   		grafo g;
	*/

	int bipartido = dfs_bipartido(&g); // cor inicial é 0
	int cont, index = 0;
	int quant_branco=0, quant_preto=0;
	int casos_certos=0;
	for(int i=0;i<g.v.size();i++)
	{
		if(g.v[i].cor == 0)
		{
			quant_branco++;
		}
		else
		{
			quant_preto++;
		}

	}
	if(bipartido == 0)
	{
		for(int i=0;i<g.v.size();i++)
		{
			cont = 0;
			for(int j=0;j<g.v[i].adj.size();j++)
			{
				index = verifica_valor(g, g.v[i].adj[j].id);
				if(g.v[index].cor != g.v[i].cor)
				{ // não pode ter a mesma cor que o vizinho
					cont++;
				}
			}
			if(g.v[i].cor == 0 && cont == quant_preto && cont == g.v[i].adj.size())
			{ // o vértice branco tem que estar conectado a todos os pretos 
				casos_certos++;
			}
			else if(g.v[i].cor == 1 && cont == quant_branco && cont == g.v[i].adj.size())
			{ // o vértice preto tem que estar conectado a todos os brancos
				casos_certos++;
			}
		}
		if(casos_certos == g.id_vertices.size()){
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}
int segundo_tipo(grafo g)
{
	/*
		Método responsável por identificar o segundo tipo de nave.

		Pela descrição possui t-1 postos possíveis transferências,
		ou seja, número de vértices == número de arestas - 1.

		Parâmetros:
			grafo g_comp --> grafo de componente;
	*/
	int num_arestas = 0;

	for(int i=0;i<g.v.size();i++)
	{
		num_arestas = g.v[i].adj.size() + num_arestas;
	}
	if(g.v.size() == (num_arestas/2)+1)
	{
		return 0;
	}

	return -1;
}
grafo bfs(grafo g, int root)
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
	int index = 0, u;

	for(int i=0;i<g.v.size();i++)
	{
		g.v[i].cor = 0; // inicializando a cor do vértice
		g.v[i].pai = -1;
		g.v[i].distancia_pai = -1;
	}

	g.v[root].cor = 1; // inicializando a cor cinza do vértice
	g.v[root].distancia_pai = 0; // a distância dele para ele mesmo
	q.push_back(root); // estou adicionando o id do vértice na fila
	cout << "ROOT: " << g.v[root].id << "\n";
	while(q.size()!=0)
	{
		u = q[0]; // retirando da fila o primeiro elemento
		q.erase(q.begin()); // retirando o elemento da fila
		for(int i=0;i<g.v[u].adj.size();i++)
		{
			index = verifica_valor(g, g.v[u].adj[i].id);
			if(g.v[index].cor==0) // se a cor do vértice for branco
			{
				g.v[index].cor = 1; // nova cor vai ser cinza
				g.v[index].pai = g.v[u].id; // adicionando o id do par
				g.v[index].distancia_pai = g.v[u].distancia_pai + 1;
				cout << "Distância: " << g.v[index].id <<" "<< g.v[u].distancia_pai + 1 <<"\n";
				q.push_back(index); // adicionando o vértice na lista
			}
		}
		g.v[u].cor = 2; // a cor do vértice é preto e ele já foi processado
	}
	return g;
}
int calcula_tempo_vantagem(grafo g_temp, grafo g_dist){
	/*
		Função responsável por chamar o método de BFS para cada um par de vértice que 
		possui root diferente, recalculando assim a distância para a raíz.

		Parâmetros:
			grafo g_temp; que é o grafo que possui as conexões da nave
			grafo g_dist; que é o grafo que possui o root e o target pra calcular a distância

	*/
	vector <int> vertices = g_dist.id_vertices; // vertices que é preciso avaliar
	int index = 0, index_aux = 0;
	int tempo_vantagem = 0;
	for(int i=0;i<g_temp.v.size();i++)
	{
		g_temp = bfs(g_temp, i); // para cada root
		
		index = verifica_valor(g_dist, g_temp.v[i].id);
		/* 
			quero descobrir qual o index na lista dist, 
			do vértice g_temp.v[i].id, que de quem eu calculei a distância 
		*/
		for(int j=0;j<g_dist.v[index].adj.size();j++)
		{	// tem um desses indexes errados
			cout << g_dist.v[index].id << " " << g_dist.v[index].adj[j].id << " "<< " "<< g_temp.v[index_aux].id <<" " << g_temp.v[index_aux].distancia_pai << "\n";
			index_aux = verifica_valor(g_dist, g_dist.v[index].adj[j].id);
			tempo_vantagem = g_temp.v[index_aux].distancia_pai + tempo_vantagem;
		}
	}
	cout << tempo_vantagem << "\n";
	return tempo_vantagem/2;
}
grafo separa_grafo_por_id(vector <int> lista_id, grafo g_dist){
	/*
		Método responsável por gerar um grafo temporário com os vértices que postos
		utilizados para calcular os tempos de vantagem.

		Parametros:
			vector lista_id; lista de ids do componente em que se deve separar os postos
			grafo g_dist; o qual possui os locais dos postos
	*/
	grafo g;
	int flag = 0;
	g.id_vertices.clear();
	for(int i=0;i<g_dist.v.size();i++)
	{
		for(int j=0;j<lista_id.size();j++)
		{
			if(lista_id[j] == g_dist.v[i].id)
			{
				g.v.push_back(g_dist.v[i]);
				//cout << g.v[i].id << "\n";
				g.id_vertices.push_back(lista_id[j]);
				flag ++;
			}
		}
	}
	/*cout << "Teste FLAG-->" << flag << "\n";
	for(int i=0;i<g.id_vertices.size();i++)
	{
		cout << g.id_vertices[i] << " ";
	}
	cout << "\n Teste \n";*/
	return g;
}
void identifica_nave(grafo g_tele, grafo g_dist){
	/*
		Método responsável pela chamada dos métodos de cada uma das naves.
		Esse método age sobre a quantidade de componentes existentes no grafo.
		A quantidade de componentes é descoberta pelo métdo de busca em profundidade.
		Além disso, esse método chama as funções para o cálculo do tempo de vantagem
		de cada nave.

		Parâmetros:
			grafo g_tele;
			grafo g_dist; --> sobre esse grafo é calculado o tempo de vantagem 
	*/

	g_tele = dfs(g_tele); // calculo a busca em profundidade
	int quant_naves = g_tele.num_componentes;
	int t1 = 0, t2 = 0, t3 = 0, t4 = 0;
	grafo g_temp, g_dist_temp;
	for(int i=0;i<quant_naves;i++)
	{
		g_temp = componente_grafo(g_tele, i);
		if(primeiro_tipo(g_temp)!=-1)
		{
			t1++;
		}
		else if(segundo_tipo(g_temp)!=-1)
		{
			t2++;
		}
		else if(terceiro_tipo(g_temp)!=-1)
		{
			t3++;
		}
		else if(quarto_tipo(g_temp)!=-1)
		{
			t4++;
		}
		else
		{
			cout << "Não entrou em nenhum tipo " << i+1 << "\n";
		}
		
		g_dist_temp = separa_grafo_por_id(g_temp.id_vertices, g_dist);
		calcula_tempo_vantagem(g_temp, g_dist_temp);
		g_dist_temp.v.clear();
		g_dist_temp.id_vertices.clear();
		g_temp.v.clear(); // limpando a lista de vértices adjacências
		g_temp.id_vertices.clear(); // limpando a lista de id de vértices
	}
	cout << t1 << " " << t2 << " " << t3 << " " << t4;
}

int main(){

	int n, m;
	grafo g, g_tele;
	leitura_arquivo(&g, &g_tele, &n, &m, "entrada.txt");
	identifica_nave(g_tele, g);
	return 0;
}