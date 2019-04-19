#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

class vertices{
	public:
	vector <vertices> adj; // essa variável vai guardar a lista de adjacência do vértice
	int id; //identificador do vértice
	int flag = 0; // vai representar a cor no dfs e no bfs
	int tempo_d = -1;
	int tempo_f = -1;
	int componente = 0; // vai identificar as componentes
	int cor = -1; // parâmetro para ajudar identificar se o grafo é bipartido
};

class grafo{
	public:
	vector <vertices> v; // guarda a os vértices
	vector <int> id_vertices; //guarda o id dos vértices
	int num_componentes;
	int bipartido = 2; // vai ser utilizado para identificar se o grafo é bipartido
};

int verifica_valor(grafo g, int valor){

	for(int i=0;i<g.id_vertices.size();i++){
		if(valor == g.id_vertices[i]){
			return i;
		}
	}
	return -1;
}

int verifica_lista_adjacencia(vector <vertices> v, int valor){

	for(int i=0;i<v.size();i++){
		if(valor == v[i].id){
			return i;
		}
	}
	return -1;

}
grafo adiciona_vertice(grafo g, int *i, int valor){
	int index = *i;
	g.id_vertices.push_back(valor); // adicionei na lista de vértices do grafo
	g.v[index].id = valor; // dei um id ao valor
	*i = *i + 1;
	
	return g;
}
grafo leitura_vertices(grafo g, int *i, int valor, int ant_valor){

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
int dfs_bipartido(grafo g, int i, int cor){ //eu tenho que passar aqui por parametro, o grafo
	
	g.v[i].cor = cor;
	int index, bipartido;
	for(int k=0;k<g.v[i].adj.size();k++){
		index = verifica_valor(g, g.v[i].adj[k].id);
		if(g.v[index].cor == -1){ // a cor ainda não foi alterada
			bipartido = dfs_bipartido(g, index, 1-cor);
			if(bipartido==-1){
				return -1;
			}
		}
		else{
			if(g.v[index].cor == cor){ 
				// a cor do vizinho tem que ser diferente para ser bipartido
				return -1;
			}
		}
	}
	return 0;
}
grafo dfs(grafo g, int param_bipartido){ // 0 false 1 true

	int tempo = 0;
	int componentes = 0;
	int bipartido;
	for(int i=0;i<g.v.size();i++){
		if (param_bipartido == 0 && g.v[i].flag == 0 && g.v[i].id != 0){ // quer dizer que a cor é branca
			componentes = componentes + 1;
			g.v[i].componente = componentes;
			g = dfs_visit(g, i, &tempo);

		}
		else if (param_bipartido == 1 && g.v[i].id != 0){
			if(g.v[i].cor == -1){
				bipartido = dfs_bipartido(g, i, 0);
				if(bipartido == -1){
					g.bipartido = -1;
				}
				else{ // caso seja bipartido
					g.bipartido = 0;
				}
			}
			cout << "Bipartido ? "<< g.bipartido << "\n";
			break;
		}
	}
	g.num_componentes = componentes;
	return g;
}

grafo componente_grafo(grafo g, int i){
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
	return 1;
}
int quarto_tipo(grafo g){
	// no quarto tipo dos os vértices tem que ter degree igual
	int degree = g.v[0].adj.size();
	for(int i = 1; i<g.v.size();i++){ 
		if(g.v[i].adj.size()!=degree){
			return -1;
		}
	}
	return 1;
}

void identifica_nave(grafo g_tele){

	g_tele = dfs(g_tele, 0); // calculo a busca em profundidade
	int quant_naves = g_tele.num_componentes, index, t;
	grafo g_temp;
	for(int i=0;i<quant_naves;i++){
		g_temp = componente_grafo(g_tele, i);
		t = primeiro_tipo(g_temp);
		if(t==-1){
			t = quarto_tipo(g_temp);
		}
		cout << t << "\n";
		g_temp.v.clear(); // limpando a lista de vértices adjacências
		g_temp.id_vertices.clear(); // limpando a lista de id de vértices

	}

}

int main(){

	
	int n, m;
	grafo g, g_tele;
	leitura_arquivo(&g, &g_tele, &n, &m, "entrada.txt");
	cout << "Fez a leitura correta \n";
	identifica_nave(g_tele);
	
	/*for(int i=0;i<g_tele.v.size();i++){
		cout << g_tele.v[i].id << " ";
		for(int j=0;j<g_tele.v[i].adj.size();j++){
			cout << g_tele.v[i].adj[j].id << " ";
		}
		cout << "\n";
	}
	cout << "grafo 2 " << "\n";
	for(int i=0;i<g.v.size();i++){
		cout << g.v[i].id << " ";
		for(int j=0;j<g.v[i].adj.size();j++){
			cout << g.v[i].adj[j].id << " ";
		}
		cout << "\n";
	}*/




	return 0;
}