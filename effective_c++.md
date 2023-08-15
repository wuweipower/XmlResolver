# 第一章 类型推导
## item1 理解模板类型推导
```cpp
template<typename T>
void f(ParamType param);

f(expr);
```
在编译期间，编译器使用 expr 进行两个类型推导：一个是针对 T 的，另一个是针对 ParamType
的。这两个类型通常是不同的，因为 ParamType 包含一些修饰，比如 const 和引用修饰符。

### 情景一 ParamType 是一个指针或引用，但不是通用引用
```cpp
template<typename T>
void f(T& param);
int x=27;           
const int cx=x;     
const int& rx=x

f(x);           // T is int, param is int&
f(cx);          // T is const int, param is const int&
f(rx);          // T is const int, param is const int&
```
**const传给T&保留const**  
**引用性在传递过程中会被忽略**  
如果T& 改为const T& f(cx)和f(rx)的T会推导为int  
以上规则使用于T*的情况

### 情景二 ParamType 是一个通用引用

- 如果 expr 是左值， T 和 ParamType 都会被推导为左值引用。这非常不寻常，第一，这是模板类型推导中唯一一种 T 被推导为引用的情况。第二，虽然 ParamType 被声明为右值引用类型，但是最后推导的结果是左值引用。
- 如果 expr 是右值，就使用正常的（也就是情景一）推导规则

```cpp
template<typename T>
void f(T&& param);

int x=27;
const int cx=x;
const int & rx=cx;
f(x);       //x 是左值，所以 T 是 int& ，
            //param 类型也是 int&
f(cx);      //cx 是左值，所以 T 是 const int& ，
            //param 类型也是 const int&
f(rx);      //rx 是左值，所以 T 是 const int& ，
            //param 类型也是 const int&
f(27);      //27 是右值，所以 T 是 int
            //param 类型就是 int&&
```

### 情景三： ParamType 既不是指针也不是引用
当 ParamType 既不是指针也不是引⽤时，我们通过传值（pass-by-value）的⽅式处理：
```cpp
template<typename T> void f(T param); //以传值的⽅式处理param
```
1. 和之前⼀样，如果 expr 的类型是⼀个引⽤，忽略这个引⽤部分
2. 如果忽略 expr 的引⽤性（reference-ness）之后， expr 是⼀个 const ，那就再忽略 const 。如果它是 volatile ，也忽略 volatile （ volatile 对象不常⻅，它通常⽤于驱动 程序的开发中。关于 volatile 的细节请参⻅Item40）

```cpp
int x=27; //如之前⼀样 c
onst int cx=x; //如之前⼀样 
const int & rx=cx; //如之前⼀样 
f(x); //T和param的类型都是int 
f(cx); //T和param的类型都是int 
f(rx); //T和param的类型都是int
```

只有在传值给形参时才会忽略 const （和 volatile ）这⼀点很重要. 对于reference-to-const 和pointer-to-const 形参来说， expr 的常量性 const ness在推导时会被保留
```cpp
template<typename T> void f(T param); //仍然以传值的⽅式处理param 
const char* const ptr = //ptr是⼀个常量指针，指向常量对象 
"Fun with pointers"; 
f(ptr); //传递const char * const类型的实参
        //param 是 const char*
```
在这⾥，解引⽤符号（*）的右边的 const 表⽰ ptr 本⾝是⼀个 const ： ptr 不能被修改为指向 其它地址，也不能被设置为null（解引⽤符号左边的 const 表⽰ ptr 指向⼀个字符串，这个字符 串是 const ，因此字符串不能被修改）。当 ptr 作为实参传给 f ，组成这个指针的每⼀⽐特都被 拷⻉进 param 。像这种情况， ptr ⾃⾝的值会被传给形参，根据类型推导的第三条规则， ptr ⾃⾝的常量性 const ness将会被省略，所以 param 是 const char\* ，

#### 数组的情况
两种类型（ const char* 和 const char[13] ）是不⼀样的，但是由于数组退化为指针的规则， 编译器允许这样的代码。

