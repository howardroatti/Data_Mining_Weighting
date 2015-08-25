#ifndef RFXIDF_H_
#define RFXIDF_H_

#include "Utils.h"

void weightsRFxIDF(long l, long c, long norm){//Relevance Frequency x IDF
	int C = labelsAccountability.size();
	map<long, float > RF;
	float soma = 0, TFxRFxIDF, TF;

	for(int j = 0; j < c; j++){
		long a = 0, b = 0, category = 0;
		RF[j] = -1;
		if(colRemove.size() > 0){//Base Reduzida
			if( find(colRemove.begin(), colRemove.end(), j) == colRemove.end() ){

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

		}else{//Base Normal

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

	}

	map<long, float> IDF;

	/*Calcula o IDF dos termos*/
	for(int j = 0; j < c; j++){
		long soma = 0;

		if(colRemove.size() > 0){//Base Reduzida
			if( find(colRemove.begin(), colRemove.end(), j) == colRemove.end() ){

				for(int i = 0; i < l; i++){
					if( dataSet[i][j] > 0){
						soma++;
					}
				}
				IDF[j] = log2(l / soma);

			}
		}else{//Base Normal

			for(int i = 0; i < l; i++){
				if( dataSet[i][j] > 0){
					soma++;
				}
			}
			IDF[j] = log2(l / soma);

		}

	}

	for(int i = 0; i < l; i++){
		for(int j = 0; j < c; j++){
			if(colRemove.size() > 0){//Base Reduzida
				if( find(colRemove.begin(), colRemove.end(), j) == colRemove.end() ){

					//		TF = 1 + log2( dataSet(i,j) );//TF normalizada baseado em Baeza-Yates
					TF = dataSet[i][j];//Frequencia bruta baseado em Baeza-Yates

					float divisor = 0;
					if(norm == 1 && TF != 0){//Normalizar
						for(int k = 0; k < c; k++){
							divisor+= pow((TF * RF[k] * IDF[k]),2);
						}

						if((TF * RF[j] * IDF[j]) == 0){
							dataSetOut[i][j] = TF * RF[j] * IDF[j];//TFxIDF
						}else{
							dataSetOut[i][j] = TF * RF[j] * IDF[j] / sqrt(divisor);//TFxIDF
						}

					}else{
						dataSetOut[i][j] = TF * RF[j] * IDF[j];//TFxRF
					}
				}
			}else{//Base Normal

				//		TF = 1 + log2( dataSet(i,j) );//TF normalizada baseado em Baeza-Yates
				TF = dataSet[i][j];//Frequencia bruta baseado em Baeza-Yates

				float divisor = 0;
				if(norm == 1 && TF != 0){//Normalizar
					for(int k = 0; k < c; k++){
						divisor+= pow((TF * RF[k] * IDF[k]),2);
					}

					if((TF * RF[j] * IDF[j]) == 0){
						dataSetOut[i][j] = TF * RF[j] * IDF[j];//TFxIDF
					}else{
						dataSetOut[i][j] = TF * RF[j] * IDF[j] / sqrt(divisor);//TFxIDF
					}

				}else{
					dataSetOut[i][j] = TF * RF[j] * IDF[j];//TFxRF
				}
			}
		}
	}
}

#endif /* RFXIDF_H_ */
