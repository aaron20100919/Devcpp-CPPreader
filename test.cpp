#pragma GCC optimize(0)
#include<bits/stdc++.h>
#define fre(x) freopen(#x".in","r",stdin),freopen(#x".out","w",stdout);
#define heap priority_queue
using namespace std;
typedef long long ll;
inline ll read()
{
	char c = getchar();
	ll x = 0, y = 1;
	while(c < 48 || c > 57) y = c == 45 ? -1 : 1, c = getchar();
	while(c >= 48 && c <= 57) x = (x << 1) + (x << 3) + (c ^ 48), c = getchar();
	return x * y;
}

const int N = 1e4 + 10;
const int INF = 0x3f3f3f3f;

int main()
{
	long long int a, b, sum = 0;
	cin >> a >> b;
	if(a == 0 && b == 0)
	{
		for(int i = 1; i <= 1000000000; i++)
		{
			sum++;
		}
		sum -= 999999999;
		for(int i = 1; i <= 1000000000; i++)
		{
			sum++;
		}
		sum -= 999999999;
		for(int i = 1; i <= 1000000000; i++)
		{
			sum = 0;
		}
		cout << sum << endl;
	}
	else
	{
		while(a--)
		{
			for(int i = 1; i <= 1000000000; i++)
			{
				sum++;
			}
			sum -= 999999999;
		}
		while(b--)
		{
			for(int i = 1; i <= 1000000000; i++)
			{
				sum++;
			}
			sum -= 999999999;
		}
		cout << sum << endl;
	}
	return 0;
} 
