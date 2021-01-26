//
// Created by 刘玉晓 on 2021-01-25.
//

#include <stddef.h>

#ifndef STL_ITERATOR_H
#define STL_ITERATOR_H

#endif //STL_ITERATOR_H

struct InputIteratorTag {
};
struct OutputIteratorTag {
};
struct ForwardIteratorTag : public InputIteratorTag {
};
struct BidirectionalIteratorTag : public ForwardIteratorTag {
};
struct RandomAccessIteratorTag : public BidirectionalIteratorTag {
};

template<class T, class Distance>
struct InputIterator {
    typedef InputIteratorTag iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef T *pointer;
    typedef T &reference;
};

struct OutputIterator {
    typedef OutputIteratorTag iterator_category;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;
};

template<class T, class Distance>
struct ForwardIterator {
    typedef ForwardIteratorTag iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef T *pointer;
    typedef T &reference;
};


template<class T, class Distance>
struct BidirectionalIterator {
    typedef BidirectionalIteratorTag iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef T *pointer;
    typedef T &reference;
};

template<class T, class Distance>
struct RandomAccessIterator {
    typedef RandomAccessIteratorTag iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef T *pointer;
    typedef T &reference;
};


/*
 * 上述迭代器应用待补充
 * */

template<class Category, class T, class Distance = ptrdiff_t,
        class Pointer = T *, class Reference = T &>
struct Iterator {
    typedef Category iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef Reference reference;
};
/*iterator编程
 * 依次特化普通版本
 * 指针版本
 * 常指针*/
template<class itertaor>
struct IteratorTraits {
    typedef typename itertaor::iterator_category iterator_category;
    typedef typename itertaor::value_type value_type;
    typedef typename itertaor::difference_type difference_type;
    typedef typename itertaor::pointer pointer;
    typedef typename itertaor::reference reference;

};

template<class T>
struct IteratorTraits<T *> {
    typedef RandomAccessIteratorTag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T *pointer;
    typedef T &reference;
};

template<class T>
struct IteratorTraits<const T *> {
    typedef RandomAccessIteratorTag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T *pointer;
    typedef T &reference;
};
//tarits编程结束


/*
 * 为五种迭代器单独设置函数
 * */
//1 返回类别函数
template<class T, class Distance>
inline InputIteratorTag
ItearotCategory(const InputIterator<T, Distance> &) { return InputIteratorTag(); }

inline OutputIteratorTag iterator_category(const OutputIterator &) { return OutputIteratorTag(); }

template<class T, class Distance>
inline ForwardIteratorTag
ItearotCategory(const ForwarIterator <T, Distance> &) { return ForwardIteratorTag(); }

template<class T, class Distance>
inline BidirectionalIteratorTag
ItearotCategory(const BidirectionalIterator<T, Distance> &) { return BidirectionalIteratorTag(); }

template<class T, class Distance>
inline RandomAccessIteratorTag
ItearotCategory(const RandomAccessIterator<T, Distance> &) { return RandomAccessIteratorTag(); }

template<class T>
inline RandomAccessIteratorTag ItearotCategory(const T *) { return RandomAccessIteratorTag(); }
//1 返回类别函数结束

//2 返回值的类型
template<class T, class Distance>
inline T *ValueType(const InputIterator<T, Distance> &) { return (T *) (0); }

template<class T, class Distance>
inline T *ValueType(const ForwarIterator <T, Distance> &) { return (T *) (0); }

template<class T, class Distance>
inline T *ValueType(const BidirectionalIterator<T, Distance> &) { return (T *) (0); }

template<class T, class Distance>
inline T *ValueType(const RandomAccessIterator<T, Distance> &) { return (T *) (0); }

template<class T>
inline T *ValueType(const T *) { return (T *) (0); }
//2 返回值函数结束


//3 距离类型函数
template<class T, class Distance>
inline Distance *DistanceType(const InputIterator<T, Distance> &) {
    return (Distance *) (0);
}

template<class T, class Distance>
inline Distance *DistanceType(const ForwarIterator <T, Distance> &) {
    return (Distance *) (0);
}

template<class T, class Distance>
inline Distance *
DistanceType(const BidirectionalIterator<T, Distance> &) {
    return (Distance *) (0);
}

template<class T, class Distance>
inline Distance *
DistanceType(const RandomAccessIterator<T, Distance> &) {
    return (Distance *) (0);
}

template<class T>
inline ptrdiff_t *DistanceType(const T *) { return (ptrdiff_t *) (0); }
//3 距离类型函数结束

//4 距离大小函数
template<class InputIterator>
inline typename IteratorTraits<InputIterator>::difference_type
DistanceInner(InputIterator first, InputIterator last, InputIteratorTag) {
    typename IteratorTraits<InputIterator>::difference_type n = 0;
    while (first != last) {
        ++first;
        ++n;
    }
    return n;
}

template<class RandomIterator>
inline typename IteratorTraits<RandomIterator>::difference_type
DistanceInner(RandomIterator first, RandomIterator last, RandomAccessIteratorTag) {
    return last - first;
}

template<class InputIterator>
inline typename IteratorTraits<InputIterator>::difference_type
Distance(InputIterator first, InputIterator last) {
    typedef typename IteratorTraits<InputIterator>::iterator_category category;
    return DistanceInner(first, last, category());
}
//4 距离大小函数结束

//5 前进函数
template<class BidirectionalIterator, class Distance>
inline void AdvanceInner(BidirectionalIterator &i, Distance n, BidirectionalIteratorTag) {
    if (n >= 0)
        while (n--)++i;
    else
        while (n++)--i;
}

