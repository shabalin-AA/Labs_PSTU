#include <iostream>
using namespace std;

int join(int q, int p, int r)
{
	cout<<q*100+p*10+r<<endl;
	return q*100 + p*10 + r;
}

int main()
{
	int x, a, b, c;
	bool flag1 = false, flag2 = false;
	while((flag1==false)||(flag2==false))
	{
		cout<<"enter a three-digit integer ";
		cin>>x;
		if ((x>999)||(x<100))
		{
			cout<<"it must be three-digit"<<endl;
			flag1 = false;
		}
		else
		{
			flag1 = true;
		}
		a = x/100;
		b = x/10 - a*10;
		c = x%10;
		if ((a==b)||(b==c)||(c==a))
		{
			cout<<"digits shouldn't be repeated "<<endl;
			flag2 = false;
		}
		else
		{
			flag2 = true;
		}
	}
	
	int max = 0, n;
	
	n = join(a,b,c);
	if (n>max)	{max = n;}
	n = join(a,c,b);
	if (n>max)	{max = n;}
	n = join(b,a,c);
	if (n>max)	{max = n;}
	n = join(b,c,a);
	if (n>max)	{max = n;}
	n = join(c,b,a);
	if (n>max)	{max = n;}
	n = join(c,a,b);
	if (n>max)	{max = n;}
	
	cout<<"max is "<<max;
	return 0;
}
