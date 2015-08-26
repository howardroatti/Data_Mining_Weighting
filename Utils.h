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
	int C = labelsAccountability.size();
	map<long, float > QUIQUAD;
	float soma = 0, media, tamClass;

	for(int j = 0; j < c; j++){
		long a = 0, b = 0, v_c = 0, d = 0, category = 0;
		QUIQUAD[j] = 0;
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

			tamClass = classes[labelsIt_a->first].size();
			float probabilidade_doc_in_classe = tamClass/l;

			long divisor = (a+v_c)*(b+d)*(a+b)*(v_c+d);
			if(divisor != 0){
				//				if((float)(l*pow((a*d)-(v_c*b),2)) / (float)(divisor) > QUIQUAD[j]){
				QUIQUAD[j] += (probabilidade_doc_in_classe) * ((float)(l*pow((a*d)-(v_c*b),2)) / (float)(divisor));
				//				}
			}else{
				QUIQUAD[j] += 0;
			}

			//		printf("%d - %.6g\t%d\t%d\t%d\t%d\t%d\n", category, QUIQUAD(j, category), a, b, v_c, d, divisor);
			//		category++;

		}

		soma += QUIQUAD[j];

	}

	media = soma/c;
	printf("Media do Quiquadrado calculada para redução de dimensionalidade: %.2g\n", media);
	printf("Media do Quiquadrado enviada para redução de dimensionalidade: %.2g\n", minValue);

	if(minValue != 0){
		media = minValue;
	}

	for(int j = 0; j < c; j++){
		if(QUIQUAD[j] < media){
			colRemove.push_back(j);
		}
	}

	if(colRemove.size() > 0){
		printf("Original(%ld)->Reduzido para(%ld)", c, (c-colRemove.size()));
	}

}

void weightsTF(long l, long c){
	ofstream freqFile("./matrix.tf");

	if(colRemove.size() > 0){//Base Reduzida

		if(out == 1){//Matrix Market
			freqFile << "%%MatrixMarket matrix coordinate real general" << endl;
			freqFile << l << " " << c << " " << (l * (c - colRemove.size())) << endl;
		}

		for(int i = 0; i < l; i++){

			if(out == 2){//libSVM
				freqFile << labels[i] << " ";
			}

			for(int j = 0; j < c; j++){
				if( find(colRemove.begin(), colRemove.end(), j) == colRemove.end() ){

					if(out == 0){//Densa
						freqFile << dataSet[i][j] << " ";
					}else if(out == 1 && dataSet[i][j] != 0){//Matrix Market
						freqFile << (i+1) << " " << (j+1) << " " << dataSet[i][j] << endl;
					}else if(out == 2 && dataSet[i][j] != 0){//libSVM
						freqFile << j << ":" << dataSet[i][j] << " ";
					}

				}
			}

			if(out != 1){
				freqFile << endl;
			}
		}

		//		cout << "Quantidade de Termos: " << c << endl;
		//		cout << "Quantidade de Termos Apos a Remocao: " << (c - colRemove.size()) << endl;

	}else{//Base Normal

		if(out == 1){//Matrix Market
			freqFile << "%%MatrixMarket matrix coordinate real general" << endl;
			freqFile << l << " " << c << (l * c) << endl;
		}

		for(int i = 0; i < l; i++){

			if(out == 2){//libSVM
				freqFile << labels[i] << " ";
			}

			for(int j = 0; j < c; j++){

				if(out == 0){//Densa
					freqFile << dataSet[i][j] << " ";
				}else if(out == 1 && dataSet[i][j] != 0){//Matrix Market
					freqFile << (i+1) << " " << (j+1) << " " << dataSet[i][j] << endl;
				}else if(out == 2 && dataSet[i][j] != 0){//libSVM
					freqFile << j << ":" << dataSet[i][j] << " ";
				}

			}

			if(out != 1){
				freqFile << endl;
			}

		}

	}

	freqFile.close();
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

	long nonZeroes = 0;
	for(int i = 0; i < l; i++){
		for(int j = 0; j < c; j++){
			if(dataSetOut[i][j] != 0)
				nonZeroes++;
		}
	}

	if(out == 0){//Densa
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
	}else if(out == 1){//mtx
		strcat(nameFile, ".mtx");
		ofstream freqFile(nameFile);

		freqFile << "%%MatrixMarket matrix coordinate real general" << endl;
		if(colRemove.size() > 0){
			freqFile << l << " " << (c-colRemove.size()) << " " << nonZeroes << endl;
		}else{
			freqFile << l << " " << c << " " << nonZeroes << endl;
		}

		for(int i = 0; i < l; i++){
			for(int j = 0; j < c; j++){
				if(dataSetOut[i][j] != 0)
					freqFile << (i+1) << " " << (j+1) << " " << dataSetOut[i][j] << endl;
			}
		}
		freqFile.close();
	}else if(out == 2){//libSVM
		strcat(nameFile, ".libsvm");
		ofstream freqFile(nameFile);

		for(int i = 0; i < l; i++){
			freqFile << labels[i] << " ";
			for(int j = 0; j < c; j++){
				if(dataSetOut[i][j] != 0)
					freqFile << j << ":" << dataSetOut[i][j] << " ";
			}
			freqFile << endl;
		}
		freqFile.close();
	}

}

#endif /* UTILS_H_ */
