project ("AxonEngine.tests.gtest")

# TEST FILES
find_package(GTest REQUIRED)
enable_testing()

set (LIST_LIBRARIES GTest::gtest GTest::gtest_main networking)
list_all_tests("test_*.cpp" ${LIST_LIBRARIES})
include_directories("../../libraries")
include_directories("../utils/testutils")