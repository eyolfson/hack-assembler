#pragma once

#include <stdbool.h>

__attribute__((always_inline))
inline bool char_is_digit(char c)
{
	return c >= '0' && c <= '9';
}
