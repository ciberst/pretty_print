#pragma once
#include <cstddef>      // std::size_t
#include <iomanip>      // std::quoted
#include <string>       // std::string
#include <tuple>        // std::tuple
#include <type_traits>  // std::declval, std::void_t,
                        // std::false_type, std::true_type
#include <utility>      // std::pair, std::tuple
                        // std::forward

#if __has_include(<variant>)
#include <variant>  // std::variant
#endif
#if __has_include(<optional>)
#include <optional>  // std::optional
#endif
namespace pretty {
    struct print_tag {};
    struct options {
        using indent_size_t = uint8_t;

        /** Enable prettify */
        bool prettify = false;

        indent_size_t indent_size = 4;

        bool compact_array = false;

        constexpr options() = default;
    };
}  // namespace pretty
namespace pretty::detail {

    enum class new_line : uint8_t {
        none = 0,
        after_data = 1 << 0,
        before_data = 1 << 1,
    };
    using new_line_underlying_type_t = std::underlying_type_t<new_line>;
    inline constexpr pretty::print_tag tag;
    inline constexpr bool operator&(new_line lhs, new_line rhs) {
        return !!(static_cast<new_line_underlying_type_t>(lhs) & static_cast<new_line_underlying_type_t>(rhs));
    }

    template <typename T, typename = void>
    struct is_iterable : std::false_type {};
    template <typename T>
    struct is_iterable<T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>>
        : std::true_type {};

    template <typename T>
    inline constexpr bool is_iterable_v = is_iterable<T>::value || std::is_array_v<T>;

    template <typename Stream, typename, typename = void>
    struct has_ostream_operator : std::false_type {};
    template <typename Stream, typename T>
    struct has_ostream_operator<Stream, T, std::void_t<decltype(std::declval<Stream&>() << std::declval<const T&>())>>
        : std::true_type {};

    template <typename Stream, typename T, typename = void>
    struct has_stream_to : std::false_type {};

    template <typename Stream, typename T>
    struct has_stream_to<
        Stream, T,
        std::void_t<decltype(stream_to(*std::declval<const T*>(), std::declval<Stream&>(), pretty::print_tag()))>>
        : std::true_type {};


    template <typename Stream, typename T>
    inline constexpr bool has_ostream_operator_v = has_ostream_operator<Stream, T>::value;

    template <typename T, typename T1, typename... Args>
    struct is_same_any_of {
        static constexpr bool value = std::is_same_v<T, T1> || (std::is_same_v<T, Args> || ...);
    };
    template <typename T, typename T1, typename... Args>
    inline constexpr bool is_same_any_of_v = is_same_any_of<T, T1, Args...>::value;


    template <typename T>
    inline constexpr bool is_char_type_v =
        is_same_any_of_v<T, unsigned char, signed char, char, char16_t, char32_t, wchar_t>;

    template <typename T, typename = void>
    struct is_c_string : std::false_type {};
    template <typename T>
    struct is_c_string<T[]> : std::conditional_t<is_char_type_v<T>, std::true_type, std::false_type> {};
    template <typename T, size_t N>
    struct is_c_string<T[N]> : std::conditional_t<is_char_type_v<T>, std::true_type, std::false_type> {};

    template <typename T>
    inline constexpr bool is_c_string_v = is_c_string<T>::value;

    template <typename T, typename = void>
    struct is_map : std::false_type {};
    template <typename T>
    struct is_map<T, std::void_t<typename T::key_type, typename T::mapped_type,
                                 decltype(std::declval<T>()[std::declval<const typename T::key_type>()])>>
        : std::true_type {};
    template <typename T>
    inline constexpr bool is_map_v = is_map<T>::value;

    template <typename T, std::size_t N, typename = std::enable_if_t<is_char_type_v<T>>>
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

