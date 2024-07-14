project(
  'genesis',
  'c',
  default_options: [
    'optimization=2'
  ]
)

incdir = include_directories('include')

linker_script = files('include/linker.lds')[0].full_path()

md_c_args = ['-mcpu=68000', '-Wno-main']

md_asm_args = md_c_args + ['-Xassembler', '--bitwise-or']

md_link_args = [
  '-nostdlib',
  '-Wl,-defsym',
  '__checksum=0xbabe',
  '-Wl,-T',
  linker_script
]


subdir('newgame')
