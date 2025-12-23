#ifndef XMLCHECK_H
#define XMLCHECK_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>
#include <algorithm>
#include "Stack.h"

using namespace std;

// ================= READ XML =================

inline string readXMLFile_withoutSpaces(const string& filetag)
{
    ifstream file(filetag);
    if (!file.is_open()) return "";

    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    string compressed;

    for (size_t i = 0; i < content.length(); i++)
    {
        if (!isspace(content[i]))
            compressed += content[i];
        else if (i > 0 && i + 1 < content.size() &&
                 isalpha(content[i - 1]) && isalpha(content[i + 1]))
            compressed += content[i];
    }
    return compressed;
}

inline string readXMLFile_withSpaces(const string& filetag)
{
    ifstream file(filetag);
    if (!file.is_open()) return "";

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// ================= CHECK CONSISTENCY =================

inline bool checkXMLConsistency(const string& xmlContent)
{
    Stack<string> tagStack;
    size_t pos = 0;

    while (pos < xmlContent.length())
    {
        if (xmlContent[pos] == '<')
        {
            size_t closePos = xmlContent.find('>', pos);
            if (closePos == string::npos) return false;

            string tag = xmlContent.substr(pos + 1, closePos - pos - 1);
            if (!tag.empty() && tag[0] != '/')
            {
                size_t spacePos = tag.find(' ');
                if (spacePos != string::npos)
                    tag = tag.substr(0, spacePos);
                tagStack.push(tag);
            }
            else
            {
                string open = tagStack.pop();
                if (open.empty() || open != tag.substr(1))
                    return false;
            }
            pos = closePos + 1;
        }
        else pos++;
    }
    return tagStack.empty();
}

// ================= FIND MISMATCH =================

inline vector<pair<size_t, string>>
findMismatchedTags(const string& xml)
{
    vector<pair<size_t, string>> result;
    vector<string> stack;
    vector<size_t> posStack;

    for (size_t i = 0; i < xml.size(); i++)
    {
        if (xml[i] != '<') continue;

        size_t end = xml.find('>', i);
        if (end == string::npos)
        {
            result.emplace_back(i, "unclosed tag");
            break;
        }

        string raw = xml.substr(i + 1, end - i - 1);
        if (raw.empty()) continue;

        bool closing = raw[0] == '/';
        bool selfClose = raw.back() == '/';

        string name = raw;
        if (closing) name = name.substr(1);
        if (selfClose) name.pop_back();

        size_t sp = name.find(' ');
        if (sp != string::npos) name = name.substr(0, sp);

        if (!closing && !selfClose)
        {
            stack.push_back(name);
            posStack.push_back(i);
        }
        else if (closing)
        {
            if (stack.empty() || stack.back() != name)
                result.emplace_back(i, "mismatched tag");
            else
            {
                stack.pop_back();
                posStack.pop_back();
            }
        }
        i = end;
    }

    while (!posStack.empty())
    {
        result.emplace_back(posStack.back(), "unclosed tag");
        posStack.pop_back();
    }
    return result;
}

// ================= CORRECT MISMATCH =================

inline string correctMismatchedTags(string xml)
{
    vector<string> stack;

    for (size_t i = 0; i < xml.size(); i++)
    {
        if (xml[i] != '<') continue;

        size_t end = xml.find('>', i);
        if (end == string::npos) break;

        string raw = xml.substr(i + 1, end - i - 1);
        if (raw.empty()) continue;

        bool closing = raw[0] == '/';
        string name = closing ? raw.substr(1) : raw;

        size_t sp = name.find(' ');
        if (sp != string::npos) name = name.substr(0, sp);

        if (!closing)
            stack.push_back(name);
        else if (!stack.empty() && stack.back() != name)
        {
            xml.replace(i + 2, name.size(), stack.back());
            stack.pop_back();
        }
        else if (!stack.empty())
            stack.pop_back();

        i = end;
    }
    return xml;
}

#endif // XMLCHECK_H
