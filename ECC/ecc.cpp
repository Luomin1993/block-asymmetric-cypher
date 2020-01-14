#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <tommath.h>
#include <time.h>


#define BIT_LEN 800 
#define KEY_LONG 128  
#define P_LONG 200    
#define EN_LONG 20   

using namespace std;

int GetPrime(mp_int *m,int lon);

void Get_B_X_Y(mp_int *x1,mp_int *y1,mp_int *b,  mp_int *a,  mp_int *p);

bool Ecc_points_mul(mp_int *qx,mp_int *qy, mp_int *px, mp_int *py,mp_int *d,mp_int *a,mp_int *p);

int Two_points_add(mp_int *x1,mp_int *y1,mp_int *x2,mp_int *y2,mp_int *x3,mp_int *y3,mp_int *a,bool zero,mp_int *p);

int chmistore(mp_int *a,FILE *fp);

int putin(mp_int *a,char *ch,int chlong);

void Ecc_encipher(mp_int *qx,mp_int *qy, mp_int *px, mp_int *py,mp_int *a,mp_int *p);

void Ecc_decipher(mp_int *k, mp_int *a,mp_int *p);

int chdraw(mp_int *a,char *ch);

int miwendraw(mp_int *a,char *ch,int chlong);


int myrng(unsigned char *dst, int len, void *dat)
{
   int x;
   for (x = 0; x < len; x++) dst[x] = rand() & 0xFF;
   return len;
}

