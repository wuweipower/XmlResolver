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
const int cx=x; //如之前⼀样 
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
int b(3.14); // 
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
{ White, Red, Rose }; //"enum class" 
WineType type; //在这个类中，type是 
… //⼀个数据成员！ 
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
C++有沉重的C包袱，使得含糊的、能被视作数值的任何类型都能隐式转换为 int。
为了防止这些转换，我们可以创建这些deleted重载函数，；过滤掉这些可能被转换的类型。  
deleted函数可以禁止一些模板的实例化，而private的话会报错。
问题是模板特例化必须位于⼀个命名空间作⽤域，⽽不是类作⽤域。deleted函数不会出现这个问题，因为它不需要⼀个不同的访问级别，且他们可以在类外被删除（因此位于命名空间作⽤域）


- ⽐起声明函数为 private 但不定义，使⽤deleted函数更好 
- 任何函数都能被删除（be deleted），包括⾮成员函数和模板实例（译注：实例化的函数）

## 条款⼗⼆：使⽤override声明重写函数

要想重写⼀个函数，必须满⾜下列要求： 
- 基类函数必须是 virtual 
- 基类和派⽣类函数名必须完全⼀样（除⾮是析构函数) 
- 基类和派⽣类函数形参类型必须完全⼀样 
- 基类和派⽣类函数常量性 const ness必须完全⼀样 
- 基类和派⽣类函数的返回值和异常说明（exception specifications）必须兼容
- 函数的引⽤限定符（reference qualifiers）必须完全⼀样

```cpp
class Widget { 
public: …
void doWork() &; //只有*this为左值的时候才能被调⽤ 
void doWork() &&; //只有*this为右值的时候才能被调⽤ 
};
```

override 和 final 
(向虚函数添加 final 可以防⽌派⽣类重写。 final 也能⽤于类，这时这个类不能⽤作基 类)
对于 override ，它只在成员函数声明结尾处才被视为关键字  
- 为重写函数加上 override 
- 成员函数引⽤限定让我们可以区别对待左值对象和右值对象（即 *this )

## 条款⼗三：优先考虑const_iterator⽽⾮iterator
- 优先考虑 const_iterator ⽽⾮ iterator 
- 在最⼤程度通⽤的代码中，优先考虑⾮成员函数版本的 begin ， end ， rbegin 等，⽽⾮同 名成员函数

## 条款⼗四：如果函数不抛出异常请使⽤noexcept
```cpp
int f(int x) throw(); //C++98⻛格，没有来⾃f的异常 
int f(int x) noexcept; //C++11⻛格，没有来⾃f的异常
```
如果在运⾏时， f 出现⼀个异常，那么就和 f 的异常说明冲突了。在C++98的异常说明中，调⽤ 栈（the call stack）会展开⾄ f 的调⽤者，在⼀些与这地⽅不相关的动作后，程序被终⽌。C++11异常说明的运⾏时⾏为有些不同：调⽤栈只是可能在程序终⽌前展开。

展开调⽤栈和可能展开调⽤栈两者对于代码⽣成（code generation）有⾮常⼤的影响。在⼀个 noexcept 函数中，当异常可能传播到函数外时，优化器不需要保证运⾏时栈（the runtime stack）处于可展开状态；也不需要保证当异常离开 noexcept 函数时， noexcept 函数中的对象按 照构造的反序析构。⽽标注“ throw() ”异常声明的函数缺少这样的优化灵活性，没加异常声明的函 数也⼀样。可以总结⼀下：

- noexcept 是函数接⼝的⼀部分，这意味着调⽤者可能会依赖它 
- noexcept 函数较之于non- noexcept 函数更容易优化 
- noexcept 对于移动语义， swap ，内存释放函数和析构函数⾮常有⽤ 
- ⼤多数函数是异常中⽴的（译注：可能抛也可能不抛异常）⽽不是 noexcept

## 条款⼗五：尽可能的使⽤constexpr
1.不过我们还是先从 constexpr 对象开始说起。这些对象，实际上，和 const ⼀样，它们是编译期 可知的。（技术上来讲，它们的值在翻译期（translation）决议，所谓翻译不仅仅包含是编译 （compilation）也包含链接（linking）

编译期可知的值“享有特权”，它们可能被存放到只读存储空间中  
注意 const 不提供 constexpr 所能保证之事，因为 const 对象不需要在编译期初始化它的值。  
简⽽⾔之，所有 constexpr 对象都是 const ，但不是所有 const 对象都是 constexpr  

```cpp
int sz; //non-constexpr变量 
…
constexpr auto arraySize1 = sz; //错误！sz的值在 //编译期不可知 
std::array<int, sz> data1; //错误！⼀样的问题 
constexpr auto arraySize2 = 10; //没问题，10是 //编译期可知常量 
std::array<int, arraySize2> data2; //没问题, arraySize2是constexpr

const auto arraySize = sz; //没问题，arraySize是sz的const复制 
std::array<int, arraySize> data; //错误，arraySize值在编译期不可知
```

