.PHONY: clean check coverage test main
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
	$(OBJDIR)/common/shape_matrix.o \
	$(OBJDIR)/common/point.o

DISCRETIZER_OBJECTS = $(OBJDIR)/discretizer/shape_translate.o \
			$(OBJDIR)/discretizer/shape_rotate.o

PUZZLE_SOLVING_OBJECTS = $(OBJDIR)/puzzle_solving/solver.o

OBJECTS = $(COMMON_OBJECTS)\
	$(DISCRETIZER_OBJECTS)\
	$(PUZZLE_OBJECTS)\
	$(PUZZLE_SOLVING_OBJECTS)

CXX = g++
CXXFLAGS = -Wall -g -MMD -std=c++11 -I$(SRCDIR)/

main: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(MAIN_OBJECT) $(OBJECTS)
	@mkdir -p $(OBJDIR) $(BINDIR)
	$(CXX) $(CXXFLAGS) -fprofile-arcs $^ -o $@

$(MAIN_OBJECT): $(OBJDIR)/%.o: $(SRCDIR)/%.cc
	@mkdir -p $(OBJDIR) $(BINDIR)
	$(CXX) $(CXXFLAGS) -Weffc++ --coverage -c $< -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cc
	@mkdir -p `dirname $@`
	$(CXX) $(CXXFLAGS) -Weffc++ --coverage -c $< -o $@

-include $(OBJECTS:.o=.d)

include $(TESTDIR)/makefile

check:
	cppcheck --enable=all --check-config --suppress=missingIncludeSystem -I src src

clean: test-clean
	rm -rf $(BINDIR) $(OBJDIR) $(COVDIR) *.o *.d *.gcno *.gcda
