project ("AxonEngine.tests.performance")

set (LIST_LIBRARIES networking testutils)
list_all_tests_separate("perf_*.cpp" ${LIST_LIBRARIES})
include_directories("../../libraries")
include_directories("../utils/testutils")

