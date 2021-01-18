//
// Created by 刘玉晓 on 2021-01-17.
//
//所有的内存分配一级级往下最终都是到达malloc层面
//但是malloc由于会在内存之中记载cookie 大小等信息
//因此对小内存的需求进行内存分配时频繁使用malloc会引起较大的浪费

#ifndef STL_ALLOCATOR_H
#define STL_ALLOCATOR_H

#include <new>//为了使用placement new
#include <cstddef>
#include <climits>
#include <iostream>


template<int inst>//inst区分类模版和类
class MallocAllocTemplate {//只有需要分配的内存大于128字节时才会调用此class内的函数
    //这也就是第二级配置器
private:

public:
    static void *allocate(size_t n) {
        void *result = malloc(n);
        if (0 == result)
            std::cerr << "NO ENOUGH MEMORY" << "\n";
        return result;
    }

    static void deallocate(void *p, size_t) {
        free(p);
    }

    static void *reallocate(void *p, size_t, size_t n) {
        void *result = realloc(p, n);
        if (0 == result)
            std::cerr << "NO ENOUGH MEMORY" << "\n";
        return result;
    }

};

typedef MallocAllocTemplate<0> MallocAlloc;

template<int inst>
class DefaultAlloc {
private:
    enum {
        ALIGN = 8
    };//128字节为分界线
    enum {
        MAX_BYTES = 128
    };//以8个字节为单位分配
    enum {
        _NFREELISTS = 16
    };//位数16组链表
    static size_t RoundUp(size_t bytes) {
        return ((bytes + ALIGN) / ALIGN) * ALIGN;
    }

    union Obj {
        union Obj *MemFreeListLink;
        char MemClientData[1];
    };

    static Obj *SFreeList[];

    static size_t SFreeListIndex(size_t bytes) {//找到要在8条链表之中的哪一条取内存块
        return (((bytes) + ALIGN - 1) / ALIGN - 1);
    }

    //这个n一定是8的整数倍数
    static void *SRefill(size_t size) {
        int num = 20;   //一次想要20块
        char *chunk = SChunkAlloc(size, num);//num传的是引用,返回真正分配了的内存块数量
        Obj **MyfreeList;
        Obj *result;
        Obj *CurrentObj;
        Obj *NextObj;
        if (1 == num)
            return chunk;
        else {// 切成一个个的大小为size的小块加入freelist
            result = (Obj *) chunk;
            MyfreeList = SFreeList + SFreeListIndex(size);//找到了对应的链表
            NextObj = (Obj *) (chunk + size);//因为大小是size
            *MyfreeList = NextObj;
            for (int i = 1;; i++) {
                CurrentObj = NextObj;
                NextObj = (Obj *) ((char *) NextObj + size);
                if (num - 1 == i) {
                    CurrentObj->MemFreeListLink = 0;
                    break;
                } else {
                    CurrentObj->MemFreeListLink = NextObj;
                }
            }
            return result;
        }

    }

    static char *SChunkAlloc(size_t size, int &n) {
        char *result;
        size_t TotalBytes = size * n;
        size_t BytesLeft = SEndFree - SStartFree;
        if (BytesLeft >= TotalBytes) {//可以分配这么多的内存
            result = SStartFree;
            SStartFree += TotalBytes;
            return result;
        } else {
            size_t BytesToGet = TotalBytes * 2 + RoundUp(SHeapSize * 16);
            if (BytesLeft > 0) {
                Obj** MyfreeList=SFreeList + SFreeListIndex(BytesLeft);//找到了对应的链表
                

            }
        }
    }

    // 记录分配的状态,内存池起始位置和结束位置
    static char *SStartFree;
    static char *SEndFree;
    static size_t SHeapSize;
public:
    static void *Allocate(size_t n) {

        if (n > MAX_BYTES) {
            return (MallocAlloc::allocate(n));
        } else {
            Obj **MyFreeList = SFreeList + SFreeListIndex(n);//确定空闲链表
            Obj *result = *MyFreeList;
            if (result == 0)//没有空闲的了,尝试再要点内存
                result = SRefill(RoundUp(n));
            else
                result = result->MemFreeListLink;
            return result;
        }
    }

};


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

        Pointer allocate(SizeType n, const void *hint = 0) {
            return _allocate((DifferenceType) n, (Pointer) 0); //格式必须一摸一样
        }

        void deallocate(Pointer pointer, SizeType sizeType) {
            _dealloc(pointer);
        }

        void construct(Pointer pointer, ConstReference constReference) {
            _construct(pointer, constReference);
        }

        void destroy(Pointer pointer) {
            _deconstruct(pointer);
        }

        Pointer address(Reference reference) {
            return (Pointer) (&reference);
        }

        ConstPointer const_reference(ConstReference constReference) {
            return (ConstPointer) &constReference;
        }

        SizeType max_size() const {
            return (SizeType) (UINT_MAX / sizeof(T));
        }
    };
}
#endif //STL_ALLOCATOR_H
