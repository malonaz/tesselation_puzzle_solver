include ./dep/googletest/make/Makefile

CPPFLAGS += -isystem $(SRCDIR)/

TESTS = $(TESTDIR)/common/shape_matrix_test.o

test: $(BINDIR)/$(TESTTARGET)

$(BINDIR)/$(TESTTARGET): $(TESTS) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@  -lpthread