```cpp
template<typename T> void f(T param);
const char name[] = "J. P. Briggs";
f(name); //name是⼀个数组，但是T被推导为const char*

template<typename T> void f(T& param);
f(name); //T 被推导为了真正的数组！这个类型包括了数组的⼤⼩，在这个例⼦中 T 被推导为 const char[13]

template<typename T, std::size_t N> 
constexpr std::size_t arraySize(T (&)[N]) noexcept 
{
return N;  
}
```
在C++中不只是数组会退化为指针，函数类型也会退化为⼀个函数指针，我们对于数组类型推导的 全部讨论都可以应⽤到函数类型推导和退化为函数指针上来

```cpp
void someFunc(int, double); //someFunc是⼀个函数， //类型是void(int, double) 
template<typename T> void f1(T param); //传值给f1 
template<typename T> void f2(T & param); //传引⽤给f2 
f1(someFunc); //param被推导为指向函数的指针， //类型是void(*)(int, double) 
f2(someFunc); //param被推导为指向函数的引⽤， //类型是void(&)(int, double)

```

- 在模板类型推导时，有引⽤的实参会被视为⽆引⽤，他们的引⽤会被忽略 
- 对于通⽤引⽤的推导，左值实参会被特殊对待 
- 对于传值类型推导， const 和/或 volatile 实参会被认为是non- const 的和non- volatile 的
- 在模板类型推导时，数组名或者函数名实参会退化为指针，除⾮它们被⽤于初始化引⽤


## 理解auto

除了以下情况，auto 后面的变量类型就是模板中的param的类型，行为是一致的。  
```cpp
auto x = {1,2}; // /类型是std::initializer_list<int>，
```
对于花括号的处理是 auto 类型推导和模板类型推导唯⼀不同的地⽅
```cpp
template<typename T> //带有与x的声明等价的 
void f(T param); //形参声明的模板 
f({ 11, 23, 9 });//错误！不能推导出T

//然⽽如果在模板中指定 T 是 std::initializer_list<T> ⽽留下未知 T ,模板类型推导就能正常⼯ 作：
template<typename T> void f(std::initializer_list<T> initList);
f({ 11, 23, 9 }); //ok
```

C++14允许 auto ⽤于函数返回值并 会被推导（参⻅Item3），⽽且C++14的lambda函数也允许在形参声明中使⽤ auto 。但是在这些 情况下 auto 实际上使⽤模板类型推导的那⼀套规则在⼯作，⽽不是 auto 类型推导，所以说下⾯ 这样的代码不会通过编译
```cpp
auto createInitList() { 
    return { 1, 2, 3 }; //错误！不能推导{ 1, 2, 3 }的类型 
    }
```

## 理解decltype

decltype 只是简单的返回名字或者表达式的类型。返回的是完整的类型  

- decltype 总是不加修改的产⽣变量或者表达式的类型。 
- 对于 T 类型的不是单纯的变量名的左值表达式， decltype 总是产出 T 的引⽤即 T& 。
- C++14⽀持 decltype(auto) ，就像 auto ⼀样，推导出类型，但是它使⽤ decltype 的规则 进⾏推导。

在C++11中，decltype 最主要的⽤途就是⽤于声明函数模板，⽽这个函数返回类型依赖于形参类型。  
对⼀个 T 类型的容器使⽤ operator[] 通常会返回⼀个 T& 对象，⽐如 std::deque 就是这样。但是 std::vector 有⼀个例外，对于 std::vector<bool> ， operator[]不会返回bool& ，它会返回⼀个全新的对象

```cpp
template<typename Container, typename Index> //可以⼯作， 
auto authAndAccess(Container& c, Index i) //但是需要改良 
->decltype(c[i]) 
{ authenticateUser(); return c[i]; }
// 如果只是auto c[i]可能返回引用，而auto推导没有引用，编译器报错
// 改为delctype(auto) auto 说明符表⽰这个类型将会被推导， decltype 说明 decltype 的规则将会被⽤到这个推导过程中 去掉->decltype(c[i])
// 改进，Container&改为Container&&
// 在这个模板中，我们不知道我们操纵的容器的类型是什么，那意味着我们同样不知道它使⽤的索 引对象（index objects）的类型，对⼀个未知类型的对象使⽤传值通常会造成不必要的拷⻉，对 程序的性能有极⼤的影响，还会造成对象切⽚⾏为
// 改进，return c[i];改为return std::forward<Container>(c)[i];
// 改进，除了上面。为了C++11 加上->decltype(std::forward<Container>(c)[i])
```

