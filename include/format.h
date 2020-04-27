#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);  // TODO: See src/format.cpp


std::string to_string_with_precision(const float a_value, const int n = 6);
};                                    // namespace Format

#endif