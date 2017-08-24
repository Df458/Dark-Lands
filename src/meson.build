main_src  = [ 'actor.c', 'inventory.c', 'item.c', 'log.c', 'main.c', 'map.c', 'point.c' ]
main_sdl  = executable('game_sdl', main_src,
                     dependencies : sdl_deps,
                     include_directories : include_directories('/usr/include/xcurses'),
                     install : false)

if not meson.is_cross_build()
main_term  = executable('game_term', main_src,
                     dependencies : term_deps,
                     c_args : term_args,
                     install : false)
endif

run_command('ctags', '-R', '.')
