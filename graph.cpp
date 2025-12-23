#include <graph.h>
#include <filesystem>
#include <climits>
#include <iostream>
#include <stack>
#include <tuple>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <set>
using namespace std;
#define inf INT_MAX

//*******************************Mohamed*************** */
bool UserData::operator==(const UserData &other) const
{
    return id == other.id;
}

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
void graph<T>::clearAllMarks()
{
    for (int i = 0; i < numVertices; i++)
    {
        marks[i] = false;
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
//*******************************mohamed*************** */

//*************Zeyad*************** */
// xml file to store graph data

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

    // Read the file line by line
    while (getline(file, line)) {

        if(line.empty()) continue;                     // Skip empty lines

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
            // Store the current user data in the map
            ID_MAP[currentUser.id] = currentUser;
            // Clear currentUser for the next user
            insertVertex(currentUser.id);
            currentUser = UserData();
        }
        

    }
    file.close();
    addEdgesFromFollowers();
}

// add edge between users based on followers
template <class T>
void graph<T>::addEdgesFromFollowers() {
    for (const auto& pair : ID_MAP) {
        const UserData& user = pair.second;
        for (const string& followerID : user.followers) {
            if (ID_MAP.find(followerID) != ID_MAP.end()) {
                addEdge(followerID, user.id, 1); // Assuming weight of 1 for each follower relationship
                ID_MAP[followerID].active_num++;
            }
        }
    }
}
// print the user map(optional for debugging)
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

    file << "digraph G {\n";       // directed graph
    file << "  rankdir=LR;\n";     // left-to-right layout (optional)

    // Define all vertices
    for (int i = 0; i < numVertices; i++) {
        file << "  \"" << vertices[i] << "\";\n";
    }

    file << "\n";

    // Define all edges without weight
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (edges[i][j] != inf) {
                file << "  \"" << vertices[i] << "\" -> \"" << vertices[j] << "\";\n";
            }
        }
    }

    file << "}\n";
    file.close();

    cout << "DOT file created: " << filename << endl;
}

//** Zeyad *********************/



//****************mostafa */

//**************mostafa ***************/
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



//************Mohamed**** */
// Network Analysis Functions Implementation Here

template <class T>
pair<string, string> graph<T>::Most_Influencer()
{
    int maxFollowers = 0;
    UserData Most;
    postData p;
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

    // Need at least 2 users
    if (users.size() < 2)
        return result;

    for (string id : users)
    {
        if (ID_MAP.find(id) == ID_MAP.end())
        {
            // wrong ID found → return empty
            return {{"ERROR", "ERROR"}};
        }
    }

    // Start with followers of first user
    vector<string> common = ID_MAP[users[0]].followers;

    // Find intersection
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

    // Convert to (ID, Name)
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

//************Mohamed**** */

//****************abdelrahman  */

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
            }
        }
    }

    return result;
}

//****************abdelrahman  */

//**************mohamed-hamdy */

template <class T>
vector<tuple<string,string,string>> graph<T>::topicPostSearch(const string& Topic) {
    vector<tuple<string,string,string>> result;

    string searchTopic = Topic;
    transform(searchTopic.begin(), searchTopic.end(), searchTopic.begin(), ::tolower);

    for (const auto& pair : ID_MAP) {
        const UserData& user = pair.second;

        for (const postData& post : user.posts) 
        {
            for (const string& topic : post.topics) // Loop through all topics
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

//**************mohamed-hamdy */
