#include <iostream>
#include "Allocator.h"
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

    //*****************test*****************//
//    std::cout << "Hello, World!" << std::endl;
//    Allocator<int > allocator;
//    int *p=allocator.allocate(100);
//    int *p1=allocator.allocate(100);
//    int *p2=allocator.allocate(100);
//    int *p3=allocator.allocate(100);
//    int *p4=allocator.allocate(100);
//    int *p5=allocator.allocate(100);
//    int *p6=allocator.allocate(100);
//    int *p7=allocator.allocate(100);
//     *p2=2;
//    *p3=4;
//    std::cout<<p<<"\n";
//    std::cout<<p1<<"\n";
//    std::cout<<p2<<"\n";
//    std::cout<<p3<<"\n";
//    std::cout<<*p<<"\n";
//    std::cout<<*p2<<"\n";
//    std::cout<<*p3<<"\n";


    //*****************test*****************//
//    Allocator<char > CharAllocator;
//    char * CharPointer1=CharAllocator.allocate(100);
//    char * CharPointer2=CharAllocator.allocate(1);
//    char * CharPointer3=CharAllocator.allocate(1);
//    char * CharPointer4=CharAllocator.allocate(1);
//    char * CharPointer5=CharAllocator.allocate(1);
//    char * CharPointer6=CharAllocator.allocate(1);
//    std::cout<<&CharPointer1<<"\n";
//    std::cout<<&CharPointer2<<"\n";
//    std::cout<<&CharPointer3<<"\n";
//    std::cout<<&CharPointer4<<"\n";
//    std::cout<<&CharPointer5<<"\n";


    //*****************test*****************//
    Allocator<Student>StudentAllocator;
    Student* StudentPointer1=StudentAllocator.allocate(1);
    Student* StudentPointer2=StudentAllocator.allocate(1);
    Student* StudentPointer3=StudentAllocator.allocate(1);
    Student* StudentPointer4=StudentAllocator.allocate(1);
    Student* StudentPointer5=StudentAllocator.allocate(1);
    std::cout<<StudentPointer1<<"\n";
    std::cout<<StudentPointer2<<"\n";
    std::cout<<StudentPointer3<<"\n";
    std::cout<<StudentPointer4<<"\n";
    std::cout<<StudentPointer5<<"\n";

    return 0;
}