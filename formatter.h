#ifndef FORMATTER_H
#define FORMATTER_H

#include <string>
#include<vector>
#include <stack>
using namespace std;
class XMLFormatter {
public:
    // Main function the CLI/GUI will call
    string formatXML(const string &xml);

private:
    // Splits XML into a list of tokens (tags + text)
    vector<string> tokenize(const string &xml);

    // Checks if token is a closing tag </tag>
    bool isClosingTag(const string &token);

    // Checks if token is an opening tag <tag>
    bool isOpeningTag(const string &token);

    // Checks if token is a self-closing tag <tag/>
    bool isSelfClosingTag(const string &token);

    // Creates indentation of depth * 4 spaces
    string indent(int level);
};

#endif