2.
- constexpr 函数可以⽤于需求编译期常量的上下⽂。如果你传给 constexpr 函数的实参在 编译期可知，那么结果将在编译期计算。如果实参的值在编译期不知道，你的代码就会被拒绝。
- 当⼀个 constexpr 函数被⼀个或者多个编译期不可知值调⽤时，它就像普通函数⼀样，运⾏时计算它的结果。这意味着你不需要两个函数，⼀个⽤于编译期计算，⼀个⽤于运⾏时计算。 constexpr全做了。


## 条款⼗六：让const成员函数线程安全

值得注意的是，因为 std::mutex 是⼀种只可移动类型（move-only type，⼀种可以移动但不能复 制的类型），所以将 m 添加进 Polynomial 中的副作⽤是使 Polynomial 失去了被复制的能⼒。不 过，它仍然可以移动。  

## 条款⼗七：理解特殊成员函数的⽣成
掌控它们⽣成和⾏为的规则类似于拷⻉系列。移动操作仅在需要的时候⽣成，如果⽣成了，就会 对类的non-static数据成员执⾏逐成员的移动。

两个拷⻉操作是独⽴的：声明⼀个不会限制编译器⽣成另⼀个。    
两个移动操作不是相互独⽴的。如果你声明了其中⼀个，编译器就不再⽣成另⼀个。
。如果逐成员移动构造有些问题，那么逐成员移动赋值同样也可能有问题。所 以声明移动构造函数阻⽌移动赋值运算符的⽣成，声明移动赋值运算符同样阻⽌编译器⽣成移动 构造函数。  

再进⼀步，如果⼀个类显式声明了拷⻉操作，编译器就不会⽣成移动操作操作。  
的解释是如 果声明拷⻉操作（构造或者赋值）就暗⽰着平常拷⻉对象的⽅法（逐成员拷⻉）不适⽤于该类， 编译器会明⽩如果逐成员拷⻉对拷⻉操作来说不合适，逐成员移动也可能对移动操作来说不合适。  

这是另⼀个⽅向。声明移动操作（构造或赋值）使得编译器禁⽤拷⻉操作(禁⽤的是⾃动⽣成的拷⻉操作，对于⽤⼾声明的 拷⻉操作不受影响)毕竟，如果逐成员移动对该类来说不合适，也没有理由指望逐成员拷⻉操作 是合适的。

_Rule of Three_规则:你声明了拷⻉构造函数，拷⻉赋值运算符，或者析构函数三者之⼀，你应该也声明其余两个  

所以仅当下⾯条件成⽴时才会⽣成移动操作（当需要时）： 
- 类中没有拷⻉操作 
- 类中没有移动操作 
- 类中没有⽤⼾定义的析构

`=default`让编译器帮你生成。  

- 特殊成员函数是编译器可能⾃动⽣成的函数：默认构造函数，析构函数，拷⻉操作，移动操作。
- 移动操作仅当类没有显式声明移动操作，拷⻉操作，析构函数时才⾃动⽣成。 
- 拷⻉构造函数仅当类没有显式声明拷⻉构造函数时才⾃动⽣成，并且如果⽤⼾声明了移动操作，拷⻉构造就是delete。拷⻉赋值运算符仅当类没有显式声明拷⻉赋值运算符时才⾃动⽣成，并且如果⽤⼾声明了移动操作，拷⻉赋值运算符就是delete。当⽤⼾声明了析构函数， 拷⻉操作的⾃动⽣成已被废弃。 
- 成员函数模板不抑制特殊成员函数的⽣成。



# 第4章 智能指针

## 条款⼗⼋：对于独占资源使⽤std::unique_ptr
移动⼀个 std::unique_ptr 将所有权从源指针转移到⽬的指针。（源指针被设为null。）拷⻉⼀个 std::unique_ptr 是不允许的  

默认情况下，销毁将通过 delete 进⾏，但是在构造过程中， std::unique_ptr 对象可以被设置为使⽤（对资源的）⾃定义删除器
```cpp
auto delInvmt = [](Investment* pInvestment) //现在在 
{ //makeInvestment⾥ 
makeLogEntry(pInvestment); delete pInvestment; 
}; 
std::unique_ptr<Investment, decltype(delInvmt)> //同之前⼀样 
pInv(nullptr, delInvmt); // 一个word

std::unique_ptr<Investment, void(*)(Investment*)>  //两个word
```

函数指针形式的删除器，通常会使 std::unique_ptr 的从⼀个字（word）⼤⼩增加到两个。对于函数对象形式的删除器来说，变化的⼤⼩取决于函数对象中存储的状态多少，⽆状态函数（stateless function）对象（⽐如不捕获变量的lambda表达式）对⼤⼩没有影响，这意味当⾃定义删除器可以实现为函数或者lambda时， 尽量使⽤lambda：

具有很多状态的⾃定义删除器会产⽣⼤尺⼨ std::unique_ptr 对象。如果你发现⾃定义删除器使 得你的 std::unique_ptr 变得过⼤，你需要审视修改你的设计  

## 条款⼗九：对于共享资源使⽤std::shared_ptr

