hello_world_src  = [ 'main.c' ]
hello_world_sdl  = executable('hello_world_sdl', hello_world_src,
                     dependencies : sdl_deps,
                     link_args : sdl_args,
                     install : false)

if not meson.is_cross_build()
hello_world_term  = executable('hello_world_term', hello_world_src,
                     dependencies : term_deps,
                     link_args : term_args,
                     install : false)
endif

run_command('ctags', '-R', '.')
