## 关于在头文件中定义变量的问题
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

## 转为绝对路径
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

## shared_ptr中this指针的问题
enable_shared_from_this and shared_from_this
```C++
auto p = std::shared_ptr<T>(this);// it will occur double free since deconstructer
auto p = std::make_shared<T>(*this)l //copy and construct a new one

auto p = shared_from_this(); // standard
```

## 有static性质变量的线程安全问题
类里面如果有mutex，这个类的拷贝构造函数就被编译器删除了，因为锁是无法赋值和move的，

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

## 很有用的宏
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

## 关于拷贝与赋值
赋值不是拷贝，赋值是更新内存中的值，而拷贝是构造一份新的，开辟了新的空间  
赋值重载函数于拷贝构造函数的调用时期：  
当两个对象都已经创建后，之间的 `=` 是赋值，如果是第二个对象是用第一个对象的来创建， 就会调用拷贝构造函数。  

## 关于move
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
## 关于解析器
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


## meta-programming
广义就是程序甲编写程序乙
```cpp
// in cpp
// store a src.py
system("python src.py")
```

狭义的就是
代码自己**操作自己本身的代码**，比如java的反射机制，一般情况下代码操作数据，元编程操作的是代码。
可以创建新的代码，类似lex和yacc，meta比较好的翻译是post（后）

模型的模型就叫做元模型

```cpp
template<class T>
void wrapper(T&& arg)
{
    // arg is always lvalue
    foo(std::forward<T>(arg));
    // Forward as lvalue or as rvalue, depending on T
}
```

## auto and decltype
auto自动推导类型，配合lambda特别好

对于非引用，非指针场景下，初始化器涉及的引用性，不可变性会丢失，因为模板实参推导规则也是这样的
```cpp
const int a = 1;
auto age = a;
a = a + 1; //ok
```

decltype：
功能:
从表达式中推断出要定义变量的类型，但却不用表达式的值去初始化变量，不会触发表达式的求值计算

如果“表达式 ”的值类别是亡值，将会推导出T&&  
如果“表达式 ”的值类别是左值，将会推导出T&  
如果“表达式 ”的值类别是纯右值，将会推导出T  
如果“表达式”被()包裹，将会推导出T&，即引用类型  
与auto区别
无引用性、常量性丢失问题，const int konst =0; decltype(konst) dk = 100; //dk类型为const int  
单独使用decltype时无需初始化，且decltype表达式无需实际求值  

decltype(auto): since c++14
以初始化表达式替换decltype(auto)中的auto，再通过decltype推导类型（必须存在初始化表达式）

## move
C++11开始，提出右值概念，用于标记哪些变量的资源是可以窃取的，配合移动构造、移动复制构造等方式实现资源的窃取
- glvalue: 泛左值
- lvalue: 左值，一般是可以取地址的值
- rvalue: 右值
- prvalue: 纯右值，一般除字符串字面量外的字面量，以及临时变量
- xvalue: 将亡值，T&&函数返回值、std::move返回值等

通过实现移动构造、移动复制构造来支持资源的移动，临时变量场景会自动使用移动构造  
对于后续逻辑中不再使用的局部变量，可以借助std::move将左值主动标记成右值概念  
注意：move只是将变量转为右值，真正窃取资源要配合移动构造  
右值引用也是为了减少拷贝  

## lambda
lambda：  
功能：匿名函数，本质类似函数对象  
捕获：  
一般只有局部变量才需要捕获  
函数对象实例化时，捕获列表相当于构造函数参数  
返回值类型：  
一般情况下，lambda无需说明返回值类型，编译器会根据返回语句自动推导  
当存在多条返回语句，且类型不同时，可以通过尾置返回类型描述返回值类型，  

## shared_ptr
shared_ptr：  
同一指针可归属多个shared_ptr对象，通过引用计数记录关联者数量，当引用计数归零后，自动释放关联指针  
对于gcc一般情况，使用原子变量控制引用记录的并发增减  
内部构成：  
引用计数的控制块空间  
管理对象的指针  
通过make_shared创建的shared_ptr对象，控制块空间与管理对象的指针是连续的空间  

```cpp
struct Son;
struct Father{
    shared_ptr<Son> son_;
};
struct Son{
    shared_ptr<Father> father_;
}
auto son = make_shared<Son>();
auto father = make_shared<Father>();
son->father_ = father;
father->son_ = son;
```
1. Son与Father相互包含对方
2. father变量（shared_ptr类型）析构时，发现引用计数非0（son变量内还包含father_ `son->father_ = father;`这一句引用计数+1），故不触发释放指针空间
3. son变量（shared_ptr类型）析构时，发现引用计数非0（Father中还包含son_）故不触发指针空间释放
4. 内存泄漏发生

解决办法就是使用weak_ptr;  
数据成员son_，以及father_的类型从shared_ptr变更成了weak_ptr,son_、father_数据成员被赋值时，不会导致变量son、father的引用计数变化，故析构时不会导致循环引用问题

weak_ptr对象未析构，但关联的shared_ptr对象已析构
那么shared_ptr关联的指针会被释放吗？控制块呢？make_shared场景下呢？