- std::shared_ptr ⼤⼩是原始指针的两倍，因为它内部包含⼀个指向资源的原始指针，还包含⼀个指向资源的引⽤计数值的原始指针。（这种实现法并不是标准要求的，但是我（指原 书作者Scott Meyers）熟悉的所有标准库都这样实现。） 
- 引⽤计数的内存必须动态分配。 概念上，引⽤计数与所指对象关联起来，但是实际上被指向的对象不知道这件事情（译注：不知道有⼀个关联到⾃⼰的计数值）。因此它们没有办法存 放⼀个引⽤计数值。（⼀个好消息是任何对象——甚⾄是内置类型的——都可以由 std::shared_ptr 管理。）Item21会解释使⽤ std::make_shared 创建 std::shared_ptr 可以避免引⽤计数的动态分配，但是还存在⼀些 std::make_shared 不能使⽤的场景，这时候引⽤计数就会动态分配。 
- 递增递减引⽤计数必须是原⼦性的，因为多个reader、writer可能在不同的线程。⽐如，指向某种资源的 std::shared_ptr 可能在⼀个线程执⾏析构（于是递减指向的对象的引⽤计 数），在另⼀个不同的线程， std::shared_ptr 指向相同的对象，但是执⾏的却是拷⻉操作 （因此递增了同⼀个引⽤计数）。原⼦操作通常⽐⾮原⼦操作要慢，所以即使引⽤计数通常 只有⼀个word⼤⼩，你也应该假定读写它们是存在开销的。

赋值“ sp1 = sp2; ”会使 sp1 指向 sp2 指向的对象。直接效果就是 sp1 引⽤计数减⼀， sp2 引⽤计数加⼀。

移动 std::shared_ptr 会⽐拷⻉它要快：拷⻉要求递增引⽤计数值，移动不需要。移动赋值运算符同 理，所以移动构造⽐拷⻉构造快，移动赋值运算符也⽐拷⻉赋值运算符快。  

对于 std::unique_ptr 来说，删除器类型是智能指针类型的⼀部分。对于 std::shared_ptr 则不是：
```cpp
std::unique_ptr< //删除器类型是 
Widget, decltype(loggingDel) //指针类型的⼀部分 
> upw(new Widget, loggingDel); 

std::shared_ptr<Widget> //删除器类型不是 
spw(new Widget, loggingDel); //指针类型的⼀部分
```

两个不一样deleter的shared_ptr也可以相互赋值，而unique_ptr不行。  
另⼀个不同于 std::unique_ptr 的地⽅是，指定⾃定义删除器不会改变 std::shared_ptr 对象的 ⼤⼩。不管删除器是什么，⼀个 std::shared_ptr 对象都是两个指针⼤⼩。

我前⾯提到了 std::shared_ptr 对象包含了所指对象的引⽤计数的指针。没错，但是有点误导⼈。因为引⽤计数是另⼀个更⼤的数据结构的⼀部分，那个数据结构通 常叫做控制块（control block）。每个 std::shared_ptr 管理的对象都有个相应的控制块。控制 块除了包含引⽤计数值外还有⼀个⾃定义删除器的拷⻉，当然前提是存在⾃定义删除器。如果⽤ ⼾还指定了⾃定义分配器，控制块也会包含⼀个分配器的拷⻉。控制块可能还包含⼀些额外的数 据，正如Item21提到的，⼀个次级引⽤计数weak count  

- std::make_shared （参⻅Item21）总是创建⼀个控制块。它创建⼀个要指向的新对象，所以可以肯定 std::make_shared 调⽤时对象不存在其他控制块。 
- 当从独占指针（即 std::unique_ptr 或者 std::auto_ptr ）上构造出 std::shared_ptr 时 会创建控制块。独占指针没有使⽤控制块，所以指针指向的对象没有关联控制块。（作为构造的⼀部分， std::shared_ptr 侵占独占指针所指向的对象的独占权，所以独占指针被设置 为null） 
- 当从原始指针上构造出 std::shared_ptr 时会创建控制块。如果你想从⼀个早已存在控制块的对象上创建 std::shared_ptr ，你将假定传递⼀个 std::shared_ptr 或者 std::weak_ptr （参⻅Item20）作为构造函数实参，⽽不是原始指针。⽤ std::shared_ptr 或者 std::weak_ptr 作为构造函数实参创建 std::shared_ptr 不会创建 新控制块，因为它可以依赖传递来的智能指针指向控制块。

这些规则造成的后果就是从原始指针上构造超过⼀个 std::shared_ptr 就会让你⾛上未定义⾏为 的快⻋道，因为指向的对象有多个控制块关联。多个控制块意味着多个引⽤计数值，多个引⽤计 数值意味着对象将会被销毁多次（每个引⽤计数⼀次），比如创建this指针的shared_ptr的时候就会出现。

std::shared_ptr 给我们上了两堂课。第⼀，避免传给 std::shared_ptr 构造函数原始指针。通 常替代⽅案是使⽤ std::make_shared （参⻅Item21），不过上⾯例⼦中，我们使⽤了⾃定义删 除器，⽤ std::make_shared 就没办法做到。第⼆，如果你必须传给 std::shared_ptr 构造函数 原始指针，直接传 new 出来的结果，不要传指针变量。

```cpp
class Widget: public std::enable_shared_from_this<Widget>{};
processedWidgets.emplace_back(shared_from_this());
```

