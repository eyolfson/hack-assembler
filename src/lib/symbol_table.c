#include "hack-assembler/symbol_table.h"

#include <assert.h>
#include <stdlib.h>

#define SYMBOL_TABLE_CAPACITY 4096

struct entry {
	struct str key;
	uint16_t value;
};

struct symbol_table {
	struct entry entries[SYMBOL_TABLE_CAPACITY];
};

static uint16_t bernstein_hash(struct str s)
{
	uint16_t hash = 0;
	for (size_t i = 0; i < s.size; ++i) {
		hash = (33 * hash) + s.data[i];
	}
	return hash;
}

static struct entry *find_empty_entry(struct symbol_table *symbol_table,
                                      struct str key)
{
	assert(key.data != NULL);
	uint16_t index = bernstein_hash(key) % SYMBOL_TABLE_CAPACITY;
	uint16_t attempts_remaining = (SYMBOL_TABLE_CAPACITY - 1);
	while (true) {
		struct entry *entry = &symbol_table->entries[index];
		if (entry->key.data == NULL) {
			return entry;
		}
		assert(attempts_remaining != 0);
		++index;
		if (index == SYMBOL_TABLE_CAPACITY) {
			index = 0;
		}
		--attempts_remaining;
	}
}
	
static struct entry *find_existing_entry(struct symbol_table *symbol_table,
                                         struct str key)
{
	assert(key.data != NULL);
	uint16_t index = bernstein_hash(key) % SYMBOL_TABLE_CAPACITY;
	uint16_t attempts_remaining = (SYMBOL_TABLE_CAPACITY - 1);
	while (true) {
		struct entry *entry = &symbol_table->entries[index];
		if (entry->key.data == NULL) {
			return NULL;
		}
		else if (str_equal(entry->key, key)) {
			return entry;
		}

		if (attempts_remaining == 0) {
			return NULL;
		}
		++index;
		if (index == SYMBOL_TABLE_CAPACITY) {
			index = 0;
		}
		--attempts_remaining;
	}
}

struct symbol_table *symbol_table_create()
{
	struct symbol_table *symbol_table = calloc(1, sizeof(struct symbol_table));
	assert(symbol_table != NULL);
	return symbol_table;
}

void symbol_table_add_entry(struct symbol_table *symbol_table,
                            struct str symbol, uint16_t address)
{
	assert(!symbol_table_contains(symbol_table, symbol));
	struct entry *entry = find_empty_entry(symbol_table, symbol);
	entry->key = symbol;
	entry->value = address;
}

bool symbol_table_contains(struct symbol_table *symbol_table,
                           struct str symbol)
{
	struct entry *entry = find_existing_entry(symbol_table, symbol);
	return entry != NULL;
}

uint16_t symbol_table_get_address(struct symbol_table *symbol_table,
                                  struct str symbol)
{
	struct entry *entry = find_existing_entry(symbol_table, symbol);
	assert(entry != NULL);
	return entry->value;
}
