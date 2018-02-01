.PHONY: clean check coverage test main
OBJDIR = obj
SRCDIR = src
BINDIR = bin
TARGET = aps
TESTTARGET = test
DEMOTARGET = demo
TESTDIR = test
DEMODIR = src/demo
IMAGE_PROCESSOR_DIR = src/image_processor
SOLVER_DIR = src/solver
COVDIR = coverage
MAKEDIR = make

OPENCV_CXXFLAGS = $(shell pkg-config --cflags opencv)
OPENCV_LIBFLAGS = $(shell pkg-config --cflags --libs opencv)

CXX = g++
CXXFLAGS = -Wall -g -MMD -std=c++11 -I$(SRCDIR)/

all: $(DEMOTARGET) $(BINDIR)/$(TESTTARGET)

-include $(OBJECTS:.o=.d)

include $(MAKEDIR)/common.mk
include $(MAKEDIR)/discretizer.mk
include $(MAKEDIR)/imagereader.mk
include $(MAKEDIR)/solver.mk

include $(MAKEDIR)/demo.mk
include $(MAKEDIR)/test.mk

check:
	cppcheck --enable=all --check-config --suppress=missingIncludeSystem -I src src

clean:
	rm -rf $(BINDIR) $(OBJDIR) $(COVDIR) *.o *.d *.gcno *.gcda
