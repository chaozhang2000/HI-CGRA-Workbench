#include"CGRA.h"
#include <iostream>
#define SIZE 200
int data1[SIZE] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20
};
int result[SIZE];

void loaddataforkernel(CGRA* cgra){
	cgra->datamems[0]->writeDatas(data1,0,SIZE);
}
void printresult(CGRA*cgra){
	std::cout<<"Print result:"<<std::endl;
	for(int i = 0; i<SIZE-4;i++){
		std::cout<<"address = "<<i<<" data = "<<cgra->datamems[1]->readData(i)<<std::endl;
	} 
}


void firkernel(int *line1,int *result, long k){
	int kernel[5]= {1,2,3,2,1};
	result[k-2] = line1[k-2]*kernel[0] + line1[k-1]*kernel[1] + line1[k]*kernel[2] + line1[k+1]*kernel[3] + line1[k+2]*kernel[4];
}

void checkresult(CGRA*cgra){
  long i;
  for (i=2;i<SIZE-2;i++){
  		firkernel(data1,result,i);
  }
	for(i = 0; i<SIZE-4;i++){
			if(result[i] != cgra->datamems[1]->readData(i)){
				std::cout<<"sim result is different from the theoretical results"<<std::endl;
				std::cout<<"firresult["<<i<<"]"<<" equal "<<cgra->datamems[1]->readData(i) << "shoule be "<<result[i]<<std::endl;
				std::cout<<"Sim not PASS!"<<std::endl;
				return;
			}
	} 
	std::cout<<"Sim result equal theoretical results"<<std::endl;
	std::cout<<"Sim PASS!!!"<<std::endl;

}
