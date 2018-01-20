include ./dep/googletest/make/Makefile

TESTS = $(TESTDIR)/common/shape_matrix_test.o

test: $(BINDIR)/$(TARGET) $(BINDIR)/$(TESTTARGET)

$(BINDIR)/$(TESTTARGET): $(TESTS) gtest_main.a
	$(CXX) -isystem $(GTEST_DIR)/include $(CXXFLAGS) -I$(SRCDIR)/ $(OBJECTS) $^ -o $@ -lpthread
