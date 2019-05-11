#include "func.h"
using namespace std;

int main(int argc, char *argv[])
{
	if(argc != 5)
	{
		printf("Os parametros foram passados de forma incorreta\n");
		//exit(1);
	}
	// o argv[4] possui o nome do arquivo de saída
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
	// salvando o arquivo
	ofstream arquivo_saida;
	arquivo_saida.open(argv[4]);
	arquivo_saida << t1 << " " << t2 << " " << t3 << " " << t4 << "\n";
	arquivo_saida << tempo_vantagem;
	arquivo_saida.close();
}