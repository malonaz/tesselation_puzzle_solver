include ./dep/googletest/make/Makefile

TESTS = $(TESTDIR)/common/shape_matrix_test.o \
	$(TESTDIR)/discretizer/shape_translate_test.o

test: $(BINDIR)/$(TARGET) $(BINDIR)/$(TESTTARGET)

$(TESTS) : %.o: %.cc
	$(CXX) $(CPPFLAGS)\
		$(TESTINCLUDES)\
		$(CXXFLAGS) -c $< -o $@

$(BINDIR)/$(TESTTARGET): $(TESTS) gtest_main.a
	$(CXX) $(CPPFLAGS)\
		$(TESTINCLUDES)\
		$(CXXFLAGS) -fprofile-arcs $(OBJECTS) $^ -o $@ -lpthread

coverage: test
	./bin/test
	@mkdir -p $(COVDIR)
	@find $(SRCDIR) -name '*.cc' -exec cp {} $(COVDIR) \;
	@find $(OBJDIR) -name '*.gcno' -exec cp {} $(COVDIR) \;
	@find $(OBJDIR) -name '*.gcda' -exec cp {} $(COVDIR) \;
	@find $(COVDIR) -name '*.cc' -exec gcov -bf {} \;
	lcov --no-external -b $(SRCDIR) -o $(COVDIR)/.info -c -q -d $(COVDIR)
	genhtml -o $(COVDIR)/html $(COVDIR)/.info
