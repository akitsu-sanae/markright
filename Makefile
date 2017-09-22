TARGET_NAME = markright
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra

LDFLAGS =
LIBS =
INCLUDE = -I./include

SRCDIR = ./src
SRC = $(wildcard $(SRCDIR)/*.cpp)

BUILD_DIR = ./build
TARGET = $(BUILD_DIR)/$(TARGET_NAME)

OBJ = $(addprefix $(BUILD_DIR)/obj/, $(notdir $(SRC:.cpp=.o)))
DEPEND = $(OBJ:.o=.d)

TEST_FILES = $(wildcard ./test/*.mr)

all: $(TARGET)

-include $(DEPEND)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)


$(BUILD_DIR)/obj/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE)  -o $@ -c -MMD -MP $<


.PHONY: clean
clean:
	-rm -f $(OBJ) $(DEPEND) $(TARGET)

.PHONY: run
run: $(TARGET)
	$(TARGET)

.PHONY: test
test: $(TARGET)
	$(foreach file, $(TEST_FILES), \
		echo "testing: " $(file); \
		./build/markright $(file) > $(file:.mr=.tex) ||  echo "\033[31mtest fail markright to xelatex at " $(file) "\033[39m" || exit 50; \
	    ) \
	rm -f test/*.tex *.pdf *.aux *.log


