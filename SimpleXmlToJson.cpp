#include "SimpleXmlToJson.h"
#include <stack>
#include <vector>
#include <cctype>

using namespace std;

struct XNode
{
    string name;
    string text;
    vector<XNode> kids;

    string toJson(int indent = 0) const
    {
        string pad(indent, ' ');
        string out = pad + "\"" + name + "\": ";

        if (!kids.empty())
        {
            out += "{\n";
            for (size_t i = 0; i < kids.size(); i++)
            {
                out += kids[i].toJson(indent + 2);
                if (i + 1 < kids.size())
                    out += ",";
                out += "\n";
            }
            out += pad + "}";
        }
        else
        {
            out += "\"" + text + "\"";
        }
        return out;
    }
};

static void trim(string &s)
{
    size_t i = 0, j = s.size();
    while (i < j && isspace(s[i])) i++;
    while (j > i && isspace(s[j - 1])) j--;
    s = s.substr(i, j - i);
}

static void flushText(string &txt, stack<XNode> &st)
{
    trim(txt);
    if (!txt.empty() && !st.empty())
        st.top().text = txt;
    txt.clear();
}

// ✅ التعريف الحقيقي للدالة
string SimpleXmlToJson::convert(const string &xml)
{
    stack<XNode> st;
    XNode root;

    string tag, buffer;
    bool readingTag = false;
    bool closing = false;

    for (char c : xml)
    {
        if (c == '<')
        {
            flushText(buffer, st);
            readingTag = true;
            closing = false;
            tag.clear();
        }
        else if (c == '>')
        {
            readingTag = false;

            if (closing)
            {
                XNode done = st.top();
                st.pop();

                if (!st.empty())
                    st.top().kids.push_back(done);
                else
                    root = done;
            }
            else
            {
                st.push(XNode{tag});
            }
        }
        else if (readingTag)
        {
            if (c == '/')
                closing = true;
            else
                tag += c;
        }
        else
        {
            buffer += c;
        }
    }

    return "{\n" + root.toJson(2) + "\n}";
}
