game_sources = files(
  'main.c'
)

executable(
  'game.bin',
  'start.S',
  game_sources,
  c_args: md_c_args,
  link_args: md_link_args,
  include_directories: incdir,
)
