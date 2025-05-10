project ("AxonEngine.tests.basic_checks")
include ("CTest")

# TEST FILES
enable_testing()

list_all_tests("test_*.cxx" networking)

include_directories("../../libraries")
