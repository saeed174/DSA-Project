#ifndef GRAPH_H
#define GRAPH_H

#include <climits>
#include <iostream>
#include <tuple>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <set>
#include <stack>

using namespace std;

#define inf INT_MAX

// ================= DATA STRUCTURES =================

struct postData
{
    string body;
    vector<string> topics;
};

struct UserData
{
    string id;
    string name;
    vector<postData> posts;
    vector<string> followers;

    int active_num = 0;

    bool operator==(const UserData &other) const
    {
        return id == other.id;
    }
};

// ================= GRAPH TEMPLATE =================

template <class T>
class graph
{
private:
    int numVertices;
    int maxVertices;
    T *vertices;
    int **edges;
    bool *marks;

public:
    map<string, UserData> ID_MAP;

    graph(int maxVertic);
    ~graph();

    void makeEmpty();
    bool isEmpty() const;
    bool isFull() const;

    void insertVertex(T vertex);
    void addEdge(T vertex, T target, int weight);
    int wieght(T vertex, T target);

    void clearMarks(T vertex);
    void markVertex(T vertex);
    bool isMarked(T vertex);
    void clearAllMarks();

    // XML & Graph Construction
    void convertXMLtoGraph(const string &filename);
    void addEdgesFromFollowers();

    // Debugging & Export
    void printMap() const;
    void exportToDot(const string &filename) const;

    // Network Analysis
    pair<string, string> Most_Influencer();
    vector<pair<string, string>> MutualFollowers(const vector<string> &users);
    pair<string, string> Most_Active();
    vector<pair<string, string>> Suggest(const string &user);

    // Search
    vector<tuple<string, string, string>> WordPostSearch(const string &word);
    vector<tuple<string, string, string>> topicPostSearch(const string &Topic);
};

// ================= TEMPLATE IMPLEMENTATION =================

template <class T>
graph<T>::graph(int maxVertic)
{
    maxVertices = maxVertic;
    numVertices = 0;
    vertices = new T[maxVertices];
    marks = new bool[maxVertices];
    edges = new int *[maxVertices];
    for (int i = 0; i < maxVertices; i++)
    {
        edges[i] = new int[maxVertices];
        for (int j = 0; j < maxVertices; j++)
        {
            edges[i][j] = inf;
        }
    }
}

template <class T>
graph<T>::~graph()
{
    delete[] vertices;
    delete[] marks;
    for (int i = 0; i < maxVertices; i++)
    {
        delete[] edges[i];
    }
    delete[] edges;
}

template <class T>
void graph<T>::makeEmpty()
{
    numVertices = 0;
    for (int i = 0; i < maxVertices; i++)
    {
        for (int j = 0; j < maxVertices; j++)
        {
            edges[i][j] = inf;
        }
    }
}

template <class T>
bool graph<T>::isEmpty() const
{
    return numVertices == 0;
}

template <class T>
bool graph<T>::isFull() const
{
    return numVertices == maxVertices;
}

template <class T>
void graph<T>::insertVertex(T vertex)
{
    if (isFull())
    {
        throw "Graph is full";
    }
    vertices[numVertices] = vertex;
    marks[numVertices] = false;
    numVertices++;
}

template <class T>
void graph<T>::addEdge(T vertex, T target, int weight)
{
    int vIndex = -1;
    int tIndex = -1;
    for (int i = 0; i < numVertices; i++)
    {
        if (vertices[i] == vertex)
        {
            vIndex = i;
        }
        if (vertices[i] == target)
        {
            tIndex = i;
        }
    }
    if (vIndex == -1 || tIndex == -1)
    {
        cout << "One or both vertices not found\n";
        return;
    }
    edges[vIndex][tIndex] = weight;
}

template <class T>
int graph<T>::wieght(T vertex, T target)
{
    int vIndex = -1;
    int tIndex = -1;
    for (int i = 0; i < numVertices; i++)
    {
        if (vertices[i] == vertex)
        {
            vIndex = i;
        }
        if (vertices[i] == target)
        {
            tIndex = i;
        }
    }
    if (vIndex == -1 || tIndex == -1)
    {
        cout << "One or both vertices not found\n";
        return inf;
    }
    return edges[vIndex][tIndex];
}

template <class T>
void graph<T>::clearMarks(T vertex)
{
    for (int i = 0; i < numVertices; i++)
    {
        if (vertices[i] == vertex)
        {
            marks[i] = false;
            return;
        }
    }
}