template<class InputIterator, class Distance>
inline void AnvanceInner(InputIterator &i, Distance n, InputIteratorTag) {
    while (n--)++i;
}

template<class RandomAccessIterator, class Distance>
inline void AnvanceInner(RandomAccessIterator &i, Distance n, RandomAccessIteratorTag) {
    i += n;
}

template<class InputIterator, class Distance>
inline void Advance(InputIterator &i, Distance n) {
    AdvanceInner(i, n, IteratorCategory(i));
}
//5 前进函数结束


//
/*
 * 重载
 */
template<class Iter>
inline typename IteratorTraits<Iter>::iterator_category
IteratorCategory(const Iter &) {
    typedef typename IteratorTraits<Iter>::iterator_category Category;
    return Category();
}

template<class Iter>
inline typename IteratorTraits<Iter>::difference_type *
DistanceType(const Iter &) {
    return static_cast<typename IteratorTraits<Iter>::difference_type *>(0);
}

template<class Iter>
inline typename IteratorTraits<Iter>::value_type *
ValueType(const Iter &) {
    return static_cast<typename IteratorTraits<Iter>::value_type *>(0);
}




/*
 * 为不同类型迭代器设置重载*/

//1 后插类型
template<class Container>
class BackInsertIterator {
protected:
    Container *container;
public:
    typedef Container container_type;
    typedef OutputIteratorTag iterator_category;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;

    explicit BackInsertIterator(Container &__x) : container(&__x) {}

    BackInsertIterator<Container> &
    operator=(const typename Container::value_type &__value) {
        container->push_back(__value);
        return *this;
    }

    BackInsertIterator<Container> &operator*() { return *this; }

    BackInsertIterator<Container> &operator++() { return *this; }

    BackInsertIterator<Container> &operator++(int) { return *this; }
};
//1 后插类型结束

//2 前插类型
template<class Container>
class FrontInsertIterator {
protected:
    Container *container;
public:
    typedef Container container_type;
    typedef OutputIteratorTag iterator_category;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;

    explicit FrontInsertIterator(Container &__x) : container(&__x) {}

    FrontInsertIterator<Container> &
    operator=(const typename Container::value_type &__value) {
        container->push_front(__value);
        return *this;
    }

    FrontInsertIterator<Container> &operator*() { return *this; }

    FrontInsertIterator<Container> &operator++() { return *this; }

    FrontInsertIterator<Container> &operator++(int) { return *this; }
};

//2 前插类型结束

//3 插入类型
template<class Container>
class InsertIterator {
protected:
    Container *container;
    typename Container::iterator iter;
public:
    typedef Container container_type;
    typedef OutputIteratorTag iterator_category;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;

    InsertIterator(Container &__x, typename Container::iterator __i)
            : container(&__x), iter(__i) {}

    InsertIterator<Container> &
    operator=(const typename Container::value_type &__value) {
        iter = container->insert(iter, __value);
        ++iter;
        return *this;
    }

    InsertIterator<Container> &operator*() { return *this; }

    InsertIterator<Container> &operator++() { return *this; }

    InsertIterator<Container> &operator++(int) { return *this; }
};

//3 插入类型结束
//4 双向插入
template<class Iterator>
class reverse_iterator {
protected:
    Iterator current;
public:
    typedef typename IteratorTraits<Iterator>::iterator_category
            iterator_category;
    typedef typename IteratorTraits<Iterator>::value_type
            value_type;
    typedef typename IteratorTraits<Iterator>::difference_type
            difference_type;
    typedef typename IteratorTraits<Iterator>::pointer
            pointer;
    typedef typename IteratorTraits<Iterator>::reference
            reference;

    typedef Iterator iterator_type;
    typedef reverse_iterator<Iterator> _Self;

public:
    reverse_iterator() {}

    explicit reverse_iterator(iterator_type __x) : current(__x) {}

    reverse_iterator(const _Self &__x) : current(__x.current) {}


    iterator_type base() const { return current; }

    reference operator*() const {
        Iterator __tmp = current;
        return *--__tmp;
    }

    pointer operator->() const { return &(operator*()); }

    _Self &operator++() {
        --current;
        return *this;
    }

    _Self operator++(int) {
        _Self __tmp = *this;
        --current;
        return __tmp;
    }

    _Self &operator--() {
        ++current;
        return *this;
    }

    _Self operator--(int) {
        _Self __tmp = *this;
        ++current;
        return __tmp;
    }

    _Self operator+(difference_type __n) const {
        return _Self(current - __n);
    }

    _Self &operator+=(difference_type __n) {
        current -= __n;
        return *this;
    }

    _Self operator-(difference_type __n) const {
        return _Self(current + __n);
    }

    _Self &operator-=(difference_type __n) {
        current += __n;
        return *this;
    }

    reference operator[](difference_type __n) const { return *(*this + __n); }
};

template<class _Iterator>
inline bool operator==(const reverse_iterator<_Iterator> &__x,
                       const reverse_iterator<_Iterator> &__y) {
    return __x.base() == __y.base();
}

template<class _Iterator>
inline bool operator<(const reverse_iterator<_Iterator> &__x,
                      const reverse_iterator<_Iterator> &__y) {
    return __y.base() < __x.base();
}

template<class _Iterator>
inline typename reverse_iterator<_Iterator>::difference_type
operator-(const reverse_iterator<_Iterator> &__x,
          const reverse_iterator<_Iterator> &__y) {
    return __y.base() - __x.base();
}

template<class _Iterator>
inline reverse_iterator<_Iterator>
operator+(typename reverse_iterator<_Iterator>::difference_type __n,
          const reverse_iterator<_Iterator> &__x) {
    return reverse_iterator<_Iterator>(__x.base() - __n);
}
//4 双向插入结束
