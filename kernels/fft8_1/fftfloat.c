	#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
 
 
#define PI 3.1415926
struct complex
{
    double real;
    double image;
};
int reverse_num(int l,int oringin_num);          //反位序
struct complex complex_add(struct complex c1,struct complex c2);
struct complex complex_sub(struct complex c1,struct complex c2);
struct complex complex_multi(struct complex c1,struct complex c2);
struct complex rotation_factor(int N,int n,int k);
double mold_length(struct complex c);
void fft(int len, struct complex in_x[],struct complex out_y[]);
void kernel(struct complex *in,struct complex *w,struct complex *out,int *dist,int k, int j,int r);
 
 
 
 
int main()
{
    int sam[8] = {1,-1,1,-1,1,-1,1,-1};
    int jhg[8] = {0,0,0,0,0,0,0,0};
    struct complex x[8];
    struct complex y[8];

		struct complex w[25];
		struct complex y1[8];
    struct complex x_reverse[8];
 
    for(int i=0;i<8;i++)
    {
        x[i].real = sam[i];
        x[i].image = jhg[i];
    }
    printf("时域序列\n");
 
    for(int i=0;i<8;i++)
    {
        printf("(%.2f, %.2fi) \n",x[i].real,x[i].image);
    }

    for(int k=0;k<8;k++)
    {
        x_reverse[k] = x[reverse_num(log2(8),k)];       //反位序号操作
    }
 
    fft(8,x,y);
 
    printf("频域序列\n");
 
    for(int i=0;i<8;i++)
    {
        printf("(%.2f, %.2fi)\n",y[i].real,y[i].image);
    }

		for(int i = 0; i<25; i++){
    	w[i] = rotation_factor(8,1,i);
        printf("w%d(%.2f, %.2fi) \n",i,w[i].real,w[i].image);
		}
		int dist;

 
        dist = 1<<0;        //提前计算每一级的间隔距离
				for(int j = 0; j<dist;j++){
					for(int k = j; k<8;k+=(dist<<1)){
						kernel(x_reverse,w,y1,&dist,k,j,0);
					}
				}
        dist = 1<<1;        //提前计算每一级的间隔距离
				for(int j = 0; j<dist;j++){
					for(int k = j; k<8;k+=(dist<<1)){
						kernel(y1,w,x_reverse,&dist,k,j,1);
					}
				}
        dist = 1<<2;        //提前计算每一级的间隔距离
				for(int j = 0; j<dist;j++){
					for(int k = j; k<8;k+=(dist<<1)){
						kernel(x_reverse,w,y1,&dist,k,j,2);
					}
				}
    for(int i=0;i<8;i++)
    {
        printf("(%.2f, %.2fi)\n",y1[i].real,y1[i].image);
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
    c3.real=c1.real*c2.real - c1.image *c2.image;
    c3.image = c2.real* c1.image + c1.real*c2.image;
 
    return c3;
};
 
 
 
struct complex rotation_factor(int N,int n,int k)       //旋转因子
{
    struct complex w;
 
    w.real = cos(2* PI * n * k /N);
    w.image = - sin(2* PI * n * k /N);
 
    return w;
}
 
double mold_length(struct complex c)        //幅度
{
    return sqrt(c.real * c.real + c.image * c.image);
};
 
 
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
 
        dist = 1<<r;        //提前计算每一级的间隔距离
 
        for( j=0;j<dist;j++ )      //计算策略是拆成上下两组，先上计算，后下计算，j是计算的起始序号
        {
            for(k=j;k<len;k+=(dist<<1))     //不好解释，得画图理解
            {
                q = k+dist; //q同一组蝶形运算第二个序号
                z = k << (l - r -1);    //确定旋转因子的指数

 
                w = rotation_factor(len,1,z);
                //由于不是并行计算，必须先缓存
                tmp = in_x_mem[k];
 
                in_x_mem[k] = complex_add( in_x_mem[k] , complex_multi(w, in_x_mem[q]) );
                in_x_mem[q] = complex_sub(tmp , complex_multi(w, in_x_mem[q]) );
 
            }
        }
    }
    memcpy(out_y,in_x_mem,len*sizeof(struct complex));
}

void kernel(struct complex *in,struct complex *w,struct complex *out,int *dist,int k, int j,int r){
	int l = 3;
	int q = k + dist[0];
	int z = k << (l - r -1);
	struct complex w0 = w[z];
    out[k].real = in[k].real + in[q].real *w0.real - in[q].image * w0.image;
		out[k].image = in[k].image + in[q].real* w0.image + w0.real*in[q].image;
    out[q].real = in[k].real - in[q].real *w0.real + in[q].image * w0.image;
		out[q].image = in[k].image - in[q].real* w0.image - w0.real*in[q].image;
}