template <class T>
void graph<T>::markVertex(T vertex)
{
    for (int i = 0; i < numVertices; i++)
    {
        if (vertices[i] == vertex)
        {
            marks[i] = true;
            return;
        }
    }
}

template <class T>
bool graph<T>::isMarked(T vertex)
{
    for (int i = 0; i < numVertices; i++)
    {
        if (vertices[i] == vertex)
        {
            return marks[i];
        }
    }
    return false;
}

template <class T>
void graph<T>::clearAllMarks()
{
    for (int i = 0; i < numVertices; i++)
    {
        marks[i] = false;
    }
}

template <class T>
void graph<T>::convertXMLtoGraph(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    string line;
    bool inUserID = false;
    bool inPosts = false;
    bool inFollowers = false;
    bool inBody = false;
    bool inTopic = false;
    postData currentPost;
    UserData currentUser;

    while (getline(file, line)) {
        if(line.empty()) continue;

        if (line.find( "<user>") != string::npos)
        {
             inUserID = true;
        }
        if (line.find("<id>")!= string::npos && inUserID&& !inFollowers)
        {
            size_t start = line.find("<id>") + 4;
            size_t end = line.find("</id>");
            currentUser.id = line.substr(start, end - start);
        }

        if (line.find("<name>")!= string::npos && inUserID)
        {
            size_t start = line.find("<name>") + 6;
            size_t end = line.find("</name>");
            currentUser.name = line.substr(start, end - start);
        }

        if (line.find("<posts>")!= string::npos)
        {
            inPosts = true;
        }

        if (line.find("<post>") != string::npos && inPosts)
        {
            currentPost = postData();
        }

        if (line.find("<body>") != string::npos)
        {
            inBody = true;
            continue;
        }

        if (line.find("</body>") != string::npos)
        {
            inBody = false;
            continue;
        }

        if (inBody)
        {
            currentPost.body += line + " ";
        }

        if (line.find("<topic>") != string::npos)
        {
            inTopic = true;
            continue;
        }

        if (line.find("</topic>") != string::npos)
        {
            inTopic = false;
            continue;
        }

        if (inTopic)
        {
            currentPost.topics.push_back(line);
        }

        if (line.find("</post>") != string::npos)
        {
            currentUser.posts.push_back(currentPost);
        }

        if (line.find("<followers>")!= string::npos)
        {
            inFollowers = true;
        }
        if (line.find("<id>")!= string::npos && inFollowers)
        {
            size_t start = line.find("<id>") + 4;
            size_t end = line.find("</id>");
            string followerID = line.substr(start, end - start);
            currentUser.followers.push_back(followerID);
        }
        if (line.find("</followers>")!= string::npos)
        {
            inFollowers = false;
        }
        if (line.find("</user>")!= string::npos)
        {
            inUserID = false;
            ID_MAP[currentUser.id] = currentUser;
            insertVertex(currentUser.id);
            currentUser = UserData();
        }
    }
    file.close();
    addEdgesFromFollowers();
}

template <class T>
void graph<T>::addEdgesFromFollowers() {
    for (const auto& pair : ID_MAP) {
        const UserData& user = pair.second;
        for (const string& followerID : user.followers) {
            if (ID_MAP.find(followerID) != ID_MAP.end()) {
                addEdge(followerID, user.id, 1);
                ID_MAP[followerID].active_num++;
            }
        }
    }
}

template <class T>
void graph<T>::printMap() const {
    cout << "\n===== USERS MAP =====\n";
    for (const auto& pair : ID_MAP) {
        const UserData& user = pair.second;
        cout << "User ID   : " << user.id << endl;
        cout << "Name      : " << user.name << endl;
        cout << "Posts     :\n";
        for (const postData& p : user.posts) {
            cout << "  Body   : " << p.body << endl;
            cout << "  Topics : ";
            for (const string& t : p.topics) {
                cout << "[" << t << "] ";
            }
            cout << endl;
        }
        cout << "Followers : ";
        for (const string& f : user.followers) {
            cout << f << " ";
        }
        cout << endl;
        cout << "----------------------\n";
    }
}

template <class T>
void graph<T>::exportToDot(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }
    file << "digraph G {\n";
    file << "  rankdir=LR;\n";
    for (int i = 0; i < numVertices; i++) {
        file << "  \"" << vertices[i] << "\";\n";
    }
    file << "\n";
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (edges[i][j] != inf) {
                file << "  \"" << vertices[i] << "\" -> \"" << vertices[j] << "\";\n";
            }
        }
    }
    file << "}\n";
    file.close();
}

