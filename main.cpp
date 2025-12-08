#include <iostream>
#include <string>
#include "xmlCheck.h"
#include <vector>
#include <iostream>
#include <cstddef>
using namespace std;


int main(int argc, char* argv[]) {

    string inputFile = "sample.xml";
    string xmlContent = readXMLFile_withSpaces(inputFile);

    vector<pair<size_t, string>> mismatchedPositions = findMismatchedTags(xmlContent);
    if (mismatchedPositions.empty()) {
        cout << "No mismatched tags found." << endl;
    } else {
        for (int i = 0 ; i < mismatchedPositions.size() ; i++) {
        
            if (mismatchedPositions[i].second == "mismatched tag") {
                cout << "There are mismatched tag at : " << mismatchedPositions[i].first << " ";
                pair<size_t, string> nextIt = mismatchedPositions[i+1];
                cout << nextIt.first << "\n";
                ++i;
            }
            else{
                cout << ("There are "+ mismatchedPositions[i].second + " at : ") << mismatchedPositions[i].first << " ";
            }
        }
        cout << endl;
    }

    string correctMismatched = correctMismatchedTags(xmlContent);
    cout << correctMismatched;

    return 0;
}
