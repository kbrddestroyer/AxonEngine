project ("AxonEngine.tests.performance")

set (LIST_LIBRARIES networking)
list_all_tests("perf_*.cpp" ${LIST_LIBRARIES})
include_directories("../../libraries")
