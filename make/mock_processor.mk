$(MOCK_PROCESSOR_TARGET): $(BINDIR)/$(MOCK_PROCESSOR_TARGET)

mock_processor.o: $(SRCDIR)/mock_server/mock_processor.cc
	@echo "\tCompiling \"$@\""
	@mkdir -p `dirname $@`
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "[Done]\tCompiling \"$@\""

$(BINDIR)/$(MOCK_PROCESSOR_TARGET): mock_processor.o
	@echo "\tLinking \"$@\""
	@mkdir -p bin
	@$(CXX) $(CPPFLAGS)\
		$(TESTINCLUDES)\
		$(CXXFLAGS) -fprofile-arcs $^ -o $@
	@echo "[Done]\tLinking \"$@\""
