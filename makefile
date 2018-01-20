.PHONY: clean check coverage deps test
OBJDIR = obj
SRCDIR = src
BINDIR = bin
TARGET = aps
TESTTARGET = test
TESTDIR = test
COVDIR = coverage
MAIN_OBJECT = $(SRCDIR)/main.o
COMMON_OBJECTS = $(SRCDIR)/common/shape_matrix_io.o \
	$(SRCDIR)/common/puzzle_board.o \
	$(SRCDIR)/common/shape_matrix.o

DISCRETIZER_OBJECTS = $(SRCDIR)/discretizer/shape_translate.o

PUZZLE_OBJECTS = $(SRCDIR)/solver/imageProcessor.o \
	$(SRCDIR)/solver/solutionProcessor.o

OBJECTS = $(COMMON_OBJECTS) $(DISCRETIZER_OBJECTS) $(PUZZLE_OBJECTS)

CXX = g++
CXXFLAGS = -Weffc++ -g -MMD -std=c++11 --coverage -I$(SRCDIR)/

$(BINDIR)/$(TARGET): $(MAIN_OBJECT) $(OBJECTS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@

-include $(OBJECTS:.o=.d)

include $(TESTDIR)/makefile

deps:
	cd dep/googletest/make && make

check:
	cppcheck --enable=all --check-config --suppress=missingIncludeSystem src

# TODO: Set up tests and ensure tests are run for code coverage

coverage: $(BINDIR)/$(TARGET)
	@mkdir -p $(COVDIR)
	@find $(SRCDIR) -name '*.cc' -exec cp {} $(COVDIR) \;
	@find $(SRCDIR) -name '*.gcno' -exec cp {} $(COVDIR) \;
	@find $(SRCDIR) -name '*.gcda' -exec cp {} $(COVDIR) \;
	@cd $(COVDIR) && find . -name '*.cc' -exec gcov -bf {} \;
	@rm $(COVDIR)/*.cc
	lcov -t "result" -o $(COVDIR)/.info -c -d .
	genhtml -o coverage/html $(COVDIR)/.info

clean: test-clean
	rm -rf $(BINDIR) $(OBJECTS) $(OBJECTS:.o=.d) $(COVDIR) *.o *.d *.gcno *.gcda
