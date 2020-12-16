#include <type_traits>
#include <vector>
#include <list>
#include <set>

#include <iostream>

// is string
template <typename T, typename Properties = void>
struct IsString : std::false_type
{
};

template <typename T>
struct IsString<T, 
                std::void_t<std::enable_if_t<std::is_pointer_v<std::decay_t<T>> 
                                            && std::is_same_v<std::decay_t<std::remove_pointer_t<std::decay_t<T>>>, char>>>>
    : std::true_type
{
};

template <>
struct IsString<std::string, void> : std::true_type
{
};

static_assert(IsString<char*>::value, "char* is not a string!");
static_assert(IsString<const char*>::value, "const char* is not a string!");
static_assert(IsString<char[]>::value, "char[] is not a string!");
static_assert(IsString<const char[]>::value, "const char[] is not a string!");
static_assert(IsString<std::string>::value, "std::string is not a string!");
static_assert(!IsString<int*>::value, "int* is a string!");
static_assert(!IsString<const int*>::value, "const int* is a string!");
static_assert(!IsString<int[]>::value, "int[] is a string!");

// is container
template <typename T, typename Properties = void>
struct IsContainer : std::false_type
{
};

template <typename T>
struct IsContainer<T, std::void_t<std::enable_if_t<!IsString<T>::value 
                                                   && std::is_array_v<T>>>>
    : std::true_type
{
};

template <typename T>
struct IsContainer<T, std::void_t<std::enable_if_t<!IsString<T>::value>,
                                  decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type
{
};

static_assert(!IsContainer<char*>::value, "char* is a container!");
static_assert(!IsContainer<char[]>::value, "char[] is a container!");
static_assert(IsContainer<std::vector<int>>::value, "vector is not a container!");
static_assert(IsContainer<std::list<char>>::value, "list is not a container!");
static_assert(IsContainer<std::set<char>>::value, "set is not a container!");
static_assert(IsContainer<int[]>::value, "int[] is not a container!");

// print for all collections
template <typename T>
std::enable_if_t<IsContainer<T>::value> print(std::ostream& out, const T& container)
{
    bool needsComma = false;
    for (const auto& val : container)
    {
        if (needsComma)
            out << ", ";
        else
            needsComma = true;

        out << val;
    }
}

template <typename T, typename Properties = std::enable_if_t<IsContainer<T>::value>>
std::ostream& operator<<(std::ostream& out, const T& container)
{
    print(out, container);
    return out;
}

int main()
{
    const char hello[] = "Hello";
    const int ints[] = {1, 3, 5, 7, 9};

    std::cout << "char[]: " << hello << '\n';
    std::cout << "int[]: " << ints << '\n';
    std::cout << "ilist: " << std::initializer_list<int>{2, 4, 6, 8, 10} << '\n';
    std::cout << "vec: " << std::vector<char>{'c', 'o', 'o', 'l'} << '\n';
    std::cout << "list: " << std::list<double>{1.1, 1.2, 1.3} << '\n';
    std::cout << "set (sorted): " << std::set<int>{9, 7, 5, 2, 0} << '\n';
    std::cout << "string: " << std::string("world!") << '\n';

    return 0;
}
