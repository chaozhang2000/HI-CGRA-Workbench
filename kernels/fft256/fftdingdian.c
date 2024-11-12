	#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
 
 
#define PI 3.1415926
#define TIMES 10
#define LEN 256
struct complex
{
    int real;
    int image;
};
int reverse_num(int l,int oringin_num);          //反位序
struct complex complex_add(struct complex c1,struct complex c2);
struct complex complex_sub(struct complex c1,struct complex c2);
struct complex complex_multi(struct complex c1,struct complex c2);
struct complex rotation_factor(int N,int n,int k);
void fft(int len, struct complex in_x[],struct complex out_y[]);
void kernel(int *in,int *w,int *out,int* m,int k, int j);
 
 
 
 
int main()
{
    int sam[LEN] = {1<<TIMES,-1<<TIMES,1<<TIMES,-1<<TIMES,1<<TIMES,-1<<TIMES,1<<TIMES,-1<<TIMES};
    int jhg[LEN] = {0,0,0,0,0,0,0,0};
    struct complex x[256];
    struct complex y[256];

		struct complex w[128];
		struct complex y1[256];
    struct complex x_reverse[256];
 
    for(int i=0;i<256;i++)
    {
        x[i].real = sam[i];
        x[i].image = jhg[i];
    }
    printf("时域序列\n");
 
    for(int i=0;i<256;i++)
    {
        printf("(%d, %di) \n",x[i].real,x[i].image);
    }
    printf("换顺序\n");

    for(int k=0;k<256;k++)
    {
        x_reverse[k] = x[reverse_num(log2(256),k)];       //反位序号操作
        printf("(%d, %di) \n",x_reverse[k].real,x_reverse[k].image);
    }
    printf("旋转因子\n");

		for(int i = 0; i<128; i++){
    	w[i] = rotation_factor(256,1,i);
       printf("(%d, %di)\n",w[i].real,w[i].image);

		}
 
    fft(256,x,y);
 
    printf("频域序列\n");
 
    for(int i=0;i<256;i++)
    {
        printf("%d(%d, %di)\n",i,y[i].real,y[i].image);
    }


 
				for(int i = 0; i<log2(256);i++){
								int m = 1 << (i+1);
        for(int k=0;k<256;k+=m)
        {
        	for( int j=0;j<m/2;j++)
            {
							if(i%2 ==0)kernel((int*)x_reverse,(int*)w,(int*)y1,&m,k,j);
							if(i%2 ==1)kernel((int*)y1,(int*)w,(int*)x_reverse,&m,k,j);
            }
        }
				}
    for(int i=0;i<256;i++)
    {
        printf("(%d, %di)\n",y1[i].real,y1[i].image);
    }

		for(int i =0;i<256;i++){
			if((x_reverse[i].real != y[i].real)||(x_reverse[i].image != y[i].image)){
        printf("y %d(%d, %di) != y1 %d(%d, %di)\n",i,y[i].real,y[i].image,i,x_reverse[i].real,x_reverse[i].image);
			}
		}
 
    return 0;
}
 
struct complex complex_add(struct complex c1,struct complex c2)     //复数加法
{
    struct complex p;
 
    p.real = c1.real + c2.real;
    p.image = c1.image + c2.image;
 
    return p;
 
}
 
struct complex complex_sub(struct complex c1,struct complex c2)     //复数减
{
    struct complex p;
 
    p.real = c1.real - c2.real;
    p.image = c1.image - c2.image;
 
    return p;
 
}
 
struct complex complex_multi(struct complex c1,struct complex c2)  //复数乘法
{
    struct complex c3;
    c3.real=(c1.real*c2.real - c1.image *c2.image)>>TIMES;
    c3.image = (c2.real* c1.image + c1.real*c2.image)>>TIMES;
 
    return c3;
};
 
 
 
struct complex rotation_factor(int N,int n,int k)       //旋转因子
{
    struct complex w;
 
    w.real = (int)(cos(2* PI * n * k /N)*pow(2,TIMES));
    w.image =(int)(- sin(2* PI * n * k /N)*pow(2,TIMES));
    return w;
}
 
 
 
