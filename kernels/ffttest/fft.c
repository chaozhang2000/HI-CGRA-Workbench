#define TIMES 10
void kernel(int *inr,int *ini,int *outr,int*outi,int *Wr,int *Wi,int j, int k){
	int p = k+j;
	int q = k + j + (2>>1);
	int w_iindex = j*2+1;
	
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
