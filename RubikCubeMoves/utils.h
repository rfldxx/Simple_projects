#include <iostream>

// -----------------------------------------------------------------------
template<typename T, typename... Args>
inline void cycle_swap(T& first, Args&... args) {
    (std::swap(first, args), ...);
}

// template<typename T, typename... Args>
// T GETPOS(T first, Args... args) {
//     return T;
// }

// template<std::size_t i, typename T, typename... Args>
// auto GETPOS(T first, Args... args) {
//     return GETPOS<i>(args...);
// }


// -----------------------------------------------------------------------
// Вызов [n - исключительно число в коде (НЕ переменная ибо это макросы)] 
//      REPEAT(n)( (args) )  ->  args args ... args (n раз)
// Например: (args) = (w, )  ->  w, w, ... w,
#define REPEAT(n) REPEAT_##n

#define unpack(...) __VA_ARGS__

#define REPEAT_1(a) unpack a
#define REPEAT_2(a) unpack a REPEAT_1(a)
#define REPEAT_3(a) unpack a REPEAT_2(a)
#define REPEAT_4(a) unpack a REPEAT_3(a)
#define REPEAT_5(a) unpack a REPEAT_4(a)

