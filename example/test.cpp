#include <cassert>
#include <filesystem>
#include <iostream>
#include <map>
#include <pretty_print/pretty_print.hpp>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
using namespace std::string_literals;

template <typename T>
void is_equal_assert(const T& lhs, const T& rhs, int line) {
    if (lhs != rhs) {
        std::cerr << "===" << std::endl
                  << "ASSERT! Line: " << line << std::endl
                  << "Expect: " << lhs << std::endl
                  << "Value : " << rhs << std::endl;
        assert(false);
    }
}
#define ASSERT_EQUAL(x, y) is_equal_assert(x, y, __LINE__)
namespace pretty_test {

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

    void test_vector() {
        std::vector<int> data = {1, 2, 3, 4};
        std::stringstream ss;
        pretty::print(ss, data);
        ASSERT_EQUAL("[1, 2, 3, 4]"s, ss.str());
        ASSERT_EQUAL("[1, 2, 3, 4]"s, pretty::print(data));
    }

    void test_map() {
        std::map<int, int> data = {{1, 2}, {2, 3}, {3, 4}};
        std::stringstream ss;
        pretty::print(ss, data);
        ASSERT_EQUAL("{1: 2, 2: 3, 3: 4}"s, ss.str());
        ASSERT_EQUAL("{1: 2, 2: 3, 3: 4}"s, pretty::print(data));
    }

    void test_pair() {
        auto data = std::make_pair("123"s, 12);
        std::stringstream ss;
        pretty::print(ss, data);
        ASSERT_EQUAL(R"("123": 12)"s, ss.str());
        ASSERT_EQUAL(R"("123": 12)"s, pretty::print(data));
    }

    void test_optional() {
        std::optional<std::string> data{"string"s};
        std::stringstream ss;
        pretty::print(ss, data);
        ASSERT_EQUAL(R"("string")"s, ss.str());
        ASSERT_EQUAL(R"("string")"s, pretty::print(data));
    }

    void test_variant() {
        std::variant<int, std::string, int*> data;
        data = "123";
        std::stringstream ss;
        pretty::print(ss, data);
        ASSERT_EQUAL(R"("123")"s, ss.str());
        ASSERT_EQUAL(R"("123")"s, pretty::print(data));
    }

    void test_tuple() {
        auto data = std::make_tuple("1", 2, 3, 4.5);
        std::stringstream ss;

        pretty::print(ss, data);
        ASSERT_EQUAL(R"data(("1", 2, 3, 4.5))data"s, ss.str());
        ASSERT_EQUAL(R"data(("1", 2, 3, 4.5))data"s, pretty::print(data));
    }

    void test_c_array() {
        int data[] = {1, 2, 3, 4, 5, 6};
        int data2[2][2][2] = {{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}};
        std::stringstream ss;

        pretty::print(ss, data);
        ASSERT_EQUAL(R"([1, 2, 3, 4, 5, 6])"s, ss.str());
        ASSERT_EQUAL(R"([1, 2, 3, 4, 5, 6])"s, pretty::print(data));
        ss = std::stringstream();
        pretty::print(ss, "hello");
        ASSERT_EQUAL(R"("hello")"s, ss.str());
        ASSERT_EQUAL(R"("hello")"s, pretty::print("hello"));
        ss = std::stringstream();
        pretty::print(ss, data2);
        ASSERT_EQUAL(R"([[[1, 2], [3, 4]], [[5, 6], [7, 8]]])"s, ss.str());
        ASSERT_EQUAL(R"([[[1, 2], [3, 4]], [[5, 6], [7, 8]]])"s, pretty::print(data2));
    }

    void test_hardcore() {
        std::unordered_map<std::string, std::map<std::string, std::optional<int>>> data = {
            {"test"s, {{"1"s, 2}, {"2"s, 3}, {"3"s, 4}}},
            {"hello"s, {{"1"s, 2}, {"2"s, 3}, {"3"s, 4}}},
            {"world"s, {{"1"s, 2}, {"2"s, 3}, {"3"s, {}}}}};
        std::stringstream ss;

        pretty::print(ss, data);
        ASSERT_EQUAL(
            R"({"hello": {"1": 2, "2": 3, "3": 4}, "world": {"1": 2, "2": 3, "3": null}, "test": {"1": 2, "2": 3, "3": 4}})"s,
            ss.str());
        ASSERT_EQUAL(
            R"({"hello": {"1": 2, "2": 3, "3": 4}, "world": {"1": 2, "2": 3, "3": null}, "test": {"1": 2, "2": 3, "3": 4}})"s,
            pretty::print(data));
    }

    void test_user_data() {
        user_data data;
        std::stringstream ss;

        pretty::print(ss, data);
        ASSERT_EQUAL(R"(hello, 42)"s, ss.str());
        ASSERT_EQUAL(R"(hello, 42)"s, pretty::print(data));
    }

    void test_filesystem_path() {
        std::filesystem::path data{"/home/user/data"};
        std::stringstream ss;

        pretty::print(ss, data);
        ASSERT_EQUAL(R"("/home/user/data")"s, ss.str());
        ASSERT_EQUAL(R"("/home/user/data")"s, pretty::print(data));
    }

    void test_enum() {
        enum class color { red, green, blue };
        color data = color::green;
        std::stringstream ss;

        pretty::print(ss, data);
        ASSERT_EQUAL(R"(1)"s, ss.str());
        ASSERT_EQUAL(R"(1)"s, pretty::print(data));
        ss = std::stringstream();
        size data2 = size::large;
        pretty::print(ss, data2);
        ASSERT_EQUAL(R"(large)"s, ss.str());
        ASSERT_EQUAL(R"(large)"s, pretty::print(data2));
    }
}  // namespace pretty_test


void run_test() {
    using namespace pretty_test;
    test_vector();
    test_map();
    test_pair();
    test_optional();
    test_variant();
    test_tuple();
    test_c_array();
    test_hardcore();
    test_user_data();
    test_filesystem_path();
    test_enum();
}
