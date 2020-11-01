project('labyrinth', 'cpp')
compiler = meson.get_compiler('cpp')
libGLU = compiler.find_library('GLU')
executable('labyrinth',
           'camera.cpp',
           'maze.cpp',
           'main.cpp',
           dependencies: [
            dependency('glut'),
            libGLU,
            dependency('OpenGL')
           ]
)