int main(){
    cout<<"\n------------------------------------------------------------------";
	cout<<"\n          ECC Encrypted System"<<endl;	
	cout<<"\n------------------------------------------------------------------"<<endl;
   
	clock_t t_start,t_end;             
	mp_int GX;
	mp_int GY;
	mp_int K;
	mp_int A;
	mp_int B;
	mp_int QX;
	mp_int QY;
	mp_int P;
	mp_int c1x,c1y;
	mp_int c2;
	mp_int r;
	mp_int tempx,tempy;
	mp_int m;
	mp_int temp1;
	mp_int mx;
	

	mp_init(&GX);
	mp_init(&GY);
	mp_init(&K);
	mp_init(&A);
	mp_init(&B);
	mp_init(&QX);
	mp_init(&QY);
	mp_init(&P);
	mp_init(&c1x);
	mp_init(&c1y);
	mp_init(&c2);
	mp_init(&r);
	mp_init(&tempx);
	mp_init(&tempy);
	mp_init(&m);
	mp_init(&temp1);
	mp_init(&mx);


	

    time_t t;           
    srand( (unsigned) time( &t ) );

	printf("ECC Encrypted System\n");

    printf("Now Start:\n");	

    GetPrime(&P,P_LONG);
	printf("The P is:\n");	
	char temp[800]={0};
    mp_toradix(&P,temp,10);
    printf("%s\n",temp);

    GetPrime(&A,30);
	char tempA[800]={0};
	printf("The A is:\n");	
    mp_toradix(&A,tempA,10);
    printf("%s\n",tempA); 
	
	Get_B_X_Y(&GX,&GY,&B,&A,&P);

    char tempB[800]={0};
	printf("The B is:\n");	
    mp_toradix(&B,tempB,10);
    printf("%s\n",tempB); 
	
	char tempGX[800]={0};
	printf("The tempGX:\n");	
    mp_toradix(&GX,tempGX,10);
    printf("%s\n",tempGX);   

	char tempGY[800]={0};
	printf("The tempGY:\n");
    mp_toradix(&GY,tempGY,10);
    printf("%s\n",tempGY); 
	

	
    GetPrime(&K,KEY_LONG);
    char tempK[800]={0};
	printf("The tempK dB is:\n");
    mp_toradix(&K,tempK,10);
    printf("%s\n",tempK); 

	Ecc_points_mul(&QX,&QY,&GX,&GY,&K,&A,&P);
	

    char tempQX[800]={0};
	printf("The tempQX is:\n");
    mp_toradix(&QX,tempQX,10);
    printf("%s\n",tempQX); 

	char tempQY[800]={0};
	printf("The tempQY is:\n");
    mp_toradix(&QY,tempQY,10);
    printf("%s\n",tempQY); 

	FILE *fp,*fq;
	char fin_name[40],fout1_name[40],fout2_name[40];
	printf("Input the filename: /usr/share/test.txt:");
	cin>>fin_name;
	cout<<"Input the filename to save: /usr/share/test_en.txt:";
	cin>>fout1_name;
    t_start=clock();
    printf("\n\n\n Start Encrypting ...   \n\n\n");

	if((fp=fopen(fin_name,"rb"))==NULL)
	{
		printf("can not open the file!");
		exit(1);
	}
	fq=fopen(fout1_name,"wb+");

	char c[20000],miwen[280]={0};
	int len;
    len=fread(&c,1,10005,fp);
	len=len/EN_LONG;
	rewind(fp);
	for(int i=0;i<len;i++)
	{
		GetPrime(&r,100);
		fread(miwen,1,EN_LONG,fp);
		miwen[EN_LONG]=char(255);
		putin(&m,miwen,EN_LONG+1);
		
		
		Ecc_points_mul(&c1x,&c1y,&GX,&GY,&r,&A,&P);
		Ecc_points_mul(&tempx,&tempy,&QX,&QY,&r,&A,&P); 
        mp_mulmod(&m,&tempx,&P,&c2);
		chmistore(&c1x,fq);
		chmistore(&c1y,fq);
		chmistore(&c2,fq);

	}
	fclose(fp);
	fclose(fq);
	cout<<"Encrypted Finished !"<<endl;
	t_end=clock();
	cout<<"Time Costed(unit:ms):"<<t_end-t_start<<endl;
	cout<<"Input the File to DeEncrypte:/usr/share/test_en.txt: ";
	cin>>fout1_name;
	cout<<"Input the File to Save the DeEncrypted:/usr/share/test_de.txt: ";
	cin>>fout2_name;
    t_start=clock();
    printf("\n\n\n Start DeEncrypting...   \n\n\n");

	if((fp=fopen(fout1_name,"rb"))==NULL)
	{
		printf("can not open the file!");
		exit(1);
	}
	fq=fopen(fout2_name,"wb+");
	char stemp[700]={0};
	mp_int tempzero;
	mp_init(&tempzero);
	while(!feof(fp))
	{
		
		int i=0;
		while(1)
		{
			stemp[i]=fgetc(fp);
			if(i%4==0)
			{
				if(int(stemp[i]&0xFF) == 255 ) goto L1;
			}
			i++;
		}
		
L1:     miwendraw(&c1x, stemp, i);
		i=0;
		while(1)
		{
			stemp[i]=fgetc(fp);
			if(i%4==0)
			{
				if(int(stemp[i]&0xFF) == 255 ) goto L2;
			}
			i++;
		}
		
L2:     miwendraw(&c1y, stemp, i);
		i=0;
		while(1)
		{
			stemp[i]=fgetc(fp);
			if(i%4==0)
			{
				if(int(stemp[i]&0xFF) == 255 ) goto L3;
			}
			i++;
		}
				 
L3:     miwendraw(&c2, stemp, i);
		
		
		mp_zero(&tempzero);
        if(mp_cmp(&c1x, &tempzero)==0) break;
		
		Ecc_points_mul(&tempx,&tempy,&c1x,&c1y,&K,&A,&P);
		mp_invmod(&tempx,&P,&temp1);
		mp_mulmod(&temp1,&c2,&P,&m);
		
		


		int chtem;
		chtem=chdraw(&m,stemp);
		
		
		
		
		for(int kk=0;kk<chtem;kk++)
		{
			fprintf(fq,"%c",stemp[kk]);
			
		}
	}
    fclose(fp);
	fclose(fq);
	cout<<"DeEncrypte Finished !"<<endl;
	t_end=clock();
	cout<<"Time Costed(unit:ms):"<<t_end-t_start<<endl;


	char cc;
    cout<<"\n\n All Finished !\n";
	cin>>cc;
    mp_clear(&m);
	mp_clear(&GX);
	mp_clear(&GY);
	mp_clear(&K);
	mp_clear(&A);
	mp_clear(&B);
	mp_clear(&QX);
	mp_clear(&QY);
	mp_clear(&P);
	mp_clear(&tempzero);

	return 0;
}


