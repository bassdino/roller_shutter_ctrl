/*
 * Parsino.cpp
 *
 *  Created on: 18.04.2017
 *      Author: Michael
 */

#include "Parsino.h"
#include <iostream>
#include <fstream>
#include <string>         // std::string

// defines
#define NULLPTR 0

// statics

Parsino::Parsino()
{
    std::string str("rolladen_ctrl.cfg");
    this->pFilename = this->filename;
    str.copy(this->filename, FILENAME_LENGTH, 0);

}

Parsino::Parsino(const char * pFilename)
{
    for (int ii = 0; ii < FILENAME_LENGTH; ii++)
    {
        this->filename[ii] = pFilename[ii];
    }
    this->pFilename = this->filename;
}

Parsino::~Parsino()
{
    // TODO Auto-generated destructor stub
}

void Parsino::createConfigFile(const char * pFilename)
{
    std::ofstream outfile;
    outfile.open(pFilename);    // open the file
    std::cout << "Writing to file" << std::endl;
    outfile << "# This file has been created automatically #" << std::endl << std::endl;
    outfile << "NUMBER_OF_MOTORS 4" << std::endl << std::endl;
    outfile << "#Configuration of the first motor for shutter control" << std::endl << std::endl;
    outfile << "#Pin which is activated to close the shutters" << std::endl;
    outfile << "PIN_CLOSE1 0" << std::endl;
    outfile << "#Pin which is activated to open the shutters" << std::endl;
    outfile << "PIN_OPEN1 2" << std::endl;
    outfile << "#Time at which the shutters are closed (integer format)" << std::endl;
    outfile << "TIME_CLOSE1 72000" << std::endl;
    outfile << "#Time at which the shutters are opened (integer format)" << std::endl;
    outfile << "TIME_OPEN1 28800" << std::endl;

    outfile << "#Configuration of the second motor for shutter control" << std::endl << std::endl;
    outfile << "#Pin which is activated to close the shutters" << std::endl;
    outfile << "PIN_CLOSE2 4" << std::endl;
    outfile << "#Pin which is activated to open the shutters" << std::endl;
    outfile << "PIN_OPEN2 3" << std::endl;
    outfile << "#Time at which the shutters are closed (integer format)" << std::endl;
    outfile << "TIME_CLOSE2 72000" << std::endl;
    outfile << "#Time at which the shutters are opened (integer format)" << std::endl;
    outfile << "TIME_OPEN2 28800" << std::endl;

    outfile << "#Configuration of the third motor for shutter control" << std::endl << std::endl;
    outfile << "#Pin which is activated to close the shutters" << std::endl;
    outfile << "PIN_CLOSE3 6" << std::endl;
    outfile << "#Pin which is activated to open the shutters" << std::endl;
    outfile << "PIN_OPEN3 5" << std::endl;
    outfile << "#Time at which the shutters are closed (integer format)" << std::endl;
    outfile << "TIME_CLOSE3 72000" << std::endl;
    outfile << "#Time at which the shutters are opened (integer format)" << std::endl;
    outfile << "TIME_OPEN3 28800" << std::endl;

    outfile << "#Configuration of the fourth motor for shutter control" << std::endl << std::endl;
    outfile << "#Pin which is activated to close the shutters" << std::endl;
    outfile << "PIN_CLOSE4 10" << std::endl;
    outfile << "#Pin which is activated to open the shutters" << std::endl;
    outfile << "PIN_OPEN4 11" << std::endl;
    outfile << "#Time at which the shutters are closed (integer format)" << std::endl;
    outfile << "TIME_CLOSE4 72000" << std::endl;
    outfile << "#Time at which the shutters are opened (integer format)" << std::endl;
    outfile << "TIME_OPEN4 28800" << std::endl;

    outfile.close();    // close the file
}

FILE * Parsino::loadFile(const char * pFilename)
{
    FILE * pFile;

    pFile = fopen(pFilename, "r");
    if (pFile == NULL)
        createConfigFile(pFilename);

    pFile = fopen(pFilename, "r");

    return pFile;

}

bool Parsino::check_line(std::string line, std::string searchString)
{
    std::size_t found, raute;
    raute = line.find("#");
    bool retVal = false;

    found = line.find(searchString);
    if (found < raute)
    {
        retVal = true;
    }

    return retVal;
}


bool Parsino::get_integer(std::string searchString, int * pValue)
{
    char buffer[100];
    bool returnVal = false;

    FILE * pFile = this->loadFile(this->pFilename);

    while (!feof(pFile))
    {
        if (fgets(buffer, 100, pFile) == NULL)
            break; // cancel while loop
        //parse line
        std::string str(buffer);
        if(check_line(str, searchString) == true)
        {
            std::size_t found = str.find(" ");
            std::string tempstr = str.substr(found+1);
            *pValue = atoi(tempstr.c_str());
        }
    }
    fclose(pFile);

    return returnVal;
}
