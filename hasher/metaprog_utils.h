#pragma once

//
// This is a constexpr for loop
//
template <auto Start, auto End, auto Inc, class F>
constexpr void constexpr_for(F&& f)
{
    if constexpr (Start < End)
    {
        f(std::integral_constant<decltype(Start), Start>());
        constexpr_for<Start + Inc, End, Inc>(f);
    }
}

//
// Example use case of a lambda applied over an integral range.
//
// constexpr_for<0, 5, 1>([&sum](auto i){ cout << i << endl; });
//

//
// This is a constexpr foreach over args.
//
template <class F, class... Args>
constexpr void constexpr_for(F&& f, Args&&... args)
{
    (f(std::forward<Args>(args)), ...);
}

//
// Example use case of a lambda applied to each argument of a function.
//
// template <class... Args>
// void print_all(Args const&... args)
// {
//     constexpr_for([](auto const& v) {
//         std::cout << v << std::endl;
//     }, args...);
// }

template <class F, class Tuple>
constexpr void constexpr_for_tuple(F&& f, Tuple&& tuple)
{
    constexpr size_t cnt = std::tuple_size_v<std::decay_t<Tuple>>;

    constexpr_for<size_t(0), cnt, size_t(1)>([&](auto i) {
        f(std::get<i.value>(tuple));
    });
}

//
// Example use case of a lambda applied to each member of a tuple.
//
// constexpr_for_tuple(
//     [](auto const& v) { std::cout << v << std::endl; },
//     std::make_tuple(1, 'c', true));


//
// This is a type converter to apply optional to each member of a tuple.
//
template<typename ARG>
struct OptionalizeTuple {
    template<typename T>
    struct Optionalize {
        using type = std::optional<T>;
    };

    template<typename Tuple, std::size_t... Is>
    static constexpr auto tupleOptionalizeImpl(const Tuple& tpl, std::index_sequence<Is...>) {
        return std::make_tuple(typename Optionalize<std::tuple_element_t<Is, Tuple>>::type(std::get<Is>(tpl))...);
    }

    template<typename... Ts>
    static constexpr auto tupleOptionalize(const std::tuple<Ts...>& tpl) {
        return tupleOptionalizeImpl(tpl, std::index_sequence_for<Ts...>{});
    }

    using type = decltype(tupleOptionalize(ARG{}));
};
