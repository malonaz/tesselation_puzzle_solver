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
		$(CXXFLAGS) $(OBJECTS) $^ -o $@ -lpthread
