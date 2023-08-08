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

const_cast主要是用来去除const限制，但是地址还是一样的，修改后，原来的值依然不变。

const int &age = 10;
int& age = 10; //error

赋值不是拷贝，赋值是更新内存中的值，而拷贝是构造一份新的，开辟了新的空间
赋值重载函数于拷贝构造函数的调用时期：
当两个对象都已经创建后，之间的 `=` 是赋值，如果是第二个对象是用第一个对象的来创建，就会调用拷贝构造函数。
关于左值右值，函数返回值是右值，std::move转为右值。而，窃取资源，也就是浅拷贝后并且将原来的设置为null，是在移动构造函数中完成的，所以需要类自己提供，而不是你move可以后就可以窃取资源。

函数传递参数例如下面其实里面形参是这样做的`A a = std::move(b)` 就只是加个=
```cpp
A func(A a)
{
    return a;// 返回值是右值类型
}
TEST(function,r)
{
    A b(4);
    func(std::move(b));// A c = func(std::move(b))结果与下面一致，好像是编译器优化
}
```

```cpp
4 constructor called
150090325 move constructor called
166190768 move constructor called
166190768 deconstructer called
150090325 deconstructer called
4 deconstructer called
```

xml树形结构的核心就是递归函数的编写
```cpp
parse(T parent, T previous_sibling)
{
    ...
    elem;
    before_elem_end_tag
    if(has_child_)
    {
        elem->append_child(parse(T, nullptr))
    }
    after_elem_end_tag
    if (next_is_not_parent_end_tag)
    {
        elem->set_next_sibling(parse(parent, elem));
        return elem;
    }
    else if(next_is_parent_end_tag)
    {
        return elem;
    }
}
```

项目的优化建议
1. 构造函数放在初始化列表
2. 不提交.gitignore
3. 简单的get函数最后加上空格和const
4. 尽量返回const string&
5. 不要在头文件using，容易污染
6. C++11 数据成员声明的时候就可以定义
7. 使用shared_prt 继承enable_shared_from_this 和使用shared_from_this
8. 一般可以在Comment类中定义一个: `typedef std::shard_ptr<Comment> ptr`;别的地方就可以`Comment::ptr parse_comment()`;或者在Comment头文件 `typedef std::shard_ptr<Comment> Command_Ptr `; 别的地方就可以 `Comment_Ptr parse_comment()`; 方便命名
9. 如果形参不需要改动使用const string&
10. 一般不要自己抛出异常
11. string默认为空
12. 时刻加上考虑越界的条件
13. 头文件中不要定义变量（会造成重复定义），声明extern即可，用另外一个文件定义，然后链接在一起
14. 使用了static最好加上__thread保证线程安全
15. nullptr不能赋值给string
16. 可以折叠switch中的case如果这几个case返回值一样
17. 不应该出现多个shared_ptr指向同一个裸指针，会double free
18. const 不能move
19. 
