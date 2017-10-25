#include "hack-assembler/code.h"

#include <assert.h>

int main()
{
	assert(code_jump(str_from_c_str("JMP")) == 0b111);

	return 0;
}
