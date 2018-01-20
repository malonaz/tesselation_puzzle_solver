.PHONY: clean check coverage test
OBJDIR = obj
SRCDIR = src
BINDIR = bin
TARGET = aps
TESTTARGET = test
TESTDIR = test
COVDIR = coverage

MAIN_OBJECT = $(OBJDIR)/main.o

COMMON_OBJECTS = $(OBJDIR)/common/shape_matrix_io.o \
	$(OBJDIR)/common/puzzle_board.o \
	$(OBJDIR)/common/shape_matrix.o

DISCRETIZER_OBJECTS = $(OBJDIR)/discretizer/shape_translate.o

PUZZLE_SOLVING_OBJECTS = $(OBJDIR)/puzzle_solving/solver.o

PUZZLE_OBJECTS = $(OBJDIR)/solver/imageProcessor.o \
	$(OBJDIR)/solver/solutionProcessor.o

OBJECTS = $(COMMON_OBJECTS)\
	$(DISCRETIZER_OBJECTS)\
	$(PUZZLE_OBJECTS)\
	$(PUZZLE_SOLVING_OBJECTS)

CXX = g++
CXXFLAGS = -Wall -g -MMD -std=c++11 -I$(SRCDIR)/

$(BINDIR)/$(TARGET): $(MAIN_OBJECT) $(OBJECTS)
	@mkdir -p bin obj
	$(CXX) $(CXXFLAGS) $^ -o $@ -lgcov

$(MAIN_OBJECT): $(OBJDIR)/%.o: $(SRCDIR)/%.cc
	$(CXX) $(CXXFLAGS) -Weffc++ --coverage -c $< -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cc
	@mkdir -p `dirname $@`
	$(CXX) $(CXXFLAGS) -Weffc++ --coverage -c $< -o $@

-include $(OBJECTS:.o=.d)

include $(TESTDIR)/makefile

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
	rm -rf $(BINDIR) $(OBJDIR) $(COVDIR) *.o *.d *.gcno *.gcda
