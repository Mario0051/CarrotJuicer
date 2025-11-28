#pragma once
#include <string>
#include <cstring>

template <typename CharT, size_t N>
struct SplitString {
    static constexpr size_t SplitIndex = (N * 3) / 4;

    CharT part1[SplitIndex + 1]{};
    CharT part2[N - SplitIndex + 1]{};

    constexpr SplitString(const CharT(&str)[N]) {
        for (size_t i = 0; i < SplitIndex; ++i)
            part1[i] = str[i];
        part1[SplitIndex] = '\0';

        for (size_t i = SplitIndex; i < N; ++i)
            part2[i - SplitIndex] = str[i];
        part2[N - SplitIndex] = '\0';
    }
};

template <typename CharT, size_t N>
struct StackString {
    CharT buffer[N];

    template <size_t N1, size_t N2>
    __forceinline StackString(const CharT(&part1)[N1], const CharT(&part2)[N2]) {
        constexpr size_t offset = N1 - 1;

        std::memcpy(buffer, part1, offset * sizeof(CharT));

        std::memcpy(buffer + offset, part2, (N - offset) * sizeof(CharT));
    }

    operator const CharT*() const { return buffer; }
    const CharT* c_str() const { return buffer; }
    operator std::basic_string<CharT>() const { return std::basic_string<CharT>(buffer, N - 1); }
};

#define BUILD_PATH(s) ([]() { \
    constexpr size_t n = sizeof(s) / sizeof(char); \
    constexpr auto split = SplitString<char, n>(s); \
    return StackString<char, n>(split.part1, split.part2); \
}())

#define BUILD_PATH_W(s) ([]() { \
    constexpr size_t n = sizeof(s) / sizeof(wchar_t); \
    constexpr auto split = SplitString<wchar_t, n>(s); \
    return StackString<wchar_t, n>(split.part1, split.part2); \
}())
