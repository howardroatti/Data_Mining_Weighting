#ifndef ICFBASED_H_
#define ICFBASED_H_

#include "Utils.h"

void weightsICFBASED(long l, long c, long norm){//ICFBASED
	map<long, float> ICF;
	int C = labelsAccountability.size();
	map<long, float > ICFBASED;
	float soma = 0, TFxICFBASED, TF;

	/*ICF*/
	for(int j = 0; j < c; j++){
		long c_i = 0;
		if(colRemove.size() > 0){//Base Reduzida
			if( find(colRemove.begin(), colRemove.end(), j) == colRemove.end() ){
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
		}else{//Base Normal
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
	}

	/*RF*/
	for(int j = 0; j < c; j++){
		long a = 0, b = 0, category = 0;
		ICFBASED[j] = -1;
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

					if(log2(2 + ( a / std::max(long(1), b) ) * ICF[j]) > ICFBASED[j]){
						ICFBASED[j] = log2(2 + ( a / std::max(long(1), b) ) * ICF[j]);
					}
					//		printf("%d - %.6g\t%d\t%d\t%.6g\n", category, ICFBASED(j, category), a, b, ICF(j,0));
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

				if(log2(2 + ( a / std::max(long(1), b) ) * ICF[j]) > ICFBASED[j]){
					ICFBASED[j] = log2(2 + ( a / std::max(long(1), b) ) * ICF[j]);
				}
				//		printf("%d - %.6g\t%d\t%d\t%.6g\n", category, IdataSetOutCFBASED(j, category), a, b, ICF(j,0));
				//		category++;

			}

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
							divisor+= pow((TF * ICFBASED[k]),2);
						}

						if((TF * ICFBASED[j]) == 0){
							dataSet[i][j] = TF * ICFBASED[j];//TFxIDF
						}else{
							dataSet[i][j] = TF * ICFBASED[j] / sqrt(divisor);//TFxIDF
						}

					}else{
						dataSet[i][j] = TF * ICFBASED[j];//TFxRF
					}

				}
			}else{//Base Normal

				//		TF = 1 + log2( dataSet(i,j) );//TF normalizada baseado em Baeza-Yates
				TF = dataSet[i][j];//Frequencia bruta baseado em Baeza-Yates

				float divisor = 0;
				if(norm == 1 && TF != 0){//Normalizar
					for(int k = 0; k < c; k++){
						divisor+= pow((TF * ICFBASED[k]),2);
					}

					if((TF * ICFBASED[j]) == 0){
						dataSet[i][j] = TF * ICFBASED[j];//TFxIDF
					}else{
						dataSet[i][j] = TF * ICFBASED[j] / sqrt(divisor);//TFxIDF
					}

				}else{
					dataSet[i][j] = TF * ICFBASED[j];//TFxRF
				}


			}
		}
	}

}

#endif /* ICFBASED_H_ */
