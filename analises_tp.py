import os
import statistics
import subprocess

def leitura(arquivo):

	arq = open(arquivo, 'r')
	info = arq.read()
	info = info.split('\n')
	arq.close()

	return info

def leitura_saida(arquivo):

	arq = open(arquivo, 'r')
	info = arq.read()
	info = info.split('\n')
	info = info[0].split(' ')
	arq.close()
	for i in range(0, len(info)):
		info[i] = int(info[i])
	return sum(info)


if __name__ == '__main__':


	i=0
	quant_testes = os.listdir('TestesMonitor/in')
	saidas = os.listdir('TestesMonitor/out')
	quant_testes.sort()
	k = 0
	while(i<len(quant_testes)):
		info = leitura('TestesMonitor/in/'+quant_testes[i])
		n = int(info[0].split(' ')[0]) ## n
		m = int(info[0].split(' ')[1]) ## m
		j = 1
		vertex = []
		edges = []
		while(j<m):
			vertex.append(info[j].split(' ')[0])
			vertex.append(info[j].split(' ')[1])
			edges.append(info[j].split(' '))
			j=j+1

		vertex = set(vertex)
		num_componentes = leitura_saida('TestesMonitor/out/'+saidas[i])
		#while(k<10):
		#	os.system('./executar.sh TestesMonitor/in/' + quant_testes[i] + ' saida.txt')
			#time.append(float(time.split(' ')[0]))
		#	k=k+1
		times = leitura('time_out')
		for m in range(0, len(times)):
			times[m] = list(filter(('m').__ne__, times[m]))
			times[m] = list(filter(('s').__ne__, times[m]))
			times[m] = float(''.join(times[m]).replace(',', '.'))
		time = []

		while(k<10*(i+1)):
			time.append(times[k])
			k=k+1
		
		tabela = quant_testes[i] + ' & $' + str(m) + '$ & $' + str(len(edges)) +'$ & ' + f'$ {str(num_componentes)} ' + '$ & $' + str(round(statistics.mean(time), 4))  + 's$ & $' + str(round(statistics.stdev(time), 4))
		tabela = tabela + '$ & \\\\ \\hline'
		print(tabela)

		i=i+1
	
