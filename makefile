.PHONY: clean check coverage
CXX = g++
CXXFLAGS = -Weffc++ -g -MMD -std=c++11 -fprofile-arcs -ftest-coverage
OBJDIR = obj
SRCDIR = src
BINDIR = bin
TARGET = aps
COVDIR = coverage
MAIN_OBJECT = $(SRCDIR)/main.o
COMMON_OBJECTS = $(SRCDIR)/common/shape_matrix_io.o \
	$(SRCDIR)/common/puzzle_board.o \
	$(SRCDIR)/common/shape_matrix.o

DISCRETIZER_OBJECTS = $(SRCDIR)/discretizer/shape_translate.o

PUZZLE_OBJECTS = $(SRCDIR)/solver/imageProcessor.o \
	$(SRCDIR)/solver/solver.o \
	$(SRCDIR)/solver/solutionProcessor.o

OBJECTS = $(MAIN_OBJECT) $(COMMON_OBJECTS) $(DISCRETIZER_OBJECTS) $(PUZZLE_OBJECTS)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@

-include $(OBJECTS:.o=.d)

check:
	cppcheck --enable=all --check-config --suppress=missingIncludeSystem src
	
coverage:
	@mkdir -p $(COVDIR)
	@find $(SRCDIR) -name '*.cc' -exec cp {} $(COVDIR) \;
	@find $(SRCDIR) -name '*.gcno' -exec cp {} $(COVDIR) \;
	@find $(SRCDIR) -name '*.gcda' -exec cp {} $(COVDIR) \;
	@cd $(COVDIR) && find . -name '*.cc' -exec gcov -bf {} \;
	@rm $(COVDIR)/*.cc

clean:
	rm -rf $(BINDIR) $(OBJECTS) $(OBJECTS:.o=.d) $(COVDIR) *.o *.d
