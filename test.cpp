#include<iostream>
#include<vector>
using namespace std;
typedef struct pair{
	int x;
	int y;
} Pair;


int main()
{
	Pair* p;
	vector<int> v1(100,0);

	vector<int> v2;

	v2 = v1;

	char str[100];
	str[0] = 1;
	str[1] = 0;
	str[2] = 0;
	str[3] = 0;
	str[4] = 1;
	str[5] = 0;
	str[6] = 0;
	str[7] = 0;

	p = (Pair*)str;

	cout<<p->x<<" "<<p->y<<"\n";

	return 0;
}
