$(SOLVER_PROGRAM_TARGET): $(BINDIR)/$(SOLVER_PROGRAM_TARGET)

SOLVER_PROGRAM_OBJECTS = $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(shell find $(SRCDIR)/solver_program/ -type f -name '*.cc'))

$(SOLVER_PROGRAM_OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cc
	@echo "\tCompiling \"$@\""
	@mkdir -p `dirname $@`
	@$(CXX) $(CXXFLAGS) -Weffc++ --coverage -c $< -o $@  -lcrypto
	@echo "[Done]\tCompiling \"$@\""

$(BINDIR)/$(SOLVER_PROGRAM_TARGET): $(COMMON_OBJECTS) $(DLX_OBJECTS) $(SOLVER_OBJECTS) $(SOLVER_PROGRAM_OBJECTS)
	@echo "\tLinking \"$@\""
	@mkdir -p bin
	@$(CXX) $(CPPFLAGS)\
		$(CXXFLAGS) -fprofile-arcs $^ -o $@  -lcrypto
	@echo "[Done]\tLinking \"$@\""
