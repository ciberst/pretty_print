#include <forward_list>
#include <iostream>
#include <map>
#include <optional>
#include <unordered_map>
#include <variant>
#include <vector>
#include "pretty_print/pretty_print.hpp"

int main() {
	using namespace std::string_literals;
	using pretty::pretty_print;

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

	pretty_print(std::cout, pair) << std::endl;
	pretty_print(std::cout, a) << std::endl;
	pretty_print(std::cout, "hello") << std::endl;
	pretty_print(std::cout, map) << std::endl;
	pretty_print(std::cout, mapmap) << std::endl;
	pretty_print(std::cout, c_arr) << std::endl;
	pretty_print(std::cout, tuple) << std::endl;
	pretty_print(std::cout, words1) << std::endl;
	pretty_print(std::cout, opt) << std::endl;
	pretty_print(std::cout, null_opt) << std::endl;

	std::cout << "=====================================" << std::endl;

	std::cout << pretty_print(pair) << std::endl
	          << pretty_print(a) << std::endl
	          << pretty_print("hello") << std::endl
	          << pretty_print(map) << std::endl
	          << pretty_print(mapmap) << std::endl
	          << pretty_print(c_arr) << std::endl
	          << pretty_print(tuple) << std::endl
	          << pretty_print(words1) << std::endl
	          << pretty_print(opt) << std::endl
	          << pretty_print(null_opt) << std::endl;

	return 0;
}
