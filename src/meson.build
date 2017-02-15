main_src  = [ 'actor.c', 'main.c', 'map.c' ]
main_sdl  = executable('game_sdl', main_src,
                     dependencies : sdl_deps,
                     link_args : sdl_args,
                     install : false)

if not meson.is_cross_build()
main_term  = executable('game_term', main_src,
                     dependencies : term_deps,
                     link_args : term_args,
                     install : false)
endif

run_command('ctags', '-R', '.')
