#include "Json2CsvConverter.hpp"

int main() {
    std::string output_csv = "../data/output.csv";
    Json2CsvConverter converter(output_csv, true);

    std::cout << "\nSelect mode:\n";
    std::cout << "1. Convert single JSON file\n";
    std::cout << "2. Convert all JSON files in a directory\n";
    std::cout << "\nEnter choice (1 or 2): ";

    int mode = 0;
    std::cin >> mode;

    if (mode == 1) {
        std::string json_file;
        std::cout << "Enter JSON file path: ";
        std::cin >> json_file;
        int count = converter.convert_file(json_file);
        std::cout << "Processed " << count << " records from " << json_file << std::endl;
    }
    if (mode == 2) {
        std::string folder;
        std::cout << "Enter directory path: ";
        std::cin >> folder;
        int total_count = converter.convert_directory(folder);
        std::cout << "Processed " << total_count << " records from directory " << folder << std::endl;
    } 
    if (mode != 1 && mode != 2) {
        std::cout << "Invalid choice. Exiting." << std::endl;
    }

    return 0;
}
