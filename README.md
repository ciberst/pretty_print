[![Build Status](https://travis-ci.org/ciberst/pretty_print.svg?branch=master)](https://travis-ci.org/ciberst/pretty_print)
[![Build status](https://ci.appveyor.com/api/projects/status/ognl9dahv7vkhwnf?svg=true)](https://ci.appveyor.com/project/ciberst/pretty-print)
# pretty_print
pretty_print is a cross-platform library for a pretty print of various data.

pretty_print - это кроссплатформенная библиотека для красивой печати различных данных.
Вам не нужно больше писать циклы, чтоб распечатать тот или иной std-контейнер, массив данных.  Вы с помощью данной библиотеки можете распечатать std::tuple, std::variant. Смешанные типы тоже поддерживаются!

## Example

```cpp
#include <iostream>
#include <pretty_print/pretty_print.hpp>
#include <vector>
int main() {
    std::vector<int> a = {1, 2, 3, 4};
    pretty::print(std::cout, a) << std::endl;
    return 0;
}
```

## Output

```
[1, 2, 3, 4]
```

## Требования к компилятору
* Поддержка C++17


## Обзор основных возможностей библиотеки

### std::vector
```cpp
std::vector<int> a = {1, 2, 3, 4};
pretty::print(std::cout, a) << std::endl;
```
Output
```
[1, 2, 3, 4]
```
### std::map

```cpp
std::map<int, int> map = {{1, 2}, {2, 3}, {3, 4}};
pretty::print(std::cout, map) << std::endl;
```
Output
```
{1: 2, 2: 3, 3: 4}
```
### std::pair

```cpp
using namespace std::string_literals;
auto pair = std::make_pair("123"s, 12);
pretty::print(std::cout, pair) << std::endl;
```
Output
```
"123": 12
```

### std::optional
```cpp
std::optional<std::string> opt{"string"s};
pretty::print(std::cout, opt) << std::endl;
```
Output
```
"string"
```

### std::variant
```cpp
std::variant<int, std::string> variant;
variant = "123";
pretty::print(std::cout, variant) << std::endl;
```
Output
```
"123"
```

### std::tuple
```cpp
auto tuple = std::make_tuple("1", 2, 3, 4.5);
pretty::print(std::cout, tuple) << std::endl;
```
Output
```
("1", 2, 3, 4.5)
```
### C-array and C-string
```cpp
int c_arr[] = {1, 2, 3, 4, 5, 6};
int data2[2][2][2] = {{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}};
pretty::print(std::cout, c_arr) << std::endl;
pretty::print(std::cout, "hello") << std::endl;
pretty::print(std::cout, data2) << std::endl;
```
Output
```
[1, 2, 3, 4, 5, 6]
"hello"
[[[1, 2], [3, 4]], [[5, 6], [7, 8]]]
```

### Enum

```cpp
enum class color { red, green, blue };
color data = color::green;
pretty::print(std::cout, data) << std::endl;
```
Output
```
1
```

### hardcore example :-)
```cpp
using namespace std::string_literals;
std::unordered_map<std::string, std::map<std::string, std::optional<int>>> mapmap = {
        {"test"s, {{"1"s, 2}, {"2"s, 3}, {"3"s, 4}}},
        {"hello"s, {{"1"s, 2}, {"2"s, 3}, {"3"s, 4}}},
        {"world"s, {{"1"s, 2}, {"2"s, 3}, {"3"s, {}}}}};
pretty::print(std::cout, mapmap) << std::endl;
```
Output
```
{"test": {"1": 2, "2": 3, "3": 4}, "world": {"1": 2, "2": 3, "3": null}, "hello": {"1": 2, "2": 3, "3": 4}}
```
