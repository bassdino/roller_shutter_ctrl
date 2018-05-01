/*
 * Parsino.h
 *
 *  Created on: 18.04.2017
 *      Author: Michael
 */

#ifndef PARSINO_H_
#define PARSINO_H_

#include <cstdio>
#include <string>         // std::string
#include <cstdlib>

#define FILENAME_LENGTH 33

class Parsino
{
private:
    char filename[FILENAME_LENGTH];
    char * pFilename;
    int to_int(char const *s);
    bool check_line(std::string line, std::string searchString);
    void createConfigFile(const char * pFilename);
public:
    Parsino(void);
    Parsino(const char * pFilename);
    FILE * loadFile(const char * pFilename);
    virtual ~Parsino();
    bool get_integer(std::string searchString, int * pValue);
};

#endif /* PARSINO_H_ */
