# Find Google Test package
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add test executable
add_executable(runTests HoleTest.cpp)

# Link against your library and Google Test
target_link_libraries(runTests 
    ${GTEST_LIBRARIES} 
    pthread  # Important for Google Test on Linux
    RoombaLibrary  # Your project's library
)

# Register the test with CTest
add_test(
    NAME testSuite
    COMMAND runTests
)
