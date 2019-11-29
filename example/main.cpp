#include <any>
#include <filesystem>
#include <forward_list>
#include <iostream>
#include <map>
#include <optional>
#include <pretty_print/pretty_print.hpp>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

struct user_data {
    int a = 42;
    std::string str = "hello";
};

enum class size { large, small, medium };

std::ostream& operator<<(std::ostream& out, size val) {
    switch (val) {
        case size::large:
            out << "large";
            break;
        case size::medium:
            out << "medium";
            break;
        case size::small:
            out << "small";
            break;
    }
    return out;
}


template <class T>
T& operator,(T& val, const user_data empty) {
    (void)empty;
    return val;
}
template <class T>
const T& operator,(const T& val, const user_data empty) {
    (void)empty;
    return val;
}

std::ostream& operator<<(std::ostream& out, const user_data& data) {
    out << data.str << ", " << data.a;
    return out;
}

void print_vector() {
    std::vector<int> data = {1, 2, 3, 4};
    pretty::print(std::cout, data) << std::endl;
}

void print_map() {
    std::map<int, int> data = {{1, 2}, {2, 3}, {3, 4}};
    pretty::print(std::cout, data) << std::endl;
}

void print_pair() {
    using namespace std::string_literals;
    auto data = std::make_pair("123"s, 12);
    pretty::print(std::cout, data) << std::endl;
}

void print_optional() {
    using namespace std::string_literals;
    std::optional<std::string> data{"string"s};
    pretty::print(std::cout, data) << std::endl;
}

void print_variant() {
    std::variant<int, std::string, int*> data;
    data = "123";
    pretty::print(std::cout, data) << std::endl;
}

void print_tuple() {
    auto data = std::make_tuple("1", 2, 3, 4.5);
    pretty::print(std::cout, data) << std::endl;
}

void print_c_array() {
    int data[] = {1, 2, 3, 4, 5, 6};
    pretty::print(std::cout, data) << std::endl;
    pretty::print(std::cout, "hello") << std::endl;
}

void print_hardcore() {
    using namespace std::string_literals;
    std::unordered_map<std::string, std::map<std::string, std::optional<int>>> data = {
        {"test"s, {{"1"s, 2}, {"2"s, 3}, {"3"s, 4}}},
        {"hello"s, {{"1"s, 2}, {"2"s, 3}, {"3"s, 4}}},
        {"world"s, {{"1"s, 2}, {"2"s, 3}, {"3"s, {}}}}};
    pretty::print(std::cout, data) << std::endl;
}

void print_user_data() {
    user_data data;
    pretty::print(std::cout, data) << std::endl;
}

void print_filesystem_path() {
    std::filesystem::path data{"/home/user/data"};
    pretty::print(std::cout, data) << std::endl;
}

void print_enum() {
    enum class color { red, green, blue };
    color data = color::green;
    pretty::print(std::cout, data) << std::endl;

    size data2 = size::large;
    pretty::print(std::cout, data2) << std::endl;
}


int main() {
    print_vector();
    print_map();
    print_pair();
    print_optional();
    print_variant();
    print_tuple();
    print_c_array();
    print_hardcore();
    print_user_data();
    print_filesystem_path();
    print_enum();
    return 0;
}
