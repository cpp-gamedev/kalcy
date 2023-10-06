add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE
  kalcy::kalcy-compile-options
)

target_include_directories(${PROJECT_NAME} PUBLIC
  include
  "${CMAKE_CURRENT_BINARY_DIR}/include"
)

target_compile_features(${PROJECT_NAME} PUBLIC
  cxx_std_20
)

configure_file(src/build_version.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/include/kalcy/build_version.hpp" @ONLY)

target_sources(${PROJECT_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS include FILES
  include/kalcy/error.hpp
  include/kalcy/enum_array.hpp
  include/kalcy/env.hpp
  include/kalcy/expr.hpp
  include/kalcy/eval.hpp
  include/kalcy/kalcy.hpp
  include/kalcy/parser.hpp
  include/kalcy/scanner.hpp
  include/kalcy/token.hpp
)

target_sources(${PROJECT_NAME} PRIVATE
  src/env.cpp
  src/error.cpp
  src/eval.cpp
  src/expr.cpp
  src/parser.cpp
  src/scanner.cpp
)
