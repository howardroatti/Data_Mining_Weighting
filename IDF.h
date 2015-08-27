#ifndef IDF_H_
#define IDF_H_

#include "Utils.h"

void weightsIDF(long l, long c, long norm){

	map<long, float> IDF;
	float TFxIDF, TF;

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

	/*Calcula o TFxIDF e armazena em arquivo*/
	for(int i = 0; i < l; i++){
		for(int j = 0; j < c; j++){
			if(colRemove.size() > 0){//Base Reduzida
				if( find(colRemove.begin(), colRemove.end(), j) == colRemove.end() ){

					//TF = 1 + log2( dataSet(i,j) );//TF normalizada baseado em Baeza-Yates
					TF = dataSet[i][j];//Frequencia bruta baseado em Baeza-Yates

					float divisor = 0;
					if(norm == 1 && TF != 0){//Normalizar
						for(int k = 0; k < c; k++){
							divisor+= pow((TF * IDF[k]),2);
						}

						if((TF * IDF[j]) == 0){
							dataSet[i][j] = TF * IDF[j];//TFxIDF
						}else{
							dataSet[i][j] = TF * IDF[j] / sqrt(divisor);//TFxIDF
						}

					}else{
						dataSet[i][j] = TF * IDF[j];//TFxIDF
					}
				}
			}else{//Base Normal

				//TF = 1 + log2( dataSet(i,j) );//TF normalizada baseado em Baeza-Yates
				TF = dataSet[i][j];//Frequencia bruta baseado em Baeza-Yates

				float divisor = 0;
				if(norm == 1 && TF != 0){//Normalizar
					for(int k = 0; k < c; k++){
						divisor+= pow((TF * IDF[k]),2);
					}

					if((TF * IDF[j]) == 0){
						dataSet[i][j] = TF * IDF[j];//TFxIDF
					}else{
						dataSet[i][j] = TF * IDF[j] / sqrt(divisor);//TFxIDF
					}

				}else{
					dataSet[i][j] = TF * IDF[j];//TFxIDF
				}
			}
		}
	}
}

#endif /* IDF_H_ */
