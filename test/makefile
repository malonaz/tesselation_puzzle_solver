include ./dep/googletest/make/Makefile

TESTS = $(TESTDIR)/common/shape_matrix_test.o

test: $(BINDIR)/$(TARGET) $(BINDIR)/$(TESTTARGET)

$(TESTS) : %.o: %.cc
	$(CXX) $(CPPFLAGS)\
		$(TESTINCLUDES)\
		$(CXXFLAGS) -c $< -o $@

$(BINDIR)/$(TESTTARGET): $(TESTS) gtest_main.a
	$(CXX) $(CPPFLAGS)\
		$(TESTINCLUDES)\
		$(CXXFLAGS) $(OBJECTS) $^ -o $@ -lpthread -lgcov

coverage: test
	./bin/test
	@mkdir -p $(COVDIR)
	@find $(SRCDIR) -name '*.cc' -exec cp {} $(COVDIR) \;
	@find $(OBJDIR) -name '*.gcno' -exec cp {} $(COVDIR) \;
	@find $(OBJDIR) -name '*.gcda' -exec cp {} $(COVDIR) \;
	@cd $(COVDIR) && find . -name '*.cc' -exec gcov -bf {} \;
	lcov -o $(COVDIR)/.info -c -d $(COVDIR)
	genhtml -o $(COVDIR)/html $(COVDIR)/.info
