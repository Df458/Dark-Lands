ex_src  = [ 'main.c', 'animation.c' ]
ex_sdl  = executable('example_sdl', ex_src,
                     dependencies : sdl_deps,
                     link_args : sdl_args,
                     install : false)

if not meson.is_cross_build()
ex_term  = executable('example_term', ex_src,
                     dependencies : term_deps,
                     link_args : term_args,
                     install : false)
endif

run_command('ctags', '-R', '.')