如果⼀个不是单纯变量名的左值表达式的类型是 T ，那么 decltype 会把这个表达式的类型报告为 T& 。
```cpp
int x;
decltype((x)); // 为int&
```
- decltype 总是不加修改的产⽣变量或者表达式的类型。 
- 对于 T 类型的不是单纯的变量名的左值表达式， decltype 总是产出 T 的引⽤即 T& 。
- C++14⽀持 decltype(auto) ，就像 auto ⼀样，推导出类型，但是它使⽤ decltype 的规则 进⾏推导。

## 条款四：学会查看类型推导结果

1. IDE 简单的把⿏标 移到它们的上⾯
2. 使用编译器
```cpp
template<typename T> //只对TD进⾏声明 
class TD; //TD == "Type Displayer"
TD<decltype(x)> xType; //引出包含x和y 
TD<decltype(y)> yType; //的类型的错误消息
error: aggregate 'TD<int> xType' has incomplete type and cannot be defined 
error: aggregate 'TD<const int *> yType' has incomplete type and cannot be defined
```
3. 运行时输出
```cpp
std::cout << typeid(x).name() << '\n'; //显⽰x和y的类型 
std::cout << typeid(y).name() << '\n'; //不同编译器不一样
```

# auto
## 条款五：优先考虑auto⽽⾮显式类型声明
```cpp
for(const std::pair<std::string, int>& p : m) {
//⽤p做⼀些事 
}
//std::unordered_map的key是 const 的 std::pair 的类型不是 std::pair<std::string, int> ，⽽是 std::pair<const std::string, int> 这样会有拷贝发生
// 所以用auto就避免这种情况
```
auto 变量必须初始化，通常它可以避免⼀些移植性(不同平台上的内置类型的字节大小不太一样)和效率性的问题，也使得重构更⽅便， 还能让你少打⼏个字


## 条款六：auto推导若⾮⼰愿，使⽤显式类型初始化惯⽤法
std::vector<bool>::reference 之所以存在是因为 std::vector<bool> 规定了使⽤⼀个打包形 式（packed form）表⽰它的 bool ，每个 bool 占⼀个bit。那给 std::vector 的 operator[] 带 来了问题，因为 std::vector<T> 的 operator[] 应当返回⼀个 T& ，但是C++禁⽌对 bit s的引 ⽤。⽆法返回⼀个 bool& ， std::vector<bool> 的 operator[] 返回⼀个⾏为类似于 bool& 的对 象。要想成功扮演这个⻆⾊， bool& 适⽤的上下⽂ std::vector<bool>::reference 也必须⼀样 能适⽤。在 std::vector<bool>::reference 的特性中，使这个原则可⾏的特性是⼀个可以向 bool 的隐式转化。（不是 bool& ，是** bool **。要想完整的解释 std::vector<bool>::reference 能模拟 bool& 的⾏为所使⽤的⼀堆技术可能扯得太远了，所以 这⾥简单地说隐式类型转换只是这个⼤型⻢赛克的⼀⼩块）

`bool highPriority = features(w)[5]; //显式的声明highPriority的类型`这⾥， features 返回⼀个 std::vector<bool> 对象后再调⽤ operator[] ， operator[] 将会返 回⼀个 std::vector<bool>::reference 对象，然后再通过隐式转换赋值给 bool 变量 highPriority 。 highPriority 因此表⽰的是 features 返回的 std::vector<bool> 中的第五个bit，这也正如我们所期待的那样。

`auto highPriority = features(w)[5]; //推导highPriority的类型`同样的， features 返回⼀个 std::vector<bool> 对象，再调⽤ operator[] ， operator[] 将会 返回⼀个 std::vector<bool>::reference 对象，但是现在这⾥有⼀点变化了， auto 推导 highPriority 的类型为 std::vector<bool>::reference ，但是 highPriority 对象没有第五bit的值。

- 不可⻅的代理类可能会使 auto 从表达式中推导出“错误的”类型 
- 显式类型初始器惯⽤法强制 auto 推导出你想要的结果

