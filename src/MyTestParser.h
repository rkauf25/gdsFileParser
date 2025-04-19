/*
 * MIT License
 *
 * Copyright (c) 2017 EDDR Software, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * Changes:
 * 2017-01-01: First & Last Name: What you did.
 * 2017-06-10: Kevin Nesmith: Initial contribution.
 *
 */

#include <stdlib.h>

#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <iomanip>
#include <fstream>

#include "gdsFileParser.h"

using namespace std;

#define SET_2W setfill('0') << setw(2)

class MyTestParser : public gdsfp::gdsFileParser
{

public:

    ofstream humanReadableFile;

    MyTestParser(std::string humanReadableInputFilePath) {
        humanReadableFile.open(humanReadableInputFilePath, std::ios::out);
        if (!humanReadableFile.is_open()) {
            cerr << "Human readable output file couldn't be opened\n";
            exit(1);
        }
    }

    ~MyTestParser() {
        humanReadableFile.close();
    }

protected:
    virtual void onParsedGDSVersion(unsigned short version) {
        humanReadableFile << "GDSII Version: " << version << endl;
    };
    virtual void onParsedModTime(short year, short month, short day,
                                 short hour, short minute, short sec) {
        humanReadableFile << "Modified Time: " << endl;

        if(year==0 && month==0) {
            humanReadableFile << "\tNot recorded." << endl;
        } else {
            humanReadableFile << "\t" << year << "-" << SET_2W << month <<  "-" << SET_2W <<
                 day << " " << SET_2W << hour << ":" << SET_2W << minute <<
                 ":" << SET_2W << sec << endl;
        }
    };
    virtual void onParsedAccessTime(short year, short month, short day,
                                    short hour, short minute, short sec) {
        humanReadableFile << "Accessed Time: " << endl;

        if(year==0 && month==0) {
            humanReadableFile << "\tNot recorded." << endl;
        } else {
            humanReadableFile << "\t" << year << "-" << SET_2W << month <<  "-" << SET_2W <<
                 day << " " << SET_2W << hour << ":" << SET_2W << minute <<
                 ":" << SET_2W << sec << endl;
        }
    };
    virtual void onParsedLibName(const char *libName) {
        humanReadableFile << "LibName: " << libName << endl;
    };
    virtual void onParsedUnits(double userUnits, double dbUnits) {
        humanReadableFile << "UserUnits: " << setprecision(9) << fixed << userUnits << endl;
        humanReadableFile << "DBUnits: " << setprecision(9) << fixed << dbUnits << endl;
    };
    virtual void onParsedStrName(const char *strName) {
        humanReadableFile << "StrName: " << strName << endl;
    };
    virtual void onParsedBoundaryStart() {
        humanReadableFile << "Boundry start" << endl;
    };
    virtual void onParsedPathStart() {
        humanReadableFile << "Path start" << endl;
    };
    virtual void onParsedBoxStart() {
        humanReadableFile << "Box start" << endl;
    };
    virtual void onParsedEndElement() {
        humanReadableFile << "Element end" << endl;
    };
    virtual void onParsedEndStructure() {
        humanReadableFile << "Structure end" << endl;
    };
    virtual void onParsedEndLib() {
        humanReadableFile << "Lib end" << endl;
    };
    virtual void onParsedColumnsRows(unsigned short columns,
                                     unsigned short rows) {
        humanReadableFile << "Columns: " << columns << " Rows: " << rows << endl;
    };
    virtual void onParsedPathType(unsigned short pathType) {
        humanReadableFile << "PathType: " << pathType << endl;
    };
    virtual void onParsedStrans(short strans) {
        humanReadableFile << "Strans: " << strans << endl;
    };
    virtual void onParsedPresentation(short font, short valign,
                                      short halign) {
        humanReadableFile << "Font: " << font << endl;
        humanReadableFile << "Valign: " << valign << endl;
        humanReadableFile << "Halign: " << halign << endl;
    };
    virtual void onParsedNodeStart() {
        humanReadableFile << "Node start" << endl;
    };
    virtual void onParsedTextStart() {
        humanReadableFile << "Text start" << endl;
    };
    virtual void onParsedSrefStart() {
        humanReadableFile << "Sref start" << endl;
    };
    virtual void onParsedArefStart() {
        humanReadableFile << "Aref start" << endl;
    };
    virtual void onParsedSname(const char *sname) {
        humanReadableFile << "Sname: " << sname << endl;
    };
    virtual void onParsedString(const char *str) {
        humanReadableFile << "String: " << str << endl;
    };
    virtual void onParsedPropValue(const char *propValue) {
        humanReadableFile << "Prop Value: " << propValue << endl;
    };
    virtual void onParsedXY(int count, int x[], int y[]) {
        humanReadableFile << "XY: " << count << endl;

        for(int i=0; i<count; ++i) {
            humanReadableFile << "(" << x[i] << "," << y[i] << ")";
        }

        humanReadableFile << endl;
    };
    virtual void onParsedLayer(unsigned short layer) {
        humanReadableFile << "Layer: " << layer << endl;
    };
    virtual void onParsedWidth(int width) {
        humanReadableFile << "Width: " << width << endl;
    };
    virtual void onParsedDataType(unsigned short dataType) {
        humanReadableFile << "Data Type: " << dataType << endl;
    };
    virtual void onParsedTextType(unsigned short textType) {
        humanReadableFile << "Text Type: " << textType << endl;
    };
    virtual void onParsedAngle(double angle) {
        humanReadableFile << "Angle: " << angle << endl;
    };
    virtual void onParsedMag(double mag) {
        humanReadableFile << "Mag: " << mag << endl;
    };
    virtual void onParsedBeginExtension(unsigned short bext) {
        humanReadableFile << "Begin Extension: " << bext << endl;
    };
    virtual void onParsedEndExtension(unsigned short eext) {
        humanReadableFile << "End Extension: " << eext << endl;
    };
    virtual void onParsedPropertyNumber(unsigned short propNum) {
        humanReadableFile << "Property Number: " << propNum << endl;
    };
    virtual void onParsedNodeType(unsigned short nodeType) {
        humanReadableFile << "Node Type: " << nodeType << endl;
    };
    virtual void onParsedBoxType(unsigned short boxType) {
        humanReadableFile << "Box Type: " << boxType << endl;
    };
};


// ****************************************************************************
// main()
//
// This is the top level function that tests the parser.
// ****************************************************************************
// int main(int argc, char *argv[])
// {
//     if(argc<2) {
//         cerr << "Missing GDSII file as the only parameter." << endl;
//         cerr << "Usage: ./testParser /path/to/file.gds" << endl;
//         return 1;
//     }

//     MyTestParser parser;
//     return parser.parse(argv[1]);
// }

