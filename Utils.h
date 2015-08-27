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
char *mDensa, *mMarket, *lClasses, sLin[10], sCol[10], sEnt[10], tmpLabel[20]; 
int lin, col, ent, i, j, out;

FILE *entrada, *arqClasses;

map<long, map<long, float> >dataSet;
map<long, map<long, float> >dataSetOut;
std::vector< string > labels;
map< string, long > labelsAccountability;
map< string, std::vector<long> > classes;
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
	map<long, float > QUIQUAD;
	float soma = 0, media, tamClass, A, B, C, D, probabilidade_doc_in_classe, divisor, totalOcorrencias;

	printf("Media do Quiquadrado enviada para redução de dimensionalidade: %.6g\n", minValue);

	for(int j = 0; j < c; j++){
		totalOcorrencias = 0;
		for(int i = 0; i < c; i++){//Contabiliza as ocorrencias do termo na base
			if( dataSet[i][j] != 0 ){
				totalOcorrencias++;
			}
		}
		QUIQUAD[j] = 0;
		for(map< string, long >::iterator labelsIt_a = labelsAccountability.begin(); labelsIt_a != labelsAccountability.end(); ++labelsIt_a){
			A = 0;
			for(std::vector<long>::iterator it = classes[labelsIt_a->first].begin(); it != classes[labelsIt_a->first].end(); ++it){
				if( dataSet[(*it)][j] != 0 ){//Contabiliza as ocorrencias do termo na classe
					A++;
				}
			}

			tamClass = classes[labelsIt_a->first].size();
			probabilidade_doc_in_classe = tamClass/l;
			B = totalOcorrencias - A;//Contabiliza os documentos que possuem o termo e nao sao da classe
			C = tamClass - A;//Contabiliza os documentos da classe que nao possuem o termo
			D = l - A - B - C;//Contabiliza os documentos que nao possuem o termo e nao pertencem a classe
			divisor = (A+C)*(B+D)*(A+B)*(C+D);
			if(divisor != 0){
				QUIQUAD[j] += (probabilidade_doc_in_classe) * ((float)(l*pow((A*D)-(C*B),2)) / (float)(divisor));
			}else{
				QUIQUAD[j] += 0;
			}

		}
		soma += QUIQUAD[j];
	}

	media = soma/c;
	printf("Media do Quiquadrado calculada para redução de dimensionalidade: %.6g\n", media);

	if(minValue != 0){
		media = minValue;
	}

	for(int j = 0; j < c; j++){
		if(QUIQUAD[j] < media){
			colRemove.push_back(j);
		}
	}

	if(colRemove.size() > 0){
		printf("Original(%ld)->Reduzido para(%ld)\n", c, (c-colRemove.size()));
	}

}

void weightsTF(long l, long c){
	printf("Nada a ser feito.\n");
}

void outGenerate(long l, long c, int _opc){
	char nameFile[80];
	strcpy(nameFile, "./matrix_");

	if(_opc == 1){
		strcat(nameFile, "idf");
	}else if(_opc == 2){
		strcat(nameFile, "icf");
	}else if(_opc == 3){
		strcat(nameFile, "rf");
	}else if(_opc == 4){
		strcat(nameFile, "icfbased");
	}else if(_opc == 5){
		strcat(nameFile, "rfxidf");
	}else if(_opc == 6){
		strcat(nameFile, "quiquad");
	}

	//Contabiliza os nao nulos para as saidas mtx e libSVM
	long nonZeroes = 0;
	for(int i = 0; i < l; i++){
		for(int j = 0; j < c; j++){
			if(dataSetOut[i][j] != 0)
				nonZeroes++;
		}
	}

	if(out == 0){
//##########################Densa##########################################
		strcat(nameFile, ".dns");
		ofstream freqFile(nameFile);

		if(colRemove.size() > 0){
			freqFile << l << " " << (c-colRemove.size()) << " " << nonZeroes << endl;
		}else{
			freqFile << l << " " << c << " " << nonZeroes << endl;
		}

		for(int i = 0; i < l; i++){
			for(int j = 0; j < c; j++){
				freqFile << dataSetOut[i][j] << " ";
			}
			freqFile << endl;
		}
		freqFile.close();
	}else if(out == 1){
//##########################mtx##########################################
		strcat(nameFile, ".mtx");
		ofstream freqFile(nameFile);

		freqFile << "%%MatrixMarket matrix coordinate real general" << endl;
		if(colRemove.size() > 0){
			freqFile << l << " " << (c-colRemove.size()) << " " << nonZeroes << endl;
			for(int i = 0; i < l; i++){
				int contador = 0;
				for(int j = 0; j < c && contador < (c-colRemove.size()); j++, contador++){
					if( find(colRemove.begin(), colRemove.end(), j) == colRemove.end() ){
						if(dataSetOut[i][j] != 0)
							freqFile << (i+1) << " " << (contador+1) << "\t" << dataSetOut[i][j] << endl;
					}
				}
			}
		}else{
			freqFile << l << " " << c << " " << nonZeroes << endl;

			for(int i = 0; i < l; i++){
				for(int j = 0; j < c; j++){
					if(dataSetOut[i][j] != 0)
						freqFile << (i+1) << " " << (j+1) << "\t" << dataSetOut[i][j] << endl;
				}
			}

		}

		freqFile.close();
	}else if(out == 2){
//##########################libSVM##########################################
		strcat(nameFile, ".libsvm");
		ofstream freqFile(nameFile);

		if(colRemove.size() > 0){

			for(int i = 0; i < l; i++){
				freqFile << labels[i] << " ";
				int contador = 0;
				for(int j = 0; j < c && contador < (c-colRemove.size()); j++, contador++){
					if( find(colRemove.begin(), colRemove.end(), j) == colRemove.end() ){
						if(dataSetOut[i][j] != 0)
							freqFile << contador << ":" << dataSetOut[i][j] << " ";
					}
				}
				freqFile << endl;
			}
		}else{

			for(int i = 0; i < l; i++){
				freqFile << labels[i] << " ";
				for(int j = 0; j < c; j++){
					if(dataSetOut[i][j] != 0)
						freqFile << j << ":" << dataSetOut[i][j] << " ";
				}
				freqFile << endl;
			}

		}

		freqFile.close();
	}

}

#endif /* UTILS_H_ */
