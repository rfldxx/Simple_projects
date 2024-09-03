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

#define SAME_R(i) REPEAT_##i(
#define REPEAT_1(...) __VA_ARGS__
#define REPEAT_2(...) __VA_ARGS__ SAME_R(1) __VA_ARGS__)
#define REPEAT_3(...) __VA_ARGS__ SAME_R(2) __VA_ARGS__)
#define REPEAT_4(...) __VA_ARGS__ SAME_R(3) __VA_ARGS__)
#define REPEAT_5(...) __VA_ARGS__ SAME_R(4) __VA_ARGS__)

