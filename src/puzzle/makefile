DEMO_TARGET = demo

demo: $(BINDIR)/$(DEMOTARGET)

DEMO_OBJECTS = $(OBJDIR)/puzzle/demo1.o


$(DEMO_OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cc
	@echo "\tCompiling \"$@\""
	@mkdir -p `dirname $@`
	@$(CXX) $(CXXFLAGS) -Weffc++ --coverage -c $< -o $@
	@echo "[Done]\tCompiling \"$@\""

$(BINDIR)/$(DEMOTARGET): $(OBJECTS) $(DEMO_OBJECTS)
	@echo "\tLinking \"$@\""
	@mkdir -p bin
	@$(CXX) $(CPPFLAGS)\
		$(TESTINCLUDES)\
		$(CXXFLAGS) -fprofile-arcs $^ -o $@
	@echo "[Done]\tLinking \"$@\""
