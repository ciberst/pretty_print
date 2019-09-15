#include <any>
#include <forward_list>
#include <iostream>
#include <map>
#include <optional>
#include <pretty_print/pretty_print.hpp>
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

int main() {
    using namespace std::string_literals;


    std::vector<int> a = {1, 2, 3, 4};
    std::map<int, int> map = {{1, 2}, {2, 3}, {3, 4}};
    auto pair = std::make_pair("123"s, 12);
    std::unordered_map<std::string, std::map<std::string, std::optional<int>>> mapmap = {
        {"test"s, {{"1"s, 2}, {"2"s, 3}, {"3"s, 4}}},
        {"hello"s, {{"1"s, 2}, {"2"s, 3}, {"3"s, 4}}},
        {"world"s, {{"1"s, 2}, {"2"s, 3}, {"3"s, {}}}}};
    int c_arr[] = {1, 2, 3, 4, 5, 6};
    auto tuple = std::make_tuple("1", 2, 3, 4.5);
    std::forward_list<std::string> words1{"the", "frogurt", "is", "also", "cursed"};
    std::optional<std::string> opt{"string"s};
    std::optional<std::string> null_opt;
    std::variant<int, std::string> variant;
    variant = "123";

    std::vector<std::variant<int, std::string, std::pair<double, int>>> vector_varints = {
        std::variant<int, std::string, std::pair<double, int>>{1},
        std::variant<int, std::string, std::pair<double, int>>{"string"},
        std::variant<int, std::string, std::pair<double, int>>{std::pair{2.5, 1}},
        std::variant<int, std::string, std::pair<double, int>>{}};

    user_data user_data;

    // pretty::print(std::cout, std::any{123}) << std::endl;

    pretty::print(std::cout, vector_varints) << std::endl;
    pretty::print(std::cout, variant) << std::endl;
    pretty::print(std::cout, pair) << std::endl;
    pretty::print(std::cout, a) << std::endl;
    pretty::print(std::cout, "hello") << std::endl;
    pretty::print(std::cout, map) << std::endl;
    pretty::print(std::cout, mapmap) << std::endl;
    pretty::print(std::cout, c_arr) << std::endl;
    pretty::print(std::cout, tuple) << std::endl;
    pretty::print(std::cout, words1) << std::endl;
    pretty::print(std::cout, opt) << std::endl;
    pretty::print(std::cout, null_opt) << std::endl;
    pretty::print(std::cout, user_data) << std::endl;

    std::cout << "=====================================" << std::endl;

    pretty::print_ti(std::cout, vector_varints) << std::endl;
    pretty::print_ti(std::cout, variant) << std::endl;
    pretty::print_ti(std::cout, pair) << std::endl;
    pretty::print_ti(std::cout, a) << std::endl;
    pretty::print_ti(std::cout, "hello") << std::endl;
    pretty::print_ti(std::cout, map) << std::endl;
    pretty::print_ti(std::cout, mapmap) << std::endl;
    pretty::print_ti(std::cout, c_arr) << std::endl;
    pretty::print_ti(std::cout, tuple) << std::endl;
    pretty::print_ti(std::cout, words1) << std::endl;
    pretty::print_ti(std::cout, opt) << std::endl;
    pretty::print_ti(std::cout, null_opt) << std::endl;
    pretty::print_ti(std::cout, user_data) << std::endl;

    std::cout << "=====================================" << std::endl;

    std::cout << pretty::print(vector_varints) << std::endl
              << pretty::print(variant) << std::endl
              << pretty::print(pair) << std::endl
              << pretty::print(a) << std::endl
              << pretty::print("hello") << std::endl
              << pretty::print(map) << std::endl
              << pretty::print(mapmap) << std::endl
              << pretty::print(c_arr) << std::endl
              << pretty::print(tuple) << std::endl
              << pretty::print(words1) << std::endl
              << pretty::print(opt) << std::endl
              << pretty::print(null_opt) << std::endl
              << pretty::print(user_data) << std::endl;

    std::cout << "=====================================" << std::endl;

    std::cout << pretty::print_ti(vector_varints) << std::endl
              << pretty::print_ti(variant) << std::endl
              << pretty::print_ti(pair) << std::endl
              << pretty::print_ti(a) << std::endl
              << pretty::print_ti("hello") << std::endl
              << pretty::print_ti(map) << std::endl
              << pretty::print_ti(mapmap) << std::endl
              << pretty::print_ti(c_arr) << std::endl
              << pretty::print_ti(tuple) << std::endl
              << pretty::print_ti(words1) << std::endl
              << pretty::print_ti(opt) << std::endl
              << pretty::print_ti(null_opt) << std::endl
              << pretty::print_ti(user_data) << std::endl;

    std::cout << "=====================================" << std::endl;
    std::cout << pretty::print_args(vector_varints, variant) << std::endl;
    std::cout << "=====================================" << std::endl;
    pretty::print_args(std::cout, vector_varints, variant) << std::endl;
    pretty::print_args(std::cout, "hello", "world", 123, 456) << std::endl;
    pretty::print_args(std::cout, "hello", "world", 123, 456);
    pretty::print_args(std::cout, pair, a) << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << pretty::print_args(vector_varints, variant) << std::endl;
    std::cout << "=====================================" << std::endl;

    pretty::print_line(std::cout, vector_varints, variant);

    return 0;
}
