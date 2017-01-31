project('darklands', 'c',
        version : '0.0.1',
        default_options : ['c_std=c11'])

compiler = meson.get_compiler('c')

common_deps = [ compiler.find_library('m', required : false) ]
sdl_deps  = common_deps + [ compiler.find_library('pdcurses'), dependency('sdl') ]
term_deps = common_deps + [ dependency('ncurses') ]

common_args = []
sdl_args = common_args + []
term_args = common_args + [ '-D', 'TERMBUILD' ]

subdir('examples/hello_world')
subdir('examples/demo_app')