# 现代C++
## 条款七：区别使⽤()和{}创建对象
赋值和初始化不一样。赋值是两个变量和对象都已经初始化了，而初始化对象使用一个已经初始化的对象来初始化。如下：

```cpp
Widget w1; //调⽤默认构造函数 
Widget w2 = w1; //不是赋值运算，调⽤拷⻉构造函数 
w1 = w2; //是赋值运算，调⽤拷⻉赋值运算符（copy operator=）
```
⼀般来说，初 始化值要⽤()或者{}括起来或者放到等号"="的右边  
```cpp
class Widget{ 
private: 
int x{ 0 }; //没问题，x初始值为0 
int y = 0; //也可以 
int z(0); //错误！ 
}
```
另⼀⽅⾯，不可拷⻉的对象（例如 std::atomic ——⻅Item40）可以使⽤花括号初始化或者⼩括 号初始化，但是不能使⽤"="初始化
```cpp
std::atomic<int> ai1{ 0 }; //没问题 
std::atomic<int> ai2(0); //没问题 
std::atomic<int> ai3 = 0; //错误！
```


- 括号初始化是最⼴泛使⽤的初始化语法，它防⽌变窄转换，并且对于C++最令⼈头疼的解析 有天⽣的免疫性 
- 在构造函数重载决议中，括号初始化尽最⼤可能与 std::initializer_list 参数匹配，即 便其他构造函数看起来是更好的选择 
- 对于数值类型的 std::vector 来说使⽤花括号初始化和⼩括号初始化会造成巨⼤的不同
- 在模板类选择使⽤⼩括号初始化或使⽤花括号初始化创建对象是⼀个挑战。


在C++中，括号的使用方式在初始化上有以下区别：
1. 聚合初始化（Aggregate Initialization）：
使用花括号 {} 进行初始化时，可以进行聚合初始化，适用于数组、结构体和类等聚合类型的对象。
```cpp
Copy
int arr[] = {1, 2, 3}; // 数组的聚合初始化
struct Point {
    int x;
    int y;
};
Point p = {10, 20}; // 结构体的聚合初始化
```
使用花括号 {} 进行聚合初始化时，可以按顺序或指定成员名称的方式进行初始化。

2.列表初始化（List Initialization）：
使用圆括号 () 进行初始化时，可以进行列表初始化，适用于各种类型的对象。
```cpp
Copy
int x(5); // 使用圆括号进行初始化
std::vector<int> vec{1, 2, 3}; // 列表的初始化
```
列表初始化可以用于标量类型、类类型、容器类型等各种对象的初始化，并支持自动类型推导。

3.隐式类型转换和初始化错误的处理：
使用圆括号 () 进行初始化时，如果存在隐式类型转换，编译器可能会执行隐式类型转换，而使用花括号 {} 进行初始化时，编译器会更严格地检查初始化的正确性，避免了某些类型转换和初始化错误。
```cpp
Copy
int a = 3.14; // 隐式类型转换，a 的值为 3
int b(3.14); // 编译错误，不允许将 double 类型直接转换为 int 类型
int c{3.14}; // 编译错误，不允许将 double 类型直接转换为 int 类型
```
总的来说，使用花括号 {} 进行初始化更加通用且严格，可以实现聚合初始化、列表初始化以及更好的类型检查。
而圆括号 () 进行初始化则更多地用于传统的初始化方式，并且可能存在隐式类型转换的情况。具体选择哪种初始化方式取决于您的需求和编程上下文。

## 先考虑nullptr⽽⾮0和NULL

```cpp
void f(int); //三个f的重载函数 
void f(bool); 
void f(void*); 
f(0); //调⽤f(int)⽽不是f(void*) 
f(NULL); //可能不会被编译，⼀般来说调⽤f(int)， //绝对不会调⽤f(void*)
```

nullptr 的优点是它不是整型。⽼实说它也不是⼀个指针类型，但是你可以把它认为是所有类型 的指针。   
nullptr 的真正类型是 std::nullptr_t ，在⼀个完美的循环定义以后， std::nullptr_t ⼜被定义为 nullptr 。   
std::nullptr_t 可以隐式转换为指向任何内置类型的 指针，这也是为什么 nullptr 表现得像所有类型的指针。  
使⽤ nullptr 调⽤ f 将会调⽤ void* 版本的重载函数，因为 nullptr 不能被视作任何整型：  