    struct ostream {  // struct ostream
        template <std::size_t Nested, class Stream, class T>
        static void print_map(const options& params, Stream& out, const T& data);
        template <std::size_t Nested, class Stream, class T>
        static void print_array(const options& params, Stream& out, const T& data);
        template <std::size_t Nested, class Stream, class T>
        static Stream& ostream_impl(const options& params, Stream& out, const T& data);
        template <std::size_t Nested, class Stream, typename T, typename V,
                  typename = std::enable_if_t<!detail::has_ostream_operator_v<Stream, std::pair<T, V>>>>
        static Stream& ostream_impl(const options& params, Stream& out, const std::pair<T, V>& data);
        template <std::size_t Nested, class Stream, typename... Args,
                  typename = std::enable_if_t<!detail::has_ostream_operator_v<Stream, std::tuple<Args...>>>>
        static Stream& ostream_impl(const options& params, Stream& out, const std::tuple<Args...>& data);
#if __has_include(<optional>)
        template <std::size_t Nested, class Stream, typename T,
                  typename = std::enable_if_t<!detail::has_ostream_operator_v<Stream, std::optional<T>>>>
        static Stream& ostream_impl(const options& params, Stream& out, const std::optional<T>& data);
#endif
#if __has_include(<variant>)
        template <std::size_t Nested, class Stream, typename T, typename... Ts>
        static Stream& ostream_impl(const options& params, Stream& out, const std::variant<T, Ts...>& data);
#endif
    };  // struct ostream


    template <typename Stream>
    constexpr void append_indent(Stream& out, options::indent_size_t indent) {
        for (options::indent_size_t i = 0; i != indent; ++i) out << ' ';
    }

    template <std::size_t Nested, new_line AddNewline, typename Stream>
    constexpr void append(const options& params, Stream& out) {
        if (params.prettify) {
            if constexpr (!!(AddNewline & new_line::before_data)) out << "\n";
            for (std::size_t i = 0; i != Nested; ++i) {
                append_indent(out, params.indent_size);
            }
            if constexpr (!!(AddNewline & new_line::after_data)) out << "\n";
        }
    }

    template <std::size_t Nested, new_line AddNewline, typename Stream, typename T>
    constexpr void append(const options& params, Stream& out, T&& data) {
        if (params.prettify) {
            if constexpr (!!(AddNewline & new_line::before_data)) out << "\n";
            for (std::size_t i = 0; i != Nested; ++i) {
                append_indent(out, params.indent_size);
            }
            out << std::forward<T>(data);
            if constexpr (!!(AddNewline & new_line::after_data)) {
                out << "\n";
            }
        } else {
            out << std::forward<T>(data);
        }
    }

    template <std::size_t Nested, class Stream, class Tuple, std::size_t... Is>
    void print_tuple_impl(const options& params, Stream& out, const Tuple& value, std::index_sequence<Is...>) {
        ((void)(append<Nested, new_line::none>(params, out, (Is == 0 ? "" : ", ")),
                (void)ostream::ostream_impl<Nested>(params, out, std::get<Is>(value))),
         ...);
    }

    template <std::size_t Nested, class Stream, class T>
    void ostream::print_map(const options& params, Stream& out, const T& data) {
        std::string delimiter;
        append<0, new_line::after_data>(params, out, '{');
        for (const auto& el : data) {
            if (!delimiter.empty()) {
                append<0, new_line::after_data>(params, out, delimiter);
            }
            append<Nested + 1, new_line::none>(params, out);

            ostream_impl<Nested>(params, out, detail::quoted_helper(el));

            delimiter = ", ";
        }
        append<Nested, new_line::before_data>(params, out, '}');
    }

    template <std::size_t Nested, class Stream, class T>
    void ostream::print_array(const options& params, Stream& out, const T& data) {
        std::string delimiter;
        using under_layer_t = std::remove_reference_t<decltype(*std::begin(data))>;
        if (params.compact_array) {
            if constexpr (!is_iterable_v<under_layer_t>) {
                append<0, new_line::none>(params, out, '[');
            } else {
                append<0, new_line::after_data>(params, out, '[');
            }
            for (const auto& el : data) {
                if (!delimiter.empty()) {
                    if constexpr (is_iterable_v<under_layer_t>) {
                        append<0, new_line::after_data>(params, out, delimiter);
                    } else {
                        append<0, new_line::none>(params, out, delimiter);
                    }
                }
                if constexpr (is_iterable_v<under_layer_t>) {
                    append<Nested + 1, new_line::none>(params, out);
                }
                ostream_impl<Nested + 1>(params, out, detail::quoted_helper(el));
                delimiter = ", ";
            }
            if constexpr (!is_iterable_v<under_layer_t>) {
                append<0, new_line::none>(params, out, ']');
            } else {
                append<Nested, new_line::before_data>(params, out, ']');
            }
        } else {
            append<0, new_line::after_data>(params, out, '[');
            for (const auto& el : data) {
                if (!delimiter.empty()) {
                    append<0, new_line::after_data>(params, out, delimiter);
                }
                append<Nested + 1, new_line::none>(params, out);
                ostream_impl<Nested + 1>(params, out, detail::quoted_helper(el));
                delimiter = ", ";
            }
            append<Nested, new_line::before_data>(params, out, ']');
        }
    }

