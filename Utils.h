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

void weightsIDF(long l, long c, long norm){

	map<long, float> IDF;
	float soma = 0, TFxIDF, TF;

	/*Calcula o IDF dos termos*/
	for(int j = 0; j < c; j++){
	    long soma = 0;
	    for(int i = 0; i < l; i++){
		if( dataSet[i][j] > 0){
		    soma++;
		}
	    }
	    IDF[j] = log2(l / soma);
	    soma = soma + IDF[j];
	}

	/*Calcula o TFxIDF e armazena em arquivo*/
	ofstream freqFile("./matrix.idf");
	for(int i = 0; i < l; i++){
	    for(int j = 0; j < c; j++){
//		TF = 1 + log2( dataSet(i,j) );//TF normalizada baseado em Baeza-Yates
		TF = dataSet[i][j];//Frequencia bruta baseado em Baeza-Yates

		float divisor = 0;
		if(norm == 1 && TF != 0){//Normalizar
		    for(int k = 0; k < c; k++){
			divisor+= pow((TF * IDF[k]),2);
		    }

		    if((TF * IDF[j]) == 0){
			TFxIDF = TF * IDF[j];//TFxIDF
		    }else{
			TFxIDF = TF * IDF[j] / sqrt(divisor);//TFxIDF
		    }

		}else{
		    TFxIDF = TF * IDF[j];//TFxIDF
		}

	        freqFile << TFxIDF << "\t";
	    }
	    freqFile << endl;
	}
	freqFile.close();

}

void weightsICF(long l, long c, long norm){
	map<long, float> ICF;
	int C = labelsAccountability.size();
	float TFxICF, TF;

	/*Calculando o ICF*/
	for(int j = 0; j < c; j++){
	    long c_i = 0;
	    for(map< string, long >::iterator labelsIt = labelsAccountability.begin(); labelsIt != labelsAccountability.end(); ++labelsIt){
		for(std::vector<long>::iterator it = classes[labelsIt->first].begin(); it != classes[labelsIt->first].end(); ++it){
		    if( dataSet[(*it)][j] > 0){
			c_i++;
			break;
		    }
	    	}

	    }
	    ICF[j] = log2(1 + ( C / c_i ));
	}

	ofstream freqFile("./matrix.icf");
	for(int i = 0; i < l; i++){
	    for(int j = 0; j < c; j++){
//		TF = 1 + log2( dataSet(i,j) );//TF normalizada baseado em Baeza-Yates
		TF = dataSet[i][j];//Frequencia bruta baseado em Baeza-Yates
		
		float divisor = 0;
		if(norm == 1 && TF != 0){//Normalizar
		    for(int k = 0; k < c; k++){
			divisor+= pow((TF * ICF[k]),2);
		    }

		    if((TF * ICF[j]) == 0){
			TFxICF = TF * ICF[j];//TFxIDF
		    }else{
			TFxICF = TF * ICF[j] / sqrt(divisor);//TFxIDF
		    }

		}else{
		    TFxICF = TF * ICF[j];//TFxIDF
		}

	        freqFile << TFxICF << "\t";
	    }
	    freqFile << endl;
	}
	freqFile.close();

}

