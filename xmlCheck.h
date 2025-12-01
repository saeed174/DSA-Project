#ifndef XMLCHECK_H
#define XMLCHECK_H
#include <string>
#include <cstdio>
#include <fstream>
#include <sstream>
#include "Stack.h"
#include <vector>
#include <cctype>
using namespace std;

string readXMLFile_withoutSpaces(string filetag)
{
    ifstream file(filetag);
    if (!file.is_open())
    {
        return "";
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    string compressedContent;

    for (int i = 0; i < content.length(); i++)
    {
        if (!isspace(content[i]))
        {
            compressedContent += content[i];
        }
        else if ((isalpha(content[i - 1]) || isalpha(content[i + 1])) && (content[i - 1] != ' ') && (content[i + 1] != ' '))
        {
            compressedContent += content[i];
        }
    }

    return compressedContent;
}


string readXMLFile_withSpaces(string filetag)
{
    ifstream file(filetag);
    if (!file.is_open())
    {
        return "";
    }

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool checkXMLConsistency(string xmlContent)
{
    Stack<string> tagStack;
    int pos = 0;

    while(pos < xmlContent.length())
    {
        if (xmlContent[pos] == '<')
        {
            int closePos = xmlContent.find('>', pos);
            if (closePos == string::npos)
            {
                return false; // opening tag without closing
            }

            string tag = xmlContent.substr(pos + 1, closePos - pos - 1);
            if (tag[0] != '/')
            {
                // neglegt attributes for simplicity
                size_t spacePos = tag.find(' ');
                if (spacePos != string::npos) {
                    tag = tag.substr(0, spacePos);
                }
                tagStack.push(tag); // Opening tag
            }
            else
            {
                string openingTag = tagStack.pop(); // Closing tag
                if (openingTag.empty() || openingTag != tag.substr(1))
                {
                    return false; // Mismatched tags
                }
            }
            pos = closePos + 1;
        }
        else
        {
            pos++;
        }
    }

    return tagStack.empty();
}

// errors can be occur: unclosed tags, unopened tags, mismatched tags, etc.
vector<size_t> findMismatchedTags(const string &xml) {
    vector<size_t> mismatchedPositions;
    vector<string> tagStack;
    vector<size_t> positionStack;
    size_t pos = 0;
    const size_t n = xml.length();

    while (pos < n) {
        if (xml[pos] == '<') {
            // skip comments/declarations
            if (pos + 4 <= n && xml.compare(pos, 4, "<!--") == 0) {
                size_t endc = xml.find("-->", pos + 4);
                pos = (endc == string::npos) ? n : endc + 3;
                continue;
            }
            if (pos + 2 <= n && xml.compare(pos, 2, "<?") == 0) {
                size_t endp = xml.find("?>", pos + 2);
                pos = (endp == string::npos) ? n : endp + 2;
                continue;
            }
            if (pos + 2 <= n && xml.compare(pos, 2, "<!") == 0) {
                size_t endd = xml.find('>', pos + 2);
                pos = (endd == string::npos) ? n : endd + 1;
                continue;
            }

            size_t closepos = xml.find('>', pos);
            if (closepos == string::npos) {
                mismatchedPositions.push_back(pos); // unclosed tag start
                break;
            }

            string raw = xml.substr(pos + 1, closepos - pos - 1);
            if (raw.empty()) { pos = closepos + 1; continue; }

            bool isClosing = raw[0] == '/';
            // last non-space char to detect self-closing
            size_t lastNonSpace = raw.find_last_not_of(" \t\r\n");
            bool selfClosing = (!isClosing && lastNonSpace != string::npos && raw[lastNonSpace] == '/');

            if (!isClosing) {
                string name = raw;
                if (selfClosing) name = name.substr(0, lastNonSpace);
                size_t spacePos = name.find(' ');
                if (spacePos != string::npos) name = name.substr(0, spacePos);
                if (!name.empty() && !selfClosing) {
                    tagStack.push_back(name);
                    positionStack.push_back(pos);
                }
                // لو هو self-closing مش محتاج حاجة
            } else {
                // closing tag: get name after '/'
                string name = raw.substr(1);
                size_t spacePos = name.find(' ');
                if (spacePos != string::npos) name = name.substr(0, spacePos);
                if (tagStack.empty() || tagStack.back() != name) {
                    // mismatch: report opening (if exists) and this closing
                    if (!positionStack.empty()) {
                        mismatchedPositions.push_back(positionStack.back());
                        positionStack.pop_back();
                        tagStack.pop_back();
                    }
                    mismatchedPositions.push_back(pos);       // '<' of closing
                } else {
                    tagStack.pop_back();
                    positionStack.pop_back();
                }
            }
            pos = closepos + 1;
        } else {
            ++pos;
        }
    }

    // remaining unclosed openings
    while (!positionStack.empty()) {
        mismatchedPositions.push_back(positionStack.back());
        positionStack.pop_back();
    }

    return mismatchedPositions;
}
#endif // XMLCHECK_H