#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

struct Node
{
    string name;
    string text;
    vector<Node> kids;

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

class SimpleXmlToJson
{
public:
    string convert(const string &xml)
    {
        stack<Node> st;
        Node root;

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
                    Node done = st.top();
                    st.pop();

                    if (!st.empty())
                        st.top().kids.push_back(done);
                    else
                        root = done;
                }
                else
                {
                    st.push(Node{tag});
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

private:
    void flushText(string &txt, stack<Node> &st)
    {
        trim(txt);
        if (!txt.empty() && !st.empty())
            st.top().text = txt;
        txt.clear();
    }

    void trim(string &s)
    {
        size_t i = 0, j = s.size();
        while (i < j && isspace(s[i])) i++;
        while (j > i && isspace(s[j - 1])) j--;
        s = s.substr(i, j - i);
    }
};

int main()
{
    SimpleXmlToJson converter;

    cout << "Enter XML (end with empty line):\n";

    string xml, line;
    while (true)
    {
        getline(cin, line);
        if (line.empty())
            break;
        xml += line;
    }

    cout << "\nJSON Output:\n";
    cout << converter.convert(xml) << endl;

    return 0;
}
