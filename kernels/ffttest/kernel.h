#include"CGRA.h"
#include <iostream>
#define PI 3.1415926
#define TIMES 10
#define LEN 8
																								 
int inr[8] = {1024,1024,1024,1024,-1024,-1024,-1024,-1024};
int ini[8] = {0};
int wr[4] = {1024,724,0,-724};
int wi[4] = {0,-724,-1023,-724};
int outr[8] = {0};
int outi[8] = {0};
int m[1] = {2};


void loaddataforkernel(CGRA* cgra){
	cgra->datamems[0]->writeDatas(inr,0,8);
	cgra->datamems[1]->writeDatas(ini,0,8);
	cgra->datamems[4]->writeDatas(wr,0,4);
	cgra->datamems[5]->writeDatas(wi,0,4);
}
void printresult(CGRA*cgra){
	std::cout<<"Print result:"<<std::endl;
	for(int i = 0; i<8;i++){
		std::cout<<"datamem2:address = "<<i<<" data = "<<cgra->datamems[2]->readData(i)<<std::endl;
		std::cout<<"datamem3:address = "<<i<<" data = "<<cgra->datamems[3]->readData(i)<<std::endl;
	} 
}


void fftkernel(int *inr,int *ini,int *outr,int*outi,int *Wr,int *Wi,int j, int k){
	int p = k+j;
	int q = k + j + (2>>1);
	
	int inpr = inr[p];
	int inpi = ini[p];
	int inqr = inr[q];
	int inqi = ini[q];
	int wr = Wr[j];
	int wi = Wi[j];
    outr[p] = inpr + ((inqr *wr - inqi * wi)>>TIMES);
		outi[p] = inpi + ((inqr* wi + wr*inqi)>>TIMES);
    outr[q] = inpr + ((- inqr *wr + inqi * wi)>>TIMES);
		outi[q] = inpi+(( - inqr* wi - wr*inqi)>>TIMES);
}

void checkresult(CGRA*cgra){
        for(int k=0;k<8;k+=m[0])
        {
        	for( int j=0;j<m[0]/2;j++)
            {
							fftkernel(inr,ini,outr,outi,wr,wi,j,k);
            }
        }
	for(int i = 0; i<LEN;i++){
			if(outr[i] != cgra->datamems[2]->readData(i)){
				std::cout<<"sim result is different from the theoretical results"<<std::endl;
				std::cout<<"outr["<<i<<"]"<<" equal "<<cgra->datamems[2]->readData(i) << "shoule be "<<outr[i]<<std::endl;
				std::cout<<"Sim not PASS!"<<std::endl;
				std::cout<<"correct result should be:"<<std::endl;
				for(int j = 0; j<LEN;j++){
					std::cout<<"address = "<<j<<" data = "<<outr[j]<<std::endl;
				}
				return;
			}
	} 
	for(int i = 0; i<LEN;i++){
			if(outi[i] != cgra->datamems[3]->readData(i)){
				std::cout<<"sim result is different from the theoretical results"<<std::endl;
				std::cout<<"outi["<<i<<"]"<<" equal "<<cgra->datamems[3]->readData(i) << "shoule be "<<outr[i]<<std::endl;
				std::cout<<"Sim not PASS!"<<std::endl;
				std::cout<<"correct result should be:"<<std::endl;
				for(int j = 0; j<LEN;j++){
					std::cout<<"address = "<<j<<" data = "<<outi[j]<<std::endl;
				}
				return;
			}
	} 
	std::cout<<"Sim result equal theoretical results"<<std::endl;
	std::cout<<"Sim PASS!!!"<<std::endl;

}
