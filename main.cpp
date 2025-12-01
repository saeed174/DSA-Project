#include <iostream>
#include <string>
#include "xmlCheck.h"
#include <vector>
using namespace std;


int main(int argc, char* argv[]) {

    string inputFile = "sample.xml";
    string xmlContent = readXMLFile_withSpaces(inputFile);

    vector<size_t> mismatchedPositions = findMismatchedTags(xmlContent);
    if (mismatchedPositions.empty()) {
        cout << "No mismatched tags found." << endl;
    } else {
        cout << "Mismatched tags found at positions: ";
        for (int pos : mismatchedPositions) {
            cout << pos << " ";
        }
        cout << endl;
    }

    return 0;
}
