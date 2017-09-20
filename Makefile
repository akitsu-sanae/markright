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

all: $(TARGET)

-include $(DEPEND)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)


$(BUILD_DIR)/obj/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE)  -o $@ -c -MMD -MP $<


.PHONY: clean
clean:
	-rm -f $(OBJ) $(DEPEND) $(TARGET)

run: $(TARGET)
	$(TARGET)