int GetPrime(mp_int *m,int lon){
   mp_prime_random_ex(m, 10, lon, 
		(rand()&1)?LTM_PRIME_2MSB_OFF:LTM_PRIME_2MSB_ON, myrng, NULL);
   return MP_OKAY;
}

void Get_B_X_Y(mp_int *x1,mp_int *y1,mp_int *b, mp_int *a, mp_int *p)
{
    mp_int tempx,tempy;
    mp_int temp;
	mp_int compare;
	mp_int temp1;
	mp_int temp2;
	mp_int temp3;
	mp_int temp4;
	mp_int temp5;
	mp_int temp6;
	mp_int temp7;
	mp_int temp8;
    
	mp_init_set_int (&compare, 0);
	mp_init(&tempx);
	mp_init(&tempy);
	mp_init(&temp);
    mp_init(&temp1);
	mp_init(&temp2);
	mp_init(&temp3);
	mp_init(&temp4);
	mp_init(&temp5);
	mp_init(&temp6);
	mp_init(&temp7);
	mp_init(&temp8);

 
   while(1)
   {
     
	 
     GetPrime(b,40);
	 mp_expt_d(a, 3, &temp1);
     mp_sqr(b, &temp2);
	 mp_mul_d(&temp1, 4, &temp3);
	 mp_mul_d(&temp2, 27, &temp4);
     mp_add(&temp3, &temp4, &temp5);
	 mp_mod(&temp5,p,&temp);

     if(mp_cmp(&temp, &compare)!=0 )
	 {
		 break;
	 }
   }

   
   GetPrime(x1,30);
   mp_expt_d(x1, 3, &temp6);
   mp_mul(a, x1, &temp7);
   mp_add(&temp6, &temp7, &temp8);
   mp_add(&temp8, b, &tempx);
   mp_sqrt(&tempx, y1);



   mp_clear(&tempx);
   mp_clear(&tempy);
   mp_clear(&temp);
   mp_clear(&temp1);
   mp_clear(&temp2);
   mp_clear(&temp3);
   mp_clear(&temp4);
   mp_clear(&temp5);
   mp_clear(&temp6);
   mp_clear(&temp7);
   mp_clear(&temp8);
}

bool Ecc_points_mul(mp_int *qx,mp_int *qy, mp_int *px, mp_int *py,mp_int *d,mp_int *a,mp_int *p)
{
mp_int X1, Y1;
mp_int X2, Y2;
mp_int X3, Y3;
mp_int XX1, YY1;
mp_int A,P;

int i;
bool zero=false;
char Bt_array[800]={0};
char cm='1';

    mp_toradix(d,Bt_array,2); 

    mp_init_set_int(&X3, 0);
    mp_init_set_int(&Y3, 0);
	mp_init_copy(&X1, px);
	mp_init_copy(&X2, px);
    mp_init_copy(&XX1, px);
	mp_init_copy(&Y1, py);
	mp_init_copy(&Y2, py);
	mp_init_copy(&YY1, py);

	mp_init_copy(&A, a);
	mp_init_copy(&P, p);

	for(i=1;i<=KEY_LONG-1;i++)
	{
	   mp_copy(&X2, &X1);
	   mp_copy(&Y2, &Y1);
	   Two_points_add(&X1,&Y1,&X2,&Y2,&X3,&Y3,&A,zero,&P);  
       mp_copy(&X3, &X2);
	   mp_copy(&Y3, &Y2);
	   if(Bt_array[i]==cm)
	   {
		  
		   mp_copy(&XX1, &X1);
		   mp_copy(&YY1, &Y1);
		   Two_points_add(&X1,&Y1,&X2,&Y2,&X3,&Y3,&A,zero,&P);
		   mp_copy(&X3, &X2);
		   mp_copy(&Y3, &Y2);

	   }

	}
		
   if(zero)
   {
	   cout<<"It is Zero_Unit!";
       return false;
   }

   mp_copy(&X3, qx);
   mp_copy(&Y3, qy);

   mp_clear(&X1);
   mp_clear(&Y1);
   mp_clear(&X2);
   mp_clear(&Y2);
   mp_clear(&X3);
   mp_clear(&Y3);
   mp_clear(&XX1);
   mp_clear(&YY1);
   mp_clear(&A);
   mp_clear(&P);
  
   return true;
}


