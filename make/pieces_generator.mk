$(PIECES_GENERATOR_TARGET): $(BINDIR)/$(PIECES_GENERATOR_TARGET)

PIECES_GENERATOR_OBJECTS = $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(shell find $(SRCDIR)/random_generator/ -type f -name '*.cc'))

$(PIECES_GENERATOR_OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cc
	@echo "\tCompiling \"$@\""
	@mkdir -p `dirname $@`
	@$(CXX) $(CXXFLAGS) -Weffc++ --coverage -c $< -o $@  -lcrypto
	@echo "[Done]\tCompiling \"$@\""

$(BINDIR)/$(PIECES_GENERATOR_TARGET): $(COMMON_OBJECTS) $(PIECES_GENERATOR_OBJECTS)
	@echo "\tLinking \"$@\""
	@mkdir -p bin
	@$(CXX) $(CPPFLAGS)\
		$(TESTINCLUDES)\
		$(CXXFLAGS) -fprofile-arcs $^ -o $@  -lcrypto
	@echo "[Done]\tLinking \"$@\""