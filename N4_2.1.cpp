#include <iostream>
using namespace std;

int main()
{
	double f; //переменная для кол-ва градусов по Фаренгейту
	cout<<"enter degrees Fahrenheit: "; //предлагаем пользователю ввести градус по Фаренгейту
	cin>>f;  //принимаем с клавиатуры число
	cout<<"equivalent degrees Celsius: "<<5*(f-32)/9;  //выводим эквивалентное число градусов по Цельсию
	return 0;
}


