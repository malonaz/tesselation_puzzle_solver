CXX = g++
CXXFLAGS = -Wall -g -MMD -std=c++11
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

$(OBJDIR)/%.o : $(SRCDIR)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

-include $(OBJECTS:.o=.d)

.PHONY: clean

clean:
	rm -rf $(BINDIR) $(OBJECTS) $(OBJECTS:.o=.d) *.o *.d