int Two_points_add(mp_int *x1,mp_int *y1,mp_int *x2,mp_int *y2,mp_int *x3,mp_int *y3,mp_int *a,bool zero,mp_int *p)
{
mp_int x2x1;
mp_int y2y1;
mp_int tempk;
mp_int tempy;
mp_int tempzero;
mp_int k;
mp_int temp1;
mp_int temp2;
mp_int temp3;
mp_int temp4;
mp_int temp5;
mp_int temp6;
mp_int temp7;
mp_int temp8;
mp_int temp9;
mp_int temp10;


mp_init(&x2x1);
mp_init(&y2y1);
mp_init(&tempk);
mp_init(&tempy);
mp_init(&tempzero);
mp_init(&k);
mp_init(&temp1);
mp_init(&temp2);
mp_init_set(&temp3,2);
mp_init(&temp4);
mp_init(&temp5);
mp_init(&temp6);
mp_init(&temp7);
mp_init(&temp8);
mp_init(&temp9);
mp_init(&temp10);


   if(zero)
   {
	  mp_copy(x1, x3);
      mp_copy(y1, y3);
	  zero=false;
	  goto L;
   }
   mp_zero(&tempzero);
   mp_sub(x2, x1, &x2x1);
   if(mp_cmp(&x2x1,&tempzero)==-1)
   {
	  
	  mp_add(&x2x1, p, &temp1);
	  mp_zero(&x2x1);
      mp_copy(&temp1, &x2x1);
   }
   mp_sub(y2, y1, &y2y1);
   if(mp_cmp(&y2y1,&tempzero)==-1)
   {
     
	  mp_add(&y2y1, p, &temp2);
	  mp_zero(&y2y1);
      mp_copy(&temp2, &y2y1);
   }
   if(mp_cmp(&x2x1, &tempzero)!=0)
   {
	   
	   mp_invmod(&x2x1,p,&tempk);
	   
	   mp_mulmod(&y2y1, &tempk, p, &k);
   }
   else
   {
	   if(mp_cmp(&y2y1, &tempzero)==0)
	   {
		  
		  mp_mulmod(&temp3,y1,p,&tempy);
          mp_invmod(&tempy,p,&tempk);
          mp_sqr(x1, &temp4);     
		  mp_mul_d(&temp4, 3, &temp5);
		  mp_add(&temp5, a, &temp6);
          mp_mulmod(&temp6, &tempk, p, &k);
		  
	   }
	   else
	   {
		  zero=true;
		  goto L;
	   }
   }  
 
   mp_sqr(&k, &temp7);
   mp_sub(&temp7, x1, &temp8);
   mp_submod(&temp8, x2, p, x3);
 
   mp_sub(x1, x3, &temp9);
   mp_mul(&temp9, &k, &temp10);
   mp_submod(&temp10, y1, p, y3);


L:

   mp_clear(&x2x1);
   mp_clear(&y2y1);
   mp_clear(&tempk);
   mp_clear(&tempy);
   mp_clear(&tempzero);
   mp_clear(&k);
   mp_clear(&temp1);
   mp_clear(&temp2);
   mp_clear(&temp3);
   mp_clear(&temp4);
   mp_clear(&temp5);
   mp_clear(&temp6);
   mp_clear(&temp7);
   mp_clear(&temp8);
   mp_clear(&temp9);
   mp_clear(&temp10);

   return 1;

}


