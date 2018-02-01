$(IMAGE_PROCESSOR_TARGET): $(BINDIR)/$(IMAGE_PROCESSOR_TARGET)

IMAGE_PROCESSOR_OBJECTS = $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(shell find $(SRCDIR)/image_processor/ -type f -name '*.cc'))

$(IMAGE_PROCESSOR_OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cc
	@echo "\tCompiling \"$@\""
	@mkdir -p `dirname $@`
	@$(CXX) $(CXXFLAGS) -Weffc++ --coverage -c $< -o $@
	@echo "[Done]\tCompiling \"$@\""

$(BINDIR)/$(IMAGE_PROCESSOR_TARGET): $(COMMON_OBJECTS) $(DISCRETIZER_OBJECTS) $(IMAGE_READER_OBJECTS) $(IMAGE_PROCESSOR_OBJECTS)
	@echo "\tLinking \"$@\""
	@mkdir -p bin
	@$(CXX) $(CPPFLAGS)\
		$(TESTINCLUDES)\
		$(CXXFLAGS) -fprofile-arcs $^ -o $@ $(OPENCV_LIBFLAGS)
	@echo "[Done]\tLinking \"$@\""