void convertDotToJpg(const string& dotFile, const string& jpgFile)
{
    string command = "dot -Tjpg \"" + dotFile + "\" -o \"" + jpgFile + "\"";
    int result = system(command.c_str());

    if (result != 0) {
        cerr << "Failed to convert DOT to JPG. Make sure Graphviz is installed.\n";
    } else {
        cout << "JPG file created: " << jpgFile << endl;
    }
}

template <class T>
pair<string, string> graph<T>::Most_Influencer()
{
    int maxFollowers = 0;
    UserData Most;
    for (const auto &pair : ID_MAP)
    {
        const UserData &user = pair.second;
        if (user.followers.size() > maxFollowers)
        {
            maxFollowers = user.followers.size();
            Most = user;
        }
    }
    if (maxFollowers == 0)
        return {"", ""};
    return {Most.id, Most.name};
}

template <class T>
vector<pair<string, string>> graph<T>::MutualFollowers(const vector<string> &users)
{
    vector<pair<string, string>> result;
    if (users.size() < 2)
        return result;
    for (string id : users)
    {
        if (ID_MAP.find(id) == ID_MAP.end())
        {
            return {{"ERROR", "ERROR"}};
        }
    }
    vector<string> common = ID_MAP[users[0]].followers;
    for (int i = 1; i < users.size(); i++)
    {
        vector<string> temp;
        for (string f : common)
        {
            for (string u : ID_MAP[users[i]].followers)
            {
                if (f == u)
                {
                    temp.push_back(f);
                    break;
                }
            }
        }
        common = temp;
    }
    for (string id : common)
    {
        if (ID_MAP.find(id) != ID_MAP.end())
        {
            result.push_back({id, ID_MAP[id].name});
        }
    }
    return result;
}

template <class T>
pair<string, string> graph<T>::Most_Active()
{
    int max = -1;
    UserData Most;
    for (const auto &pair : ID_MAP)
    {
        const UserData &user = pair.second;
        if (user.active_num > max)
        {
            max = user.active_num;
            Most = user;
        }
    }
    if (max == -1)
        return {"", ""};
    return {Most.id, Most.name};
}

template <class T>
vector<pair<string, string>> graph<T>::Suggest(const string &user)
{
    vector<pair<string, string>> result;
    if (ID_MAP.find(user) == ID_MAP.end())
        return {{"ERROR", "ERROR"}};
    vector<string> &directFollowers = ID_MAP[user].followers;
    set<string> suggestionsSet;
    for (const string &followerID : directFollowers)
    {
        if (ID_MAP.find(followerID) == ID_MAP.end())
            continue;
        for (const string &fofID : ID_MAP[followerID].followers)
        {
            if (fofID != user && find(directFollowers.begin(), directFollowers.end(), fofID) == directFollowers.end())
                suggestionsSet.insert(fofID);
        }
    }
    for (const string &id : suggestionsSet)
    {
        result.push_back({id, ID_MAP[id].name});
    }
    return result;
}

template <class T>
vector<tuple<string,string,string>> graph<T>::WordPostSearch(const string& word) {
    vector<tuple<string,string,string>> result;
    string searchWord = word;
    transform(searchWord.begin(), searchWord.end(),searchWord.begin(),::tolower);
    for (const auto& pair : ID_MAP)
    {
        const UserData& user = pair.second;
        for (const postData& post : user.posts)
        {
            string body = post.body;
            transform(body.begin(), body.end(), body.begin(), ::tolower);
            if (body.find(searchWord) != string::npos)
            {
                result.push_back(make_tuple(user.id, user.name, post.body));
				break;
            }
        }
    }
    return result;
}

template <class T>
vector<tuple<string,string,string>> graph<T>::topicPostSearch(const string& Topic) {
    vector<tuple<string,string,string>> result;
    string searchTopic = Topic;
    transform(searchTopic.begin(), searchTopic.end(), searchTopic.begin(), ::tolower);
    for (const auto& pair : ID_MAP) {
        const UserData& user = pair.second;
        for (const postData& post : user.posts)
        {
            for (const string& topic : post.topics)
            {
                string lowerTopic = topic;
                transform(lowerTopic.begin(), lowerTopic.end(), lowerTopic.begin(), ::tolower);
                if (lowerTopic.find(searchTopic) != string::npos) {
                    result.push_back(make_tuple(user.id, user.name, post.body));
                    break;
                }
            }
        }
    }
    return result;
}

#endif // GRAPH_H