#pragma once
#include <sstream>  // std::stringstream
#include <string_view>
#include <type_traits>  // std::is_same_v
#include <typeinfo>

#include "internal/detail_pretty_print.hpp"

namespace pretty {

    /** pretty C-array print
     * @param out Stream
     * @param data C-array
     * @return Stream */
    template <class Stream, typename T, size_t N>
    constexpr Stream& pretty_print(Stream& out, const T (&data)[N]) {
        if constexpr (!(std::is_same_v<T, char> || std::is_same_v<T, unsigned char>))
            detail::print_array(out, data);
        else
            detail::ostream::ostream_impl<0>(out, data);
        return out;
    }

    /** pretty data print
     * @param out Stream
     * @param data data
     * @return Stream */
    template <class Stream, class T>
    constexpr Stream& pretty_print(Stream& out, const T& data) {
        detail::ostream::ostream_impl<0>(out, data);
        return out;
    }

    /** pretty data print
     * @param data data
     * @return std::string */
    template <class T>
    std::string pretty_print(const T& data) {
        std::stringstream out;
        pretty_print(out, data);
        return out.str();
    }

    /** pretty C-array print with type inforamation at the beginning
     * @param out Stream
     * @param data C-array
     * @return Stream */
    template <class Stream, typename T, size_t N>
    constexpr Stream& pretty_print_ti(Stream& out, const T (&data)[N]) {
        out << typeid(T).name() << "[" << N << "]@";
        pretty_print(out, data);
        return out;
    }

    /** pretty data print with type inforamation at the beginning
     * @param out Stream
     * @param data data
     * @return Stream */
    template <class Stream, class T>
    constexpr Stream& pretty_print_ti(Stream& out, const T& data) {
        out << typeid(T).name() << "@";
        pretty_print(out, data);
        return out;
    }

    /** pretty data print with type inforamation at the beginning
     * @param data data
     * @return std::string */
    template <class T>
    std::string pretty_print_ti(const T& data) {
        return std::string(typeid(T).name()).append("@").append(pretty_print(data));
    }

    /** pretty data print
     * @param out Stream
     * @param args variadic data
     * @return Stream */
    template <class Stream, class... Args, typename = std::enable_if_t<(sizeof...(Args) > 1)>>
    constexpr Stream& pretty_print_args(Stream& out, Args&&... args) {
        ((pretty_print(out, std::forward<Args>(args)) << ' '), ...);
        return out;
    }


    /** pretty data print
     * @param args variadic data
     * @return std::string */
    template <class... Args, typename = std::enable_if_t<(sizeof...(Args) > 1)>>
    std::string pretty_print_args(Args&&... args) {
        std::string result;
        ((result.append(pretty_print(std::forward<Args>(args))).append(" ")), ...);
        return result;
    }

}  // namespace pretty
