//
// SLFromFile.cpp
// 2023-03-29
// Ivan Konishchev
//

#include "SLFromFile.h"

void SLFromFile::createRandomFile(const std::string &fileName, int startIndex, int lastIndex)
{
    auto start = std::chrono::high_resolution_clock::now();

    _fileName = fileName;
    // for generator random number
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(startIndex, lastIndex);

    // file
    std::fstream fs;
    fs.open(_fileName, std::ifstream::out | std::ofstream::trunc);
    if (fs.is_open()) // проверяем, что файл успешно открыт
    {
        long long int currentSizeByte{0};
        while (currentSizeByte < _fileSizeByte)
        {
            std::string randomNum = std::to_string(dist(gen));
            fs << randomNum << ' ';
            currentSizeByte += randomNum.size() + 1;
        }
        std::cout << "Create file, name: " << _fileName
                  << " File size byte: " << currentSizeByte << "\n";
    }
    fs.close();

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Create file Elapsed time: " << elapsed.count() << " sec" << std::endl;
}

void SLFromFile::sortFile(const std::string &fileName)
{
    // start sorted file
    auto start = std::chrono::high_resolution_clock::now();

    std::fstream fs;
    fs.open(_fileName, std::fstream::in);
    if (fs.is_open())
    {
        while (!fs.eof())
        {
            int *array;
            int size{0};
            size = readFilePartToArray(fs, array);
            Sorting::quickSort(array, 0, size);
            mergeToTempFiles(array, size);

            delete[] array;
        }
    }
    fs.close();
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double>  elapsed = finish - start;
    std::cout << "Sort file elapsed time: " << elapsed.count() << " sec" << std::endl;
    mergingFiles("negative.txt", "positive.txt");
}

// Private methods

int SLFromFile::readFilePartToArray(std::fstream &fs, int *&array)
{

    array = new int[_partSize];
    int indexSize;
    for (indexSize = 0; indexSize < _partSize && !fs.eof(); indexSize++)
    {
        fs >> array[indexSize];
    }

    if (indexSize == 1)
    {
        return 0;
    }

    if (indexSize < _partSize)
    {
        int *tempArray = new int[indexSize];
        for (int i = 0; i < indexSize; i++)
        {
            tempArray[i] = array[i];
        }
        delete[] array;
        array = tempArray;
        return indexSize - 1;
    }
    return _partSize;
}

void SLFromFile::mergeToTempFiles(const int *array, int size)
{
    std::fstream negative;
    std::fstream positive;
    std::fstream negativeTemp;
    std::fstream positiveTemp;
    negative.open("negative.txt", std::fstream::in);
    positive.open("positive.txt", std::fstream::in);
    negativeTemp.open("negativeTemp.txt", std::fstream::out | std::fstream::trunc);
    positiveTemp.open("positiveTemp.txt", std::fstream::out | std::fstream::trunc);

    if (negativeTemp.is_open() || positiveTemp.is_open())
    {
        int valNeg;
        int valPos;
        negative >> valNeg;
        positive >> valPos;
        int index{0};
        bool isPosEof{false};
        bool isNegEof{false};
        while (index < size)
        {
            if (array[index] >= 0) // положительные
            {
                if (!positive.eof() && positive.is_open())
                {
                    if (array[index] < valPos)
                    {
                        positiveTemp << array[index] << " ";
                        index++;
                    }
                    else if (array[index] == valPos)
                    {
                        positiveTemp << array[index] << " ";
                        positiveTemp << valPos << " ";
                        positive >> valPos;

                        index++;
                    }
                    else
                    {
                        positiveTemp << valPos << " ";
                        positive >> valPos;
                    }
                }
                else // закончился файл positive если index < size то дописать оставшееся в файл
                {
                    if (isNegEof)
                    {
                        break;
                    }
                    isPosEof = true;
                    positiveTemp << array[index] << " ";
                    index++;
                }
            }
            else
            {
                if (!negative.eof() && negative.is_open()) // отрицательные
                {
                    if (array[index] < valNeg)
                    {
                        negativeTemp << array[index] << " ";
                        index++;
                    }
                    else if (array[index] == valNeg)
                    {
                        negativeTemp << valNeg << " ";
                        negativeTemp << array[index] << " ";
                        index++;
                        negative >> valNeg;
                    }
                    else
                    {
                        negativeTemp << valNeg << " ";
                        negative >> valNeg;
                    }
                }
                else
                {
                    if (isPosEof)
                    {
                        break;
                    }
                    isNegEof = true;
                    negativeTemp << array[index] << " ";
                    index++;
                }
            }
        }
        // закончился файл positive если index < size то дописать оставшееся в файл
        // index == size?;
        if (index < size)
        {
            while (index < size)
            {
                if (array[index] >= 0)
                {
                    positiveTemp << array[index] << " ";
                }
                else
                {
                    negativeTemp << array[index] << " ";
                }
                index++;
            }
        }
    }

    negative.close();
    positive.close();
    negativeTemp.close();
    positiveTemp.close();
    if (std::filesystem::copy_file("negativeTemp.txt", "negative.txt",
                                   std::filesystem::copy_options::overwrite_existing))
    {
        std::cout << "copy file: "
                  << "negativeTemp.txt"
                  << " to file: "
                  << "negative.txt"
                  << " Status: Ok!\n";
    }
    else
    {
        std::cout << "copy file: "
                  << "negativeTemp.txt"
                  << " to file: "
                  << "negative.txt"
                  << " Status: Error!\n";
    }

    if (std::filesystem::copy_file("positiveTemp.txt", "positive.txt",
                                   std::filesystem::copy_options::overwrite_existing))
    {
        std::cout << "copy file: "
                  << "positiveTemp.txt"
                  << " to file: "
                  << "positive.txt"
                  << " Status: Ok!\n";
    }
    else
    {
        std::cout << "copy file: "
                  << "negativeTemp.txt"
                  << " to file: "
                  << "positive.txt"
                  << " Status: Error!\n";
    }
}

void SLFromFile::mergingFiles(const std::string &file1, const std::string &file2)
{
    bool isSucces{false};
    int randomFileSize;
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream in(_fileName, std::ifstream::ate | std::ifstream::binary);

    if (in.is_open())
    {
        randomFileSize = in.tellg();
        in.close();
    }

    std::fstream output;
    std::fstream fileLessVal;
    std::fstream fileGreatVal;
    fileLessVal.open(file1, std::fstream::in);
    fileGreatVal.open(file2, std::fstream::in);
    output.open(_fileName, std::fstream::out | std::fstream::trunc);
    if (output.is_open())
    {
        int value;
        if (fileLessVal.is_open())
        {
            while (!fileLessVal.eof())
            {
                fileLessVal >> value;
                output << value << " ";
            }
            fileLessVal.close();
        }

        if (fileGreatVal.is_open())
        {
            while (!fileGreatVal.eof())
            {
                fileGreatVal >> value;
                output << value << " ";
            }
            fileGreatVal.close();
        }
        isSucces = true;
        output.close();
    }

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    if (isSucces)
    {
        std::cout << "Success! Elapsed time: " << elapsed.count() << " sec" << std::endl;
    }
    else
    {
        std::cout << "Error concatenation files" << std::endl;
    }

    std::ifstream out(_fileName, std::ifstream::ate | std::ifstream::binary);
    if (out.is_open())
    {
        int randomFileSize = out.tellg();
        std::cout << "Размер исходного файла -> " << randomFileSize << " байт.";
        std::cout << " : Размер отсортированного файла -> " << randomFileSize << " байт.\n";
        out.close();
    }
}