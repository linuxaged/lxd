#ifndef SMALLVECTOR_H
#define SMALLVECTOR_H

#include <array>

namespace lxd {

template<typename T, size_t N>
class SmallVector
{
public:
    SmallVector() : _size(0), _capacity(N), _heapData(_stackData.data())
    {
    }
    T &operator[](size_t n);
    void push_back(T const &e);

    size_t size() const {return _size;}
private:
    std::array<T, N> _stackData;
    size_t _size;
    size_t _capacity;
    T *_heapData;
};

template<typename T, size_t N>
T &SmallVector<T,N>::operator[](size_t n)
{
   return _heapData[n];
};

template<typename T, size_t N>
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

template <typename T, typename C, size_t const N>
class SmallVectorIteratorType
{
public:
    SmallVectorIteratorType(C &container, size_t const index) : _container(container), _index(index){}
    bool operator!=(SmallVectorIteratorType const &other) const { return _index != other._index; }
    T const &operator*() const { return _container[_index]; }
    SmallVectorIteratorType const &operator++() { ++_index; return *this; }
private:
    C &_container;
    size_t _index;
};

template <typename T, size_t const N>
using SmallVectorIterator = SmallVectorIteratorType<T, SmallVector<T, N>, N>;

template <typename T, size_t const N>
using SmallVectorConstIterator = SmallVectorIteratorType<T, SmallVector<T, N> const, N>;

template <typename T, size_t const N>
inline SmallVectorIterator<T, N> begin(
  SmallVector<T, N> &container)
{
  return SmallVectorIterator<T, N>(container, 0);
}

template <typename T, size_t const N>
inline SmallVectorIterator<T, N> end(
  SmallVector<T, N> &container)
{
  return SmallVectorIterator<T, N>(
    container, container.size());
}

template <typename T, size_t const N>
inline SmallVectorConstIterator<T, N> begin(
  SmallVector<T, N> const &container)
{
  return SmallVectorConstIterator<T, N>(
    container, 0);
}

template <typename T, size_t const N>
inline SmallVectorConstIterator<T, N> end(
  SmallVector<T, N> const &container)
{
  return SmallVectorConstIterator<T, N>(
    container, container.size());
}

}

#endif // SMALLVECTOR_H
