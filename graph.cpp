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

//** Zeyad *********************/



//****************mostafa */

//**************mostafa ***************/




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

//****************abdelrahman  */

//**************mohamed-hamdy */

//**************mohamed-hamdy */
