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
    pretty_print(std::cout, a) << std::endl;
    return 0;
}
```

## Output

```
{1, 2, 3, 4}
```
