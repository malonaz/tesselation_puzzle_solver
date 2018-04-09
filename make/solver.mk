SOLVER_OBJECTS = $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(shell find $(SRCDIR)/solver/ -type f -name '*.cc'))

$(SOLVER_OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cc
	@echo "\tCompiling \"$@\""
	@mkdir -p `dirname $@`
	@$(CXX) $(CXXFLAGS) $(OPENCV_CXXFLAGS) -Wall --coverage -c $< -o $@ -lcrypto
	@echo "[Done]\tCompiling \"$@\""
