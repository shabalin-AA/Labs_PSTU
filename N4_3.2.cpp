#include <iostream>
using namespace std;

int main(){
	char a, b, c, d, e; 				  //переменные для символов
	cout<<"enter five symbols: "<<endl;  //предлагаем пользователю ввести пять символов
	cin>>a>>b>>c>>d>>e; 				//принимаем пять символов с клавиатуры
	cout<<endl<<e<<"\t"<<d<<"\t"<<c<<"\t"<<b<<"\t"<<a; //выводим символы в обратном порядке через таб
	return 0;
}



