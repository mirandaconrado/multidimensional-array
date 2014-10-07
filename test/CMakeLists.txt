add_executable(run_tests.bin EXCLUDE_FROM_ALL
  array.cpp
  const_array.cpp
  const_slice.cpp
  const_view.cpp
  slice.cpp
  size.cpp
  view.cpp
)

target_link_libraries(run_tests.bin gtest gtest_main
)

add_custom_target(test COMMAND run_tests.bin
                       DEPENDS run_tests.bin)
