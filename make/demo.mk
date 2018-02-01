$(DEMO_TARGET): $(BINDIR)/$(DEMO_TARGET)
	
DEMO_OBJECTS = $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(shell find $(SRCDIR)/demo/ -type f -name '*.cc'))

$(DEMO_OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cc
	@echo "\tCompiling \"$@\""
	@mkdir -p `dirname $@`
	@$(CXX) $(CXXFLAGS) -Weffc++ --coverage -c $< -o $@
	@echo "[Done]\tCompiling \"$@\""

$(BINDIR)/$(DEMO_TARGET): $(COMMON_OBJECTS) $(SOLVER_OBJECTS) $(DISCRETIZER_OBJECTS) $(IMAGE_READER_OBJECTS) $(DEMO_OBJECTS)
	@echo "\tLinking \"$@\""
	@mkdir -p bin
	@$(CXX) $(CPPFLAGS)\
		$(TESTINCLUDES)\
		$(CXXFLAGS) -fprofile-arcs $^ -o $@ $(OPENCV_LIBFLAGS)
	@echo "[Done]\tLinking \"$@\""
