#pragma once
#include <sstream>      // std::stringstream
#include <string>       // std::string
#include <type_traits>  // std::is_same_v
#include <typeinfo>     // std::type_info::name

#include "internal/detail_pretty_print.hpp"

namespace pretty {

    /** pretty data print
     * @param out Stream
     * @param data data
     * @return Stream */
    template <class Stream, class T>
    constexpr Stream& print(Stream& out, const T& data) {
        detail::ostream::ostream_impl<0>(out, data);
        return out;
    }

    /** pretty data print
     * @param data data
     * @return std::string */
    template <class T>
    std::string print(const T& data) {
        std::stringstream out;
        print(out, data);
        return out.str();
    }

    /** pretty data print with type inforamation at the beginning
     * @param out Stream
     * @param data data
     * @return Stream */
    template <class Stream, class T>
    constexpr Stream& print_ti(Stream& out, const T& data) {
        out << typeid(T).name() << "@";
        print(out, data);
        return out;
    }

    /** pretty data print with type inforamation at the beginning
     * @param data data
     * @return std::string */
    template <class T>
    std::string print_ti(const T& data) {
        return std::string(typeid(T).name()).append("@").append(print(data));
    }

    /** pretty data print
     * @param out Stream
     * @param args variadic data
     * @return Stream */
    template <class Stream, class... Args, typename = std::enable_if_t<(sizeof...(Args) > 1)>>
    constexpr Stream& print_args(Stream& out, Args&&... args) {
        ((print(out, std::forward<Args>(args)) << ' '), ...);
        return out;
    }

    /** pretty data print
     * @param args variadic data
     * @return std::string */
    template <class... Args, typename = std::enable_if_t<(sizeof...(Args) > 1)>>
    std::string print_args(Args&&... args) {
        std::string result;
        ((result.append(print(std::forward<Args>(args))).append(" ")), ...);
        return result;
    }

    /** pretty data print line
     * @param out Stream
     * @param args variadic data
     * @return Stream */
    template <class Stream, class... Args, typename = std::enable_if_t<(sizeof...(Args) > 1)>>
    void print_line(Stream& out, Args&&... args) {
        ((print(out, std::forward<Args>(args)) << ' '), ...);
        out << '\n';
    }

}  // namespace pretty