void weightsRF(long l, long c, long norm){//Relevance Frequency
	int C = labelsAccountability.size();
	map<long, float > RF;
	float soma = 0, TFxRF, TF;

	for(int j = 0; j < c; j++){
	    long a = 0, b = 0, category = 0;
	    RF[j] = -1;
	    for(map< string, long >::iterator labelsIt_a = labelsAccountability.begin(); labelsIt_a != labelsAccountability.end(); ++labelsIt_a){
		for(map< string, long >::iterator labelsIt_b = labelsAccountability.begin(); labelsIt_b != labelsAccountability.end(); ++labelsIt_b){

		    for(std::vector<long>::iterator it = classes[labelsIt_b->first].begin(); it != classes[labelsIt_b->first].end(); ++it){
			if( dataSet[(*it)][j] > 0 ){
			    if(strcmp(labelsIt_a->first.c_str(), labelsIt_b->first.c_str()) == 0){
				a++;
			    }else{
				b++;
			    }			    
			}
		    }

		}

		if(log2(2 + ( a / std::max(long(1), b) )) > RF[j]){
		    RF[j] = log2(2 + ( a / std::max(long(1), b) ));
		}
//		printf("%d - %.6g\t%d\t%d\n", category, RF(j,0), a, b);
//		category++;
	    }	    

	}

	ofstream freqFile("./matrix.rf");
	for(int i = 0; i < l; i++){
	    for(int j = 0; j < c; j++){
//		TF = 1 + log2( dataSet(i,j) );//TF normalizada baseado em Baeza-Yates
		TF = dataSet[i][j];//Frequencia bruta baseado em Baeza-Yates

		float divisor = 0;
		if(norm == 1 && TF != 0){//Normalizar
		    for(int k = 0; k < c; k++){
			divisor+= pow((TF * RF[k]),2);
		    }

		    if((TF * RF[j]) == 0){
			TFxRF = TF * RF[j];//TFxIDF
		    }else{
			TFxRF = TF * RF[j] / sqrt(divisor);//TFxIDF
		    }

		}else{
		    TFxRF = TF * RF[j];//TFxRF
		}

	        freqFile << TFxRF << "\t";
	    }
	    freqFile << endl;
	}
	freqFile.close();

}

void weightsICFBASED(long l, long c, long norm){//ICFBASED
	map<long, float> ICF;
	int C = labelsAccountability.size();
	map<long, float > ICFBASED;
	float soma = 0, TFxICFBASED, TF;

	/*ICF*/
	for(int j = 0; j < c; j++){
	    long c_i = 0;
	    for(map< string, long >::iterator labelsIt = labelsAccountability.begin(); labelsIt != labelsAccountability.end(); ++labelsIt){
		for(std::vector<long>::iterator it = classes[labelsIt->first].begin(); it != classes[labelsIt->first].end(); ++it){
		    if( dataSet[(*it)][j] > 0){
			c_i++;
			break;
		    }
	    	}

	    }
	    ICF[j]= float ( C / c_i );
//	    printf("%g\t%d\t%d\n", ICF(j,0), C, c_i);
	}

	/*RF*/
	for(int j = 0; j < c; j++){
	    long a = 0, b = 0, category = 0;
	    ICFBASED[j] = -1;
	    for(map< string, long >::iterator labelsIt_a = labelsAccountability.begin(); labelsIt_a != labelsAccountability.end(); ++labelsIt_a){
		for(map< string, long >::iterator labelsIt_b = labelsAccountability.begin(); labelsIt_b != labelsAccountability.end(); ++labelsIt_b){

		    for(std::vector<long>::iterator it = classes[labelsIt_b->first].begin(); it != classes[labelsIt_b->first].end(); ++it){
			if( dataSet[(*it)][j] > 0 ){
			    if(strcmp(labelsIt_a->first.c_str(), labelsIt_b->first.c_str()) == 0){
				a++;
			    }else{
				b++;
			    }			    
			}
		    }

		}

		if(log2(2 + ( a / std::max(long(1), b) ) * ICF[j]) > ICFBASED[j]){
		    ICFBASED[j] = log2(2 + ( a / std::max(long(1), b) ) * ICF[j]);
		}
//		printf("%d - %.6g\t%d\t%d\t%.6g\n", category, ICFBASED(j, category), a, b, ICF(j,0));
//		category++;

	    }	    

	}

	ofstream freqFile("./matrix.icfbased");
	for(int i = 0; i < l; i++){
	    for(int j = 0; j < c; j++){
//		TF = 1 + log2( dataSet(i,j) );//TF normalizada baseado em Baeza-Yates
		TF = dataSet[i][j];//Frequencia bruta baseado em Baeza-Yates

		float divisor = 0;
		if(norm == 1 && TF != 0){//Normalizar
		    for(int k = 0; k < c; k++){
			divisor+= pow((TF * ICFBASED[k]),2);
		    }

		    if((TF * ICFBASED[j]) == 0){
			TFxICFBASED = TF * ICFBASED[j];//TFxIDF
		    }else{
			TFxICFBASED = TF * ICFBASED[j] / sqrt(divisor);//TFxIDF
		    }

		}else{
		    TFxICFBASED = TF * ICFBASED[j];//TFxRF
		}

	        freqFile << TFxICFBASED << "\t";
	    }
	    freqFile << endl;
	}
	freqFile.close();

}

