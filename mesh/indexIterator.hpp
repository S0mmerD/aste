#pragma once
#include <boost/iterator/iterator_facade.hpp>

template <typename Derived, typename ValueT, typename ReferenceT = ValueT&>
class IndexIterator : public boost::iterator_facade<IndexIterator<Derived, ValueT, ReferenceT>, ValueT, boost::random_access_traversal_tag, ReferenceT>
{
    protected:
        friend class boost::iterator_core_access;
        void increment();
        void decrement();
        bool equal(const IndexIterator& other) const;
        ReferenceT dereference() const;
        void advance(int num);
        int distance_to(const IndexIterator& other) const;
        const Derived& impl() const;
        Derived& impl();
    public:
        int _index;
};

template <typename A, typename B, typename C>
void IndexIterator<A, B, C>::increment()
{
    _index++;
}

template <typename A, typename B, typename C>
void IndexIterator<A, B, C>::decrement()
{
    _index--;
}

template <typename A, typename B, typename C>
bool IndexIterator<A, B, C>::equal(const IndexIterator& other) const
{
    bool equal = (_index == other._index);
    equal &= this->impl()._equal(other.impl());
    return equal;
}

template <typename A, typename B, typename ReferenceT>
ReferenceT IndexIterator<A, B, ReferenceT>::dereference() const
{
    return impl()._dereference();
}

template <typename A, typename B, typename C>
void IndexIterator<A, B, C>::advance(int num)
{
    _index += num;
}

template <typename A, typename B, typename C>
int IndexIterator<A, B, C>::distance_to(const IndexIterator& other) const
{
    return other._index - _index;
}

template <typename Derived, typename B, typename C>
const Derived& IndexIterator<Derived, B, C>::impl() const
{
    return *static_cast<const Derived*>(this);
}

template <typename Derived, typename B, typename C>
Derived& IndexIterator<Derived, B, C>::impl()
{
    return *static_cast<Derived*>(this);
}
