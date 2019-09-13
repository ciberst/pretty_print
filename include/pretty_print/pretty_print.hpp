#pragma once
#include <sstream>		// std::stringstream
#include <type_traits>	// std::is_same_v

#include "internal/detail_pretty_print.hpp"

namespace pretty {

    // pretty print
    template <class Stream, typename T, size_t N>
    constexpr Stream& pretty_print(Stream& out, const T (&data)[N]) {
        if constexpr (!(std::is_same_v<T, char> || std::is_same_v<T, unsigned char>))
            detail::print_array(out, data);
        else
            detail::ostream::ostream_impl(out, data);
        return out;
    }

    // pretty print
    template <class Stream, class T>
    Stream& pretty_print(Stream& out, const T& data) {
        detail::ostream::ostream_impl(out, data);
        return out;
    }

    // pretty print
    template <class T>
    std::string pretty_print(const T& data) {
        std::stringstream out;
        pretty_print(out, data);
        return out.str();
    }

}  // namespace pretty