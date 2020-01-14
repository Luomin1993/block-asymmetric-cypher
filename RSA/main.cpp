#include<stdio.h>
#include <sys/time.h>
#include<stdlib.h>
#include"RSA.hpp"
#include <iostream>

using namespace std;

int main()
{
	char filename[1024],pub_name[1024],pri_name[1024],output[1024];
	int tag;
	LLONG_INTEGER n;
	while(1)
	{
		puts("1.Create Key\n2.Encrypt\n3.De Encrypt");
		scanf("%d",&tag);
		struct timeval t1, t2;
		float deltaT;
		switch(tag)
		{
			case 1:
				puts("Input public key filename:");
				fflush(stdin);
				gets(pub_name);
				puts("Input private key filename:");
				gets(pri_name);
				RSA::Create_Key(pub_name,pri_name);
				break;
			case 2:
				puts("Input public key filename:");
				fflush(stdin);
				gets(pub_name);
				//puts("Input an INT to Encrypt");
				//scanf("%I64d",&n);
				//printf("The Encrypted INT is %d\n",RSA::Encrypt_Int16(pub_name,n));
				puts("Input the filename to Encrypt:");
				gets(filename);
				puts("Input the filename to save Encrypt:");
				gets(output);
                gettimeofday(&t1, NULL);
				RSA::Encrypt(filename,output,pub_name);
				gettimeofday(&t2, NULL);
	            deltaT = (t2.tv_sec-t1.tv_sec) * 1000000 + t2.tv_usec-t1.tv_usec;
	            cout<<endl<<"The time costed:\t"<<deltaT<<"\t (unit:us)"<<endl;
				break;
			case 3:
				puts("Input private key filename:");
				fflush(stdin);
				gets(pri_name);
				//puts("Input an INT to DeEncrypt");
				//scanf("%I64d",&n);
				//printf("The DeEncrypted INT is %d\n",RSA::DeEncrypt_Int16(pri_name,n));
				puts("Input the filename to DeEncrypt:");
				gets(filename);
				puts("Input the filename to save DeEncrypt:");
				gets(output);
                gettimeofday(&t1, NULL);
				RSA::DeEncrypt(filename,output,pri_name);
				gettimeofday(&t2, NULL);
	            deltaT = (t2.tv_sec-t1.tv_sec) * 1000000 + t2.tv_usec-t1.tv_usec;
	            cout<<endl<<"The time costed:\t"<<deltaT<<"\t (unit:us)"<<endl;
				break;
		}
	}
	return 0;
}