## 条款⼆⼗：当std::shared_ptr可能悬空时使⽤ std::weak_ptr
std::weak_ptr 不是⼀个独⽴的智能指针。它是 std::shared_ptr 的增强.  
std::shared_ptr 上创建 std::weak_ptr 时两者指向相同的对象，但是 std::weak_ptr 不会影 响所指对象的引⽤计数  
```cpp
if (wpw.expired()) … //如果wpw没有指向对象… 原子性操作

std::shared_ptr<Widget> spw1 = wpw.lock(); //如果wpw过期，spw1就为空 
auto spw2 = wpw.lock(); //同上，但是使⽤auto
std::shared_ptr<Widget> spw3(wpw); //如果wpw过期，抛出std::bad_weak_ptr异常
```

缓存可以用一个map缓存。记录一下id和对象  
缓存对象的指针需要知道它是否已经悬空，因为当⼯⼚客⼾端使⽤完⼯⼚产⽣的对象后，对 象将被销毁，关联的缓存条⽬会悬空。所以缓存应该使⽤ std::weak_ptr  
缓存可能会累积过期的 std::weak_ptr ，这些指针对应了不再使⽤的 Widget （也已经被销毁了）

这使subjects很容易发布状态更改通知。subjects对控 制observers的⽣命周期（即它们什么时候被销毁）没有兴趣，但是subjects对确保另⼀件事具有 极⼤的兴趣，那事就是⼀个observer被销毁时，不再尝试访问它。⼀个合理的设计是每个subject持有⼀个 std::weak_ptr s容器指向observers，因此可以在使⽤前检查是否已经悬空。

循环引用是因为那个两个指针的引用次数都是二，所以析构的时候，两个结构的指针都还剩下一个，所以释放不了  

- ⽤ std::weak_ptr 替代可能会悬空的 std::shared_ptr 。 
- std::weak_ptr 的潜在使⽤场景包括：缓存、观察者列表、打破 std::shared_ptr 环状结构


## 条款⼆⼗⼀：优先考虑使⽤std::make_unique和 std::make_shared，⽽⾮直接使⽤

第三个 make 函数 是 std::allocate_shared 。它⾏为和 std::make_shared ⼀样，只不过第⼀个参数是⽤来动态 分配内存的allocator对象。  

```cpp
auto upw1(std::make_unique<Widget>()); //使⽤make函数 
std::unique_ptr<Widget> upw2(new Widget); //不使⽤make函数 
auto spw1(std::make_shared<Widget>()); //使⽤make函数 
std::shared_ptr<Widget> spw2(new Widget); //不使⽤make函数
```
使⽤ new 的版本重复了类型，但是 make 函数的版本没有。源代码中的重复增加了编译的时间， 会导致⽬标代码冗余，并且通常会让代码库使⽤更加困难。

```cpp
processWidget(std::shared_ptr<Widget>(new Widget), //潜在的资源泄漏！ 
computePriority());
```
在运⾏时，⼀个函数的实参必须先被计算，这个函数再被调⽤.
1. 执⾏“ new Widget ”
2. 执⾏ computePriority
3. 运⾏ std::shared_ptr 构造函数

如果2异常就会泄漏

改为纯粹的函数就不会出现问题了
```cpp
processWidget(std::make_shared<Widget>(), //没有潜在的资源泄漏 
computePriority());
```

std::make_shared 的⼀个特性（与直接使⽤ new 相⽐）是效率提升。使⽤ std::make_shared 允许编译器⽣成更⼩，更快的代码，并使⽤更简洁的数据结构  

new的代码需要进⾏内存分配，但它实际上**执⾏了两次**(但不是重复)。Item19解释了每个 std::shared_ptr 指向⼀个控制块，其中包含被指向对象的引⽤计数，还有其他东西。这个控制块的内存在 std::shared_ptr 构造函数中分配。因此，直接使⽤ new 需要为 Widget 进⾏⼀次内 存分配，为控制块再进⾏⼀次内存分配。

如果使⽤ std::make_shared 代替：⼀次分配⾜矣。这是因为 std::make_shared 分配⼀块内存，同时容纳了 Widget 对象和控制块。 **这种优化减少了程序的静态⼤⼩，因为代码只包含⼀个内存分配调⽤，并且它提⾼了可执⾏代码的速度**，因为内存只分配⼀次。此外，使⽤ std::make_shared **避免了对控制块中的某些簿记信息**的需要，潜在地减少了程序的总内存占⽤。

make函数也有不能被使用的地方
- make 函数都不允许指定⾃定义删除器
- 当构造函数重载，有使⽤ std::initializer_list 作为参数的重载形式和不⽤其作为参数的的重载形式，⽤花括号创建的 对象更倾向于使⽤ std::initializer_list 作为形参的重载形式，⽽⽤⼩括号创建对象将调⽤不 ⽤ std::initializer_list 作为参数的的重载形式。 make 函数会将它们的参数完美转发给对象 构造函数，但是它们是使⽤⼩括号还是花括号？花括号初始化⽆法完美转发。但是可以先用auto存起来再传入构造函数。
- ⼀些类重载了 operator new 和 operator delete 。这种系列⾏为不太适⽤于 std::shared_ptr 对⾃定义分配（通过 std::allocate_shared ）和释放（通过⾃定义删除器）的⽀持，因为 std::allocate_shared 需要的内存总⼤⼩不等于动态分配的对象⼤⼩，还需要再加上控制块⼤⼩。因此，使⽤ make 函数 去创建重载了 operator new 和 operator delete 类的对象是个典型的糟糕想法。
- 

