typedef long long LLONG_INTEGER;
typedef unsigned long long US_LLONG_INTEGER;
namespace RSA
{
	void Init(); 
	bool IsPrimer(LLONG_INTEGER n); 
	void Encrypt(char *filename,char *output,char *pub_key_file); 
	void DeEncrypt(char *filename,char *output,char *pri_key_file); 
	void GetPrimers(LLONG_INTEGER &p,LLONG_INTEGER &q); 
	void Create_Key(char *pub_key_file,char *pri_key_file);  
	LLONG_INTEGER Gcd(LLONG_INTEGER a,LLONG_INTEGER b); 
	void Ex_Gcd(LLONG_INTEGER a,LLONG_INTEGER b,LLONG_INTEGER &x,LLONG_INTEGER &y); 
	LLONG_INTEGER Mod(US_LLONG_INTEGER a,US_LLONG_INTEGER b,US_LLONG_INTEGER n);  //a^b%c
	int Encrypt_Int16(char *pub_key_file,US_LLONG_INTEGER in);
	int DeEncrypt_Int16(char *pri_key_file,US_LLONG_INTEGER in);
};
