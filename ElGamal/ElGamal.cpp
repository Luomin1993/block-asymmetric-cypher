#include<ctime>
#include"BigNumber.h"
#include <sys/time.h>

#define LENGTH 5000*10

using namespace std;


BigNum I=1,o=0,e=2;

int main()
{
     BigNum p,q,I(1),e(2),O(0),middle,g,x,y,k,k1,C1,C2,M1,M2,M; 
     //BigNum test[20000];
     struct timeval t1, t2, t3;
     float deltaT;
     //gettimeofday(&t1, NULL);
     //string A;
     //int middle;
      
    cout<<"*********************************************************************"<<endl;
    cout<<"*                         ElGamal Cryptosystem                        *"<<endl;     
    cout<<"*********************************************************************"<<endl<<endl;
        
    middle.Random(0);
    cout<<"The Random is:"<<endl;
    middle.print();
    cout<<endl;
    p.GeneratePrime();
    g.Random(p-I);
    /*do
    {
        cout<<"1"<<endl;
        q.GeneratePrime();  
        p=q*e+I;
    }while(p.Rabin_Miller()!=1);
    cout<<"****************·"<<endl;
    do
    {
        cout<<"2"<<endl;
        g.Random(p-I);
    }while((g.power_mod(e,p)==I)||(g.power_mod(q,p)==I));
    cout<<"****************·"<<endl;*/
    cout<<"The prime p is:"<<endl;
    p.print(); 
    cout<<endl;
    cout<<"The origin root g of prime p is:"<<endl;
    g.print();
    cout<<endl;
    x.Random(p);
    y=g.power_mod(x,p);
    cout<<"The public key y is:"<<endl;
    y.print();
    cout<<endl;
    //d.ex_euclid(e,t); 
    cout<<"The private key x is:"<<endl;
    x.print();
    cout<<endl;
    //a=BigNum(A);
    //cout<<endl;

    k.GeneratePrime();
    unsigned char temp[LENGTH/8 + 1];
    unsigned int  read,last_read = LENGTH/8,cnt = 0;
    char plain_fullpath[256], cipher_fullpath[256], xx_plain_fullpath[256];
    cout<<"Input the path of M txt"<<endl;
    cin>>plain_fullpath;
    cout<<"Input the path to save C txt"<<endl;
    cin>>cipher_fullpath;
    cout<<"Input the path to save deC txt"<<endl;
    cin>>xx_plain_fullpath;
    gettimeofday(&t1, NULL);
    FILE * fp_plain  = fopen(plain_fullpath,"rb");
    FILE * fp_cipher = fopen(cipher_fullpath,"wb");
    while(read = fread(temp,sizeof(unsigned char), LENGTH/8, fp_plain)  )
    {
        cnt++;
        if (read!= LENGTH/8)
        {
            int yy = read;
            for (int i = 0; i < LENGTH/8; ++i)
            {
                temp[i] = 0;
            }
            temp[yy] = '\0';
            last_read = read;
        }
        else
            temp[LENGTH/8] = '\0';

        middle.StringToBignum(temp,LENGTH/8);
        C1=g.power_mod(k,p); //the first: C1;
        C2=y.power_mod(k,p);
        C2=(C2*middle)%p;    //the second: C2;

        for (int i = 0; i < LENGTH/8; i+=4) // write into C1;
        {
            fwrite( (unsigned char *)(&C1.array[i/4]),sizeof(unsigned char),4,fp_cipher  );
        }
        for (int i = 0; i < LENGTH/8; i+=4) // write into C2;
        {
            fwrite( (unsigned char *)(&C2.array[i/4]),sizeof(unsigned char),4,fp_cipher  );
        }
    } // Finished Cipher text generating and saved;


    fwrite( (unsigned char *)(&cnt),sizeof(unsigned char),4,fp_cipher  );
    fwrite( (unsigned char *)(&last_read),sizeof(unsigned char),4,fp_cipher  );
    fclose(fp_plain);fclose(fp_cipher);

    deltaT = (t2.tv_sec-t1.tv_sec) * 1000000 + t2.tv_usec-t1.tv_usec;
    cout<<endl<<"The Encrypted time costed:\t"<<deltaT<<"\t us"<<endl;

    // ============== Part of DeEncrypt ================

    unsigned char temp1[LENGTH/8+1],temp2[LENGTH/8+1]; // read in C1 and C2;
    unsigned char t_char;

    fp_plain = fopen(xx_plain_fullpath,"wb");
    fp_cipher = fopen(cipher_fullpath,"rb");

    fseek(fp_cipher,-5,SEEK_END);
    cnt = 0;last_read = 0;
    t_char = fgetc(fp_cipher);
    cnt |= t_char;
    cnt = cnt<<8;
    for (int kk = 0; kk <=3; ++kk)
    {
        fseek(fp_cipher,-2,SEEK_CUR);
        t_char = fgetc(fp_cipher);
        cnt |= t_char;
        if (kk!=3)
        {
            cnt = cnt<<8;
        }
    }

    fseek(fp_cipher,-1,SEEK_END);
    t_char = fgetc(fp_cipher);
    last_read |= t_char;
    last_read = last_read<<8;
    for (int kk = 0; kk <=3; ++kk)
    {
        fseek(fp_cipher,-2,SEEK_CUR);
        t_char = fgetc(fp_cipher);
        last_read |= t_char;
        if (kk!=3)
        {
            last_read = last_read<<8;
        }
    }

    rewind(fp_cipher);
    int cnt1,index;
    unsigned char* ttt;
    unsigned char ttemp[4];
    for (cnt1 = 0; cnt1 < cnt; ++cnt1)
    {
        for (int m = 0; m < LENGTH/(8*4); ++m)
        {
            fread(ttemp,sizeof(unsigned char),4,fp_cipher);
            for (int mm = 3; mm >=0; mm--)
            {
                C1.array[m]  = C1.array[m]<<8;
                C1.array[m] |=      ttemp[mm];
            }
        }
        for (int m = 0; m < LENGTH/(8*4); ++m)
        {
            fread(ttemp,sizeof(unsigned char),4,fp_cipher);
            for (int mm = 3; mm >=0; mm--)
            {
                C2.array[m]  = C2.array[m]<<8;
                C2.array[m] |=      ttemp[mm];
            }
        }
        M1 = C1.power_mod(x,p);
        M2.ex_euclid(M1,p);
        M  = (C2*M2)%p; // deEncrypte one part of C into deM;
        if (cnt1!= cnt-1 || (cnt1 == cnt1-1 && last_read == LENGTH/8 ) )
        {
            for (int m = 0; m < LENGTH/(8*4); ++m)
            //for(int m=0;m<32;m++)
            {
                ttt = (unsigned char*)&(M.array[m]);
                ttt+=3;
                for(int mm=0;mm<4;mm++)
                {
                    fputc( *(ttt-mm) , fp_plain );
                }
            }
        }
        else
        {
            int xx = 0;
            for (int m = 0; m < last_read/4; ++m,++xx)
            {
                ttt = (unsigned char*)&(M.array[m]);
                ttt+=3;
                for(int mm=0;mm<4;mm++)
                {
                    fputc( *(ttt-mm) , fp_plain );
                }
            }
            ttt = (unsigned char*)&(M.array[xx]);
            ttt+=3;
            for (int mm = 0; mm < last_read%4; ++mm)
            {
                fputc((*ttt-mm),fp_plain);
            }
        }
    }

    fclose(fp_cipher);fclose(fp_plain);

    //C1=g.power_mod(k,p);
    //C2=y.power_mod(k,p);
    //C2=(C2*middle)%p;
    //cout<<"Starting Calculating ..."<<endl;
    //cout<<"The Info to encrypt:"<<endl;
    //C1.print();
    //cout<<endl;
    //cout<<"The Info to deEncrypted:"<<endl;
    //C2.print();
    //cout<<endl;
    //M1=C1.power_mod(x,p);
    //M2.ex_euclid(M1,p);
    //M=(C2*M2)%p;
    //cout<<"The DeEncrypted Info:"<<endl;
    //M.print();
    //cout<<endl;
    gettimeofday(&t3, NULL);
    deltaT = (t3.tv_sec-t2.tv_sec) * 1000000 + t3.tv_usec-t2.tv_usec;
    cout<<endl<<"The DeEncrypted time costed:\t"<<deltaT<<"\t us"<<endl;
}
