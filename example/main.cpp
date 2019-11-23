#include <any>
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

std::ostream& operator<<(std::ostream& out, const user_data& data) {
    out << data.str << ", " << data.a;
    return out;
}

void print_vector() {
    std::vector<int> a = {1, 2, 3, 4};
    pretty::print(std::cout, a) << std::endl;
}

void print_map() {
    std::map<int, int> map = {{1, 2}, {2, 3}, {3, 4}};
    pretty::print(std::cout, map) << std::endl;
}

void print_pair() {
    using namespace std::string_literals;
    auto pair = std::make_pair("123"s, 12);
    pretty::print(std::cout, pair) << std::endl;
}

void print_optional() {
    using namespace std::string_literals;
    std::optional<std::string> opt{"string"s};
    pretty::print(std::cout, opt) << std::endl;
}

void print_variant() {
    std::variant<int, std::string> variant;
    variant = "123";
    pretty::print(std::cout, variant) << std::endl;
}

void print_tuple() {
    auto tuple = std::make_tuple("1", 2, 3, 4.5);
    pretty::print(std::cout, tuple) << std::endl;
}

void print_c_array() {
    int c_arr[] = {1, 2, 3, 4, 5, 6};
    pretty::print(std::cout, c_arr) << std::endl;
    pretty::print(std::cout, "hello") << std::endl;
}

void print_hardcore() {
    using namespace std::string_literals;
    std::unordered_map<std::string, std::map<std::string, std::optional<int>>> mapmap = {
        {"test"s, {{"1"s, 2}, {"2"s, 3}, {"3"s, 4}}},
        {"hello"s, {{"1"s, 2}, {"2"s, 3}, {"3"s, 4}}},
        {"world"s, {{"1"s, 2}, {"2"s, 3}, {"3"s, {}}}}};
    pretty::print(std::cout, mapmap) << std::endl;
}


#include <complex>
int main() {
    print_vector();
    print_map();
    print_pair();
    print_optional();
    print_variant();
    print_tuple();
    print_c_array();
    print_hardcore();

    return 0;
}
