.PHONY: clean check
CXX = g++
CXXFLAGS = -Weffc++ -g -MMD -std=c++11
OBJDIR = obj
SRCDIR = src
BINDIR = bin
TARGET = aps
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

clean:
	rm -rf $(BINDIR) $(OBJECTS) $(OBJECTS:.o=.d) *.o *.d