int chmistore(mp_int *a,FILE *fp)
{

   int i,j;
   char ch;
   char chtem[4];

   mp_digit yy=(mp_digit)255;
   for (i=0; i <= a->used - 1;  i++) { 

      chtem[3]=(char)(a->dp[i] & yy);     
      chtem[2]=(char)((a->dp[i] >> (mp_digit)8) & yy);	    
	  chtem[1]=(char)((a->dp[i] >> (mp_digit)16) & yy);    
      chtem[0]=(char)((a->dp[i] >> (mp_digit)24) & yy);

      for(j=0;j<4;j++)
	  {
	      fprintf(fp,"%c",chtem[j]);
	  }
      
  }  

  ch=char(255);
  fprintf(fp, "%c", ch);
  return MP_OKAY;
}



int putin(mp_int *a,char *ch,int chlong)
{
	mp_digit *temp,yy;
	int i,j,res;
	if(a->alloc<chlong*2/7+2)
	{
		if((res=mp_grow(a,chlong*2/7+2))!=MP_OKAY)
			return res;
	}
    
	a->sign=0;
	mp_zero(a);
	temp=a->dp;
	i=0;
	yy=(mp_digit)15;

	if(chlong<4)
	{
            for(j=chlong-1;j>=0;j--)
			{
			   *temp |= (mp_digit)(ch[j] & 255);
               *temp <<= (mp_digit)CHAR_BIT;
			}
			*temp >>= (mp_digit)8;
			a->used=1;
			return MP_OKAY;
	}

	if(chlong<7)
	{
	        i+=4;
            *++temp |= (mp_digit)(ch[i-1] & yy);
		    *temp <<= (mp_digit)CHAR_BIT;
            *temp |= (mp_digit)(ch[i-2] & 255);
		    *temp <<= (mp_digit)CHAR_BIT;
            *temp |= (mp_digit)(ch[i-3] & 255);
		    *temp <<= (mp_digit)CHAR_BIT;
            *temp-- |= (mp_digit)(ch[i-4] & 255); 

            
            for(j=chlong-1;j>=i;j--)
			{  
			   *temp |= (mp_digit)(ch[j] & 255);
			   *temp <<= (mp_digit)CHAR_BIT;			                
			}
            *temp >>= (mp_digit)4;
            *temp |= (mp_digit)((ch[i-1] & 255) >> 4);  
            
			a->used=2;
			return MP_OKAY;
	}

        
	for(j=0;j<chlong/7;j++)
	{
		i+=7;
		*++temp |= (mp_digit)(ch[i-1] & 255);
		*temp <<= (mp_digit)CHAR_BIT;
        *temp |= (mp_digit)(ch[i-2] & 255);
		*temp <<= (mp_digit)CHAR_BIT;
        *temp |= (mp_digit)(ch[i-3] & 255);
		*temp <<= (mp_digit)4;
        *temp-- |= (mp_digit)((ch[i-4] & 255) >> 4);    

		*temp |= (mp_digit)(ch[i-4] & yy);      
        *temp <<= (mp_digit)CHAR_BIT;
        *temp |= (mp_digit)(ch[i-5] & 255);
		*temp <<= (mp_digit)CHAR_BIT;
        *temp |= (mp_digit)(ch[i-6] & 255);
		*temp <<= (mp_digit)CHAR_BIT;
        *temp++ |= (mp_digit)(ch[i-7] & 255); 

		temp++;
	
	}
 
	if((chlong>=7)&&(chlong%7!=0))        
	{
		if(chlong%7 < 4)           
		{
			for(j=chlong-1;j>=i;j--)
			{
			   *temp |= (mp_digit)(ch[j] & 255);
               *temp <<= (mp_digit)CHAR_BIT;
			}
			*temp >>= (mp_digit)8;
			a->used=chlong*2/7+1;
		}
		else
		{                       
			i+=4;
            *temp |= (mp_digit)(ch[i-1] & yy);
		    *temp <<= (mp_digit)CHAR_BIT;
            *temp |= (mp_digit)(ch[i-2] & 255);
		    *temp <<= (mp_digit)CHAR_BIT;
            *temp |= (mp_digit)(ch[i-3] & 255);
		    *temp <<= (mp_digit)CHAR_BIT;
            *temp++ |= (mp_digit)(ch[i-4] & 255); 

            
            for(j=chlong-1;j>=i;j--)
			{  
			   *temp |= (mp_digit)(ch[j] & 255);
			   *temp <<= (mp_digit)CHAR_BIT;			                
			}
            *temp >>= (mp_digit)4;
            *temp |= (mp_digit)((ch[i-1] & 255) >> 4);  
            
			a->used=chlong*2/7+2;
		}

	}
	else
	{
	   a->used=chlong*2/7;
	}
	return MP_OKAY;
}


