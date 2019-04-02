#ifndef SMALLVECTOR_H
#define SMALLVECTOR_H

#include <array>

namespace lxd {
template <typename T, std::size_t Capacity>
class SmallVectorIter;
}

namespace std {
    template<typename T, std::size_t Capacity>
    class iterator_traits<lxd::SmallVectorIter<T,Capacity> >
    {
    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::random_access_iterator_tag;
    };
}

namespace lxd {

template<typename T, std::size_t Capacity>
class SmallVectorIter
{
    T * buf;
    std::size_t off;
public:
    SmallVectorIter(T *buf, std::size_t off)
        : buf(buf), off(off)
    {}


    bool operator==(const SmallVectorIter &i) {
        return i.buf == buf && i.off == off;
    }
    bool operator!=(const SmallVectorIter &i) {
        return !(*this == i);
    }
    SmallVectorIter & operator++()    { ++off; return *this; }
    SmallVectorIter operator++(int) { auto t = *this; ++off; return t; }
    SmallVectorIter & operator--()    { --off; return *this; }
    SmallVectorIter operator--(int) { auto t = *this; --off; return t; }
    std::ptrdiff_t operator-(SmallVectorIter const& sibling) const { return off - sibling.off; }
    SmallVectorIter & operator+=(int amount) { off += amount; return *this; }
    SmallVectorIter & operator-=(int amount) { off -= amount; return *this; }
    bool operator<(SmallVectorIter const&sibling) const { return off < sibling.off;}
    bool operator<=(SmallVectorIter const&sibling) const { return off <= sibling.off; }
    bool operator>(SmallVectorIter const&sibling) const { return off > sibling.off; }
    bool operator>=(SmallVectorIter const&sibling) const { return off >= sibling.off; }
    T& operator[](int index) const { return *(*this + index); }
    T& operator*() const { return buf[off]; }
};

template<typename T, size_t const N>
class SmallVector
{
public:
    SmallVector() : _size(0), _capacity(N), _heapData(_stackData.data())
    {
    }
    T &operator[](size_t n);
    void push_back(T const &e);

    size_t size() const {return _size;}
    size_t capacity() const {return _capacity;}
private:
    std::array<T, N> _stackData;
    size_t _size;
    size_t _capacity;
    T *_heapData;
public:
    using iterator = SmallVectorIter<T, N>;
    using const_iterator = SmallVectorIter<const T, N>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() { return { _heapData, 0 }; }
    iterator end() { return {_heapData, _size}; }
    const_iterator begin() const { return {_heapData, 0}; }
    const_iterator end() const { return {_heapData, _size}; }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }

    reverse_iterator rbegin() { return reverse_iterator{end()}; }
    reverse_iterator rend() { return reverse_iterator{begin()}; }
    const_reverse_iterator rbegin() const { return const_reverse_iterator{end()}; }
    const_reverse_iterator rend() const { return const_reverse_iterator{begin()}; }
    const_reverse_iterator crbegin() const { return rbegin(); }
    const_reverse_iterator crend() const { return rend(); }
};

template<typename T, size_t const N>
void SmallVector<T,N>::push_back(T const &e)
{
    if (_size == _capacity) {
        _capacity = static_cast<size_t>(_capacity * 1.5);
        auto newHeap = new T[_capacity];
        std::copy(_heapData, _heapData + _size, newHeap);
        if (_heapData != _stackData.data()) {
            delete []_heapData;
        }
        _heapData = newHeap;
    }
    _heapData[_size++] = e;
};


}

#endif // SMALLVECTOR_H