与直接使⽤ new 相⽐， std::make_shared 在⼤⼩和速度上的优势源于 std::shared_ptr 的控制 块与指向的对象放在同⼀块内存中。
如果对象类型⾮常⼤，⽽且销毁最后⼀个 std::shared_ptr 和销毁最后⼀个 std::weak_ptr 之间 的时间很⻓，那么在销毁对象和释放它所占⽤的内存之间可能会出现延迟。

对于 std::shared_ptr s，其他不建议使⽤make 函数的情况包括
(1)有⾃定义内存管理的类；
(2)特别关注内存的系统，⾮常⼤的对象，以及 std::weak_ptr s⽐对应的 std::shared_ptr s活得更久。


## 条款⼆⼗⼆：当使⽤Pimpl惯⽤法，请在实现⽂件中定义特殊成员函数
Pimpl（pointer to implementation）将类数据成员替换成⼀个指向包含具体实现的类（或结构体）的 指针，并将放在主类（primary class）的数据成员们移动到实现类（implementation class） 去，⽽这些数据成员的访问将通过指针间接访问。

```cpp
class Widget //仍然在“widget.h”中 
{public: Widget(); ~Widget(); //析构函数在后⾯会分析 … 
private: struct Impl; //声明⼀个 实现结构体 
Impl *pImpl; //以及指向它的指针 
};
```
⼀个已经被声明，却还未被实现的类型，被称为未完成类型（incomplete type）。 Widget::Impl 就是这种类型。 你能对⼀个未完成类型做的事很少，但是声明⼀个指向它的指针是可以的

这样做的原因是，如果widget.h改变了，就需要重新编译，特别耗时。如果数据成员里面的用户自定义类型的头文件经常改动，并且widget.h include了很多头文件，则每次改动编译就会特别耗时。所以将数据成员放在一个结构体里面，用一个指针去指向，并且将include的头文件放在cpp，就会减少编译时间。

核心其实就是用指针的，最好要自行实现特殊函数  

- Pimpl惯⽤法通过减少在类实现和类使⽤者之间的编译依赖来减少编译时间。 
- 对于 std::unique_ptr 类型的 pImpl 指针，需要在头⽂件的类⾥声明特殊的成员函数，但 是在实现⽂件⾥⾯来实现他们。即使是编译器⾃动⽣成的代码可以⼯作，也要这么做。 
- 以上的建议只适⽤于 std::unique_ptr ，不适⽤于 std::shared_ptr 。


# 第5章 右值引⽤，移动语义，完美转发
- 移动语义使编译器有可能⽤廉价的移动操作来代替昂贵的拷⻉操作。正如拷⻉构造函数和拷 ⻉赋值操作符给了你控制拷⻉语义的权⼒，移动构造函数和移动赋值操作符也给了你控制移 动语义的权⼒。移动语义也允许创建只可移动（move-only）的类型，例如 std::unique_ptr ， std::future 和 std::thread 。 
- 完美转发使接收任意数量实参的函数模板成为可能，它可以将实参转发到其他的函数，使⽬ 标函数接收到的实参与被传递给转发函数的实参保持⼀致。

std::move 并不移动任何东西，完美转发也并不完美。
移动操作并不永远⽐复制操作更廉价。
**⾮常重要的⼀点是要牢记形参永远是左值，即使它的类型是⼀个右值引⽤。**
## 条款⼆⼗三：理解std::move和std::forward

std::move 和 std::forward 仅仅是执⾏转换（cast）的函数（事实上是函数模板）。

返回的肯定是右值引用，因为remove_reference后就为确定类型了，就不需要推断了
```cpp
template<typename _Tp>
constexpr typename std::remove_reference<_Tp>::type&&
move(_Tp&& __t) noexcept
{ return static_cast<typename std::remove_reference<_Tp>::type&&>(__t); }
```

move会保存原有的const
如果move一个const对象，那么返回的是个const的右值，然而，移动构造函数只接受的是一个non-const的，（因为要窃取资源后，设置原来的为null），所以，最终走向的可以接收右值的拷贝构造函数。

- 第⼀，不要在你希望能移动对象的时候，声明他们为 const 。 对 const 对象的移动请求会悄⽆声息的被转化为拷⻉操作。
- 第⼆点， std::move 不仅不移动任何 东西，⽽且它也不保证它执⾏转换的对象可以被移动。关于 std::move ，你能确保的唯⼀⼀件事 就是将它应⽤到⼀个对象上，你能够得到⼀个右值。

std::forward 只有在满⾜⼀定条件的情况下才执⾏转换。 std::forward 是有条件的转换  
```cpp
template<typename T> //⽤以转发param到process的模板 
void logAndProcess(T&& param)
{
    process(std::forward<T>(param));
}
```
param ，正如所有的其他函数形参⼀样，是⼀个左值。每次在函数 logAndProcess 内部对函数 process 的调⽤，都会因此调⽤函数 process 的左值重载版本。所以上面使用了forward就可以实现了重载函数的调用。

