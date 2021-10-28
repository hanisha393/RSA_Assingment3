#include <bits/stdc++.h>
using namespace std;

//Function to check if randomly generated number is prime or not 
bool isPrime(long long int n)
{
    // Corner case
    if (n <= 1)
        return false;
 
    // Check from 2 to square root of n
    for (long long int i = 2; i <= sqrt(n); i++)
    {
        if (n % i == 0)
            return false;
    }

    return true;
}

//Function to calculate GCD
long long int gcd(long long int a, long long int b)
{
    if (a == 0)
        return b;

    return gcd(b%a, a);
}

//Function to calculate extended GCD to fine inverse
long long int gcdExtended(long long int a, long long int b, long long int* x, long long int* y)
{
    if (a == 0)
    {
        *x = 0, *y = 1;
        return b;
    }
    long long int x1, y1,temp=b%a;
    long long int gcd = gcdExtended(temp, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;
 
    return gcd;
}

//Function to find mod Inverse
long long int modInverse(long long int a, long long int m)
{
    long long int x, y, res;
    long long int g = gcdExtended(a, m, &x, &y);
    if (g != 1)
        cout << "Inverse doesn't exist";
    else
    {
        res = (x % m + m) % m;
    }
    return res;
}

//Function to perform encryption
long long int encrypt(long long int m,long long int e,long long int N)
{
	long long int temp=m;
	
	//calculating c=pow(m,e)%N
	for(int i=0;i<e-1;i++)
	{
		temp=(temp*m)%N;
	}
	return temp;
}

//Function to perform decryption
long long int decrypt(long long int c,long long int d,long long int N)
{
	long long int temp=c;
	
	//calculating m=pow(c,d)%N
	for(int i=0;i<d-1;i++)
	{
		temp=(temp*c)%N;
	}
	return temp;
}

//Function to perform optimized decryption
long long int optimized_decrypt(long long int c, long long int p, long long int q,long long int d, long long int xp,long long int xq)
{
	//findinf vp and vq
	// finding c1 and c2
    long long int c1=c%p;
	long long int c2=c%q;
	cout<<"\nc1:"<<c1<<"\tc2:"<<c2;
	
	//finding d1 and d2
	long long int d1=d%(p-1);
	long long int d2=d%(q-1);
	cout<<"\nd1:"<<d1<<"\td2:"<<d2;
	
	//finding vp = pow(c1,d1)%p
	long long int vp=c1;
	for (long long int i=0;i<d1-1;i++)
	{
		vp=(vp*c1)% p;
	}
	cout<<"\nvp1:"<<vp;
	
	//finding vq=pow(c2,d2)%q
	long long int vq=c2;
	for (long long int i=0;i<d2-1;i++)
	{
		vq=(vq*c2)% q;
	}
	cout<<"\nvq1:"<<vq;
	
	//calculating decrypted message
	return (xp*vp + xq*vq)%(p*q);
}

int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL))); 
	long long int p,q,N,e,d,m,phi,rand=1,c;
	bool is_prime = false;
	
	// generating p and q prime numbers randomly
	while(is_prime == false)
	{
		p= std::rand();
		is_prime = isPrime(p);
	}
	is_prime = false;
	while(is_prime == false)
	{
		q= std::rand();
		is_prime = isPrime(q);
	}

    // calculating N and phi
	N= p*q;
	phi=(p-1)*(q-1);
	
	// selecting e randomly such that it is in 2powerk+1 format and co-prime to phi
	for (long long int i=0;i<phi;i++)
	{
		e = pow(2,rand) +1;rand++;
		if (gcd(e,phi)==1)
		{
			break;
		}
	}

    // finding d = modinverse of e
	d=modInverse(e,phi);
    
    //finding xp and xq as they are independent of message m
    long long int q_inv = modInverse(q,p);
	long long int p_inv = modInverse(p,q);
	long long int xp=q*q_inv;
	long long int xq=p*p_inv;
	
	cout<<"\n---------------------------------------------------------------------------------------------------------------------------------------------------";
	cout<<"\nPublic Key (N,e):"<<"("<<N<<","<<e<<")";
	cout<<"\nPrivate Key (p,q,d):"<<"("<<p<<","<<q<<","<<d<<")";
	cout<<"\nFields required for optimized decryption: p_inv:"<<p_inv<<"\tq_inv"<<q_inv<<"\txp:"<<xp<<"\txq:"<<xq;
	
	// generating message also randomly
    m= std::rand();
	cout<<"\n\n---------------------------------------------Message to be Encrypted:"<<m<<"---------------------------------------------------------------------";
	
	clock_t t;
    t = clock();
    c=encrypt(m,e,N);cout<<"\nEncrypted msg:"<<c;
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;cout<<"\nTime Taken for Encyption:"<<time_taken;
    
    cout<<"\n\n---------------------------------------------Caliing direct Decryption!! Message to be Decrypted:"<<c<<"-----------------------------------------";
    t = clock();
    m=decrypt(c,d,N);cout<<"\ndeccrypted msg:"<<m;
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;cout<<"\nTime Taken for Direct_Decyption:"<<time_taken;
    
    cout<<"\n\n---------------------------------------------Caliing optimized Decryption!! Message to be Decrypted:"<<c<<"--------------------------------------";
    t = clock();
    m=optimized_decrypt(c,p,q,d,xp,xq);cout<<"\n\nOptimized_Deccrypted msg:"<<m;
    t = clock() - t;
    double time_taken_opt =  ((double)t)/CLOCKS_PER_SEC;cout<<"\nTime Taken for Optimized_Decyption:"<<time_taken_opt;
    
    cout<<"\n\n-------------------------------------------------------------------------------------------------------------------------------------------------";
	cout<<"\nDifference between time taken by optimized and direct decyption:"<<time_taken-time_taken_opt<<endl;

}
