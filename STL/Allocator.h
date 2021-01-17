//
// Created by 刘玉晓 on 2021-01-17.
//

#ifndef STL_ALLOCATOR_H
#define STL_ALLOCATOR_H

#include <new>//为了使用placement new
#include <cstddef>
#include <climits>
#include <iostream>


template<int inst>
class MallocAllocTemplate {
private:
    static void *SOOMMalloc(size_t n) {
        void *result;
        void (* TempHandler)();
        for (;;) {
            TempHandler=MallocAllocOOMHandler;
            if (0 == TempHandler) {
                std::cerr << "NO ENOHGH MEMORY" << "\n";
                exit(1);
            }
            (* TempHandler)();//????
            result = malloc(n);
            if (result)
                return result;
        }
    }

    static void *SOOMRealloc(void *p,size_t n) {
        void *result;
        void (* TempHandler)();
        for (;;) {
            TempHandler=MallocAllocOOMHandler;
            if (0 == TempHandler) {
                std::cerr << "NO ENOHGH MEMORY" << "\n";
                exit(1);
            }
            (* TempHandler)();//????
            result = malloc(n);
            if (result)
                return result;
        }
    }

    static void (*MallocAllocOOMHandler)();

public:
    static void * allocate(size_t n){
        void * result=malloc(n);
        if(0==result)
            result=SOOMMalloc(n);
        return result;
    }
    static void deallocate(void *p ,size_t){
        free(p);
    }
    static void* reallocate(void* p, size_t , size_t n)
    {
        void* result = realloc(p, n);
        if (0 == result) result = SOOMRealloc(p, n);
        return result;
    }
    static void (* SetMallocHandler(void (*__f)()))()
    {
        void (* old)() = MallocAllocOOMHandler;
        MallocAllocOOMHandler = __f;
        return(old);
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

        static size_t SFreeListIndex(size_t bytes) {
            return (((bytes) + ALIGN - 1) / ALIGN - 1);
        }

        //后续可能要使用的函数
        static void *SRefill(size_t n) {}

        static char *S_chunk_alloc(size_t size, int &nobjs);

        // Chunk allocation state.
        static char *S_StartFree;
        static char *SEndFree;
        static size_t SHeapSize;
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
