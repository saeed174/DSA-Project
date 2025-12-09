#include "formatter.h"
#include <iostream>
#include<string>
#include<vector>

using namespace std;

vector<string> XMLFormatter::tokenize(const string &xml) {
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



bool XMLFormatter::isClosingTag(const string &token) {
    return token.size() > 2 && token[1] == '/';
}


bool XMLFormatter::isSelfClosingTag(const string &token) {
    return token.size() > 2 && token[token.size()-2] == '/';
}

bool XMLFormatter::isOpeningTag(const string &token) {
    return token.size() > 2 &&
           token[0] == '<' &&
           token[1] != '/' &&
           token[token.size() - 2] != '/';
}

string XMLFormatter::indent(int level) {
    return string(level * 4, ' ');   
}




string XMLFormatter::formatXML(const string &xml){
 vector <string> tokens=tokenize(xml);
 string result="";
 stack<int> st;
 
    for(const string &token : tokens){
        if (isClosingTag(token)) {
            if (!st.empty()) 
                st.pop();
            result += indent(st.size()) + token + "\n";
        }


       else if(isOpeningTag(token)){
            result += indent(st.size()) + token + "\n";
            st.push(1);
        }

        else if (isSelfClosingTag(token)) {
            result += indent(st.size()) + token + "\n";
        }

        else {
            string trimmed = token;
            bool emptyText = true;

            for (char c : trimmed)
                if (!isspace(c)) emptyText = false;

            if (!emptyText)
                result += indent(st.size()) + trimmed + "\n";
        }
    }

    return result;

}





