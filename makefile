.PHONY: clean check coverage test main
OBJDIR = obj
SRCDIR = src
BINDIR = bin
TARGET = aps
TESTTARGET = test
TESTDIR = test
DEMODIR = src/demo
IMAGE_PROCESSOR_DIR = src/image_processor
SOLVER_DIR = src/solver
COVDIR = coverage

OPENCV_CXXFLAGS = $(shell pkg-config --cflags opencv)
OPENCV_LIBFLAGS = $(shell pkg-config --cflags --libs opencv)

MAIN_OBJECT = $(OBJDIR)/main.o

COMMON_OBJECTS = $(OBJDIR)/common/shape_matrix_io.o \
	$(OBJDIR)/common/puzzle_board.o \
	$(OBJDIR)/common/shape_matrix.o \
	$(OBJDIR)/common/point.o \
	$(OBJDIR)/common/coordinates_io.o \
	$(OBJDIR)/common/debugger.o


IMAGE_PROCESSOR_OBJECTS = $(OBJDIR)/image_processor/discretizer/shape_translate.o\
		$(OBJDIR)/image_processor/imagereader/image_read.o\
		$(OBJDIR)/image_processor/discretizer/shape_rotate.o

SOLVER_OBJECTS = $(OBJDIR)/solver/solver.o

OBJECTS = $(COMMON_OBJECTS)\
	$(IMAGE_PROCESSOR_OBJECTS)\
	$(SOLVER_OBJECTS)

CXX = g++
CXXFLAGS = -Wall -g -MMD -std=c++11 -I$(SRCDIR)/

main: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(MAIN_OBJECT) $(OBJECTS)
	@echo "\tLinking \"$@\""
	@mkdir -p $(OBJDIR) $(BINDIR)
	@$(CXX) $(CXXFLAGS) -fprofile-arcs $^ -o $@ $(OPENCV_LIBFLAGS)
	@echo "[Done]\tLinking \"$@\""

$(MAIN_OBJECT): $(OBJDIR)/%.o: $(SRCDIR)/%.cc
	@echo "\tCompiling \"$@\""
	@mkdir -p $(OBJDIR) $(BINDIR)
	@$(CXX) $(CXXFLAGS) -Weffc++ --coverage -c $< -o $@
	@echo "[Done]\tCompiling \"$@\""

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cc
	@echo "\tCompiling \"$@\""
	@mkdir -p `dirname $@`
	@$(CXX) $(CXXFLAGS) -Weffc++ --coverage -c $< -o $@
	@echo "[Done]\tCompiling \"$@\""

-include $(OBJECTS:.o=.d)

include $(TESTDIR)/makefile
include $(DEMODIR)/makefile
include $(IMAGE_PROCESSOR_DIR)/makefile
include $(SOLVER_DIR)/makefile

check:
	cppcheck --enable=all --check-config --suppress=missingIncludeSystem -I src src

clean: test-clean
	rm -rf $(BINDIR) $(OBJDIR) $(COVDIR) *.o *.d *.gcno *.gcda