当且仅当传递给函数 logAndProcess 的⽤以初始化 param 的实参是⼀个右值时， param 会被 转换为⼀个右值。这就是 std::forward 做的事情。这就是为什么 std::forward 是⼀个有条件的 转换：它的实参⽤右值初始化时，转换为⼀个右值。


这里就是通用引用了
```cpp
  template<typename _Tp>
    constexpr _Tp&&
    forward(typename std::remove_reference<_Tp>::type& __t) noexcept
    { return static_cast<_Tp&&>(__t); }
```


## 条款⼆⼗四：区分通⽤引⽤与右值引⽤
这两种情况的共同之处就是都存在类型推导（type deduction）。
```cpp
template<typename T> void f(T&& param); //param是⼀个通⽤引⽤
auto&& var2 = var1; //var2是⼀个通⽤引⽤
```

- 如果⼀个函数模板形参的类型为 T&& ，并且 T 需要被推导得知，或者如果⼀个对象被声明为 auto&& ，这个形参或者对象就是⼀个通⽤引⽤。 
- 如果类型声明的形式不是标准的 type&& ，或者如果类型推导没有发⽣，那么 type&& 代表⼀ 个右值引⽤。 
- 通⽤引⽤，如果它被右值初始化，就会对应地成为右值引⽤；如果它被左值初始化，就会成 为左值引⽤。

## 条款⼆⼗五：对右值引⽤使⽤std::move，对通⽤引⽤使⽤ std::forward

但是，关于对左值和右值的重载函数最重要的问题不是源代码的数量，也不是代码的运⾏时性 能。⽽是设计的可扩展性差.重载函数的数量⼏何式增⻓：n个参数的话，就要 实现2n种重载。这还不是最坏的。有的函数——实际上是函数模板——接受⽆限制个数的参数， 每个参数都可以是左值或者右值

对于这种函数，对于左值和右值分别重载就不能考虑了：通⽤引⽤是仅有的实现⽅案。对这种函 数，我向你保证，肯定使⽤ std::forward 传递通⽤引⽤形参给其他函数.  

标准化委员会远领先于开发者。早就为⼈认识到的是， makeWidget 的“拷⻉”版本可以避免复制局部变量 w 的需要，通过在分配给函数返回值的内存中构 造 w 来实现。这就是所谓的返回值优化（return value optimization，RVO），这在C++标准中已 经实现了。

如果满⾜（1）局 部对象与函数返回值的类型相同；（2）局部对象就是要返回的东西。（适合的局部对象包括⼤多 数局部变量（⽐如 makeWidget ⾥的 w ），还有作为 return 语句的⼀部分⽽创建的临时对象。编译器可能会在按值返回的函数中消除对局部对象的拷⻉（或者移动）

`return std::move(w);` 。返回局部对象的引 ⽤不满⾜RVO的第⼆个条件，所以编译器必须移动 w 到函数返回值的位置。开发者试图对要返回 的局部变量⽤ std::move 帮助编译器优化，反⽽限制了编译器的优化选项  


- 最后⼀次使⽤时，在右值引⽤上使⽤ std::move ，在通⽤引⽤上使⽤ std::forward 。 
- 对按值返回的函数要返回的右值引⽤和通⽤引⽤，执⾏相同的操作。 
- 如果局部对象可以被返回值优化消除，就绝不使⽤ std::move 或者 std::forward 。


## 条款⼆⼗六：避免在通⽤引⽤上重载

```cpp
template<typename T> void logAndAdd(T&& name) 
{ 
    auto now = std::chrono::system_lock::now(); 
    log(now, "logAndAdd"); 
    names.emplace(std::forward<T>(name)); 
}
假如传的是int，通过下标去访问，就需要重载，但是重载会出问题
```

- 对通⽤引⽤形参的函数进⾏重载，通⽤引⽤函数的调⽤机会⼏乎总会⽐你期望的多得多。 
- 完美转发构造函数是糟糕的实现，因为对于non- const 左值，它们⽐拷⻉构造函数⽽更匹 配，⽽且会劫持派⽣类对于基类的拷⻉和移动构造函数的调⽤。

## 条款⼆⼗七：熟悉通⽤引⽤重载的替代⽅法

- 放弃重载
- 传递const T&
- 传值 将按传引⽤形参替换为按值传递，
- 使⽤tag dispatch
- 约束使⽤通⽤引⽤的模板

tag diapatch如下：
```cpp
template<typename T> void logAndAdd(T&& name) 
{ 
    logAndAddImpl(
        std::forward<T>(name), 
        std::is_integral<T>()); //不那么正确 
}
```
所以如果左值 int 被传 ⼊ logAndAdd ， T 将被推断为 int& 。这不是⼀个整型类型，因为引⽤不是整型类型。这意味着 std::is_integral\<T> 对于任何左值实参返回false，即使确实传⼊了整型值。
所以改为
```cpp
logAndAddImpl( std::forward<T>(name), 
std::is_integral<typename std::remove_reference<T>::type>() );

void logAndAddImpl(T&& name, std::false_type);
void logAndAddImpl(int id, std::true_type);
```

