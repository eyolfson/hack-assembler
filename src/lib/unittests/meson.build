code = executable('code', 'code.c', include_directories : incdir, link_with : lib)
test('Code', code)

parser = executable('parser', 'parser.c', include_directories : incdir, link_with : lib)
test('Parser', parser)

symbol_table = executable('symbol_table', 'symbol_table.c', include_directories : incdir, link_with : lib)
test('Symbol table', symbol_table)
