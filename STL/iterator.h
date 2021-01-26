//
// Created by 刘玉晓 on 2021-01-25.
//

#include <stddef.h>

#ifndef STL_ITERATOR_H
#define STL_ITERATOR_H

#endif //STL_ITERATOR_H

struct InputInteratorTag {
};
struct OutputIteratorTag {
};
struct ForwardIteratorTag : public InputInteratorTag {
};
struct BidirectionalIteratorTag : public ForwardIteratorTag {
};
struct RandomAccessIteratorTag : public BidirectionalIteratorTag {
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


template<class InputInterator>
inline typename IteratorTraits<InputInterator>::difference_type
DistanceInner(InputInterator first, InputInterator last, InputInteratorTag) {
    typename IteratorTraits<InputInterator>::difference_type n = 0;
    while (first != last) {
        ++first;
        ++n;
    }
    return n;
}

template<class RandomIterator>
inline typename IteratorTraits<RandomIterator>::difference_type
DistanceInnerInner(RandomIterator first, RandomIterator last, RandomAccessIteratorTag) {
    return last - first;
}

template<class InputInterator>
inline typename IteratorTraits<InputInterator>::difference_type
Distance(InputInterator first, InputInterator last) {
    typedef typename IteratorTraits<InputInterator>::iterator_category category;
    return DistanceInner(first, last, category());
}

template<class BidirectionalIterator, class Distance>
inline void AdvanceInner(BidirectionalIterator &i, Distance n, BidirectionalIteratorTag) {
    if (n >= 0)
        while (n--)++i;
    else
        while (n++)--i;
}

template<class InputInterator, class Distance>
inline void AnvanceInner(InputInterator &i, Distance n, InputInteratorTag) {
    while (n--)++i;
}

template<class RandomAccessInterator, class Distance>
inline void AnvanceInner(RandomAccessInterator &i, Distance n, RandomAccessIteratorTag) {
    i += n;
}

template<class InputInterator, class Distance>
inline void Advance(InputInterator &i, Distance n) {
    AdvanceInner(i, n, IteratorCategory(i));
}