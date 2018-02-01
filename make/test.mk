include ./dep/googletest/make/Makefile

TEST_OBJECTS = $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(shell find $(TESTDIR)/ -type f -name '*.cc'))

$(TESTTARGET): $(BINDIR)/$(TESTTARGET)

$(TESTS) : %.o: %.cc
	@echo "\tCompiling \"$@\""
	@$(CXX) $(CPPFLAGS)\
		$(TESTINCLUDES)\
		$(CXXFLAGS) -c $< -o $@
	@echo "[Done]\tCompiling \"$@\""

$(BINDIR)/$(TESTTARGET): $(COMMON_OBJECTS) $(DISCRETIZER_OBJECTS) $(SOLVER_OBJECTS) $(TEST_OBJECTS) gtest_main.a
	@echo "\tLinking \"$@\""
	@mkdir -p bin
	@$(CXX) $(CPPFLAGS)\
		$(TESTINCLUDES)\
		$(CXXFLAGS) -fprofile-arcs $^ -o $@ -lpthread $(OPENCV_LIBFLAGS)
	@echo "[Done]\tLinking \"$@\""

coverage: clean test
	./bin/test
	@mkdir -p $(COVDIR)
	@find $(SRCDIR) -name '*.cc' -exec cp {} $(COVDIR) \;
	@find $(OBJDIR) -name '*.gcno' -exec cp {} $(COVDIR) \;
	@find $(OBJDIR) -name '*.gcda' -exec cp {} $(COVDIR) \;
	@find $(COVDIR) -name '*.cc' -exec gcov -bf {} \;
	lcov --no-external -b $(SRCDIR) -o $(COVDIR)/.info -c -q -d $(COVDIR)
	genhtml -o $(COVDIR)/html $(COVDIR)/.info