- 通⽤引⽤和重载的组合替代⽅案包括使⽤不同的函数名，通过lvalue-reference-to- const 传 递形参，按值传递形参，使⽤tag dispatch。 
- 通过 std::enable_if 约束模板，允许组合通⽤引⽤和重载使⽤，但它也控制了编译器在哪 种条件下才使⽤通⽤引⽤重载。 
- 通⽤引⽤参数通常具有⾼效率的优势，但是可⽤性就值得斟酌。

## 条款⼆⼗⼋：理解引⽤折叠
```cpp
template<typename T> 
class Widget { 
    public: 
    typedef T&& RvalueRefToT; … 
};


```
- 引⽤折叠发⽣在四种情况下：模板实例化， auto 类型推导， typedef 与别名声明的创建和使⽤， decltype 。 
- 当编译器在引⽤折叠环境中⽣成了引⽤的引⽤时，结果就是单个引⽤。有左值引⽤折叠结果 就是左值引⽤，否则就是右值引⽤。 
- 通⽤引⽤就是在特定上下⽂的右值引⽤，上下⽂是通过类型推导区分左值还是右值，并且发 ⽣引⽤折叠的那些地⽅。

## 条款⼆⼗九：假定移动操作不存在，成本⾼，未被使⽤

主要是看移动构造函数是否实现的更有效吧

std::string 提供了常数时间的移动操作和线性时间的复制操作。这听起来移动⽐复 制快多了，但是可能不⼀定。许多字符串的实现采⽤了⼩字符串优化（small string
optimization，SSO）。“⼩”字符串（⽐如⻓度⼩于15个字符的）存储在了 std::string 的缓冲区 中，并没有存储在堆内存，移动这种存储的字符串并不必复制操作更快。

存在⼏种情况，C++11的移动语义并⽆优势
- 没有移动操作：要移动的对象没有提供移动操作，所以移动的写法也会变成复制操作。 
- 移动不会更快：要移动的对象提供的移动操作并不⽐复制速度更快。
- 移动不可⽤：进⾏移动的上下⽂要求移动操作不会抛出异常，但是该操作没有被声明为 noexcept。
- 源对象是左值：除了极少数的情况外（例如Item25），只有右值可以作为移动操作的来源。

## 条款三⼗：熟悉完美转发失败的情况

-当模板类型推导失败或者推导出错误类型，完美转发会失败。 
导致完美转发失败的实参种类有花括号初始化，作为空指针的 0 或者 NULL ，仅有声明的整 型 static const 数据成员，模板和重载函数的名字，位域。


# 第6章 lambda表达式
闭包（enclosure）是lambda创建的运⾏时对象。依赖捕获模式，闭包持有被捕获数据的副 本或者引⽤。
闭包类（closure class）是从中实例化闭包的类。每个lambda都会使编译器⽣成唯⼀的闭包类

## 条款三⼗⼀：避免使⽤默认捕获模式
默认就是[]中没有指定变量。
- 默认的按引⽤捕获可能会导致悬空引⽤。 
- 默认的按值捕获对于悬空指针很敏感（尤其是 this 指针），并且它会误导⼈产⽣lambda是 独⽴的想法。


## 条款三⼗⼆：使⽤初始化捕获来移动对象到闭包中
使⽤初始化捕获可以让你指定：
1. 从lambda⽣成的闭包类中的数据成员名称；
2. 初始化该成员的表达式；
[lambda里面的变量=外面的] 这样初始化

C++11中这样模拟：
1. 将要捕获的对象移动到由 std::bind 产⽣的函数对象中；
2. 将“被捕获的”对象的引⽤赋予给lambda


## 条款三⼗三：对auto&&形参使⽤decltype以std::forward它们

## 条款三⼗四：考虑lambda⽽⾮std::bind

# 第7章 并发API
## 条款三⼗五：优先考虑基于任务的编程⽽⾮基于线程的编程
```cpp
int doAsyncWork(); 
std::thread t(doAsyncWork); // thread
auto fut = std::async(doAsyncWork); //“fut”表⽰“future task
```
硬件线程（hardware threads）是真实执⾏计算的线程。现代计算机体系结构为每个CPU核 ⼼提供⼀个或者多个硬件线程
软件线程（software threads）（也被称为系统线程（OS threads、system threads））是 操作系统（假设有⼀个操作系统。有些嵌⼊式系统没有。）管理的在硬件线程上执⾏的线 程。通常可以存在⽐硬件线程更多数量的软件线程，因为当软件线程被阻塞的时候（⽐如I/O、同步锁或者条件变量），操作系统可以调度其他未阻塞的软件线程执⾏提供吞吐量。
std::thread 是C++执⾏过程的对象，并作为软件线程的句柄（handle）。有些 std::thread 对象代表“空”句柄，即没有对应软件线程，因为它们处在默认构造状态（即没 有函数要执⾏）；有些被移动⾛（移动到的 std::thread 就作为这个软件线程的句柄）；有 些被 join （它们要运⾏的函数已经运⾏完）；有些被 detach （它们和对应的软件线程之间 的连接关系被打断）。

