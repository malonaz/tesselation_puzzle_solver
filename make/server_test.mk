$(SERVER_TEST_TARGET): $(BINDIR)/$(SERVER_TEST_TARGET)

test_solver_server.o: $(SRCDIR)/demo/test_solver_server.cc
	@echo "\tCompiling \"$@\""
	@mkdir -p `dirname $@`
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "[Done]\tCompiling \"$@\""

$(BINDIR)/$(SERVER_TEST_TARGET): test_solver_server.o
	@echo "\tLinking \"$@\""
	@mkdir -p bin
	@$(CXX) $(CPPFLAGS)\
		$(TESTINCLUDES)\
		$(CXXFLAGS) -fprofile-arcs $^ -o $@
	@echo "[Done]\tLinking \"$@\""SERVER_TEST_
