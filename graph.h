#ifndef GRAPH_H
#define GRAPH_H

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

    bool operator==(const UserData &other) const;
    int active_num = 0;
};

template <class T>
class graph
{
private:
    int numVertices;
    int maxVertices;
    T *vertices;
    int **edges;
    bool *marks;
    map<string, UserData> ID_MAP;
    int active_num;

public:
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
    void printGraph() const;
    void exportToDot(const string &filename) const;

    // Network Analysis
    pair<string, string> Most_Influencer();
    vector<pair<string, string>> MutualFollowers(const vector<string> &users);
    pair<string, string> Most_Active();
    vector<pair<string, string>> Suggest(const string &user);
};

// External helper
void convertDotToJpg(const string &dotFile, const string &jpgFile);

#endif // GRAPH_H