

#include <climits>
#include <iostream>
#include <stack>
#include <tuple>
#include <queue>
using namespace std;
#define inf INT_MAX


template <class T>
class graph {
private:
    int numVertices;
    int maxVertices;
    T* vertices;
    int** edges;
    bool* marks;

public:
    graph(int maxVertic);
    ~graph();
    void makeEmpty();
    bool isEmpty() const;
    bool isFull() const;
    void insertVertex(T vertex);
    void addEdge(T vertex, T target , int weight);
    int wieght(T vertex, T target);
    void clearMarks(T vertex);
    void markVertex(T vertex);
    bool isMarked(T vertex);
    void DFS_traversal(T startVertex);
    int DFS_weight(T startVertex, T endVertex);
    string DFS_shortestPath(T startVertex, T endVertex);
    void BFS_traversal(T startVertex);
    int BFS_weight(T startVertex, T endVertex);
    string BFS_shortestPath(T startVertex, T endVertex);
    void clearAllMarks();
};

template <class T>
graph<T>::graph(int maxVertic) {
    maxVertices = maxVertic;
    numVertices = 0;
    vertices = new T[maxVertices];
    marks = new bool[maxVertices];
    edges = new int*[maxVertices];
    for (int i = 0; i < maxVertices; i++) {
        edges[i] = new int[maxVertices];
        for (int j = 0; j < maxVertices; j++) {
            edges[i][j] = inf;
        }
    }
}

template <class T>
void graph<T>::clearAllMarks()
{
    for (int i = 0; i < numVertices; i++) {
        marks[i] = false;
    }
}

template <class T>
graph<T>::~graph() {
    delete[] vertices;
    delete[] marks;
    for (int i = 0; i < maxVertices; i++) {
        delete[] edges[i];
    }
    delete[] edges;
}

template <class T>
void graph<T>::makeEmpty() {
    numVertices = 0;
    for (int i = 0; i < maxVertices; i++) {
        for (int j = 0; j < maxVertices; j++) {
            edges[i][j] = inf;
        }
    }
}

template <class T>
bool graph<T>::isEmpty() const {
    return numVertices == 0;
}

template <class T>
bool graph<T>::isFull() const {
    return numVertices == maxVertices;
}

template <class T>
void graph<T>::insertVertex(T vertex) {
    if (isFull()) {
        throw "Graph is full";
    }
    vertices[numVertices] = vertex;
    marks[numVertices] = false;
    numVertices++;
}

template <class T>
void graph<T>::addEdge(T vertex, T target , int weight) {
    int vIndex = -1;
    int tIndex = -1;
    for (int i = 0; i < numVertices; i++) {
        if (vertices[i] == vertex) {
            vIndex = i;
        }
        if (vertices[i] == target) {
            tIndex = i;
        }
    }
    if (vIndex == -1 || tIndex == -1) {
        cout << "One or both vertices not found\n";
        return;
    }
    edges[vIndex][tIndex] = weight;
}

template <class T>
int graph<T>::wieght(T vertex, T target) {
    int vIndex = -1;
    int tIndex = -1;
    for (int i = 0; i < numVertices; i++) {
        if (vertices[i] == vertex) {
            vIndex = i;
        }
        if (vertices[i] == target) {
            tIndex = i;
        }
    }
    if (vIndex == -1 || tIndex == -1) {
        cout << "One or both vertices not found\n";
        return inf;
    }
    return edges[vIndex][tIndex];
}

template <class T>
void graph<T>::clearMarks(T vertex) {
    for (int i = 0; i < numVertices; i++) {
        if (vertices[i] == vertex) {
            marks[i] = false;
            return;
        }
    }
}

template <class T>
void graph<T>::markVertex(T vertex) {
    for (int i = 0; i < numVertices; i++) {
        if (vertices[i] == vertex) {
            marks[i] = true;
            return;
        }
    }
}

template <class T>
bool graph<T>::isMarked(T vertex) {
    for (int i = 0; i < numVertices; i++) {
        if (vertices[i] == vertex) {
            return marks[i];
        }
    }
    return false;
}

template <class T>
void graph<T>::DFS_traversal(T startVertex) {
    int startIndex = -1;
    for (int i = 0; i < numVertices; i++) {
        if (vertices[i] == startVertex) {
            startIndex = i;
            break;
        }
    }
    if (startIndex == -1) {
        cout << "Start vertex not found\n";
        return;
    }

    std::stack<int> s;
    s.push(startIndex);
    marks[startIndex] = true;

    while (!s.empty()) {
        int currentIndex = s.top();
        s.pop();
        cout << vertices[currentIndex] << " ";

        for (int i = 0; i < numVertices; i++) {
            if (edges[currentIndex][i] != inf && !marks[i]) {
                s.push(i);
                marks[i] = true;
            }
        }
    }
    clearAllMarks();
}

