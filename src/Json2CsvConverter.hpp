#pragma once
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class Json2CsvConverter {
public:
    // Constructor with output CSV file path and optional debug flag
    Json2CsvConverter(const std::string& output_csv, bool debug = false);
    
    // Default destructor
    ~Json2CsvConverter() = default;

    // Convert a single JSON file to CSV
    int convert_file(const std::string& json_file);

    // Convert all JSON files in specified directory
    int convert_directory(const std::string& folder);

private:
    std::string output_csv_;
    bool debug_;
    bool header_written_;
    std::ofstream ofs_;

    // Timing variables
    long long processing_duration_ms_ = 0;
    long long total_duration_ms_ = 0;
    int total_records_ = 0;

    // Helper functions
    size_t find_next_object(const std::string& s, size_t start);
    size_t find_end_object(const std::string& s, size_t start);
    std::string clean(const std::string& s);
    std::string get_value(const std::string& key, const std::string& block);
    std::vector<std::string> parse_array(const std::string& s);
    std::string extract_nested_object(const std::string& block, const std::string& key);

    // Write CSV header if not already written
    void write_csv_header();

    // Write CSV row
    void write_csv_row(
        const std::string& timestamp,
        const std::string& frameNum,
        const std::string& error,
        const std::string& gesturePresence,
        const std::string& gesture,
        const std::string& ktoGesture,
        const std::vector<std::string>& gestureFeatures,
        const std::string& procTime_inter,
        const std::string& procTime_transmit,
        const std::string& power1v8,
        const std::string& power3v3,
        const std::string& power1v2,
        const std::string& power1v2RF,
        const std::string& tempRx,
        const std::string& tempTx,
        const std::string& tempPM,
        const std::string& tempDIG,
        const std::string& presenceThreshold);

    // Get all JSON files in specified directory
    std::vector<std::string> get_json_files(const std::string& directory);

    // Print timing results
    void print_timing_result();
};
