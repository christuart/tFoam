#ifndef RECONSTRUCT_H
#define RECONSTRUCT_H

#include <type_traits>

template <class T, typename... Args>
void Reconstruct(T& x, Args&&... args)
{
    static_assert(!std::has_virtual_destructor<T>::value, "Unsafe"); 
    x.~T();
    new (&x) T(std::forward<Args>(args)...);
}

#endif
