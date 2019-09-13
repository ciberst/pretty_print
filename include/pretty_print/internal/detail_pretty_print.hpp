#pragma once
#include <iomanip>	// std::quoted
#include <type_traits>
#include <variant>

namespace pretty::detail {

    template <typename T, typename = void>
    struct is_iterable : std::false_type {};
    template <typename T>
    struct is_iterable<T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>>
        : std::true_type {};

    template <typename, typename = void>
    struct has_ostream_operator : std::false_type {};

    template <typename T>
    struct has_ostream_operator<T, decltype(void(std::declval<std::ostream&>() << std::declval<const T&>()))>
        : std::true_type {};

    static_assert(!is_iterable<std::pair<int, int>>::value, "test failed");
    static_assert(is_iterable<std::string>::value, "test failed");
    static_assert(!has_ostream_operator<std::pair<int, int>>::value, "test failed");
    static_assert(has_ostream_operator<std::string>::value, "test failed");

    template <typename T, size_t N,
              typename = std::enable_if_t<std::is_same_v<T, char> || std::is_same_v<T, unsigned char>>>
    auto quoted_helper(const T (&s)[N]) noexcept {
        return std::quoted(s);
    }

    inline auto quoted_helper(const char* c) noexcept { return std::quoted(c); }

    inline auto quoted_helper(const std::string& s) noexcept { return std::quoted(s); }

    template <class CharT, class Traits>
    auto quoted_helper(std::basic_string_view<CharT, Traits> s) noexcept {
        return std::quoted(s);
    }

    inline auto quoted_helper(std::string& s) noexcept { return std::quoted(s); }

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
        template <class Stream, class T>
        static Stream& ostream_impl(Stream& out, const T& data) {
            if constexpr (detail::is_iterable<T>::value && !detail::has_ostream_operator<T>::value) {
                std::string delimiter;
                out << "{";
                for (const auto& el : data) {
                    out << delimiter;
                    ostream_impl(out, detail::quoted_helper(el));
                    delimiter = ", ";
                }
                out << "}";
            } else if constexpr (detail::has_ostream_operator<T>::value) {
				out << detail::quoted_helper(data);
                return out;
			}
            else {
                static_assert(detail::has_ostream_operator<T>::value, "not support [ostream& operator<<(ostream& out, const T& data)]");
            }

            return out;
		}

        template <class Stream, typename T, typename V,
                  typename = std::enable_if_t<!detail::has_ostream_operator<std::pair<T, V>>::value>>
        static Stream& ostream_impl(Stream& out, const std::pair<T, V>& data) {
            if constexpr (detail::has_ostream_operator<std::pair<T, V>>::value)
                out << data;
            else {
                ostream_impl(out, detail::quoted_helper(data.first));
                out << ": ";
                ostream_impl(out, detail::quoted_helper(data.second));
            }
            return out;
		}

        template <class Stream, typename... Args,
                  typename = std::enable_if_t<!detail::has_ostream_operator<std::tuple<Args...>>::value>>
        static Stream& ostream_impl(Stream& out, const std::tuple<Args...>& data) {
            out << "(";
            detail::print_tuple_impl<0, std::tuple_size<std::tuple<Args...>>::value>::print(out, data);
            out << ")";
            return out;
        }

        template <class Stream, typename T,
                  typename = std::enable_if_t<!detail::has_ostream_operator<std::optional<T>>::value>>
        static Stream& ostream_impl(Stream& out, const std::optional<T>& data) {
            if (data) {
                ostream_impl(out, detail::quoted_helper(data.value()));
            } else {
                out << "NULL";
            }
            return out;
		}

        template <class Stream, typename T, typename... Ts>
        static Stream& ostream_impl(Stream& out, const std::variant<T, Ts...>& data) {
            if (data.index() != std::variant_npos) {
                std::visit([&out](const auto& t) { ostream_impl(out, t); }, data);
                return out;
            }
            out << "VARIANT_NPOS";
            return out;
        }
    };

    template <class Stream, typename T, size_t N>
    Stream& print_array(Stream& out, const T (&data)[N]) {
        std::string delimiter;
        out << "[";
        for (const auto& el : data) {
            out << delimiter;
            ostream::ostream_impl(out, detail::quoted_helper(el));
            delimiter = ", ";
        }
        out << "]";
        return out;
    }


}  // namespace pretty::detail
