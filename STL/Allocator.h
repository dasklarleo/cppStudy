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
class DefaultAllocTemplate {
private:
    enum {
        ALIGN = 8
    };//128字节为分界线
    enum {
        MAX_BYTES = 128
    };//以8个字节为单位分配
    enum {
        NFREELISTS = 16
    };//位数16组链表
    static size_t RoundUp(size_t bytes) {
        return ((bytes + ALIGN) / ALIGN) * ALIGN;
    }

    union Obj {//嵌入式指针,每一个元素（内存区域对象）即可以当作下一个元素的指针
        //每个节点的值是使用前者构成的单向链表的首地址
        union Obj *MemFreeListLink;
        char MemClientData[1];
    };

    static Obj *SFreeList[];

    static size_t SFreeListIndex(size_t bytes) {//找到要在8条链表之中的哪一条取内存块
        return (((bytes) + ALIGN - 1) / ALIGN - 1);
    }

    //这个size一定是8的整数倍数
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
            MyfreeList = SFreeList + SFreeListIndex(size);//找到了数组对应大小的链表
            NextObj = (Obj *) (chunk);//将申请下来的内存进行分块？？？为什么第一块就要加一个size而不是指向开头
            *MyfreeList = NextObj;//链表指向空闲的内存
            for (int i = 1;; i++) {//分割申请下来的chunk
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

    //返回指向一块内存空间的指针（字节为单位）
    static char *SChunkAlloc(size_t size, int &n) {

        char *result;
        size_t TotalBytes = size * n;//总共需要这么多的内存
        size_t BytesLeft = SEndFree - SStartFree;//目前内存池有这么多的内存
        if (BytesLeft >= TotalBytes) {//可以分配这么多的内存
            result = SStartFree;//下面将减去已经分配好了的，从start开始到start+ToTalBytes都是分配出去的
            SStartFree += TotalBytes;
            return result;

        } else if (BytesLeft >= size) {//能分配几个分配几个

            n = (int) (BytesLeft / size);//只能分配n个
            TotalBytes = size * n;
            result = SStartFree;
            SStartFree += TotalBytes;
            return result;

        } else {//一个都分配不了了
            //调整free list，将内存中的残余内存空间编入
            size_t BytesToGet = TotalBytes * 2 + RoundUp(SHeapSize / 16);
            if (BytesLeft > 0) {//把还剩的一点找出来 很节约的程序员
                Obj **MyfreeList = SFreeList + SFreeListIndex(BytesLeft);//找到了还剩的内存块对应的链表
                ((Obj *) (SStartFree))->MemFreeListLink = *MyfreeList;
                *MyfreeList = (Obj *) SStartFree;
            }
            //配置heap空间，用来填充内存池
            SStartFree = (char *) malloc(BytesToGet);
            if (SStartFree == 0)
                std::cerr << "NO ENOUGH MEMORY" << "\n";
            SHeapSize += BytesToGet;
            SEndFree = SStartFree + BytesToGet;
            return (SChunkAlloc(size, n));//循环调用自己,修正n
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
            if (result == 0) {//没有空闲的了,尝试再要点内存
                void *r = SRefill(RoundUp(n));
                return r;
            }
            result = result->MemFreeListLink;
            *MyFreeList = result;
            return result;
        }
    }

    static void DeAllocate(void *p, size_t n) {
        Obj **MyFreeListLink;
        Obj *q = (Obj *) p;
        if (n > (size_t) MAX_BYTES) {
            MallocAlloc::deallocate(p, n);
            return;
        }
        MyFreeListLink = SFreeList + SFreeListIndex(n);
        q->MemFreeListLink = *MyFreeListLink;
        *MyFreeListLink = q;
    }

    void *Reallocate(void *p, size_t OldSize, size_t NewSize) {
        void *result;
        size_t CopySize;

        if (OldSize > (size_t) MAX_BYTES && NewSize > (size_t) MAX_BYTES) {
            return (realloc(p, NewSize));
        }
        if (RoundUp(OldSize) == RoundUp(NewSize)) return (p);
        result = Allocate(NewSize);
        CopySize = NewSize > OldSize ? OldSize : NewSize;
        memcpy(result, p, CopySize);
        DeAllocate(p, OldSize);
        return (result);
    }

};

typedef DefaultAllocTemplate<0> Alloc;
template<int inst>
char *DefaultAllocTemplate<inst>::SStartFree = 0;

template<int inst>
char *DefaultAllocTemplate<inst>::SEndFree = 0;

template<int inst>
size_t DefaultAllocTemplate<inst>::SHeapSize = 0;

template<int inst>
typename DefaultAllocTemplate<inst>::Obj *DefaultAllocTemplate<inst>::SFreeList[DefaultAllocTemplate<inst>::NFREELISTS] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

template<class T>
class Allocator {
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
        typedef Allocator<U> others;
    };

    Pointer address(Reference reference) {
        return (Pointer) (&reference);
    }

    ConstPointer const_reference(ConstReference constReference) {
        return (ConstPointer) &constReference;
    }

    Pointer allocate(SizeType n, const void *hint = 0) {
        return n != 0 ? static_cast<T *>(Alloc::Allocate(n * sizeof(T))) : 0; //格式必须一摸一样
    }

    void deallocate(Pointer pointer, SizeType n) {
        Alloc::DeAllocate(pointer, n);
    }

    SizeType max_size() const {
        return (SizeType) (UINT_MAX / sizeof(T));
    }

    void construct(Pointer pointer, ConstReference constReference) {
        new(pointer) T(constReference);
    }

    void destroy(Pointer pointer) {
        pointer->~T();
    }
};

template<class T, class Allocater>
class simple_alloc {
public:
    static T *allocate(size_t n) {
        return n != 0 ? static_cast<T *>(Alloc::Allocate(n * sizeof(T))) : 0;
    }

    static T *allocate(void) {
        return (T *) Allocater::allocate(sizeof(T));
    }

    static void deallocate(T *p, size_t n) {
        if (0 != n) Allocater::deallocate(p, n * sizeof(T));
    }

    static void deallocate(T *p) {
        Allocater::deallocate(p, sizeof(T));
    }
};



#endif //STL_ALLOCATOR_H