template <class T>
int graph<T>::DFS_weight(T startVertex, T endVertex){
    int startIndex = -1;
    int endIndex = -1;
    for (int i = 0; i < numVertices; i++) {
        if(vertices[i] == startVertex) {
            startIndex = i;
        }
        if(vertices[i] == endVertex) {
            endIndex = i;
        }
    }
    if(startIndex == -1 || endIndex == -1) {
        cout << "One or both vertices not found\n";
        return inf;
    }

    stack<pair<int, int>> s;
    s.push(make_pair(startIndex, 0));
    marks[startIndex] = true;
    do{
        int currentIndex = s.top().first;
        int currentWeight = s.top().second;
        s.pop();
        

        if(currentIndex == endIndex) {
            clearAllMarks();
            return currentWeight;
        }

        for(int i = 0; i < numVertices; i++) {
            if(edges[currentIndex][i] != inf && !marks[i]) {
                s.push(make_pair(i, edges[currentIndex][i]+currentWeight));
                marks[i] = true;
            }
        }
    }while(!s.empty());

    clearAllMarks();

    return inf;
}

template <class T>
string graph<T>::DFS_shortestPath(T startVertex, T endVertex) {
    int startIndex = -1;
    int endIndex = -1;
    for (int i = 0; i < numVertices; i++) {
        if(vertices[i] == startVertex) {
            startIndex = i;
        }
        if(vertices[i] == endVertex) {
            endIndex = i;
        }
    }
    if(startIndex == -1 || endIndex == -1) {
        cout << "One or both vertices not found\n";
        return "";
    }

    stack<pair<int, int>> s;
    queue<int> path;
    s.push(make_pair(startIndex, 0));
    marks[startIndex] = true;
    do{
        int currentIndex = s.top().first;
        int currentWeight = s.top().second;
        s.pop();
        path.push(currentIndex);
        

        if(currentIndex == endIndex) {
            break;
        }

        for(int i = 0; i < numVertices; i++) {
            if(edges[currentIndex][i] != inf && !marks[i]) {
                s.push(make_pair(i, edges[currentIndex][i]+currentWeight));
                marks[i] = true;
            }
        }
    }while(!s.empty());

    string pathStr = "";
    
    while(!path.empty()) {
        pathStr += vertices[path.front()];
        pathStr += " -> ";
        path.pop();
    }
    pathStr = pathStr.substr(0, pathStr.length() - 4); // Remove last arrow

    clearAllMarks();
    return pathStr;
}

template <class T>
void graph<T>::BFS_traversal(T startVertex) {
    int startIndex = -1;
    for (int i = 0; i < numVertices; i++) {
        if (vertices[i] == startVertex) {
            startIndex = i;
            break;
        }
    }
    if (startIndex == -1) {
        cout << "Start vertex not found\n";
        return;
    }

    std::queue<int> q;
    q.push(startIndex);
    marks[startIndex] = true;

    while (!q.empty()) {
        int currentIndex = q.front();
        q.pop();
        cout << vertices[currentIndex] << " ";

        for (int i = 0; i < numVertices; i++) {
            if (edges[currentIndex][i] != inf && !marks[i]) {
                q.push(i);
                marks[i] = true;
            }
        }
    }
    clearAllMarks();
}

template <class T>
int graph<T>::BFS_weight(T startVertex, T endVertex){
    int minWeight = inf;
    int startIndex = -1;
    int endIndex = -1;
    for (int i = 0; i < numVertices; i++) {
        if(vertices[i] == startVertex) {
            startIndex = i;
        }
        if(vertices[i] == endVertex) {
            endIndex = i;
        }
    }
    if(startIndex == -1 || endIndex == -1) {
        cout << "One or both vertices not found\n";
        return inf;
    }

    queue<pair<int, int>> q;
    q.push(make_pair(startIndex, 0));
    marks[startIndex] = true;
    do{
        int currentIndex = q.front().first;
        int currentWeight = q.front().second;
        q.pop();
        marks[currentIndex] = true;
        if(currentIndex == endIndex) {
            if(currentWeight < minWeight) {
                minWeight = currentWeight;
            }
        }
        for(int i = 0; i < numVertices; i++) {
            if(edges[currentIndex][i] != inf && !marks[i]) {
                q.push(make_pair(i, edges[currentIndex][i]+currentWeight));
            }
        }
    }while(!q.empty());
    clearAllMarks();
    return minWeight;
}

template <class T>
string graph<T>::BFS_shortestPath(T startVertex, T endVertex) {
    int startIndex = -1;
    int endIndex = -1;
    for (int i = 0; i < numVertices; i++) {
        if(vertices[i] == startVertex) {
            startIndex = i;
        }
        if(vertices[i] == endVertex) {
            endIndex = i;
        }
    }
    if(startIndex == -1 || endIndex == -1) {
        cout << "One or both vertices not found\n";
        return "";
    }

    queue<tuple<int, int,int>> q;
    queue<tuple<int, int,int>> path;
    q.push(make_tuple(startIndex,startIndex, 0));
    marks[startIndex] = true;
    int currentIndex = startIndex;
    do{
        int lastIndex = get<1>(q.front());
        currentIndex = get<0>(q.front());
        int currentWeight = get<2>(q.front());
        q.pop();
        marks[currentIndex] = true;
        path.push(make_tuple(currentIndex, lastIndex, currentWeight));

        for(int i = 0; i < numVertices; i++) {
            if(edges[currentIndex][i] != inf && !marks[i]) {
                q.push(make_tuple(i,currentIndex,edges[currentIndex][i]+currentWeight));
            }
        }
    }while(!q.empty());

    string pathStr = "";
    // Reconstruct path from end to start with minimum weight
    // NOT Competed Yet
    clearAllMarks();
    return pathStr;
}

// mohamed ali task









//abdalrahman task












//zeyad task






