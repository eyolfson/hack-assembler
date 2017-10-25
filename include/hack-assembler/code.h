#pragma once

#include "hack-assembler/str.h"

#include <stdint.h>

uint8_t code_dest(struct str mnemonic);
uint8_t code_comp(struct str mnemonic);
uint8_t code_jump(struct str mnemonic);
