IMAGE_PROCESSOR_TARGET = image_processor

image_processor: $(BINDIR)/$(IMAGE_PROCESSOR_TARGET)

IMAGE_PROCESSOR_OBJECTS = $(OBJDIR)/image_processor/image_processor.o

$(IMAGE_PROCESSOR_OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cc
	@echo "\tCompiling \"$@\""
	@mkdir -p `dirname $@`
	@$(CXX) $(CXXFLAGS) -Weffc++ --coverage -c $< -o $@
	@echo "[Done]\tCompiling \"$@\""

$(BINDIR)/$(IMAGE_PROCESSOR_TARGET): $(OBJECTS) $(IMAGE_PROCESSOR_OBJECTS) $(IMAGE_READER_OBJECTS)
	@echo "\tLinking \"$@\""
	@mkdir -p bin
	$(CXX) $(CPPFLAGS)\
		$(CXXFLAGS) -fprofile-arcs $^ -o $@ $(OPENCV_LIBFLAGS)
	@echo "[Done]\tLinking \"$@\""