0存在模板推导为0的情况，NULL肯定不会被模板推导为指针

- 优先考虑 nullptr ⽽⾮ 0 和 NULL 
- 避免重载指针和整型


## 条款九：优先考虑别名声明⽽⾮typedef s

```cpp
//FP是⼀个指向函数的指针的同义词，它指向的函数带有 
//int和const std::string&形参，不返回任何东西 
typedef void (*FP)(int, const std::string&); //typedef 
//含义同上 
using FP = void (*)(int, const std::string&); //别名声明
```

```cpp
template<typename T>                          //MyAllocList<T>是 
using MyAllocList = std::list<T, MyAlloc<T>>; //std::list<T, MyAlloc<T>> //的同义词 
MyAllocList<Widget> lw; //⽤⼾代码

template<typename T> //MyAllocList<T>是 
struct MyAllocList { //std::list<T, MyAlloc<T>> 
    typedef std::list<T, MyAlloc<T>> type; //的同义词 
};

MyAllocList<Widget>::type lw; //⽤⼾代码
```

更糟糕的是，如果你想使⽤在⼀个模板内使⽤ typedef 声明⼀个链表对象，
⽽这个对象⼜使⽤了 模板形参，你就不得不在 typedef 前⾯加上 typename ：
```cpp
template<typename T> class Widget { //Widget<T>含有⼀个 
private: //MyAllocLIst<T>对象 
typename MyAllocList<T>::type list; //作为数据成员 … 
};

template<typename T> 
using MyAllocList = std::list<T, MyAlloc<T>>; //同之前⼀样 
template<typename T> class Widget { 
    private: MyAllocList<T> list; //没有“typename” … //没有“::type” 
    };
```
如果有人写了以下代码， MyAllocList<Wine>::type推断就会出问题，所以一定要加上typename
```cpp
template<> //当T是Wine 
class MyAllocList<Wine> { //特化MyAllocList 
private: enum class WineType //参⻅Item10了解 
{ White, Red, Rose }; //"enum class" WineType type; //在这个类中，type是 … //⼀个数据成员！ 
};

```
- typedef 不⽀持模板化，但是别名声明⽀持。 
- 别名模板避免了使⽤“ ::type ”后缀，⽽且在模板中使⽤ typedef 还需要在前⾯加上 typename
- C++14提供了C++11所有type traits转换的别名声明版本


## 条款⼗：优先考虑限域enum⽽⾮未限域enum

- C++98的 enum 即⾮限域 enum 。 
- 限域 enum 的枚举名仅在 enum 内可⻅。要转换为其它类型只能使⽤cast。 
- ⾮限域/限域 enum 都⽀持底层类型说明语法，限域 enum 底层类型默认是 int 。⾮限域 enum 没有默认底层类型。 
- 限域 enum 总是可以前置声明。⾮限域 enum 仅当指定它们的底层类型时才能前置。


## 条款⼗⼀：优先考虑使⽤deleted函数⽽⾮使⽤未定义的私有声明


- ⽐起声明函数为 private 但不定义，使⽤deleted函数更好 
- 任何函数都能被删除（be deleted），包括⾮成员函数和模板实例（译注：实例化的函数）












返回的肯定是右值引用，因为remove_reference后就为确定类型了，就不需要推断了
```cpp
template<typename _Tp>
constexpr typename std::remove_reference<_Tp>::type&&
move(_Tp&& __t) noexcept
{ return static_cast<typename std::remove_reference<_Tp>::type&&>(__t); }
```

这里就是通用引用了
```cpp
  template<typename _Tp>
    constexpr _Tp&&
    forward(typename std::remove_reference<_Tp>::type& __t) noexcept
    { return static_cast<_Tp&&>(__t); }
```

### 切片问题
对象切片问题发生在以下情况下：当将派生类对象赋值给基类对象时，如果使用的是基类的引用或指针，那么只会复制派生类对象中与基类相对应的部分，而派生类特有的成员将被丢失，从而产生了对象切片
