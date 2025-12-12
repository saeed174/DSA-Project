#include "minifier.h"
#include <iostream>
#include<string>
#include<vector>

using namespace std;

vector<string> XMLMinifier::tokenize(const string &xml) {
    vector<string> tokens;
    
    string current = "";
    bool insideTag = false;

    for (char c : xml) {
        if (c == '<') {
            if (!current.empty() )
                tokens.push_back(current);
            current = "<";
            insideTag = true;
        }
        else if (c == '>') {
            current += ">";
            tokens.push_back(current);
            current = "";
            insideTag = false;
        }
        else {
            current += c;
        }
    }

    return tokens;
}



bool XMLMinifier::isClosingTag(const string &token) {
    return token.size() > 2 && token[1] == '/';
}


bool XMLMinifier::isSelfClosingTag(const string &token) {
    return token.size() > 2 && token[token.size()-2] == '/';
}

bool XMLMinifier::isOpeningTag(const string &token) {
    return token.size() > 2 &&
           token[0] == '<' &&
           token[1] != '/' &&
           token[token.size() - 2] != '/';
}


string XMLMinifier::minifyXML(const string &xml) {
    vector<string> tokens = tokenize(xml);
    string result = "";

    for (const string &token : tokens) {
        
        if (isOpeningTag(token) || isClosingTag(token) || isSelfClosingTag(token)) {
            result += token;
            continue;
        }

        int start = 0;
        int end = token.size() - 1;

        while (start <= end && (token[start] == ' ' || token[start] == '\n' || token[start] == '\t' || token[start] == '\r'))
            start++;
        while (end >= start && (token[end] == ' ' || token[end] == '\n' || token[end] == '\t' || token[end] == '\r'))
            end--;

        for (int i = start; i <= end; i++)
            result += token[i];
    }

    return result;
}

