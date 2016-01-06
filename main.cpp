#include "tuple_tools.hpp"

struct MapExampleFunction {
    template <typename T>
    static constexpr auto call(T t) {
        return t * 2;
    }
};

struct FilterExampleFunction {
    template <typename T>
    static constexpr auto call(T) {
        return std::true_type();
    }
    static constexpr auto call(int) {
        return std::false_type();
    }
};

int main()
{
    {
        constexpr auto tp = tuple_tools::partition<2>(std::make_tuple(1, 2, 3, 4));
        static_assert(tp.first == std::make_tuple(1, 2), "");
        static_assert(tp.second == std::make_tuple(3, 4), "");
    }

    {
        constexpr auto ts = tuple_tools::slice<1, 3>(std::make_tuple(1, 2, 3, 4));
        static_assert(ts == std::make_tuple(2, 3), "");
    }

    {
        constexpr auto tr = tuple_tools::remove<1>(std::make_tuple(1, 2, 3, 4));
        static_assert(tr == std::make_tuple(1, 3, 4), "");
    }

    {
        constexpr auto ti = tuple_tools::insert<1>(std::make_tuple(1, 2, 3, 4), std::make_tuple(9, 9, 9));
        static_assert(ti == std::make_tuple(1, 9, 9, 9, 2, 3, 4), "");
    }

    {
        constexpr auto tr1 = tuple_tools::replace<1>(std::make_tuple(1, 2, 3, 4), std::make_tuple(9, 9));
        static_assert(tr1 == std::make_tuple(1, 9, 9, 4), "");
        constexpr auto tr2 = tuple_tools::replace<3>(std::make_tuple(1, 2, 3, 4), std::make_tuple(9, 9));
        static_assert(tr2 == std::make_tuple(1, 2, 3, 9, 9), "");
    }

    {
        constexpr auto tm = tuple_tools::map<MapExampleFunction>(std::make_tuple(1, 2.0f, 3, 4.0));
        static_assert(tm == std::make_tuple(2, 4.0f, 6, 8.0), "");
    }

    {
        constexpr auto tf = tuple_tools::filter<FilterExampleFunction>(std::make_tuple(1.0, 2, 3.0, 4));
        static_assert(tf == std::make_tuple(1.0, 3.0), "");
    }
    return 0;
}