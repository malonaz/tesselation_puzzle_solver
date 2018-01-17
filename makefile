CXX = g++
CXXFLAGS = -Wall -g -MMD
OBJDIR = obj
SRCDIR = src
BINDIR = bin
TARGET = aps
OBJECTS = $(SRCDIR)/main.o \
		$(SRCDIR)/common/shape_matrix_file.o \
		$(SRCDIR)/common/ShapeMatrix.o

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
