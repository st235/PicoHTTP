#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

#include <cstdint>
#include <string>
#include <vector>

namespace http {

namespace __internal {

std::vector<std::string> Split(const std::string& text, const std::string& delimiter) {
    std::vector<std::string> split;

    size_t prev_position = 0;
    size_t position = text.find(delimiter, prev_position);
    while (position != std::string::npos) {
        size_t length = position - prev_position;
        std::string token = text.substr(prev_position, length);
        split.emplace_back(token);

        prev_position = position + delimiter.length();
        position = text.find(delimiter, prev_position);
    }

    split.emplace_back(text.substr(prev_position, text.length() - prev_position));
    return split;
}

std::string Trim(const std::string& origin) {
    int32_t start_position = 0;
    while (start_position < origin.length()) {
        if (origin[start_position] == ' ' || origin[start_position] == '\t' || origin[start_position] == '\n') {
            start_position++;
            continue;
        }

        break;
    }

    int32_t end_position = origin.length() - 1;
    while (end_position >= 0) {
        if (origin[end_position] == ' ' || origin[end_position] == '\t' || origin[end_position] == '\n') {
            end_position--;
            continue;
        }

        break;
    }

    if (end_position < start_position) {
        return std::string();
    }

    return origin.substr(start_position, (end_position - start_position + 1));
}

} // namespace __internal

} // namespace http

#endif // __STRING_UTILS_H__
