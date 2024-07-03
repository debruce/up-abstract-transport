#include <iostream>
#include <utility>
#include "safe_tuple_map.h"
#include "demangler.h"

template<typename T, T... ints>
void print_sequence(std::integer_sequence<T, ints...> int_seq)
{
    std::cout << "The sequence of size " << int_seq.size() << ": ";
    ((std::cout << ints << ' '), ...);
    std::cout << '\n';
}


int main(int argc, char *argv[])
{
    using namespace std;

    using RawType = std::tuple<int, int>;

    print_sequence(std::integer_sequence<RawType>());
#if 0
    using MyType = std::tuple<std::optional<int>, std::optional<int>>;

    cout << demangler<MyType>() << endl;

    SafeTupleMap<MyType, int>   m;

    // m.insert(MyType{1,2}, make_shared<int>(12));
    // m.insert(MyType{2,1}, make_shared<int>(21));
    m.insert({MyType{1,2}, MyType{2,1}}, make_shared<int>(11));
    m.insert(MyType{2,nullopt}, make_shared<int>(20));

    m.dump(cout);

    auto ptr = m.find(MyType{2,nullopt});
    if (ptr) {
        cout << "*ptr = " << *ptr << endl;
    }
#endif
}