void Ecc_encipher(mp_int *qx,mp_int *qy, mp_int *px, mp_int *py,mp_int *a,mp_int *p){

	mp_int mx, my;
	mp_int c1x, c1y;
	mp_int c2x, c2y;
    mp_int r;
	mp_int tempx, tempy;
    bool zero=false;
    FILE *fp,*fq;
	int i;
    char miwenx[280]={0};
    char miweny[280]={0};
	char stemp[650]={0};


	mp_init(&mx);
	mp_init(&my);
	mp_init(&c1x);
	mp_init(&c1y);
	mp_init(&c2x);
	mp_init(&c2y);
    mp_init(&r);
	mp_init(&tempx);
	mp_init(&tempy);

	GetPrime(&r, 100);

	char filehead[60],filefoot[20],filename[85]={0};
    cout<<"The Filefolder to Encrypte(like:  /home/Latex  ):"<<endl;
	cin>>filehead;
    cout<<"The Extended Name like:  .tex  ):"<<endl;
	cin>>filefoot;
	strcpy(filename,filehead);
	strcat(filename,filefoot);
	


	if((fp=fopen(filename,"rb"))==NULL)
	{
		printf("can not open the file!");
		exit(1);
	}

	unsigned int FileLong=0;
	char ChTem;
	int Frequency=0;
	int Residue=0;

    while( !feof(fp) )
	{
		ChTem = fgetc( fp );
		FileLong++;
	}
    --FileLong;


	Frequency = FileLong/EN_LONG;
	Residue = FileLong%EN_LONG;

	int enlongtemp=EN_LONG/2;
	
	

	char filemi[85];
	strcpy(filemi,filehead);
	strcat(filemi,"NAME");
	strcat(filemi,filefoot);
    

 
    if((fq=fopen(filemi,"wb"))==NULL)
	{
		 printf("can not open the file!\n");
		 exit(1);
	}

    printf("\n Start Encrypting ...\n");


	rewind(fp);
	for(i=0; i<Frequency; i++)
	{   
  
	    fread(miwenx,1,EN_LONG,fp);
	    miwenx[EN_LONG]=char(255);

		fread(miweny,1,EN_LONG,fp);
	    miweny[EN_LONG]=char(255);

        putin(&mx, miwenx,EN_LONG+1);
		putin(&my, miweny,EN_LONG+1);



	    Ecc_points_mul(&c1x,&c1y,px,py,&r,a,p);
	    Ecc_points_mul(&tempx,&tempy,qx,qy,&r,a,p); 
		mp_mulmod(&mx,&tempx,p,&c2x);
		mp_mulmod(&my,&tempx,p,&c2y);
		char tempC1x[800]={0};
	    printf("The tempC1x is:\n");	
        mp_toradix(&mx,tempC1x,10);
        printf("%s\n",tempC1x);
	    
		
	    chmistore(&c1x,fq); 
		chmistore(&c1y,fq);
		chmistore(&c2x,fq);
		chmistore(&c2y,fq);

	}
	
	if ( Residue > 0)
	{
	     if (Residue <= enlongtemp ) 
		{
			fread(miwenx,1,Residue,fp);
			miwenx[Residue]=char(255);
     
			putin(&mx, miwenx,Residue+1);

			mp_zero(&my);
        
		}
		else
		{

			fread(miwenx,1,enlongtemp,fp);
			miwenx[enlongtemp]=char(255);
        

			fread(miweny,1,Residue-enlongtemp,fp);
			miweny[Residue-enlongtemp]=char(255);

			 putin(&mx, miwenx,enlongtemp+1);

			putin(&my, miweny,Residue-enlongtemp+1);
		}

	    Ecc_points_mul(&c1x,&c1y,px,py,&r,a,p);
	    Ecc_points_mul(&tempx,&tempy,qx,qy,&r,a,p); 
		mp_mulmod(&mx,&tempx,p,&c2x);
		mp_mulmod(&my,&tempx,p,&c2y);
        
	 
        
	    chmistore(&c1x,fq); 

		chmistore(&c1y,fq);

		chmistore(&c2x,fq);

		chmistore(&c2y,fq);  
	}

	
		cout<<"\nok! Finished!"<<endl;
	    cout<<"Encrypted Finished"<<endl;
	    cout<<"The Encrypted File is Saved in"<<filemi<<endl ;


	    fclose(fq);
        fclose(fp);
        mp_clear(&mx);
		mp_clear(&my);
	    mp_clear(&c1x);
	    mp_clear(&c1y);
	    mp_clear(&c2x);
	    mp_clear(&c2y);
        mp_clear(&r);
	    mp_clear(&tempx);
		mp_clear(&tempy);
     

}



