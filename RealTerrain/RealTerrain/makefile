TARGET   = Application

SOURCES := $(wildcard *.cpp)
OBJECTS := $(patsubst %.cpp,obj/%.o,$(SOURCES))
DEPENDS := $(patsubst %.cpp,obj/%.d,$(SOURCES))

CXXFLAGS = -g -I. -std=c++20
LDLIBS = -lfmt -lGL -lGLEW -lglfw
INCLUDES = -I../../_external/glfw-3.3.4/include/ -I../../_external/glm/ -I../../_external/glew-2.1.0/include/ -I../../_external/stb/

# ADD MORE WARNINGS!
WARNING := -Wall -Wextra

# .PHONY means these rules get executed even if
# files of those names exist.
.PHONY: all clean

# The first rule is the default, ie. "make",
# "make all" and "make parking" mean the same
all: bin/$(TARGET)
	./bin/$(TARGET)

clean:
	$(RM) $(OBJECTS) $(DEPENDS) $(TARGET)

# Linking the executable from the object files
bin/$(TARGET): $(OBJECTS)
	@echo "target : $(TARGET)"
	@echo "sources: $(SOURCES)"
	@echo "objects: $(OBJECTS)"
	@echo "depends: $(DEPENDS)"
	@mkdir -p bin
	$(CXX) $(WARNING) $(CXXFLAGS) $(LDLIBS) $^ -o $@

-include $(DEPENDS)

obj/%.o: %.cpp makefile
	@mkdir -p obj
	$(CXX) $(WARNING) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@
