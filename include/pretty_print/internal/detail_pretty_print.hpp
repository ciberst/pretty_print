#pragma once
#include <iomanip>  // std::quoted
#include <type_traits>
#include <variant>

namespace pretty::detail {

    template <typename T, typename = void>
    struct is_iterable : std::false_type {};
    template <typename T>
    struct is_iterable<T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>>
        : std::true_type {};
    template <typename T>
    inline constexpr bool is_iterable_v = is_iterable<T>::value;

    static_assert(!is_iterable_v<std::variant<int, int>>, "test failed");
    static_assert(is_iterable_v<std::string>, "test failed");

    template <typename Stream, typename, typename = void>
    struct has_ostream_operator : std::false_type {};
    template <typename Stream, typename T>
    struct has_ostream_operator<Stream, T, std::void_t<decltype(std::declval<Stream&>() << std::declval<const T&>())>>
        : std::true_type {};

    template <typename Stream, typename T>
    inline constexpr bool has_ostream_operator_v = has_ostream_operator<Stream, T>::value;

    static_assert(!has_ostream_operator_v<std::ostream, std::pair<int, int>>, "test failed");
    static_assert(has_ostream_operator_v<std::ostream, std::string>, "test failed");

    template <typename T, typename T1, typename... Args>
    struct is_same_any_of {
        static constexpr bool value = std::is_same_v<T, T1> || (std::is_same_v<T, Args> || ...);
    };
    template <typename T, typename T1, typename... Args>
    inline constexpr bool is_same_any_of_v = is_same_any_of<T, T1, Args...>::value;

    static_assert(is_same_any_of_v<char, int, char, double>, "test failed");

    template <class T>
    struct make_signed_or_return {
        using type = std::conditional_t<std::is_signed_v<T>, T, typename std::make_signed_t<T>>;
    };
    template <class T>
    using make_signed_or_return_t = typename make_signed_or_return<T>::type;

    static_assert(std::is_same_v<char, make_signed_or_return_t<char>>, "test failed");
    static_assert(std::is_same_v<signed char, make_signed_or_return_t<unsigned char>>, "test failed");


    template <typename T>
    static constexpr bool is_char_type_v =
        is_same_any_of_v<make_signed_or_return_t<T>, signed char, char, char16_t, char32_t, wchar_t>;

    static_assert(is_char_type_v<char>, "test failed");
    static_assert(is_char_type_v<unsigned char>, "test failed");

    template <typename T, size_t N, typename = std::enable_if_t<is_char_type_v<T>>>
    auto quoted_helper(const T (&s)[N]) noexcept {
        return std::quoted(s);
    }

    inline auto quoted_helper(const char* c) noexcept { return std::quoted(c); }

    inline auto quoted_helper(const std::string& s) noexcept { return std::quoted(s); }

    inline auto quoted_helper(std::string& s) noexcept { return std::quoted(s); }

    template <class CharT, class Traits>
    auto quoted_helper(std::basic_string_view<CharT, Traits> s) noexcept {
        return std::quoted(s);
    }

    template <class T>
    decltype(auto) quoted_helper(T&& v) noexcept {
        return std::forward<T>(v);
    }

    template <std::size_t I, std::size_t N>
    struct print_tuple_impl {
        template <class Stream, class T>
        static void print(Stream& out, const T& value) {
            if (!!I) out << ", ";
            out << detail::quoted_helper(std::get<I>(value));
            print_tuple_impl<I + 1, N>::print(out, value);
        }
    };

    template <std::size_t I>
    struct print_tuple_impl<I, I> {
        template <class Stream, class T>
        static void print(Stream&, const T&) noexcept {}
    };

    struct ostream {
        template <size_t Nested, class Stream, class T>
        static Stream& ostream_impl(Stream& out, const T& data) {
            if constexpr (detail::is_iterable_v<T> && !detail::has_ostream_operator_v<Stream, T>) {
                std::string delimiter;
                out << '{';
                for (const auto& el : data) {
                    out << delimiter;
                    ostream_impl<Nested + 1>(out, detail::quoted_helper(el));
                    delimiter = ", ";
                }
                out << '}';
            } else if constexpr (detail::has_ostream_operator_v<Stream, T>) {
                out << detail::quoted_helper(data);
                return out;
            } else {
                static_assert(detail::has_ostream_operator_v<Stream, T>,
                              "not support [ostream& operator<<(ostream& out, const T& data)]");
            }

            return out;
        }

        template <size_t Nested, class Stream, typename T, typename V,
                  typename = std::enable_if_t<!detail::has_ostream_operator_v<Stream, std::pair<T, V>>>>
        static Stream& ostream_impl(Stream& out, const std::pair<T, V>& data) {
            if constexpr (detail::has_ostream_operator_v<Stream, std::pair<T, V>>)
                out << data;
            else {
                if (!!Nested) out << '{';
                ostream_impl<Nested + 1>(out, detail::quoted_helper(data.first));
                out << ": ";
                ostream_impl<Nested + 1>(out, detail::quoted_helper(data.second));
                if (!!Nested) out << '}';
            }
            return out;
        }

        template <size_t Nested, class Stream, typename... Args,
                  typename = std::enable_if_t<!detail::has_ostream_operator_v<Stream, std::tuple<Args...>>>>
        static Stream& ostream_impl(Stream& out, const std::tuple<Args...>& data) {
            out << "(";
            detail::print_tuple_impl<0, std::tuple_size_v<std::tuple<Args...>>>::print(out, data);
            out << ")";
            return out;
        }

        template <size_t Nested, class Stream, typename T,
                  typename = std::enable_if_t<!detail::has_ostream_operator_v<Stream, std::optional<T>>>>
        static Stream& ostream_impl(Stream& out, const std::optional<T>& data) {
            if (data) {
                ostream_impl<Nested + 1>(out, detail::quoted_helper(data.value()));
            } else {
                out << "NULL";
            }
            return out;
        }

        template <size_t Nested, class Stream, typename T, typename... Ts>
        static Stream& ostream_impl(Stream& out, const std::variant<T, Ts...>& data) {
            if (data.index() != std::variant_npos) {
                std::visit([&out](const auto& t) { ostream_impl<Nested + 1>(out, t); }, data);
                return out;
            }
            out << "VARIANT_NPOS";
            return out;
        }
    };

    template <class Stream, typename T, size_t N>
    Stream& print_array(Stream& out, const T (&data)[N]) {
        std::string delimiter;
        out << '[';
        for (const auto& el : data) {
            out << delimiter;
            ostream::ostream_impl<0>(out, detail::quoted_helper(el));
            delimiter = ", ";
        }
        out << ']';
        return out;
    }

}  // namespace pretty::detail