int miwendraw(mp_int *a,char *ch,int chlong)
{
    mp_digit *temp;
    int i,j,res;

    if(a->alloc<chlong/4)
	{
		if((res=mp_grow(a,chlong/4))!=MP_OKAY)
			return res;
	}

	a->alloc=chlong/4;
    a->sign=0;
	mp_zero(a);
	temp=a->dp;
	i=0;

	for(j=0;j<chlong/4;j++)
	{
		i+=4;
		*temp |= (mp_digit)(ch[i-4] & 255);
		*temp <<= (mp_digit)CHAR_BIT;
        *temp |= (mp_digit)(ch[i-3] & 255);
		*temp <<= (mp_digit)CHAR_BIT;
        *temp |= (mp_digit)(ch[i-2] & 255);
		*temp <<= (mp_digit)CHAR_BIT;
        *temp++ |= (mp_digit)(ch[i-1] & 255); 
	}
    a->used=chlong/4;
    return MP_OKAY;
}


int chdraw(mp_int *a,char *ch)
{
	int i,j;
	mp_digit *temp,xx,yy;

	temp=a->dp;
	i=0;
	yy=(mp_digit)255;  
	xx=(mp_digit)15;  

	for(j=0;j<a->used/2;j++)  
	{
		i+=7;
        ch[i-4]=(char)(*++temp & xx);
        ch[i-3]=(char)((*temp >> (mp_digit)4) & yy);	
		ch[i-2]=(char)((*temp >> (mp_digit)12) & yy);
        ch[i-1]=(char)((*temp-- >> (mp_digit)20) & yy);

		ch[i-7]=(char)(*temp & yy);
		ch[i-6]=(char)((*temp >> (mp_digit)8) & yy);
		ch[i-5]=(char)((*temp >> (mp_digit)16) & yy);
		ch[i-4] <<= 4;
		ch[i-4]+=(char)((*temp++ >> (mp_digit)24) & xx);
		temp++;
	}
	if(a->used%2!=0)  
	{
		ch[i++] = (char)(*temp & yy);
		ch[i++] = (char)((*temp >> (mp_digit)8) & yy);
		ch[i++] = (char)((*temp >> (mp_digit)16) & yy);
	}
	--i;
    while(int(ch[i]&0xFF) != 255 && i>0) i--;
	return i;
   
}

