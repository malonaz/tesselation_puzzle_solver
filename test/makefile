include ./dep/googletest/make/Makefile

TESTS = $(TESTDIR)/common/shape_matrix_test.o \
	$(TESTDIR)/discretizer/shape_translate_test.o \
	$(TESTDIR)/discretizer/shape_rotate_test.o

test: $(BINDIR)/$(TESTTARGET)

$(TESTS) : %.o: %.cc
	@echo "\tCompiling \"$@\""
	@$(CXX) $(CPPFLAGS)\
		$(TESTINCLUDES)\
		$(CXXFLAGS) -c $< -o $@
	@echo "[Done]\tCompiling \"$@\""

$(BINDIR)/$(TESTTARGET): $(OBJECTS) $(TESTS) gtest_main.a
	@echo "\tLinking \"$@\""
	@mkdir -p bin
	@$(CXX) $(CPPFLAGS)\
		$(TESTINCLUDES)\
		$(CXXFLAGS) -fprofile-arcs $^ -o $@ -lpthread
	@echo "[Done]\tLinking \"$@\""

coverage: test
	./bin/test
	@mkdir -p $(COVDIR)
	@find $(SRCDIR) -name '*.cc' -exec cp {} $(COVDIR) \;
	@find $(OBJDIR) -name '*.gcno' -exec cp {} $(COVDIR) \;
	@find $(OBJDIR) -name '*.gcda' -exec cp {} $(COVDIR) \;
	@find $(COVDIR) -name '*.cc' -exec gcov -bf {} \;
	lcov --no-external -b $(SRCDIR) -o $(COVDIR)/.info -c -q -d $(COVDIR)
	genhtml -o $(COVDIR)/html $(COVDIR)/.info
