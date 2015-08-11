#ifndef QUIQUAD_H_
#define QUIQUAD_H_

#include "Utils.h"

void weightsQUIQUAD(long l, long c, long norm){//Quiquadado
	int C = labelsAccountability.size();
	map<long, float > QUIQUAD;
	float soma = 0, TFxQUIQUAD, TF;

	for(int j = 0; j < c; j++){
		long a = 0, b = 0, v_c = 0, d = 0, category = 0;
		QUIQUAD[j] = -1;
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
		}else{

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

	}

	//printf("Vai gravar!\n");

	ofstream freqFile("./matrix.quiquad");
	for(int i = 0; i < l; i++){
		for(int j = 0; j < c; j++){
			if(colRemove.size() > 0){//Base Reduzida
				if( find(colRemove.begin(), colRemove.end(), j) == colRemove.end() ){

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
			}else{

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
		}
		freqFile << endl;
	}
	freqFile.close();

}

#endif /* QUIQUAD_H_ */
