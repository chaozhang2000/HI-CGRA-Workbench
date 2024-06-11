#include"CGRA.h"
#include <iostream>
int conv3data1[20] ={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
int conv3data2[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
int conv3data3[20] ={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
int conv3result[20];

void loaddataforkernel(CGRA* cgra){
	cgra->datamems[0]->writeDatas(conv3data1,0,20);
	cgra->datamems[1]->writeDatas(conv3data2,0,20);
	cgra->datamems[2]->writeDatas(conv3data3,0,20);
}
void printresult(CGRA*cgra){
	std::cout<<"Print result:"<<std::endl;
	for(int i = 0; i<20;i++){
		std::cout<<"address = "<<i<<" data = "<<cgra->datamems[3]->readData(i)<<std::endl;
	} 
}


void sumkernel(int *line1,int *line2,int *line3,int *result, long k){
	result[k] = line1[k] + line2[k] + line3[k];
}

void checkresult(CGRA*cgra){
  long i;
  for (i=0;i<20;i++){
  		sumkernel(conv3data1,conv3data1,conv3data1,conv3result,i);
  }
	for(i = 0; i<20;i++){
			if(conv3result[i] != cgra->datamems[3]->readData(i)){
				std::cout<<"sim result is different from the theoretical results"<<std::endl;
				std::cout<<"Sim not PASS!"<<std::endl;
				return;
			}
	} 
	std::cout<<"Sim result equal theoretical results"<<std::endl;
	std::cout<<"Sim PASS!!!"<<std::endl;

}