int reverse_num(int l,int oringin_num)          //反位序
{
    int q=0,m=0;
 
    for(int k=l-1;k>=0;k--)
    {
        q = oringin_num % 2;
        m += q*(1<<k);
        oringin_num = oringin_num / 2;
    }
 
    return m;
}
 
 
void fft(int len, struct complex in_x[],struct complex out_y[])
{
    /*
        param len 序列长度，目前只能是2的指数
        param in_x输入的序列
        param out_y输出的序列
    */
 
    int l,k,r,z,dist,q,j;       //l是级
    struct complex w,tmp;
    struct complex in_x_mem[len];
 
    l = log2(len);
 
    for(k=0;k<len;k++)
    {
        in_x_mem[k] = in_x[reverse_num(l,k)];       //反位序号操作
    }
 
    for(r = 0;r<l;r++)      //遍历每一级蝶形运算
    {
 
        int m  = 1<<(r+1);        //提前计算每一级的间隔距离
 
        for(k=0;k<len;k+=m)
        {
        for( j=0;j<m/2;j++)
            {
                q = k+j+m/2; 
                w = rotation_factor(len,1,j);
                tmp = in_x_mem[k+j];
                in_x_mem[k+j] = complex_add( in_x_mem[k+j] , complex_multi(w, in_x_mem[q]) );
                in_x_mem[q] = complex_sub(tmp , complex_multi(w, in_x_mem[q]) );
								if(r == 1){
								if(q==2){
									printf("q=2\n");
        					printf("in_x_mem k+j%d(%d, %di)\n",k+j,in_x_mem[k+j].real,in_x_mem[k+j].image);
        					printf("in_x_mem q(%d, %di)\n",in_x_mem[q].real,in_x_mem[q].image);
        					printf("w(%d, %di)\n",w.real,w.image);
								}
								if(k+j==2){
									printf("k+j=2\n");
        					printf("in_x_mem k+j(%d, %di)\n",in_x_mem[k+j].real,in_x_mem[k+j].image);
        					printf("in_x_mem q%d(%d, %di)\n",q,in_x_mem[q].real,in_x_mem[q].image);
        					printf("w(%d, %di)\n",w.real,w.image);
								}
								}
            }
        }
    }
    memcpy(out_y,in_x_mem,len*sizeof(struct complex));
}

/*
void kernel(int *in,int *w,int *out,int* m,int k, int j){
	int p = k+j;
	int q = k + j + (*m)/2;
	int wr = w[j*2];
	int wi = w[j*2+1];
    out[2*p] = in[p*2] + ((in[q*2] *wr - in[q*2+1] * wi)>>TIMES);
		out[2*p+1] = in[p*2+1] + ((in[q*2]* wi + wr*in[q*2+1])>>TIMES);
    out[q*2] = in[p*2] + ((- in[q*2] *wr + in[q*2+1] * wi)>>TIMES);
		out[q*2+1] = in[p*2+1]+(( - in[q*2]* wi - wr*in[q*2+1])>>TIMES);
}
void kernel(int *in,int *w,int *out,int* m,int k, int j){
	int p = k+j;
	int q = k + j + ((*m) >> 1);
	int w_rindex = j*2;
	int w_iindex = j*2+1;
	int wr = w[w_rindex];
	int wi = w[w_iindex];
	int p_rindex = p*2;
	int p_iindex = p*2+1;
	int q_rindex = q*2;
	int q_iindex = q*2+1;
    out[p_rindex] = in[p_rindex] + ((in[q_rindex] *wr - in[q_iindex] * wi)>>TIMES);
		out[p_iindex] = in[p_iindex] + ((in[q_rindex]* wi + wr*in[q_iindex])>>TIMES);
    out[q_rindex] = in[p_rindex] + ((- in[q_rindex] *wr + in[q_iindex] * wi)>>TIMES);
		out[q_iindex] = in[p_iindex]+(( - in[q_rindex]* wi - wr*in[q_iindex])>>TIMES);
}
*/
void kernel(int *in,int *w,int *out,int* m,int k, int j){
	int p = k+j;
	int q = k + j + ((*m)>>1);
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
/*
void kernel(struct complex *in,struct complex *w,struct complex *out,int *dist,int k, int j,int r){
	int l = 3;
	int q = k + dist[0];
	int z = k << (l - r -1);
	struct complex w0 = w[z];
    out[k].real = in[k].real + ((in[q].real *w0.real - in[q].image * w0.image)>>TIMES);
		out[k].image = in[k].image + ((in[q].real* w0.image + w0.real*in[q].image)>>TIMES);
    out[q].real = in[k].real + ((- in[q].real *w0.real + in[q].image * w0.image)>>TIMES);
		out[q].image = in[k].image+(( - in[q].real* w0.image - w0.real*in[q].image)>>TIMES);
}
*/
