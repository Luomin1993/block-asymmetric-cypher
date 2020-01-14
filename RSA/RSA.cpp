#include"RSA.hpp"
#include<string.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include <stdint.h>
LLONG_INTEGER Primer[6541];  
void RSA::Init()
{
	bool tag[65536];
	memset(tag,0x00,sizeof(tag));
	LLONG_INTEGER i,j;
	for(i=3;i<256;i+=2)
		if(tag[i]==false)
			for(j=i*i;j<65536;j+=i)
				tag[j]=true;
	for(Primer[0]=2,j=1,i=3;i<65536;i+=2)
		if(tag[i]==false)
			Primer[j++]=i;
}
bool RSA::IsPrimer(LLONG_INTEGER n)
{
	LLONG_INTEGER m=(LLONG_INTEGER)sqrt(n)+1,i;
	for(i=0;Primer[i]<=m;i++)
		if(n%Primer[i]==0)
			return false;
	return true;
}
LLONG_INTEGER RSA::Gcd(LLONG_INTEGER a,LLONG_INTEGER b)
{
	for(LLONG_INTEGER t;b;t=a%b,a=b,b=t);
	return a;
}
void RSA::Ex_Gcd(LLONG_INTEGER a,LLONG_INTEGER b,LLONG_INTEGER &x,LLONG_INTEGER &y)
{
	if(b==0)   
	{
		x=1,y=0;
		return;
	}
	Ex_Gcd(b,a%b,x,y);   
	LLONG_INTEGER t=x;  
	x=y,y=t-a/b*y; 
}
void RSA::GetPrimers(LLONG_INTEGER &p,LLONG_INTEGER &q)
{
	srand(time(NULL));
	do
	{
		p=rand()%1000+4000;
	}
	while(IsPrimer(p)==false);
	do
	{
		q=rand()%1000+4000;
	}
	while(IsPrimer(q)==false);
}
LLONG_INTEGER RSA::Mod(US_LLONG_INTEGER a,US_LLONG_INTEGER b,US_LLONG_INTEGER n)
{
	US_LLONG_INTEGER t=1;
	a%=n;
	while(b)
	{
		if(b%2)
			t=(t*a)%n;
		a=(a*a)%n;
		b/=2;
	}
	return (LLONG_INTEGER)t;
}
void RSA::Create_Key(char *pub_key_file,char *pri_key_file)
{
	LLONG_INTEGER p,q,pub_key,pri_key,m,n,t;  
	Init();
	do
	{
		GetPrimers(p,q);
		n=p*q;
		m=(p-1)*(q-1);
		do
			pub_key=(rand()%1000+500)*(rand()%1000+500);
		while(Gcd(m,pub_key)!=1);
		Ex_Gcd(pub_key,m,pri_key,t);  //pub_key*pri_key=1mod(m);
	}
	while(pri_key<1e5);
	printf("p=%I64d q=%I64d n=%I64d pri=%I64d pub=%I64d\n",p,q,n,pri_key,pub_key);
	FILE *pub,*pri;
	pub=fopen(pub_key_file,"wb");
	if(pub==NULL)
		return;
	pri=fopen(pri_key_file,"wb");
	if(pri==NULL)
		return;
	fwrite(&pub_key,sizeof(LLONG_INTEGER),1,pub);
	fwrite(&n,sizeof(LLONG_INTEGER),1,pub);
	fwrite(&pri_key,sizeof(LLONG_INTEGER),1,pri);
	fwrite(&n,sizeof(LLONG_INTEGER),1,pri);
	fclose(pub);
	fclose(pri);
}
int RSA::Encrypt_Int16(char *pub_key_file,US_LLONG_INTEGER in)
{
	FILE *pub;
	LLONG_INTEGER pub_key,n;
	pub=fopen(pub_key_file,"rb");
	fread(&pub_key,sizeof(LLONG_INTEGER),1,pub);
	fread(&n,sizeof(LLONG_INTEGER),1,pub);
	fclose(pub);
	printf("pub_key=%I64d n=%I64d\n",pub_key,n);
	return Mod(in,pub_key,n);
}
int RSA::DeEncrypt_Int16(char *pri_key_file,US_LLONG_INTEGER in)
{
	FILE *pri;
	LLONG_INTEGER pri_key,n;
	pri=fopen(pri_key_file,"rb");
	fread(&pri_key,sizeof(LLONG_INTEGER),1,pri);
	fread(&n,sizeof(LLONG_INTEGER),1,pri);
	fclose(pri);
	printf("pri_key=%I64d n=%I64d\n",pri_key,n);
	return Mod(in,pri_key,n);
}
void RSA::Encrypt(char *filename,char *output,char *pub_key_file)
{
	FILE *old_file,*new_file,*pub;
	LLONG_INTEGER pub_key,n,len,i,total;
	short cleartext[10005];
	LLONG_INTEGER ciphertext[10005];
	pub=fopen(pub_key_file,"rb");
	if(pub==NULL)
		return;
	fread(&pub_key,sizeof(LLONG_INTEGER),1,pub);

	fread(&n,sizeof(LLONG_INTEGER),1,pub);
	fclose(pub);
	old_file=fopen(filename,"rb");
	if(old_file==NULL)
		return;
	total=0;
	do
	{
		len=fread(cleartext,1,20000,old_file);
		total=len+total;
	}
	while(len==20000);
	fclose(old_file);
	old_file=fopen(filename,"rb");
	if(old_file==NULL)
		return;
	new_file=fopen(output,"wb");
	if(new_file==NULL)
		return;
	fwrite(&total,sizeof(LLONG_INTEGER),1,new_file);
	do
	{
		memset(cleartext,0x0000,sizeof(cleartext));
		len=fread(cleartext,1,20000,old_file);
		if(len%2)
			len++;
		len/=2;
		for(i=0;i<len;i++)
			ciphertext[i]=(LLONG_INTEGER)Mod((unsigned short)cleartext[i],pub_key,n);
		fwrite(ciphertext,sizeof(LLONG_INTEGER),i,new_file);
	}
	while(len==10000);
	fclose(old_file);
	fclose(new_file);
}
void RSA::DeEncrypt(char *filename,char *output,char *pri_key_file)
{
	FILE *old_file,*new_file,*pri;
	LLONG_INTEGER pri_key,n,len,i,total;
	short cleartext[10005];
	LLONG_INTEGER ciphertext[10005];
	pri=fopen(pri_key_file,"rb");
	if(pri==NULL)
		return;
	fread(&pri_key,sizeof(LLONG_INTEGER),1,pri);
	fread(&n,sizeof(LLONG_INTEGER),1,pri);
	old_file=fopen(filename,"rb");
	if(old_file==NULL)
		return;
	new_file=fopen(output,"wb");
	if(new_file==NULL)
		return;
	fclose(pri);
	fread(&total,sizeof(LLONG_INTEGER),1,old_file);
	do
	{
		len=fread(ciphertext,sizeof(LLONG_INTEGER),10000,old_file);
		total-=2*len;
		for(i=0;i<len;i++)
			cleartext[i]=(short)Mod((US_LLONG_INTEGER)ciphertext[i],pri_key,n);
		if(total==-1)
			i=i*2-1;
		else
			i=i*2;
		fwrite(cleartext,1,i,new_file);
	}
	while(len==10000);
	fclose(old_file);
	fclose(new_file);
}