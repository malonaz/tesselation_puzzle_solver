
OBJDIR = obj
SRCDIR = src
BINDIR = bin
TESTDIR = test
COVDIR = coverage
MAKEDIR = make

TEST_TARGET = test
DEMO_TARGET = demo
IMAGE_PROCESSOR_TARGET = ip
SOLVER_PROGRAM_TARGET = sp

OPENCV_CXXFLAGS = $(shell pkg-config --cflags opencv)
OPENCV_LIBFLAGS = $(shell pkg-config --cflags --libs opencv)

CXX = g++
CXXFLAGS = -Wall -g -MMD -std=c++11 -pthread -fopenmp -I$(SRCDIR)/

all: $(DEMO_TARGET) $(TEST_TARGET) $(IMAGE_PROCESSOR_TARGET)

-include $(OBJECTS:.o=.d)

include $(MAKEDIR)/common.mk
include $(MAKEDIR)/discretizer.mk
include $(MAKEDIR)/imagereader.mk
include $(MAKEDIR)/solver.mk

include $(MAKEDIR)/demo.mk
include $(MAKEDIR)/test.mk
include $(MAKEDIR)/image_processor.mk
include $(MAKEDIR)/solver_program.mk

check:
	cppcheck --enable=all --check-config --suppress=missingIncludeSystem -I src src

clean:
	rm -rf $(BINDIR) $(OBJDIR) $(COVDIR) *.o *.d *.gcno *.gcda

.PHONY: clean check $(COVDIR) $(TEST_TARGET) $(DEMO_TARGET) $(IMAGE_PROCESSOR_TARGET)
