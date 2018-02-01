IMAGE_READER_OBJECTS = $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(shell find $(SRCDIR)/imagereader/ -type f -name '*.cc'))

$(IMAGE_READER_OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cc
	@echo "\tCompiling \"$@\""
	@mkdir -p `dirname $@`
	@$(CXX) $(CXXFLAGS) $(OPENCV_CXXFLAGS) -Wall --coverage -c $< -o $@
	@echo "[Done]\tCompiling \"$@\""
