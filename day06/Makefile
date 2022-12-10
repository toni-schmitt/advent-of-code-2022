SHELL := /bin/zsh

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++14 -g

#PART_1 = part_1
PART_2 = part_2

SRC_DIR = ./src
SRC_PART_1 = $(SRC_DIR)/part_1.cpp
SRC_PART_2 = $(SRC_DIR)/part_2.cpp

all: $(PART_1) $(PART_2)

$(PART_1): $(SRC_PART_1)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(PART_2): $(SRC_PART_2)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	$(RM) $(PART_1)
	$(RM) $(PART_2)

re: clean all

run: re
	./$(PART_1) input/input.txt || true
	./$(PART_2) input/input.txt || true

test: re
	./$(PART_1) input/test.txt || true
	./$(PART_2) input/test.txt || true

time: re
	time -- ./$(PART_1) input/input.txt > /dev/null
	time -- ./$(PART_2) input/input.txt > /dev/null