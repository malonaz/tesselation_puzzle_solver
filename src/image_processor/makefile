IMAGE_PROCESS_TARGET = image_processor

image_processor: $(BINDIR)/$(IMAGE_PROCESS_TARGET)

IMAGE_PROCESS_OBJECT = $(OBJDIR)/image_processor/image_processor.o

$(IMAGE_PROCESS_OBJECT): $(OBJDIR)/%.o : $(SRCDIR)/%.cc
	@echo "\tCompiling \"$@\""
	@mkdir -p `dirname $@`
	@$(CXX) $(CXXFLAGS) -Weffc++ --coverage -c $< -o $@
	@echo "[Done]\tCompiling \"$@\""

$(BINDIR)/$(IMAGE_PROCESS_TARGET): $(OBJECTS) $(IMAGE_PROCESS_OBJECT)
	@echo "\tLinking \"$@\""
	@mkdir -p bin
	@$(CXX) $(CPPFLAGS)\
		$(TESTINCLUDES)\
		$(CXXFLAGS) -fprofile-arcs $^ -o $@ $(OPENCV_LIBFLAGS)
	@echo "[Done]\tLinking \"$@\""
