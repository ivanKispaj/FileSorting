//
// SLFromFile.h
// 2023-03-29
// Ivan Konishchev
//

#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include "Sorting.h"
#include <cstdio>
#include <fstream>
#include <random>
#include <memory>

class SLFromFile
{
    double _fileSizeGb{0};
    long long int _fileSizeByte{0};
    const int _partSize = 1000000;
    std::string _fileName;
    double _executionTime;
    /// @brief
    /// @param array
    /// @return int size of array
    int readFilePartToArray(std::fstream &fs, int *&array);
    void mergeToTempFiles(const int *array, int size);
    void mergingFiles(const std::string &file1, const std::string &file2);

public:
    /// @brief Creates an SL From File object with a default file size of 2Gb
    ///        and file extension
    SLFromFile() = delete;

    /// @brief Creates an SL From File object with the specified file size
    /// @param fileSizeGb double -> File size in GB
    /// @param fileExtension string -> File extension
    SLFromFile(double fileSizeGb, const std::string &fileName) : _fileSizeGb(fileSizeGb),
                                                                 _fileSizeByte(_fileSizeGb * 1073741824),
                                                                 _fileName(fileName),
                                                                 _executionTime(0)
    {
        // Clear temporary files if isset
        std::fstream negative;
        std::fstream positive;
        negative.open("negative.txt", std::fstream::out | std::fstream::trunc);
        positive.open("positive.txt", std::fstream::out | std::fstream::trunc);
        if (negative.is_open())
        {
            negative.close();
        }
        if (positive.is_open())
        {
            positive.close();
        }
    };

    /// deinit
    ~SLFromFile() = default;

    /// @brief Creates a file with the specified name,
    ///        and random numbers from the specified range.
    ///        With the size specified when creating the SLFromFile object
    /// @param name string File name
    /// @param startIndex int starting index of the range
    /// @param lastIndex ште last index of the range
    void createRandomFile(const std::string &fileName, int startIndex, int lastIndex);
    void sortFile(const std::string &fileName);
};
