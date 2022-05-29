cmake_minimum_required(VERSION 3.20.1)
project(SmartPointer)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(smartptr_test tests/smartPtr_test.cpp)
target_link_libraries(smartptr_test
        PRIVATE
        gtest
        gtest_main
        )
target_include_directories(smartptr_test
PRIVATE "."
)

include(GoogleTest)
gtest_discover_tests(smartptr_test)
