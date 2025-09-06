#include "Json2CsvConverter.hpp"

Json2CsvConverter::Json2CsvConverter(const std::string& output_csv, bool debug)
    : output_csv_(output_csv), debug_(debug), header_written_(false), total_records_(0) {}

size_t Json2CsvConverter::find_next_object(const std::string& s, size_t start) {
    return s.find("{", start);
}

size_t Json2CsvConverter::find_end_object(const std::string& s, size_t start) {
    int depth = 0;
    for (size_t i = start; i < s.size(); ++i) {
        if (s[i] == '{') {
            depth++;
        }  
        if (s[i] == '}') {
            depth--;
            if (depth == 0) {
                return i;
            }
        }
    }
    return std::string::npos;
}

// Trim whitespace from both ends of a string
std::string Json2CsvConverter::clean(const std::string& s) {
    std::string out = s;
    while (!out.empty() && (out.front() == ' ' || out.front() == '\n' || out.front() == '\r')) {
        out.erase(out.begin());
    }
    while (!out.empty() && (out.back() == ' ' || out.back() == '\n' || out.back() == '\r')) {
        out.pop_back();
    }
    return out;
}

std::string Json2CsvConverter::get_value(const std::string& key, const std::string& block) {
    std::string pattern = "\"" + key + "\"";
    size_t pos = block.find(pattern);

    if (pos == std::string::npos) {
        return "";
    }
    
    pos = block.find(':', pos);
    
    if (pos == std::string::npos) {
        return "";
    }
        
    pos++;

    // Skip whitespace
    while (pos < block.size() && (block[pos] == ' ' || block[pos] == '\n' || block[pos] == '\t')) {
        ++pos;
    }

    // Handle array values
    if (pos < block.size() && block[pos] == '[') {
        int bracket_depth = 0;
        size_t start = pos;
        for (size_t i = pos; i < block.size(); ++i) {
            if (block[i] == '[') {
                bracket_depth++;
            }
            if (block[i] == ']') {
                bracket_depth--;
                if (bracket_depth == 0) {
                    return block.substr(start, i - start + 1);
                }
            }
        }
        return "";
    }

    // Handle string values
    if (pos < block.size() && block[pos] == '"') {
        size_t end = pos + 1;
        while (end < block.size() && block[end] != '"') {
            if (block[end] == '\\') {
                end++;  // Skip escaped characters
            }
            end++;
        }
        if (end < block.size()) {
            return block.substr(pos + 1, end - pos - 1);
        }
        return "";
    }

    // Handle numeric and other values
    size_t end = pos;
    while (end < block.size() && block[end] != ',' && block[end] != '}' && block[end] != '\n') {
        end++;
    }

    return clean(block.substr(pos, end - pos));
}

std::vector<std::string> Json2CsvConverter::parse_array(const std::string& s) {
    std::vector<std::string> out;
    if (s.empty()) {
        return out;
    }

    size_t a = s.find('['), b = s.rfind(']');
    if (a == std::string::npos || b == std::string::npos || a >= b) {
        return out;
    }

    std::string arr = s.substr(a + 1, b - a - 1);
    if (arr.empty()) {
        return out;
    }

    std::stringstream ss(arr);
    std::string num;
    while (std::getline(ss, num, ',')) {
        std::string cleaned = clean(num);
        if (!cleaned.empty()) {
            out.push_back(cleaned);
        }
    }
    return out;
}

std::string Json2CsvConverter::extract_nested_object(const std::string& block, const std::string& key) {
    std::string pattern = "\"" + key + "\"";
    size_t pos = block.find(pattern);
    if (pos == std::string::npos) {
        return "";
    }

    pos = block.find(':', pos);
    if (pos == std::string::npos) {
        return "";
    }

    pos = block.find('{', pos);
    if (pos == std::string::npos) {
        return "";
    }

    size_t end = find_end_object(block, pos);
    if (end == std::string::npos) {
        return "";
    }

    return block.substr(pos, end - pos + 1);
}

