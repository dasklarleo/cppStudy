//
// Created by 刘玉晓 on 2021-01-17.
//

#ifndef STL_ALLOCATOR_H
#define STL_ALLOCATOR_H

#include <new>//为了使用placement new
#include <cstddef>
#include <climits>
#include <iostream>

namespace Allocator {
    template<class T>
    inline T *_allocate(ptrdiff_t size, T *) {
        std::set_new_handler(0);//内存分配失败之后由我们而不是系统进行处理
        T *tmp = (T *) (::operator new((size_t) (sizeof(T) * size)));
        if (tmp == 0) {
            std::cerr << "NO ENOUGH MEMORY" << "\n";
        }//分配失败
        return tmp;
    }

    template<class T>
    inline void _dealloc(T *mem) {
        ::operator delete(mem);
    }

    template<class T1, class T2>
    inline void _construct(T1 *p, const T2 &ptr) {
        new(p) T1(ptr);//placement new，本质上是对::operator new的重载，不分配内存。
        // 调用合适的构造函数在ptr所指的地方构造一个T2对象，之后返回实参指针ptr
    }

    template<class T>
    inline void _deconstruct(T *p) {
        p->~T();
    }

    template<class T>
    class allocator {
    public:
        typedef T ValueType;
        typedef T *Pointer;
        typedef const T *ConstPointer;
        typedef T &Reference;
        typedef const T &ConstReference;
        typedef size_t SizeType;
        typedef ptrdiff_t DifferenceType;

        template<class U>
        struct Rebind {
            typedef allocator<U> others;
        };
        Pointer allocate(SizeType n, const void * hint=0){
            return _allocate((DifferenceType) n,(Pointer)0); //格式必须一摸一样
        }
        void deallocate(Pointer pointer,SizeType sizeType){
            _dealloc(pointer);
        }
        void construct(Pointer pointer,ConstReference constReference){
            _construct(pointer,constReference);
        }
        void destroy(Pointer pointer){
            _deconstruct(pointer);
        }
        Pointer address(Reference reference){
            return (Pointer)(&reference);
        }
        ConstPointer const_reference(ConstReference constReference){
            return (ConstPointer)&constReference;
        }
        SizeType max_size()const {
            return (SizeType)(UINT_MAX/ sizeof(T));
        }
    };
}
#endif //STL_ALLOCATOR_H
