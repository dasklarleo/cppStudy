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
 * 和迭代器相关的函数以便回答algorithms的问题
 * */
//1 返回类别函数
template<class T, class Distance>
inline InputIteratorTag
IteratorCategory(const InputIterator<T, Distance> &) { return InputIteratorTag(); }

inline OutputIteratorTag IteratorCategory(const OutputIterator &) { return OutputIteratorTag(); }

template<class T, class Distance>
inline ForwardIteratorTag
IteratorCategory(const ForwardIterator<T, Distance> &) { return ForwardIteratorTag(); }

template<class T, class Distance>
inline BidirectionalIteratorTag
IteratorCategory(const BidirectionalIterator<T, Distance> &) { return BidirectionalIteratorTag(); }

template<class T, class Distance>
inline RandomAccessIteratorTag
IteratorCategory(const RandomAccessIterator<T, Distance> &) { return RandomAccessIteratorTag(); }

template<class T>
inline RandomAccessIteratorTag IteratorCategory(const T *) { return RandomAccessIteratorTag(); }

template<class Iter>
inline typename IteratorTraits<Iter>::iterator_category
IteratorCategoryInner(const Iter &) {
    typedef typename IteratorTraits<Iter>::iterator_category Category;
    return Category();
}

template<class Iter>
inline typename IteratorTraits<Iter>::iterator_category
IteratorCategory(const Iter &i) { return IteratorCategoryInner(i); }


//1 返回类别函数结束

//2 返回值的类型
template<class T, class Distance>
inline T *ValueType(const InputIterator<T, Distance> &) { return (T *) (0); }

template<class T, class Distance>
inline T *ValueType(const ForwardIterator<T, Distance> &) { return (T *) (0); }

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
inline Distance *DistanceType(const ForwardIterator<T, Distance> &) {
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

//6 difference type开始
template<class Iter>
inline typename IteratorTraits<Iter>::difference_type *
DistanceTypeInner(const Iter &) {
    return static_cast<typename IteratorTraits<Iter>::difference_type *>(0);
}

template<class Iter>
inline typename IteratorTraits<Iter>::difference_type *
DistanceType(const Iter &i) { return DistanceInner(i); }
//6 difference type结束



template<class Iter>
inline typename IteratorTraits<Iter>::value_type *
ValueType(const Iter &) {
    return static_cast<typename IteratorTraits<Iter>::value_type *>(0);
}

template <class _BidirectionalIterator, class _Tp, class _Reference = _Tp&,
        class _Distance = ptrdiff_t>
class reverse_bidirectional_iterator {
    typedef reverse_bidirectional_iterator<_BidirectionalIterator, _Tp,
            _Reference, _Distance>  _Self;
protected:
    _BidirectionalIterator current;
public:
    typedef BidirectionalIteratorTag iterator_category;
    typedef _Tp                        value_type;
    typedef _Distance                  difference_type;
    typedef _Tp*                       pointer;
    typedef _Reference                 reference;

    reverse_bidirectional_iterator() {}
    explicit reverse_bidirectional_iterator(_BidirectionalIterator __x)
            : current(__x) {}
    _BidirectionalIterator base() const { return current; }
    _Reference operator*() const {
        _BidirectionalIterator __tmp = current;
        return *--__tmp;
    }
#ifndef __SGI_STL_NO_ARROW_OPERATOR
    pointer operator->() const { return &(operator*()); }
#endif /* __SGI_STL_NO_ARROW_OPERATOR */
    _Self& operator++() {
        --current;
        return *this;
    }
    _Self operator++(int) {
        _Self __tmp = *this;
        --current;
        return __tmp;
    }
    _Self& operator--() {
        ++current;
        return *this;
    }
    _Self operator--(int) {
        _Self __tmp = *this;
        ++current;
        return __tmp;
    }
};
