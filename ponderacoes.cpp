/*
 * @author Howard Roatti
 * @date 07/2015
 * @contact howardcruzroatti@gmail.com
 */

#include "Utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <getopt.h>
#include "IDF.h"
#include "ICF.h"
#include "RF.h"
#include "ICFBASED.h"
#include "RFXIDF.h"
#include "QUIQUAD.h"

using namespace std;

char *optarg;
int c, opc = 1, tipo = 0;
long norm = 0, reducto = 0;
float limite;

int main(int argc, char* argv[]) {
	out = 0;//Densa

	while ((c = getopt(argc, argv, "p:v:m:c:n:r:a:o:")) != -1) {
		switch (c) {
		case 'p' :
			opc = ENUM(optarg);
			printf("Ponderacao: %s\n", vet[opc]);
			//if(opc == 0) exit;//TF
			break;
		case 'v' :
			tipo = 0;
			mDensa = optarg;
			entrada = fopen(mDensa, "r");

			if(entrada == NULL){
				printf("Erro ao tentar abrir o arquivo %s\n", mDensa);
				fclose(entrada);
				exit(1);
			};

			printf("Matriz Densa: %s\n", mDensa);
			break;
		case 'm' :
			tipo = 1;
			mMarket = optarg;
			entrada = fopen(mMarket, "r");

			if(entrada == NULL){
				printf("Erro ao tentar abrir o arquivo %s\n", mMarket);
				fclose(entrada);
				exit(2);
			};

			fgets(lixo, MAX, entrada);//Remove o texto que expressa o tipo de documento
			printf("Matrix Market: %s\n", mMarket);
			break;
		case 'c' :
			lClasses = optarg;

//			if(opc > 1){//Se precisa das classes para calcular
				arqClasses = fopen(lClasses, "r");

				if(arqClasses == NULL){
					printf("Erro ao tentar abrir o arquivo %s\n", lClasses);
					fclose(arqClasses);
					exit(3);
				}

//			}

			printf("Classes: %s\n", lClasses);
			break;
		case 'n' :
			if(strcmp(optarg, "tf") == 0){
				norm = 0;
			}else if(strcmp(optarg, "log") == 0){
				norm = 1;
			}
			printf("Normalizar: %s\n", optarg);
			break;
		case 'r':
			if(strcmp(optarg, "quiquad") == 0 ){
				reducto = 1;
			}else if(strcmp(optarg, "docs") == 0 ){
				reducto = 2;
			}
			printf("Reducao de dimensionalidade: %s\n", optarg);
			break;
		case 'a':
			limite = atof(optarg);
			break;
		case 'o':
			if(strcmp(optarg, "mtx") == 0 ){
				out = 1;
			}else if(strcmp(optarg, "libSVM") == 0 ){
				out = 2;
			}
			printf("Tipo de Saída: %s\n", optarg);
			break;
		default:
			printf("Parâmetro não existe.\n -p -> Tipo de Ponderacao\n -v -> Matriz Densa\n -m -> Matrix Market\n -c -> Lista de Classes\n -v (tf|log)\n");
			return 0;
		}
	}


	fscanf(entrada, "%s", sLin);
	fscanf(entrada, "%s", sCol);
	fscanf(entrada, "%s", sEnt);
	lin = atoi(sLin);
	col = atoi(sCol);
	ent = atoi(sEnt);
	int linha = lin, coluna = col;
	printf("Termos(%d):Documentos(%d)\n\n", coluna, linha);//Para fins de ilustração, imprime as dimensões da matriz

	printf("Lendo o dataSet\n");

	//Ler o arquivo de vetores
	if(tipo ==  1){//mtx
		while(!feof(entrada)){
			fscanf(entrada, "%s", sLin);
			fscanf(entrada, "%s", sCol);
			fscanf(entrada, "%s", sEnt);
			lin = atoi(sLin);
			col = atoi(sCol);
			ent = atof(sEnt);
			//printf("lin(%d) col(%d) ent(%g)\n", lin, col, ent);
			if(lin <= linha)
				dataSet[lin-1][col-1] = ent;
		}
	}else{//denso
		for(int i = 0; i < linha; i++){
			for(int j = 0; j < coluna; j++){
				fscanf(entrada, "%s", sEnt);
				ent = atof(sEnt);
				if(ent > 0)
					dataSet[i][j] = ent;
			}
		}
	}

	fclose(entrada);

	//Ler o arquivo de classes
	if(opc > 1 || reducto == 1){
		printf("Lendo o arquivo de classes\n");
		for(i = 0; i < linha; i++){
			fscanf(arqClasses, "%s", tmpLabel);

			labels.push_back(tmpLabel);
			labelsAccountability[tmpLabel]++;
			classes[tmpLabel].push_back(i);
		}
		fclose(arqClasses);
	}

	/*Reducao de dimensionalidade*/
	if(reducto == 1){
		printf("Reduzindo\n");
		reductionPerQuiquad(linha, coluna, limite);
	}else if(reducto == 2){
		printf("Reduzindo\n");
		reductionPerDocs(linha, coluna, limite);
	}

	printf("Ponderando\n");

	/*Ponderador*/
	if(opc == 1){
		weightsIDF(linha, coluna, norm);
	}else if(opc == 2){
		weightsICF(linha, coluna, norm);
	}else if(opc == 3){
		weightsRF(linha, coluna, norm);
	}else if(opc == 4){
		weightsICFBASED(linha, coluna, norm);
	}else if(opc == 5){
		weightsRFxIDF(linha, coluna, norm);
	}else if(opc == 6){
		weightsQUIQUAD(linha, coluna, norm);
	}else if(opc == 0){
		weightsTF(linha, coluna);
	}else{
		printf("Em desenvolvimento\n");
	}

	printf("Salvando\n");
	outGenerate(linha, coluna, opc);

	printf("Concluido!\n");

	return 0;
}
