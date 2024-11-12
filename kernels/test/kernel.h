#include"CGRA.h"
#include <iostream>
#define SIZE 3
int A1s[SIZE][SIZE] = {{1,2,3},{6,7,8},{11,12,13}};
int B1s[SIZE][SIZE] = {{1,2,3},{6,7,8},{11,12,13}};
int resultABs[SIZE][SIZE];
int *A1 = (int*)A1s;
int *B1 = (int*)B1s;
int *resultAB1 = (int*)resultABs;

void loaddataforkernel(CGRA* cgra){
	cgra->datamems[0]->writeDatas(A1,0,SIZE*SIZE);
	cgra->datamems[1]->writeDatas(B1,0,SIZE*SIZE);
}
void printresult(CGRA*cgra){
	std::cout<<"Print result:"<<std::endl;
	for(int i = 0; i<SIZE*SIZE;i++){
		std::cout<<"address = "<<i<<" data = "<<cgra->datamems[2]->readData(i)<<std::endl;
	} 
}


void mm2kernel(int *A1, int *B1, int *resultAB1,long j,long i)
{
				resultAB1[i*SIZE+j] = A1[i*SIZE+0]*B1[j] + A1[i*SIZE+1]*B1[SIZE+j]+A1[i*SIZE+2]*B1[2*SIZE+j];
}

void checkresult(CGRA*cgra){
  long i;
	long j;
  for (i=0;i<SIZE;i++){
					for(j=0;j<SIZE;j++){
  								mm2kernel(A1, B1, resultAB1,j,i);//前面的参数是内层对应loop0开始,后面的是外层
					}
  }
	for(i = 0; i<SIZE * SIZE;i++){
			if(resultAB1[i] != cgra->datamems[2]->readData(i)){
				std::cout<<"sim result is different from the theoretical results"<<std::endl;
				std::cout<<"Sim not PASS!"<<std::endl;
				return;
			}
	} 
	std::cout<<"Sim result equal theoretical results"<<std::endl;
	std::cout<<"Sim PASS!!!"<<std::endl;

}
