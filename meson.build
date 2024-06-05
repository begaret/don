project('don', 'cpp', default_options: 'cpp_std=c++17')

include = include_directories('src/')
src = files([
  'src/error.cpp',
  'src/raw.cpp',
  'src/don.cpp',
  'src/parse.cpp'
])

executable('don-test', [src, 'src/test/main.cpp'], include_directories: include)

