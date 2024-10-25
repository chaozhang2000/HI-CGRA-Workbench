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
	for(int i = 1; i<=18;i++){
		std::cout<<"address = "<<i<<" data = "<<cgra->datamems[3]->readData(i)<<std::endl;
	} 
}


void conv3kernel(int *line1,int *line2,int *line3,int *result, long k){
	int kernel[3][3]= {{1,2,3},{4,5,6},{7,8,9}};
	result[k] = line1[k-1]*kernel[0][0] + line1[k]*kernel[0][1] + line1[k+1]*kernel[0][2]
					+line2[k-1]*kernel[1][0] + line2[k]*kernel[1][1] + line2[k+1]*kernel[1][2]
					+line3[k-1]*kernel[2][0] + line3[k]*kernel[2][1] + line3[k+1]*kernel[2][2];
}

void checkresult(CGRA*cgra){
  long i;
  for (i=1;i<19;i++){
  		conv3kernel(conv3data1,conv3data1,conv3data1,conv3result,i);
  }
	for(i = 1; i<19;i++){
			if(conv3result[i] != cgra->datamems[3]->readData(i)){
				std::cout<<"sim result is different from the theoretical results"<<std::endl;
				std::cout<<"conv3result["<<i<<"]"<<" equal "<<cgra->datamems[3]->readData(i) << "shoule be "<<conv3result[i]<<std::endl;
				std::cout<<"Sim not PASS!"<<std::endl;
				return;
			}
	} 
	std::cout<<"Sim result equal theoretical results"<<std::endl;
	std::cout<<"Sim PASS!!!"<<std::endl;

}
