#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>


#define p_num 16                 

typedef long long LLONG_INTEGER;
typedef unsigned long long US_LLONG_INTEGER;

void increase(int num,long a[])    
{
	int i,sum;
	srand((unsigned)time(NULL));    
    a[0]=1+rand()%num;
    sum=a[0];
    for(i=1;i<num;i++)
	{ 
	    a[i]=sum+1+rand()%num;
        sum=sum+a[i];
	}
}


int inv(int xin,int n0)        
{
	long n1,n2,q,r,b1,b2,t;
    if(xin==0)
       b2=0;
    else
	{ 
		n1=n0;n2=xin;b2=1;b1=0;
        do{
			r=(n1%n2);
            q=(n1-r)/n2;
            if(r==0)
			{
				if(b2<0) b2=n0+b2;
			}
            else
			{
				n1=n2;
				n2=r;
	            t=b2;
	            b2=b1-q*b2;
				b1=t;
			}

		}while(r!=0);
    }
	return(b2);
}


int gcd(long a,long b)
{
	long t;
    t=a;
    while(!((a%t==0)&&(b%t==0))) t--;
    return t;                          
}


void select_n_w(int num,long s[],int b[])     
{
    b[0]=2*s[num-1]+rand()%num+1;       
    while(1)                             
	{
		b[1]=rand()%9999+1;              
		if(gcd(b[1],b[0])==1) break;     
   }
}


void binary(short num,char *out)
{
	short a=1<<15;              
	char ch;
	int i=0;
	for(;i<16;i++)
	{
		ch=(num&a)?'1':'0';
		out[i]=ch;
		num<<=1;                
	}
}
          

int main()
{
	long sk[p_num]={0},pk[p_num]={0},b[2]={0,1};
	short buffer[20000],z[16],d_text[20000]={0};
	int w,N,i,j=0,k=0;
	long inv_w,len=0,d1=0,d2=0;
	US_LLONG_INTEGER temp1,temp2,mid,t_text[20000]={0};             
	char fin_name[40],fout1_name[40],apt[16];
	char fout2_name[40];
	LLONG_INTEGER c_text[20000]={0},e_text[20000]={0};
	FILE *fp,*fq,*fr;
	clock_t t_start,t_end;
	memset(buffer,0x00,sizeof(buffer));






	

	printf("\n********************************MH Bag Encrypted System*******************************\n");
	
    
	
	increase(p_num,sk);
	printf("The private keys generated");
	for(j=0;j<p_num;j++)
	{
		printf("%d ",sk[j]);
	}
	printf("\n");
	
	
	select_n_w(p_num,sk,b);
	w=b[1];N=b[0];
	printf("Timed Num=%d,Mod Num=%d,",w,N);
	
	
	inv_w=inv(w,N);
	
	printf("The Inv Num for the w of N inv_w=%d\n",inv_w);
	
	
	for(j=0;j<p_num;j++)
	{
		mid=w*sk[j];
		pk[j]=mid%N;
	}
	printf("The public keys generated");
	
	for(j=0;j<p_num;j++)
	{
		printf("%d  ",pk[j]);
	}
	printf("\n");

    
	
	printf("Input the file to Encrypte(eg:/usr/share/a.txt):\n");       
	gets(fin_name);
	fp=fopen(fin_name,"rb");
	if(fp==NULL)
	{
		puts("No file to Encrypte.\n");
		getchar();
		exit(0);
	}
	fclose(fp);
	puts("Input the Encrypted file to save(eg:/usr/share/a_encode.txt):");       
	gets(fout1_name);
	fq=fopen(fout1_name,"wb+");
	if(fp==NULL)
	{
		puts("No file to save...\n");
		getchar();
	}
	fclose(fq);
	puts("Start Encrypting...\n");
	t_start=clock();
	fp=fopen(fin_name,"rb");
	fq=fopen(fout1_name,"wb+");
	len=fread(buffer,2,10005,fp);
	
	
	
	for(k=0;k<len;k++)
	{
		binary(buffer[k],apt);
		         
		for(i=0;i<p_num;i++)
		{
			
			c_text[k]+=(apt[i]-48)*pk[i];
			
		}
		
	}
	fwrite(c_text,8,len,fq);
	puts("Encrypted File Established;Encrypted Finished.\n");
    puts("--------------------------------------------------------");
	fclose(fp);
	fclose(fq);
	t_end=clock();
	printf("Time Costed(Unit:ms):");
	printf("%lu\n",t_end-t_start);
	puts("Encrypted File to Save,(eg:/usr/share/a_encode.txt):");       
	gets(fout1_name);
	puts("DeEncrypted File to Save,(eg:/usr/share/a_decode.txt):");        
	gets(fout2_name);
	fr=fopen(fout2_name,"wb+");
	if(fr==NULL)
	{
		puts("No File to Save...\n");
		getchar();
	}
	fclose(fr);
	t_start=clock();
	
	
	fq=fopen(fout1_name,"rb");
	fread(e_text,8,len,fq);
	fr=fopen(fout2_name,"wb+");
	

	for(k=0;k<len;k++)
	{
		
        temp1=inv_w%N;
	    
	    
	    
	    temp2=e_text[k]%N;
	    
 	    t_text[k]=temp1*temp2; 
	    
	    t_text[k]=t_text[k]%N;        
		
		for(i=p_num-1;i>=0;i--)
		{
			if(t_text[k]<sk[i])
				z[i]=0;
			else
			{
				z[i]=1;
				t_text[k]=t_text[k]-sk[i];
			}   
		}
		
		for(j=0;j<p_num;j++)
		{
			d_text[k]+=pow(2,p_num-j-1)*z[j];
		}
		
		
	}
	puts("--------------------------------------------------------");
	puts("DeEncrypted Finished!\n");


	fwrite(d_text,2,len,fr);
	puts("DeEncrypted File Established.\n");
	
	
	t_end=clock();
	printf("Time all Costed(Unit:ms):");
	printf("%lu\n",t_end-t_start);
	printf("\n******************************** End *******************************\n");
}