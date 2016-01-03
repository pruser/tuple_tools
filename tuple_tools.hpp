#include <tuple>
#include <type_traits>
#include <utility>

namespace tuple_tools {
    template <typename... T, std::size_t... I>
    auto constexpr get_items(std::tuple<T...> t, std::index_sequence<I...>) {
        return std::make_tuple(std::get<I>(t)...);
    }

    template <std::size_t B, std::size_t E, std::size_t... I>
    constexpr auto _create_index_sequence_impl(std::index_sequence<I...>) {
        return std::index_sequence<(B + I)...>();
    }

    template <std::size_t B, std::size_t E>
    constexpr auto create_index_sequence() {
        return _create_index_sequence_impl<B, E>(std::make_index_sequence<(E - B)...>());
    }

    template <std::size_t... I1, std::size_t... I2>
    constexpr auto cat(std::index_sequence<I1...>, std::index_sequence<I2...>) {
        return std::index_sequence<I1..., I2...>();
    }

    template <typename... T, std::size_t... I1, std::size_t... I2>
    auto constexpr _partition_impl(std::tuple<T...> t, std::index_sequence<I1...>, std::index_sequence<I2...>) {
        return std::make_pair(std::make_tuple(std::get<I1>(t)...), std::make_tuple(std::get<I2>(t)...));
    }

    template <std::size_t P, typename... T>
    auto constexpr partition(std::tuple<T...> t) {
        static_assert(P >= 0, "");
        static_assert(P <= sizeof...(T), "");
        return _partition_impl(t, create_index_sequence<0, P>(), create_index_sequence<P, sizeof...(T)>());
    }

    template <std::size_t B, std::size_t E, typename... T>
    auto constexpr slice(std::tuple<T...> t) {
        static_assert(B >= 0, "");
        static_assert(B <= E, "");
        static_assert(E <= sizeof...(T), "");
        return get_items(t, create_index_sequence<B, E>());
    }

    template <std::size_t R, typename... T>
    constexpr auto  remove(std::tuple<T...> t) {
        static_assert(R >= 0, "");
        static_assert(R <= sizeof...(T), "");
        return get_items(t, cat(create_index_sequence<0, R>(), create_index_sequence<R + 1, sizeof...(T)>()));
    }


    template <std::size_t P, typename... T, typename... T1>
    constexpr auto insert(std::tuple<T...> t, std::tuple<T1...> t1) {
        static_assert(P >= 0, "");
        static_assert(P <= sizeof...(T), "");
        return std::tuple_cat(slice<0, P>(t), t1, slice<P, sizeof...(T)>(t));
    }

    template <std::size_t P, typename... T, typename... T1, typename std::enable_if<((sizeof...(T)-P)>sizeof...(T1))>::type* = nullptr>
    constexpr auto _replace_impl(std::tuple<T...> t, std::tuple<T1...> t1) {
        return std::tuple_cat(slice<0, P>(t), t1, slice<P + sizeof...(T1), sizeof...(T)>(t));
    }
    template <std::size_t P, typename... T, typename... T1, typename std::enable_if<!((sizeof...(T)-P)>sizeof...(T1))>::type* = nullptr>
    constexpr auto _replace_impl(std::tuple<T...> t, std::tuple<T1...> t1) {
        return std::tuple_cat(slice<0, P>(t), t1);
    }

     template <std::size_t P, typename... T, typename... T1>
     auto constexpr replace(std::tuple<T...> t, std::tuple<T1...> t1) {
         static_assert(P >= 0, "");
         static_assert(P <= sizeof...(T), "");
         return _replace_impl<P>(t, t1);
     }
}

