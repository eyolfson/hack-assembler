#include "hack-assembler/code.h"

#include <assert.h>

uint8_t code_dest(struct str mnemonic)
{
	if (str_equal(str_from_c_str("M"), mnemonic)) {
		return 0b001;
	}
	else if (str_equal(str_from_c_str("D"), mnemonic)) {
		return 0b010;
	}
	else if (str_equal(str_from_c_str("MD"), mnemonic)) {
		return 0b011;
	}
	else if (str_equal(str_from_c_str("A"), mnemonic)) {
		return 0b100;
	}
	else if (str_equal(str_from_c_str("AM"), mnemonic)) {
		return 0b101;
	}
	else if (str_equal(str_from_c_str("AD"), mnemonic)) {
		return 0b110;
	}
	else if (str_equal(str_from_c_str("AMD"), mnemonic)) {
		return 0b111;
	}
	else {
		assert(false);
	}
}

uint8_t code_comp(struct str mnemonic)
{
	if (str_equal(str_from_c_str("0"), mnemonic)) {
		return 0b0101010;
	}
	else if (str_equal(str_from_c_str("1"), mnemonic)) {
		return 0b0111111;
	}
	else if (str_equal(str_from_c_str("-1"), mnemonic)) {
		return 0b0111010;
	}
	else if (str_equal(str_from_c_str("D"), mnemonic)) {
		return 0b0001100;
	}
	else if (str_equal(str_from_c_str("A"), mnemonic)) {
		return 0b0110000;
	}
	else if (str_equal(str_from_c_str("!D"), mnemonic)) {
		return 0b0001101;
	}
	else if (str_equal(str_from_c_str("!A"), mnemonic)) {
		return 0b0110001;
	}
	else if (str_equal(str_from_c_str("-D"), mnemonic)) {
		return 0b0001111;
	}
	else if (str_equal(str_from_c_str("-A"), mnemonic)) {
		return 0b0110011;
	}
	else if (str_equal(str_from_c_str("D+1"), mnemonic)) {
		return 0b0011111;
	}
	else if (str_equal(str_from_c_str("A+1"), mnemonic)) {
		return 0b0110111;
	}
	else if (str_equal(str_from_c_str("D-1"), mnemonic)) {
		return 0b0001110;
	}
	else if (str_equal(str_from_c_str("A-1"), mnemonic)) {
		return 0b0110010;
	}
	else if (str_equal(str_from_c_str("D+A"), mnemonic)) {
		return 0b0000010;
	}
	else if (str_equal(str_from_c_str("D-A"), mnemonic)) {
		return 0b0010011;
	}
	else if (str_equal(str_from_c_str("A-D"), mnemonic)) {
		return 0b0000111;
	}
	else if (str_equal(str_from_c_str("D&A"), mnemonic)) {
		return 0b0000000;
	}
	else if (str_equal(str_from_c_str("D|A"), mnemonic)) {
		return 0b0010101;
	}
	else if (str_equal(str_from_c_str("M"), mnemonic)) {
		return 0b1110000;
	}
	else if (str_equal(str_from_c_str("!M"), mnemonic)) {
		return 0b1110001;
	}
	else if (str_equal(str_from_c_str("-M"), mnemonic)) {
		return 0b1110011;
	}
	else if (str_equal(str_from_c_str("M+1"), mnemonic)) {
		return 0b1110111;
	}
	else if (str_equal(str_from_c_str("M-1"), mnemonic)) {
		return 0b1110010;
	}
	else if (str_equal(str_from_c_str("D+M"), mnemonic)) {
		return 0b1000010;
	}
	else if (str_equal(str_from_c_str("D-M"), mnemonic)) {
		return 0b1010011;
	}
	else if (str_equal(str_from_c_str("M-D"), mnemonic)) {
		return 0b1000111;
	}
	else if (str_equal(str_from_c_str("D&M"), mnemonic)) {
		return 0b1000000;
	}
	else if (str_equal(str_from_c_str("D|M"), mnemonic)) {
		return 0b1010101;
	}
	else {
		assert(false);
	}
}

uint8_t code_jump(struct str mnemonic)
{
	if (str_equal(str_from_c_str("JGT"), mnemonic)) {
		return 0b001;
	}
	else if (str_equal(str_from_c_str("JEQ"), mnemonic)) {
		return 0b010;
	}
	else if (str_equal(str_from_c_str("JGE"), mnemonic)) {
		return 0b011;
	}
	else if (str_equal(str_from_c_str("JLT"), mnemonic)) {
		return 0b100;
	}
	else if (str_equal(str_from_c_str("JNE"), mnemonic)) {
		return 0b101;
	}
	else if (str_equal(str_from_c_str("JLE"), mnemonic)) {
		return 0b110;
	}
	else if (str_equal(str_from_c_str("JMP"), mnemonic)) {
		return 0b111;
	}
	else {
		assert(false);
	}
}
