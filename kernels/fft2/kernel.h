#include"CGRA.h"
#include <iostream>
#define PI 3.1415926
#define TIMES 10
#define LEN 8
																								 
int butterfly_i[LEN*2] = {4096,0,0,0,0,0,0,0,-4096,0,0,0,0,0,0,0};
int butterfly_o[LEN*2] = {0};
int w[LEN] = {1024,0,724,-724,0,-1023,-724,-724};


void loaddataforkernel(CGRA* cgra){
	cgra->datamems[0]->writeDatas(butterfly_i,0,LEN*2);
	cgra->datamems[2]->writeDatas(w,0,LEN);
}
void printresult(CGRA*cgra){
	std::cout<<"Print result:"<<std::endl;
	for(int i = 0; i<LEN*2;i++){
		std::cout<<"address = "<<i<<" data = "<<cgra->datamems[1]->readData(i)<<std::endl;
	} 
}


/*
 * j = 2
 * p = 2
 * q = 6
 * p_rindex = 4
 * p_iindex = 5
 * q_iindex =	13 
 * q_rindex = 12
 * w_rindex = 4
 * w_iindex = 5
 * out[5] = in[5] + ((in[12]*w[5] + w[4]*in[13])) = 
 *
 * Node8  4
 * Node9->CGRANode0->cycle21 load 0
 * Node11 5
 * Node12->CGRANode2->cycle23 load 0
 * Node14 12
 * Node15->CGRANode3->cycle22 load 1
 * Node17 13
 * Node18->CGRANode2->cycle24 load 0
 * Node20 4
 * Node21->CGRANode9->cycle20 load 0
 * Node23 5
 * Node24->CGRANode9->cycle21 load 0 should be -1023
 */
void fftkernel(int *in,int *w,int *out,int j, int k){
	int p = k+j;
	int q = k + j + (8>>1);
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
        for(int k=0;k<8;k+=8)
        {
        	for( int j=0;j<8/2;j++)
            {
							fftkernel(butterfly_i,w,butterfly_o,j,k);
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
