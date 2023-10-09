project(kalcy-repl)

add_executable(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE
  kalcy::kalcy
  kalcy::kalcy-compile-options
)

target_sources(${PROJECT_NAME} PRIVATE
  repl.cpp
)
