#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <string.h>
#include <vector>
#include <iterator>
#include <algorithm>
#include <ctime>
#include <map>
#include <math.h>
#include <fstream>

using namespace std;

#define TAM 7
#define MAX 10000
#define CHUNKSIZE 1

char lixo[MAX];
char *mDensa, *mMarket, *lClasses, sLin[10], sCol[10], sEnt[10]; 
int lin, col, ent, i, j;
FILE *entrada, *arqClasses;

map<long, map<long, float> >dataSet;

std::vector< string > labels;
map< string, long > labelsAccountability;
map< string, std::vector<long> > classes;
char tmpLabel[20];

std::vector<long> colRemove;

const char* vet[TAM] = {
		"tf",
		"idf",
		"icf",
		"rf",
		"icfbased",
		"rfxidf",
		"quiquad"
};

int ENUM(char *opc) { 
	int cont = 0; 
	while (cont < TAM) { 
		if(strcmp (vet[cont], opc) == 0) {
			break;
		}
		cont++;
	}
	return cont;
}

void reductionPerDocs(long l, long c, long minTermsDocs){

	for(int j = 0; j < c; j++){
		int qtdDocs = 0;//Numero de documentos que o termo ocorre
		for(int i = 0; i < l && qtdDocs <= minTermsDocs ; i++){
			if(dataSet[i][j] != 0){
				qtdDocs++;
			}
		}
		if(qtdDocs < minTermsDocs){
			colRemove.push_back(j);
		}
	}

}

void reductionPerQuiquad(long l, long c, float minValue){
	int C = labelsAccountability.size();
	map<long, float > QUIQUAD;
	float soma = 0, media;

	for(int j = 0; j < c; j++){
		long a = 0, b = 0, v_c = 0, d = 0, category = 0;
		QUIQUAD[j] = -1;
		for(map< string, long >::iterator labelsIt_a = labelsAccountability.begin(); labelsIt_a != labelsAccountability.end(); ++labelsIt_a){
			for(map< string, long >::iterator labelsIt_b = labelsAccountability.begin(); labelsIt_b != labelsAccountability.end(); ++labelsIt_b){

				for(std::vector<long>::iterator it = classes[labelsIt_b->first].begin(); it != classes[labelsIt_b->first].end(); ++it){
					if( dataSet[(*it)][j] > 0 ){
						if(strcmp(labelsIt_a->first.c_str(), labelsIt_b->first.c_str()) == 0){
							a++;
						}else{
							b++;
						}
					}else{
						if(strcmp(labelsIt_a->first.c_str(), labelsIt_b->first.c_str()) == 0){
							v_c++;
						}else{
							d++;
						}
					}
				}

			}

			long divisor = (a+v_c)*(b+d)*(a+b)*(v_c+d);
			if(divisor != 0){
				if((float)(l*pow((a*d)-(v_c*b),2)) / (float)(divisor) > QUIQUAD[j]){
					QUIQUAD[j] = (float)(l*pow((a*d)-(v_c*b),2)) / (float)(divisor);
				}
			}else{
				QUIQUAD[j] = 1;
			}

			soma += QUIQUAD[j];

			//		printf("%d - %.6g\t%d\t%d\t%d\t%d\t%d\n", category, QUIQUAD(j, category), a, b, v_c, d, divisor);
			//		category++;

		}

	}

	media = soma/c;
	printf("Media do Quiquadrado calculada para redução de dimensionalidade: %.2g\n", media);
	printf("Media do Quiquadrado enviada para redução de dimensionalidade: %.2g\n", media);

	if(minValue != 0){
		media = minValue;
	}

	for(int j = 0; j < c; j++){
		if(QUIQUAD[j] < media){
			colRemove.push_back(j);
		}
	}

}

void weightsTF(long l, long c){
	ofstream freqFile("./matrix.tf");

	if(colRemove.size() > 0){

		for(int i = 0; i < l; i++){
			for(int j = 0; j < c; j++){
				if( find(colRemove.begin(), colRemove.end(), j) == colRemove.end() )
					freqFile << dataSet[i][j] << "\t";
			}
			freqFile << endl;
		}

		//	cout << "Quantidade de Termos: " << c << endl;
		//	cout << "Quantidade de Termos Apos a Remocao: " << (c - colRemove.size()) << endl;

	}else{

		for(int i = 0; i < l; i++){
			for(int j = 0; j < c; j++){
				freqFile << dataSet[i][j] << "\t";
			}
			freqFile << endl;
		}

	}

	freqFile.close();
}

#endif /* UTILS_H_ */
