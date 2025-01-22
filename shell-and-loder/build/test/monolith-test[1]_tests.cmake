add_test([=[HelloWorld.ExactMatch]=]  /workspaces/cpp-monolith-template/build/test/monolith-test [==[--gtest_filter=HelloWorld.ExactMatch]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[HelloWorld.ExactMatch]=]  PROPERTIES WORKING_DIRECTORY /workspaces/cpp-monolith-template/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  monolith-test_TESTS HelloWorld.ExactMatch)