void Ecc_decipher(mp_int *k, mp_int *a,mp_int *p){

	mp_int c1x, c1y;
	mp_int c2x, c2y;
    mp_int tempx, tempy;
	mp_int mx, my;
    mp_int temp;

	mp_init(&temp);
	mp_init(&c1x);
	mp_init(&c1y);
    mp_init(&c2x);
	mp_init(&c2y);
	mp_init(&tempx);
	mp_init(&tempy);
    mp_init(&mx);
	mp_init(&my);

	mp_int tempzero;
	mp_init(&tempzero);

    int i;
	char stemp[700]={0};
    FILE *fp,*fq;
    bool zero=false;


	char filehead[60],filefoot[20],filename[85]={0};
    cout<<"The Filefolder where to DeEncrypte(like: /home/Latex  ):"<<endl;
	cin>>filehead;
    cout<<"The File Extended Name to DeEncrypte(like: .tex  ):"<<endl;
	cin>>filefoot;
	strcpy(filename,filehead);
	strcat(filename,filefoot);

    printf("\n Start DeEncrypting ... \n");

    if((fp=fopen(filename,"rb"))==NULL)
	{
		 printf("can not open the file!");
		 exit(1);
	}
 
   
    char filemi[80];
	strcpy(filemi, filehead);
	strcat(filemi, "NAME_de");
    strcat(filemi, filefoot);

    if((fq=fopen(filemi,"wb"))==NULL)
	{
		 printf("can not open the file!");
		 exit(1);
	}


	rewind(fp);
    while(!feof(fp))
	{
         i=0;
		 while(1)
		{
		     stemp[i]=fgetc(fp);
		     if(i%4==0)
			{
                 if(int(stemp[i]&0xFF) == 255 ) goto L1;
			}
		    i++;
		}
		     
L1:     miwendraw(&c1x, stemp, i);
         i=0;
		 while(1)
		{
		     stemp[i]=fgetc(fp);
		     if(i%4==0)
			{
                 if(int(stemp[i]&0xFF) == 255 ) goto L2;
			}
		    i++;
		}
		     
L2:     miwendraw(&c1y, stemp, i);
	     i=0;
		 while(1)
		{
		     stemp[i]=fgetc(fp);
		     if(i%4==0)
			{
                 if(int(stemp[i]&0xFF) == 255 ) goto L3;
			}
		    i++;
		}
		     
L3:     miwendraw(&c2x, stemp, i);
	            i=0;
		 while(1)
		{
		     stemp[i]=fgetc(fp);
		     if(i%4==0)
			{
                 if(int(stemp[i]&0xFF) == 255 ) goto L4;
			}
		    i++;
		}
		     
L4:     miwendraw(&c2y, stemp, i);

	    mp_zero(&tempzero);
        if(mp_cmp(&c1x, &tempzero)==0) break;

        
		Ecc_points_mul(&tempx,&tempy,&c1x,&c2y,k,a,p);
		mp_invmod(&tempx,p,&temp);
		mp_mulmod(&temp,&c2x,p,&mx);
		mp_mulmod(&temp,&c2y,p,&my);
	
	    
		int chtem;
	    chtem=chdraw(&mx,stemp);
     

		
    
		for(int kk=0;kk<chtem;kk++)
		{
	         fprintf(fq,"%c",stemp[kk]);
			       
		}

	    chtem=chdraw(&my,stemp);
	         
	     
		  for(int kk=0;kk<chtem;kk++)
		{
	          fprintf(fq,"%c",stemp[kk]);
			
		}
		  
	    
	}

   	cout<<"\n ok!Finished!"<<endl;
	cout<<"The DeEncrypting Files are in  "<<filemi<<endl;

    fclose(fq);
    fclose(fp);
    mp_clear(&c1x);
	mp_clear(&c1y);
	mp_clear(&c2x);
	mp_clear(&c2y);
    mp_clear(&tempx);
	mp_clear(&tempy);
	mp_clear(&mx);
	mp_clear(&my);
    mp_clear(&temp);

}


