$(MOCK_SOLVER_TARGET): $(BINDIR)/$(MOCK_SOLVER_TARGET)

mock_solver.o: $(SRCDIR)/mock_server/mock_solver.cc
	@echo "\tCompiling \"$@\""
	@mkdir -p `dirname $@`
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "[Done]\tCompiling \"$@\""

$(BINDIR)/$(MOCK_SOLVER_TARGET): mock_solver.o
	@echo "\tLinking \"$@\""
	@mkdir -p bin
	@$(CXX) $(CPPFLAGS)\
		$(CXXFLAGS) $^ -o $@
	@echo "[Done]\tLinking \"$@\""
