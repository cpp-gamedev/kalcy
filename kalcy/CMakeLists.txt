add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE
  kalcy::kalcy-compile-options
)

target_include_directories(${PROJECT_NAME} PUBLIC
  include
)

target_include_directories(${PROJECT_NAME} PRIVATE
  src
)

target_compile_features(${PROJECT_NAME} PUBLIC
  cxx_std_20
)

target_sources(${PROJECT_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS include FILES
  include/kalcy/kalcy.hpp
)

target_sources(${PROJECT_NAME} PRIVATE
  src/impl/enum_array.hpp
  src/impl/scanner.cpp
  src/impl/scanner.hpp
  src/impl/token.hpp

  src/kalcy.cpp
)
