$(PUZZLE_BREAKER_TARGET): $(BINDIR)/$(PUZZLE_BREAKER_TARGET)

PUZZLE_BREAKER_OBJECTS = $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(shell find $(SRCDIR)/puzzle_breaker/ -type f -name '*.cc'))

$(PUZZLE_BREAKER_OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cc
	@echo "\tCompiling \"$@\""
	@mkdir -p `dirname $@`
	@$(CXX) $(CXXFLAGS) -Weffc++ --coverage -c $< -o $@  -lcrypto
	@echo "[Done]\tCompiling \"$@\""

$(BINDIR)/$(PUZZLE_BREAKER_TARGET): $(COMMON_OBJECTS) $(PUZZLE_BREAKER_OBJECTS)
	@echo "\tLinking \"$@\""
	@mkdir -p bin
	@$(CXX) $(CPPFLAGS)\
		$(TESTINCLUDES)\
		$(CXXFLAGS) -fprofile-arcs $^ -o $@  -lcrypto
	@echo "[Done]\tLinking \"$@\""
