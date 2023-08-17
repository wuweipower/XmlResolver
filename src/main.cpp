#include<iostream>

using namespace std;
namespace std{
    int x  =1;
}

namespace A{
    int B = 3;
}
void func()
{
    long long str[0xffff];
}
void func5()
{
    long long str[UINT32_MAX];
}
class C
{
    const int a=1;
};
int main()
{
    cout<<"hello"<<endl;
    const char * str = "123";
    const char * str1 = "123";
    cout<<boolalpha<<(str == str1)<<endl;
    char *p = "hello";
    cout<<(str == p)<<endl;
    cout<<(1>2) ? 4 : 3;
}
