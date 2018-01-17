EXE = solver
CXX = g++
CXXFLAGS = -Wall -g -MMD
OBJECTS = main.o imageProcessor.o solver.o solutionProcessor.o

$(EXE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJECTS): %.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $<

-include $(OBJECTS:.o=.d)



.Phony: clean test

clean:
	rm -rf $(EXE) $(OBJECTS) $(OBJECTS:.o=.d)

test:
	make $(EXE)
	clear
	./solver
