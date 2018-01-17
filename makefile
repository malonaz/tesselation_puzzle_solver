CXX = g++
CXXFLAGS = -Wall -g -MMD
OBJDIR = obj
SRCDIR = src
BINDIR = bin
TARGET = aps
MAIN_OBJECT = $(SRCDIR)/main.o
COMMON_OBJECTS = $(SRCDIR)/common/shape_matrix_file.o \
		$(SRCDIR)/common/ShapeMatrix.o

PUZZLE_OBJECTS = $(SRCDIR)/solver/imageProcessor.o \
		$(SRCDIR)/solver/solver.o \
		$(SRCDIR)/solver/solutionProcessor.o

OBJECTS = $(MAIN_OBJECT) $(COMMON_OBJECTS) $(PUZZLE_OBJECTS)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJDIR)/%.o : $(SRCDIR)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

-include $(OBJECTS:.o=.d)

.PHONY: clean

clean:
	rm -rf $(BINDIR) $(OBJECTS) $(OBJECTS:.o=.d)
