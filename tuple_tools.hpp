#include <tuple>
#include <type_traits>
#include <utility>

namespace tuple_tools {
    template <std::size_t I, std::size_t P, std::size_t S>
    struct _partition_impl {
       template <
           typename... T,
           typename... T1,
           typename... T2,
           typename std::enable_if<(sizeof...(T1) < P) > ::type* = nullptr
       >
       static constexpr auto partition(std::tuple<T...> t, std::tuple<T1...> t1, std::tuple<T2...> t2) {
           return _partition_impl<I + 1, P, S>::partition(t, std::tuple_cat(t1, std::make_tuple(std::get<I>(t))), t2);
       }

       template <
           typename... T,
           typename... T1,
           typename... T2,
           typename std::enable_if<(!(sizeof...(T1) < P)) > ::type* = nullptr
       >
       static constexpr auto partition(std::tuple<T...> t, std::tuple<T1...> t1, std::tuple<T2...> t2) {
           return _partition_impl<I + 1, P, S>::partition(t, t1, std::tuple_cat(t2, std::make_tuple(std::get<I>(t))));
       }
    };

    template <std::size_t P, std::size_t S>
    struct _partition_impl<S, P, S> {
       template <
           typename... T,
           typename... T1,
           typename... T2
       >
       static constexpr auto partition(std::tuple<T...> t, std::tuple<T1...> t1, std::tuple<T2...> t2) {
           return std::make_pair(t1, t2);
       }
    };

    template <std::size_t P, typename... T>
    auto constexpr partition(std::tuple<T...> t) {
       static_assert(P >= 0, "");
       static_assert(P <= sizeof...(T), "");
       return _partition_impl<0, P, sizeof...(T)>::partition(t, std::tuple<>{}, std::tuple<>{});
    }

    template <std::size_t I, std::size_t E>
    struct _slice_impl {
       template <
           typename... T,
           typename... T1
       >
       static constexpr auto slice(std::tuple<T...> t, std::tuple<T1...> t1) {
           return _slice_impl<I + 1, E>::slice(t, std::tuple_cat(t1, std::make_tuple(std::get<I>(t))));
       }
    };

    template <std::size_t E>
    struct _slice_impl<E, E> {
       template <
           typename... T,
           typename... T1
       >
       static constexpr auto slice(std::tuple<T...> t, std::tuple<T1...> t1) {
           return t1;
       }
    };

    template <std::size_t B, std::size_t E, typename... T>
    auto constexpr slice(std::tuple<T...> t) {
       static_assert(B >= 0, "");
       static_assert(B <= E, "");
       static_assert(E <= sizeof...(T), "");
       return _slice_impl<B, E>::slice(t, std::tuple<>{});
    }

    template <std::size_t I, std::size_t R, std::size_t S>
    struct _remove_impl {
       template <typename... T, typename... T1>
       static constexpr auto remove(std::tuple<T...> t, std::tuple<T1...> t1) {
           return _remove_impl<I+1, R, S>::remove(t, std::tuple_cat(t1, std::make_tuple(std::get<I>(t))));
       }
    };
    template <std::size_t R, std::size_t S>
    struct _remove_impl<R, R, S> {
       template <typename... T, typename... T1>
       static constexpr auto remove(std::tuple<T...> t, std::tuple<T1...> t1) {
           return _remove_impl<R+1, R, S>::remove(t, t1);
       }
    };
    template <std::size_t R, std::size_t S>
    struct _remove_impl<S, R, S> {
       template <typename... T, typename... T1>
       static constexpr auto remove(std::tuple<T...> t, std::tuple<T1...> t1) {
           return t1;
       }
    };


    template <std::size_t R, typename... T>
    constexpr auto  remove(std::tuple<T...> t) {
       static_assert(R >= 0, "");
       static_assert(R <= sizeof...(T), "");
       return _remove_impl<0, R, sizeof...(T)>::remove(t, std::tuple<>{});
    }

     template <std::size_t P, typename... T, typename... T1>
     constexpr auto insert(std::tuple<T...> t, std::tuple<T1...> t1) {
         static_assert(P >= 0, "");
         static_assert(P <= sizeof...(T), "");
         return std::tuple_cat(slice<0, P>(t), t1, slice<P, sizeof...(T)>(t));
     }
     template <std::size_t P>
     struct _replace_impl {
         template <typename... T, typename... T1, typename std::enable_if<((sizeof...(T)-P)>sizeof...(T1))>::type* = nullptr>
         static constexpr auto replace(std::tuple<T...> t, std::tuple<T1...> t1) {
             return std::tuple_cat(slice<0, P>(t), t1, slice<P + sizeof...(T1), sizeof...(T)>(t));
         }
         template <typename... T, typename... T1, typename std::enable_if<!((sizeof...(T)-P)>sizeof...(T1))>::type* = nullptr>
         static constexpr auto replace(std::tuple<T...> t, std::tuple<T1...> t1) {
             return std::tuple_cat(slice<0, P>(t), t1);
         }
     };
     template <std::size_t P, typename... T, typename... T1>
     auto constexpr replace(std::tuple<T...> t, std::tuple<T1...> t1) {
         static_assert(P >= 0, "");
         static_assert(P <= sizeof...(T), "");
         return _replace_impl<P>::replace(t, t1);
     }
}

