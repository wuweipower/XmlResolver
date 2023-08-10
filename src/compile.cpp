#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;

int main()
{
    // 优化建议就是处理死循环
    string src = "print(\"hello world\")";
    fstream fs;
    ofstream f;
    f.open("src.py");
    f<<src;
    f.close();
    system("python3.8 src.py > res.txt");

    fs.open("res.txt");
    fs.rdbuf();
    std::stringstream buffer;
    buffer << fs.rdbuf();
    string res = buffer.str();
    cout<<res<<endl;
    fs.close();
}
