SOLVER_MODULE_TARGET = solver_module

solver_module: $(BINDIR)/$(SOLVER_MODULE_TARGET)

SOLVER_MODULE_OBJECT = $(OBJDIR)/solver/solver_module.o

$(SOLVER_MODULE_OBJECT): $(OBJDIR)/%.o : $(SRCDIR)/%.cc
	@echo "\tCompiling \"$@\""
	@mkdir -p `dirname $@`
	@$(CXX) $(CXXFLAGS) -Weffc++ --coverage -c $< -o $@
	@echo "[Done]\tCompiling \"$@\""

$(BINDIR)/$(SOLVER_MODULE_TARGET): $(OBJECTS) $(SOLVER_MODULE_OBJECT)
	@echo "\tLinking \"$@\""
	@mkdir -p bin
	@$(CXX) $(CPPFLAGS)\
		$(TESTINCLUDES)\
		$(CXXFLAGS) -fprofile-arcs $^ -o $@ $(OPENCV_LIBFLAGS)
	@echo "[Done]\tLinking \"$@\""