    template <std::size_t Nested, class Stream, class T>
    Stream& ostream::ostream_impl(const options& params, Stream& out, const T& data) {
        if constexpr (has_stream_to<Stream, T>::value) {
            return stream_to(data, out, tag);
        } else if constexpr (detail::is_iterable_v<T> && !detail::is_c_string_v<T> &&
                             ((!detail::has_ostream_operator_v<Stream, T>) || std::is_array_v<T>)) {
            if constexpr (is_map_v<T>) {
                print_map<Nested>(params, out, data);
            } else {
                print_array<Nested>(params, out, data);
            }
        } else if constexpr (detail::has_ostream_operator_v<Stream, T>) {
            append<0, new_line::none>(params, out, detail::quoted_helper(data));
        } else if constexpr (std::is_enum_v<T>) {
            append<0, new_line::none>(params, out, static_cast<std::underlying_type_t<T>>(data));
        } else {
            static_assert(detail::has_ostream_operator_v<Stream, T> && !std::is_enum_v<T>,
                          "not support [ostream& operator<<(ostream& out, const T& data)]");
        }

        return out;
    }

    template <std::size_t Nested, class Stream, typename T, typename V, typename>
    Stream& ostream::ostream_impl(const options& params, Stream& out, const std::pair<T, V>& data) {
        if constexpr (detail::has_ostream_operator_v<Stream, std::pair<T, V>>) {
            append<Nested, new_line::none>(out, data);
        } else {
            ///*if (!!Nested) */ out << '{';
            ostream_impl<Nested + 1>(params, out, detail::quoted_helper(data.first));
            append<0, new_line::none>(params, out, ": ");
            ostream_impl<Nested + 1>(params, out, detail::quoted_helper(data.second));
            ///*if (!!Nested)*/ out << '}';
        }
        return out;
    }

    template <std::size_t Nested, class Stream, typename... Args, typename>
    Stream& ostream::ostream_impl(const options& params, Stream& out, const std::tuple<Args...>& data) {
        append<0, new_line::none>(params, out, "(");
        detail::print_tuple_impl<Nested>(params, out, data, std::index_sequence_for<Args...>{});
        append<0, new_line::none>(params, out, ")");
        return out;
    }

#if __has_include(<optional>)
    template <std::size_t Nested, class Stream, typename T, typename>
    Stream& ostream::ostream_impl(const options& params, Stream& out, const std::optional<T>& data) {
        if (data) {
            ostream_impl<Nested>(params, out, detail::quoted_helper(data.value()));
        } else {
            append<0, new_line::none>(params, out, "null");
        }
        return out;
    }
#endif

#if __has_include(<variant>)
    template <std::size_t Nested, class Stream, typename T, typename... Ts>
    Stream& ostream::ostream_impl(const options& params, Stream& out, const std::variant<T, Ts...>& data) {
        if (data.index() != std::variant_npos) {
            std::visit([&out, &params](const auto& t) { ostream_impl<Nested>(params, out, t); }, data);
            return out;
        }
        append<0, new_line::none>(params, out, "VARIANT_NPOS");
        return out;
    }
#endif


    /// static assert test
    static_assert(is_same_any_of_v<char, int, char, double>, "test failed");

#if __has_include(<variant>)
    static_assert(!is_iterable_v<std::variant<int, int>>, "test failed");
#endif
    static_assert(is_iterable_v<std::string>, "test failed");

    static_assert(!has_ostream_operator_v<std::ostream, std::pair<int, int>>, "test failed");
    static_assert(has_ostream_operator_v<std::ostream, std::string>, "test failed");

    static_assert(is_char_type_v<char>, "test failed");
    static_assert(is_char_type_v<signed char>, "test failed");
    static_assert(is_char_type_v<char>, "test failed");
    static_assert(is_char_type_v<char16_t>, "test failed");
    static_assert(is_char_type_v<char>, "test failed");
    static_assert(is_char_type_v<char32_t>, "test failed");
    static_assert(is_char_type_v<wchar_t>, "test failed");
    static_assert(!is_char_type_v<uint16_t>, "test failed");
    static_assert(!is_char_type_v<uint32_t>, "test failed");

}  // namespace pretty::detail
