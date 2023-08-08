关于在头文件中定义变量的问题。
核心就是include就是复制粘贴
a.h
```c++
int x = 1;
```
或者
```c++
int x; //默认赋值为0 因为是全局变量。
```


t1.cpp
```c++
#include "a.h"
```

t2.cpp
```c++
#include "a.h"
```

链接的时候，t1 和 t2里面都是全局的`int x = 1;` 就是在全局范围内定义了两次。

如果a.h
```c++
static int x = 1;
// static在全局的话，表明这个变量属于这个文件
```


或者
```c++
const int x =1;// 因为常量不会被第二次定义，编写的时候IDE会提示错误
```

或者
```c++
extern int x;// 只是声明，在其他文件定义，然后使用的话，就是链接的那个文件的值
             // 重新定义也是可以的，因为这里只是声明。
```
关于extern， 没有初始化的话就是声明，因为你可能没有include这个变量或者函数的文件，你可以extern后链接这个库，就可以使用了。

将相对路径转为绝对路径，除了自己写转换拼接的方式可以使用一下代码
```c++
char* abs = new char[1024];
// or char abs[1024]; //in the stack
#ifdef _WIN32
_fullpath(abs,filename,1024);
#else
realpath(filename,abs);
#endif
```

enable_shared_from_this and shared_from_this
```C++
auto p = std::shared_ptr<T>(this);// it will occur double free since deconstructer
auto p = std::make_shared<T>(*this)l //copy and construct a new one

auto p = shared_from_this(); // standard
```

类里面如果有mutex，这个类的拷贝构造函数就被编译器删除了，因为锁是无法赋值和move的，所以要显示private =delete

具有static性质的变量，容易发生线程不安全，可以使用__thread每个线程保存一份。
```c++
const char* print(const string name)
{
    static __thread string str;
    str = "I'm thread " + name;
    return str.c_str();
}

void task_1()
{
    while(true)
    {
        string temp = print("t1");
        if(temp != "I'm thread t1")
        {
            std::cerr<<"error t1 "<<temp<<std::endl;
        }
    }

}
void task_2()
{
    while(true)
    {
        string temp = print("t2");
        if(temp != "I'm thread t2")
        {
            std::cerr<<"error t2 "<<temp<<std::endl;
        }
    }

}
TEST(thread,safaty)
{
    using std::thread;
    auto thread_1 = thread(task_1);
    auto thread_2 = thread(task_2);
    thread_1.join();
    thread_2.join();
}
```

static_assert 编译期间的断言，
assert是运行期间的判断，并且会强制中止程序

很有用的宏
```cpp
__LINE__
__func__
__FILE__
__PRETTY_FUNCTION__
assert
#define NAME(name) (#name)

多行宏用\换行
```