void Json2CsvConverter::write_csv_header() {
    ofs_ << "timestamp,frameNum,error,gesturePresence,gesture,ktoGesture";
    for (int i = 0; i < 16; ++i) {
        ofs_ << ",gestureFeatures_" << i;
    }
    ofs_ << ",procTimeData_interFrameProcTime,procTimeData_transmitOutTime";
    ofs_ << ",powerData_power1v8,powerData_power3v3,powerData_power1v2,powerData_power1v2RF";
    ofs_ << ",tempData_tempRx,tempData_tempTx,tempData_tempPM,tempData_tempDIG";
    ofs_ << ",presenceThreshold\n";
    header_written_ = true;
}

void Json2CsvConverter::write_csv_row(
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
    const std::string& presenceThreshold) {
    ofs_ << timestamp << "," << frameNum << "," << error << "," << gesturePresence << "," << gesture << "," << ktoGesture;
    for (size_t i = 0; i < 16; ++i) {
        ofs_ << "," << (i < gestureFeatures.size() ? gestureFeatures[i] : "");
    }
    ofs_ << "," << procTime_inter << "," << procTime_transmit;
    ofs_ << "," << power1v8 << "," << power3v3 << "," << power1v2 << "," << power1v2RF;
    ofs_ << "," << tempRx << "," << tempTx << "," << tempPM << "," << tempDIG;
    ofs_ << "," << presenceThreshold << "\n";
}

std::vector<std::string> Json2CsvConverter::get_json_files(const std::string& directory) {
    std::vector<std::string> json_files;
    try {
        if (!fs::exists(directory)) {
            std::cerr << "Directory '" << directory << "' does not exist." << std::endl;
            return json_files;
        }
        if (!fs::is_directory(directory)) {
            std::cerr << "'" << directory << "' is not a directory." << std::endl;
            return json_files;
        }
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().string();
                std::string extension = entry.path().extension().string();
                std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
                if (extension == ".json") {
                    json_files.push_back(filename);
                }
            }
        }
        std::sort(json_files.begin(), json_files.end());
    } catch (const fs::filesystem_error& ex) {
        std::cerr << "Filesystem error: " << ex.what() << std::endl;
    }
    return json_files;
}

void Json2CsvConverter::print_timing_result() {
    std::cout << "\n========== Timing Results ==========" << std::endl;
    std::cout << "Record processing time: " << processing_duration_ms_ << " ms" << std::endl;
    std::cout << "Total execution time: " << total_duration_ms_ << " ms" << std::endl;
    std::cout << "Average time per record: "
              << (total_records_ > 0 ? (double)processing_duration_ms_ / total_records_ : 0) << " ms" << std::endl;
    std::cout << "Total records processed: " << total_records_ << std::endl;
}

