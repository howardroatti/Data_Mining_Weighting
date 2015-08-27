#ifndef QUIQUAD_H_
#define QUIQUAD_H_

#include "Utils.h"

void weightsQUIQUAD(long l, long c, long norm){//Quiquadado
	map<long, float > QUIQUAD;
	float soma = 0, TF, tamClass, A, B, C, D, probabilidade_doc_in_classe, divisor, totalOcorrencias;

	for(int j = 0; j < c; j++){
		totalOcorrencias = 0;
		for(int i = 0; i < c; i++){//Contabiliza as ocorrencias do termo na base
			if( dataSet[i][j] != 0 ){
				totalOcorrencias++;
			}
		}
		QUIQUAD[j] = 0;

		if(colRemove.size() > 0){
			if( find(colRemove.begin(), colRemove.end(), j) == colRemove.end() ){
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
						if(((float)(l*pow((A*D)-(C*B),2)) / (float)(divisor)) > QUIQUAD[j])
							QUIQUAD[j] = ((float)(l*pow((A*D)-(C*B),2)) / (float)(divisor));
					}else{
						QUIQUAD[j] = 1;
					}

				}
			}
		}else{
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
					if(((float)(l*pow((A*D)-(C*B),2)) / (float)(divisor)) > QUIQUAD[j])
						QUIQUAD[j] = ((float)(l*pow((A*D)-(C*B),2)) / (float)(divisor));
				}else{
					QUIQUAD[j] = 1;
				}

			}
		}
	
	}

	//printf("Vai gravar!\n");
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
							dataSet[i][j] = TF * QUIQUAD[j];//TFxIDF
						}else{
							dataSet[i][j] = TF * QUIQUAD[j] / sqrt(divisor);//TFxIDF
						}

					}else{
						dataSet[i][j] = TF * QUIQUAD[j];//TFxRF
					}
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
						dataSet[i][j] = TF * QUIQUAD[j];//TFxIDF
					}else{
						dataSet[i][j] = TF * QUIQUAD[j] / sqrt(divisor);//TFxIDF
					}

				}else{
					dataSet[i][j] = TF * QUIQUAD[j];//TFxRF
				}
			}
		}
	}
}

#endif /* QUIQUAD_H_ */
