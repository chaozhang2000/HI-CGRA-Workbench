#define TIMES 10
void kernel(int *in,int *w,int j, int k){
	int p = k+j;
	int q = k + j + (2>>1);
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
    in[p_rindex] = inpr + ((inqr *wr - inqi * wi)>>TIMES);
		in[p_iindex] = inpi + ((inqr* wi + wr*inqi)>>TIMES);
    in[q_rindex] = inpr + ((- inqr *wr + inqi * wi)>>TIMES);
		in[q_iindex] = inpi+(( - inqr* wi - wr*inqi)>>TIMES);
}