int Json2CsvConverter::convert_file(const std::string& json_file) {
    ofs_.open(output_csv_, std::ios::app);

    if (!ofs_.is_open()) {
        std::cerr << "Cannot create/open output.csv file." << std::endl;
        return 1;
    }

    if (!header_written_) {
        write_csv_header();
    }

    std::ifstream ifs(json_file);
    if (!ifs.is_open()) {
        std::cerr << "Cannot open json file: " << json_file << std::endl;
        return 1;
    }

    std::string json((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();

    // Find the data array
    size_t dpos = json.find("\"data\"");
    if (dpos == std::string::npos) {
        std::cerr << "Cannot find 'data' field in " << json_file << std::endl;
        return 1;
    }

    dpos = json.find('[', dpos);
    if (dpos == std::string::npos) {
        std::cerr << "Cannot find data array start in " << json_file << std::endl;
        return 1;
    }

    // Find the end of the data array
    int bracket_depth = 0;
    size_t dend = dpos;
    for (size_t i = dpos; i < json.size(); ++i) {
        if (json[i] == '[') {
            bracket_depth++;
        }
        if (json[i] == ']') {
            bracket_depth--;
            if (bracket_depth == 0) {
                dend = i;
                break;
            }
        }
    }

    if (dend == dpos) {
        std::cerr << "Cannot find data array end in " << json_file << std::endl;
        return 1;
    }

    size_t cur = dpos + 1;
    int rec_count = 0;
    while (cur < dend) {
        // Find next object start
        cur = find_next_object(json, cur);
        if (cur == std::string::npos || cur >= dend) {
            break;
        }

        // Find object end
        size_t end = find_end_object(json, cur);
        if (end == std::string::npos || end >= dend) {
            break;
        }

        std::string block = json.substr(cur, end - cur + 1);

        // Extract timestamp
        std::string timestamp = get_value("timestamp", block);

        // Extract frameData object
        std::string frameDataBlock = extract_nested_object(block, "frameData");
        if (frameDataBlock.empty()) {
            std::cerr << "Warning: frameData not found in record " << rec_count + 1 << std::endl;
            cur = end + 1;
            continue;
        }

        // Extract basic frame data
        std::string frameNum = get_value("frameNum", frameDataBlock);
        std::string error = get_value("error", frameDataBlock);
        std::string gesturePresence = get_value("gesturePresence", frameDataBlock);
        std::string gesture = get_value("gesture", frameDataBlock);
        std::string ktoGesture = get_value("ktoGesture", frameDataBlock);

        // Extract gesture features array
        std::vector<std::string> gestureFeatures = parse_array(get_value("gestureFeatures", frameDataBlock));
        while (gestureFeatures.size() < 16) {
            gestureFeatures.push_back("");
        }

        // Extract nested objects
        std::string procTimeBlock = extract_nested_object(frameDataBlock, "procTimeData");
        std::string procTime_inter = get_value("interFrameProcTime", procTimeBlock);
        std::string procTime_transmit = get_value("transmitOutTime", procTimeBlock);

        std::string powerBlock = extract_nested_object(frameDataBlock, "powerData");
        std::string power1v8 = get_value("power1v8", powerBlock);
        std::string power3v3 = get_value("power3v3", powerBlock);
        std::string power1v2 = get_value("power1v2", powerBlock);
        std::string power1v2RF = get_value("power1v2RF", powerBlock);

        std::string tempBlock = extract_nested_object(frameDataBlock, "tempData");
        std::string tempRx = get_value("tempRx", tempBlock);
        std::string tempTx = get_value("tempTx", tempBlock);
        std::string tempPM = get_value("tempPM", tempBlock);
        std::string tempDIG = get_value("tempDIG", tempBlock);

        std::string presenceThreshold = get_value("presenceThreshold", frameDataBlock);

        write_csv_row(timestamp, frameNum, error, gesturePresence, gesture, ktoGesture,
                      gestureFeatures, procTime_inter, procTime_transmit,
                      power1v8, power3v3, power1v2, power1v2RF,
                      tempRx, tempTx, tempPM, tempDIG, presenceThreshold);

        rec_count++;

        // debugging output
        if (debug_) {
            std::cout << "Record #" << rec_count << " written (frameNum: " << frameNum << ")" << std::endl;
        }

        // Move to next record
        cur = end + 1;
    }
    ofs_.close();
    return rec_count;
}

int Json2CsvConverter::convert_directory(const std::string& folder) {
    // Timing start
    auto total_start = std::chrono::high_resolution_clock::now();
    auto processing_start = std::chrono::high_resolution_clock::now();

    std::vector<std::string> json_files = get_json_files(folder);
    if (json_files.empty()) {
        std::cerr << "No JSON files found in '" << folder << "' directory." << std::endl;
        return 0;
    }

    if (debug_) {
        std::cout << "Found " << json_files.size() << " JSON files in '" << folder << "' directory:" << std::endl;
        for (const auto& file : json_files) {
            std::cout << "  - " << fs::path(file).filename().string() << std::endl;
        }
        std::cout << std::endl;
    }

    int total_records = 0;
    header_written_ = false;

    for (size_t i = 0; i < json_files.size(); ++i) {
        if (debug_) {
            std::cout << "\n========== Processing " << fs::path(json_files[i]).filename().string() << " ==========" << std::endl;
        }
            
        int file_records = convert_file(json_files[i]);
        total_records += file_records;

        if (debug_) {
            if (file_records > 0) {
                std::cout << "Completed " << fs::path(json_files[i]).filename().string() << ": " << file_records << " records processed" << std::endl;
            } else {
                std::cout << "Failed to process " << fs::path(json_files[i]).filename().string() << std::endl;
            }
        }
    }
    
    auto processing_end = std::chrono::high_resolution_clock::now();
    auto total_end = std::chrono::high_resolution_clock::now();
    processing_duration_ms_ = std::chrono::duration_cast<std::chrono::milliseconds>(processing_end - processing_start).count();
    total_duration_ms_ = std::chrono::duration_cast<std::chrono::milliseconds>(total_end - total_start).count();
    total_records_ = total_records;

    print_timing_result();
    return total_records;
}
