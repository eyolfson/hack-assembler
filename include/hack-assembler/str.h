#pragma once

#include "hack-assembler/char.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

struct str {
	const char *data;
	size_t size;
};

__attribute__((always_inline))
inline struct str str_from_c_str(const char *s)
{
	struct str str = { .data = s, .size = strlen(s) };
	return str;
}

__attribute__((always_inline))
inline bool str_equal(struct str s1, struct str s2)
{
	return s1.size == s2.size && memcmp(s1.data, s2.data, s2.size) == 0;
}

__attribute__((always_inline))
inline const char *str_end(struct str str)
{
	return str.data + str.size;
}

__attribute__((always_inline))
inline bool str_is_null(struct str str)
{
	return str.data == NULL;
}

__attribute__((always_inline))
inline bool str_starts_with_digit(struct str str)
{
	if (str.size > 0 && char_is_digit(str.data[0])) {
		return true;
	}
	else {
		return false;
	}
}

__attribute__((always_inline))
inline uint16_t str_to_uint15_t(struct str str)
{
	uint16_t result = 0;
	uint16_t factor = 1;
	assert(str.size > 0);
	size_t i = str.size;

	/* Horrible hacky overflow check */
	assert(str.size <= 5);
	if (str.size == 5) {
		assert(str.data[0] <= '3');
		if (str.data[0] == '3') {
			assert(str.data[1] <= '2');
			if (str.data[1] == '2') {
				assert(str.data[2] <= '7');
				if (str.data[2] == '7') {
					assert(str.data[3] <= '6');
					if (str.data[3] == '6') {
						assert(str.data[4] <= '7');
					}
				}
			}
		}
	}

	while (true) {
		--i;

		char c = str.data[i];
		assert(char_is_digit(c));
		result += (c - '0') * factor;
		factor *= 10;

		if (i == 0) {
			break;
		}
	}
	return result;
}