void weightsRFxIDF(long l, long c, long norm){//Relevance Frequency x IDF
	int C = labelsAccountability.size();
	map<long, float > RF;
	float soma = 0, TFxRFxIDF, TF;

	for(int j = 0; j < c; j++){
	    long a = 0, b = 0, category = 0;
	    RF[j] = -1;
	    for(map< string, long >::iterator labelsIt_a = labelsAccountability.begin(); labelsIt_a != labelsAccountability.end(); ++labelsIt_a){
		for(map< string, long >::iterator labelsIt_b = labelsAccountability.begin(); labelsIt_b != labelsAccountability.end(); ++labelsIt_b){

		    for(std::vector<long>::iterator it = classes[labelsIt_b->first].begin(); it != classes[labelsIt_b->first].end(); ++it){
			if( dataSet[(*it)][j] > 0 ){
			    if(strcmp(labelsIt_a->first.c_str(), labelsIt_b->first.c_str()) == 0){
				a++;
			    }else{
				b++;
			    }			    
			}
		    }

		}

		if(log2(2 + ( a / std::max(long(1), b) )) > RF[j]){
		    RF[j] = log2(2 + ( a / std::max(long(1), b) ));
		}
//		printf("%d - %.6g\t%d\t%d\n", category, RF(j,0), a, b);
//		category++;

	    }	   


	}

	map<long, float> IDF;

	/*Calcula o IDF dos termos*/
	for(int j = 0; j < c; j++){
	    long soma = 0;
	    for(int i = 0; i < l; i++){
		if( dataSet[i][j] > 0){
		    soma++;
		}
	    }
	    IDF[j] = log2(l / soma);
	}

	ofstream freqFile("./matrix.rfxidf");
	for(int i = 0; i < l; i++){
	    for(int j = 0; j < c; j++){
//		TF = 1 + log2( dataSet(i,j) );//TF normalizada baseado em Baeza-Yates
		TF = dataSet[i][j];//Frequencia bruta baseado em Baeza-Yates

		float divisor = 0;
		if(norm == 1 && TF != 0){//Normalizar
		    for(int k = 0; k < c; k++){
			divisor+= pow((TF * RF[k] * IDF[k]),2);
		    }

		    if((TF * RF[j] * IDF[j]) == 0){
			TFxRFxIDF = TF * RF[j] * IDF[j];//TFxIDF
		    }else{
			TFxRFxIDF = TF * RF[j] * IDF[j] / sqrt(divisor);//TFxIDF
		    }

		}else{
		    TFxRFxIDF = TF * RF[j] * IDF[j];//TFxRF
		}

	        freqFile << TFxRFxIDF << "\t";
	    }
	    freqFile << endl;
	}
	freqFile.close();

}

void weightsQUIQUAD(long l, long c, long norm){//Quiquadado
	int C = labelsAccountability.size();
	map<long, float > QUIQUAD;
	float soma = 0, TFxQUIQUAD, TF;

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

//		printf("%d - %.6g\t%d\t%d\t%d\t%d\t%d\n", category, QUIQUAD(j, category), a, b, v_c, d, divisor);
//		category++;

	    }	    

	}

//printf("Vai gravar!\n");

	ofstream freqFile("./matrix.quiquad");
	for(int i = 0; i < l; i++){
	    for(int j = 0; j < c; j++){
//		TF = 1 + log2( dataSet(i,j) );//TF normalizada baseado em Baeza-Yates
		TF = dataSet[i][j];//Frequencia bruta baseado em Baeza-Yates

		float divisor = 0;
		if(norm == 1 && TF != 0){//Normalizar
		    for(int k = 0; k < c; k++){
			divisor+= pow((TF * QUIQUAD[k]),2);
		    }

		    if((TF * QUIQUAD[j]) == 0){
			TFxQUIQUAD = TF * QUIQUAD[j];//TFxIDF
		    }else{
			TFxQUIQUAD = TF * QUIQUAD[j] / sqrt(divisor);//TFxIDF
		    }

		}else{
		    TFxQUIQUAD = TF * QUIQUAD[j];//TFxRF
		}

	        freqFile << TFxQUIQUAD << "\t";
	    }
	    freqFile << endl;
	}
	freqFile.close();

}

#endif /* UTILS_H_ */