unique_ptr：  
功能简介    
与shared_ptr类似，但unique_ptr不支持拷贝，只支持移动，故一个指针只能归属一个    unique_ptr对象  
unique_ptr因为不支持共享，故没有像shared_ptr类似的引用计数控制块  
unique_ptr的设计目标:  
提供成本与裸指针接近，且自动管理资源生命周期的能力  

c++17，开始支持结构化绑定声明
```cpp
std::map<std::string,int> users{“tom”:10,”jerry”:10};
for (const auto &[name, age] : users) {
    std::cout << “name: “ << name
    std::cout <<”, age: “ << age
    std::cout << std::endl;
}
```

# cpp
通常，main()被启动函数调用，而启动代码是编译器加在代码中的，是程序与操作系统之间的桥梁  
字符的数字表示\0 \ox \u \U分别用八进制，十六进制，unicode 8个十六进制位，unicode 16个十六进制位  
int是计算机最自然的类型，一般情况下会有整型提升。
在算术表达式的计算中，不同类型的数据会进行类型转换。  
强制类型转换不会改变变量本身，而是创建了一个新的，指定类型的值。  
引入什么cast是为了更加安全。

编译器不会检查[]下标是否有效
struct也可以使用列表初始化
```cpp
struct A
{
    string name;
    int age : 4;//位字段
}
A a{"",1};
```
共用体每次智能存储一个值，共用体长度为最大成员长度  
匿名共用体没有名称，其成员将成为位于相同地址处的变量，显然，每次只有一个成员是当前成员  

```cpp
struct A
{
    int a;
    union id
    {
        long id_n;
        char id_c;
    } id_val;
}
A a;
cout<<a.id_val.id_n;

struct A
{
    int a;
    union
    {
        long id_n;
        char id_c;
    };
}
A a;
cout<<a.id_n;//不需要中间变量了
```

```
while(i++,j++)不行 for可以
```

逗号运算符是从左到右进行计算的

## 指针与const
1. 指向常量的对象，防止通过指针进行改变
```cpp
int age = 1;
const int* p = &age;
// 对*p的任何操作都是不合理的
// 常量地址不能赋值给普通指针
```
2. 将指针本身声明为常量，防止改变指针指向的位置
```cpp
int* const p = &age;// p只能指向这个地址了，不过可以修改地址代表的值
```

多使用const  
可以避免无意间的修改  
const能能使函数处理const和非const实参

c风格的字符串内置结束符，不以空值字符结尾的char数组不是字符串

函数指针
```cpp
类型 (*名字) (参数列表)
int (*p) (int,int);// p就是函数指针变量
使用typedef简化
typedef int (*func_ptr)(int,int); //func_ptr就是int (int ,int)函数指针类型
```

## 模板
非模板 > 显式具体化 > 模板
```cpp
template<typename T> void swap(T&,T&);

显式具体化
template <> void swap<int>(int&,int&);
template <> void swap(int&,int&); 
//不过需要单独提供函数体
```

函数模板本身不会生成函数定义，只是用于生成函数定义的方案。  
编译器使用模板为特定类型生成函数定义是，得到的是模板实例  
函数调用的时候，编译器会生成函数的实例，成为隐式实例化。  
C++现在允许显示实例化，直接命令编译器创建特定的实例。如swap\<int>()
```cpp
//直接生成了实例，不需要像上面一样单独另外写个函数体
//这个只是声明
template void swap<int>(int,int); 

//可以不提前声明，直接使用
swap<int>(a,b);
```

**不建议将变量的定义放在头文件中，以避免重定义错误。头文件应当专注于声明函数、类、结构体、模板，#define,const,内联函数，枚举等的接口，以及定义类型别名和常量的声明。变量的定义应当放在源文件中，以确保每个源文件只有一个定义。**

如果在头文件中声明但是，多个包含它的cpp文件又定义它，那肯定是重定义  
前面说的只是声明所以多个cpp包含，并没有又定义

\<>将在存储标准头文件的主机系统中的文件系统中查找，但是文件名是双引号在当前工作目录或者源代码目录，如果没找到就去标准位置找

在同一文件中，只能将同一个头文件包含一次，但是多个文件都可以自己包含一份，链接

由于链接，同名不同文件的同名变量注意作用域

```cpp
static int a;//只在本文件
int a;// 链接的文件都可以访问
void f()
{
    static int a;// 只有函数内部可以访问
}
```


```cpp
volatile int a;
//如果程序中，两次用到a，可能编译器会将a的值缓存在寄存器，而不是重新找。
//volatitle就是告诉编译器不要做这种优化
//因为a可能会变
mutable int a;
const的对象或者成员函数可以修改它
```

全局变量加了const相当于加了static  
const全局变量多个文件包含后，链接不会出现问题

**new和replace new**

```cpp
//转换函数
class A
{
    int c;
    operator int() const
    {
        return c;
    }
}
```
防止隐式转换加上explicit

使用虚函数是，每个对象添加了一个隐藏成员，这个成员保存了一个指向函数地址数组的指针。这个数组叫虚函数表，虚函数表中存储了为类对象进行声明的虚函数的地址。

虚函数成本：
- 每个对象都都将增大，增大量为存储地址的空间
- 每个类都要创建一个虚函数地址表
- 每个函数调用都要去表中查找地址

析构函数最好为虚函数
```cpp
Parent* b = new child;
delete b;

//如果析构不是虚函数，则delete不了派生类
//如果是，则会先调用child的析构，再调用parent
```
