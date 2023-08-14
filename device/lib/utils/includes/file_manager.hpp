#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>

class FileManager {
public:
    static std::string loadFile(const std::string& path) {
        boost::filesystem::path filePath(path);
        if (!boost::filesystem::exists(filePath) || !boost::filesystem::is_regular_file(filePath)) {
            throw std::runtime_error("File not found or not a regular file.");
        }

        std::ifstream fileStream(filePath.string(), std::ios::in);
        if (!fileStream.is_open()) {
            throw std::runtime_error("Failed to open the file for reading.");
        }

        std::string content((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
        fileStream.close();
        return content;
    }

    static void writeFile(const std::string& path, const std::string& content) {
        std::ofstream fileStream(path, std::ios::out | std::ios::trunc);
        if (!fileStream.is_open()) {
            throw std::runtime_error("Failed to open the file for writing.");
        }
        fileStream << content;
        fileStream.close();
    }
};
