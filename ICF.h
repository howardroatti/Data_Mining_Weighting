#ifndef ICF_H_
#define ICF_H_

#include "Utils.h"

void weightsICF(long l, long c, long norm){
	map<long, float> ICF;
	int C = labelsAccountability.size();
	float TFxICF, TF;

	/*Calculando o ICF*/
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
				ICF[j] = log2(1 + ( C / c_i ));

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
			ICF[j] = log2(1 + ( C / c_i ));

		}

	}

	ofstream freqFile("./matrix.icf");
	for(int i = 0; i < l; i++){
		for(int j = 0; j < c; j++){
			if(colRemove.size() > 0){//Base Reduzida
				if( find(colRemove.begin(), colRemove.end(), j) == colRemove.end() ){

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
			}else{//Base Normal

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

		}
		freqFile << endl;
	}
	freqFile.close();

}

#endif /* ICF_H_ */
