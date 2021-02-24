# traits

traits，又被叫做特性萃取技术，说得简单点就是提取“被传进的对象”对应的返回类型，**让同一个接口实现对应的功能**，而无需在代码运行的时候进行判断

## 示例

首先，在算法中运用迭代器时，很可能会用到**其相应型别**（迭代器所指之物的型别）。假设算法中有必要声明一个变量，以“迭代器**所指**对象的型别”为型别，该怎么办呢？解决的方法是使用function template的参数推导机制代码如下所示

```c++
template <class I, class T>
void func_impl(I iter, T t) {
        T tmp; // 这里就是迭代器所指物的类型新建的对象
        // ... 功能实现
}

template <class I>
inline void func(I iter) {
        func_impl(iter, *iter); // 传入iter和iter所指的值，class自动推导
}

int main() {
    int i;
    func(&i);
}
```

然而template参数推导机制推导的只是参数，**无法推导函数的返回值类型**。万一需要推导函数的传回值，就无能为力了

尽管可以使用内嵌类型，例如下面所示的代码，主要思想就是先声明一个使用模版的结构体，结构体的模版根据传入的类型声明五种类型；然后再将结构体的类型作为返回值类型 

```c++
template <class T>
struct MyIter {
    typedef T value_type; // 内嵌型别声明
    // ...
};

template <class I>
typename I::value_type 
func(I ite) {
    return *ite;
}

// ...
MyIter<int> ite(new int(8));
cout << func(ite);
```

