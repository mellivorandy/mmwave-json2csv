CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra

TARGET = json2csv
SRC = main.cpp Json2CsvConverter.cpp

all: $(TARGET)
	@echo
	@echo "Build done."
	@echo
	# ./json2csv.exe  # Uncomment to run after build

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

.PHONY: all clean