对⽐基于线程的编程⽅式，基于任务的设计为开发者避免了⼿动线程管理的痛苦，并且⾃然提供 了⼀种获取异步执⾏程序的结果（即返回值或者异常）的⽅式。

仍然存在⼀些场景直接使 ⽤ std::thread 会更有优势：
- 你需要访问⾮常基础的线程API。
- 你需要且能够优化应⽤的线程使⽤
- 你需要实现C++并发API之外的线程技术，


## 条款三⼗六：如果有异步的必要请指定std::launch::async
std::launch::async 启动策略意味着 f 必须异步执⾏，即在不同的线程。  
std::launch::deferred 启动策略意味着 f 仅当在 std::async 返回的future上调⽤ get 或 者 wait 时才执⾏  
可能让⼈惊奇的是， std::async 的默认启动策略——你不显式指定⼀个策略时它使⽤的那个——不是上⾯中任意⼀个。相反，是求或在⼀起的。

- ⽆法预测 f 是否会与 t 并发运⾏，因为 f 可能被安排延迟运⾏。 ⽆
- 法预测 f 是否会在与某线程相异的另⼀线程上执⾏，这个某线程在 fut 上调⽤ get 或 wait 。如果对 fut 调⽤函数的线程是 t ，含义就是⽆法预测 f 是否在异于 t 的另⼀线程上 执⾏。 
- ⽆法预测 f 是否执⾏，因为不能确保在程序每条路径上，都会不会在 fut 上调⽤ get 或者 wait 。

- std::async 的默认启动策略是异步和同步执⾏兼有的。 
- 这个灵活性导致访问 thread_local s的不确定性，隐含了任务可能不会被执⾏的意思，会影 响调⽤基于超时的 wait 的程序逻辑。 
- 如果异步执⾏任务⾮常关键，则指定 std::launch::async 。


## 条款三⼗七：使std::thread在所有路径最后都不可结合

可结合状态的 std::thread 对应于正在运⾏或者可能要运⾏的异步执⾏线程。 ⽐如，对应于⼀个阻塞的（blocked）或者等待调度的线程的 std::thread 是可结合的，对应于运 ⾏结束的线程的 std::thread 也可以认为是可结合的。  

不可结合的 std::thread 对象包括：
- 默认构造的 std::thread s。这种 std::thread 没有函数执⾏，因此没有对应到底层执⾏线程上
- 已经被移动⾛的 std::thread 对象。移动的结果就是⼀个 std::thread 原来对应的执⾏线程现在对应于另⼀个 std::thread
- 已经被 join 的 std::thread 。在 join 之后， std::thread 不再对应于已经运⾏完了的执⾏线程。
- 已经被 detach 的 std::thread 。 detach 断开了 std::thread 对象与执⾏线程之间的连接。

- 在所有路径上保证 thread 最终是不可结合的。 
- 析构时 join 会导致难以调试的表现异常问题。 
- 析构时 detach 会导致难以调试的未定义⾏为。 
- 声明类数据成员时，最后声明 std::thread 对象。


## 条款三⼗⼋：关注不同线程句柄的析构⾏为

## 条款三⼗九：对于⼀次性事件通信考虑使⽤void的futures
这个代码质量高
```cpp
std::unique_lock<std::mutex> lk(m); //跟之前⼀样 
cv.wait(lk, [] { return flag; });
```
- 对于简单的事件通信，基于条件变量的设计需要⼀个多余的互斥锁，对检测和反应任务的相 对进度有约束，并且需要反应任务来验证事件是否已发⽣。 
- 基于flag的设计避免的上⼀条的问题，但是是基于轮询，⽽不是阻塞。 
- 条件变量和flag可以组合使⽤，但是产⽣的通信机制很不⾃然。 
- 使⽤ std::promise 和future的⽅案避开了这些问题，但是这个⽅法使⽤了堆内存存储共享 状态，同时有只能使⽤⼀次通信的限制


## 条款四⼗：对于并发使⽤std::atomic，对于特殊内存使⽤ volatile

# 第8章 微调
## 条款四⼗⼀：对于移动成本低且总是被拷⻉的可拷⻉形参，考虑按值传递
- 对于可拷⻉，移动开销低，⽽且⽆条件被拷⻉的形参，按值传递效率基本与按引⽤传递效率⼀致，⽽且易于实现，还⽣成更少的⽬标代码。 
- 通过构造拷⻉形参可能⽐通过赋值拷⻉形参开销⼤的多。 
- 按值传递会引起切⽚问题，所说不适合基类形参类型。

## 条款四⼗⼆：考虑使⽤置⼊代替插⼊

- 原则上，置⼊函数有时会⽐插⼊函数⾼效，并且不会更差。 
- 实际上，当以下条件满⾜时，置⼊函数更快：（1）值被构造到容器中，⽽不是直接赋值； （2）传⼊的类型与容器的元素类型不⼀致；（3）容器不拒绝已经存在的重复值。 
- 置⼊函数可能执⾏插⼊函数拒绝的类型转换。


### 切片问题
对象切片问题发生在以下情况下：当将派生类对象赋值给基类对象时，如果使用的是基类的引用或指针，那么只会复制派生类对象中与基类相对应的部分，而派生类特有的成员将被丢失，从而产生了对象切片
