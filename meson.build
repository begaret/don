project('don', 'cpp', 
  default_options: [
    'cpp_std=c++17'
])

don_include = include_directories('include/')
test_include = include_directories('src/')
src = files([
  'src/error.cpp',
  'src/raw.cpp',
  'src/object.cpp',
  'src/don.cpp',
  'src/parse.cpp'
])

# define = '-DDON_USE_TSL_MAP'
define = '-DDON_USE_UNORDERED_MAP'

executable('don-test',
  [src, 'src/test/main.cpp'], 
  include_directories: [test_include, don_include],
  cpp_args: define 
)

static_library('don', src,
  include_directories: don_include,
  cpp_args: define
)

