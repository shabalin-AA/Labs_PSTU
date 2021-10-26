#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

int main()
{
	srand(time(NULL));
	int n, s;
	n = rand() % 100 -(-1);
	
	cout<<"there is a number from 1 to 100. you have to guess it in 7 attempts"<<endl;
	for (int i=0; i<8; i++)
	{
		if (i==7)
		{
			cout<<"you lose!";
		}
		else
		{
			cout<<"attempt "<<i+1<<endl;
			cin>>s;
			if (s>n)
			{
				cout<<"your number is greater"<<endl;
			}
			else
			{
				if (s<n)
				{
					cout<<"your number is less"<<endl;
				}
				else
				{
					cout<<"you won!"<<endl;
					break;
				}
			}
		}
	}
	return 0;
}
