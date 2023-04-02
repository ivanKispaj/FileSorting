//
// main.cpp
// 2023-03-29
// Ivan Konishchev
//

#include <iostream>
#include "SLFromFile.h"
#include <fstream>
int main(int argc, const char *argv[])
{

    std::string fileName = "random.txt";
    // Create random number file size 2 ->2Gb, 1 -> 1GB, 0.5 -> 500Mb, 0.2 -> 200Mb ;
    SLFromFile manager(0.25, fileName);
    manager.createRandomFile(fileName, -213224, 394445);

    // sorting file
    manager.sortFile(fileName);

    return 0;
}
