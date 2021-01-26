#include <iostream>
#include "Allocator.h"
#include "iterator.h"
class Student{
public:
    char* name="leo";
    int age=99;
    int grade=100;
private:
    int sex=1;
protected:
    char * lover="NO";
};

int main() {
    int a[100]={1,2,3,4,5,6,7,8,9,10};
    IteratorTraits<int* >iterator;
    Advance(iterator,6);
    std::cout<<a[iterator.]

    return 0;
}