#include"CGRA.h"
#include <iostream>
#define PI 3.1415926
#define TIMES 10
#define LEN 1024
																								 
int butterfly_i[LEN*2] = {1024,0,1024,0,1024,0,1024,0,-1024,0,-1024,0,-1024,0,-1024,0};
int butterfly_o[LEN*2] = {0};
int w[LEN] = {1024,0,724,-724,0,-1023,-724,-724};
int m[LEN] = {8};


void loaddataforkernel(CGRA* cgra){
	cgra->datamems[0]->writeDatas(butterfly_i,0,LEN*2);
	cgra->datamems[2]->writeDatas(w,0,LEN);
	cgra->datamems[3]->writeDatas(m,0,1);
}
void printresult(CGRA*cgra){
	std::cout<<"Print result:"<<std::endl;
	for(int i = 0; i<LEN*2;i++){
		std::cout<<"address = "<<i<<" data = "<<cgra->datamems[1]->readData(i)<<std::endl;
	} 
}


/*
 * k=0
 * j=0
 * p=0
 * q=1
 * w_rindex = 0
 * w_iindex = 1
 * p_rindex = 0
 * p_iindex = 1
 * q_rindex = 2
 * q_iindex = 3
 * out[0] = in[0] + ((in[2]*w[0] - in[3]*w[1])>>10)
 * out[0] = 1024 + ((1024 * 1024 - 0*0) >> 10 ) = 2048
 *
 * DFGNode9->CGRANode0 cycle5 load 1024
 * DFGNode12->CGRANode2 cycle7 load 0
 * DFGNode14->CGRANode11 cycle4 getp	2
 * DFGNode15->CGRANode3 cycle6 load 1024
 * DFGNode18->CGRANode2 cycle8 load 0
 * DFGNode21->CGRANode9 cycle4 load 1024
 * DFGNode22->CGRANode9 cycle5 load 0
 * */
void fftkernel(int *in,int *w,int *out,int *m,int j, int k){
	int p = k+j;
	int q = k + j + (*m>>1);
	int w_rindex = j*2;
	int w_iindex = j*2+1;
	int p_rindex = p*2;
	int p_iindex = p*2+1;
	int q_rindex = q*2;
	int q_iindex = q*2+1;
	
	int inpr = in[p_rindex];
	int inpi = in[p_iindex];
	int inqr = in[q_rindex];
	int inqi = in[q_iindex];
	int wr = w[w_rindex];
	int wi = w[w_iindex];
    out[p_rindex] = inpr + ((inqr *wr - inqi * wi)>>TIMES);
		out[p_iindex] = inpi + ((inqr* wi + wr*inqi)>>TIMES);
    out[q_rindex] = inpr + ((- inqr *wr + inqi * wi)>>TIMES);
		out[q_iindex] = inpi+(( - inqr* wi - wr*inqi)>>TIMES);
}

void checkresult(CGRA*cgra){
        for(int k=0;k<1024;k+=2)
        {
        	for( int j=0;j<2/2;j++)
            {
							fftkernel(butterfly_i,w,butterfly_o,m,j,k);
            }
        }
	for(int i = 0; i<LEN*2;i++){
			if(butterfly_o[i] != cgra->datamems[1]->readData(i)){
				std::cout<<"sim result is different from the theoretical results"<<std::endl;
				std::cout<<"butterfly_o["<<i<<"]"<<" equal "<<cgra->datamems[1]->readData(i) << "shoule be "<<butterfly_o[i]<<std::endl;
				std::cout<<"Sim not PASS!"<<std::endl;
				std::cout<<"correct result should be:"<<std::endl;
				for(int j = 0; j<LEN*2;j++){
					std::cout<<"address = "<<j<<" data = "<<butterfly_o[j]<<std::endl;
				}
				return;
			}
	} 
	std::cout<<"Sim result equal theoretical results"<<std::endl;
	std::cout<<"Sim PASS!!!"<<std::endl;

}
