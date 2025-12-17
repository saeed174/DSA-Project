#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "xmlCheck.h"
#include "formatter.h"
#include "SimpleXmlToJson.h"
#include "minifier.h"
#include "compression_decompression.h"
#include "graph.cpp"

using namespace std;


string getArg(int& i, int argc, char* argv[]) {
    if (i + 1 >= argc) {
        cerr << "Missing argument after " << argv[i] << endl;
        exit(1);
    }
    return argv[++i];
}

/* ===== MAIN ===== */

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cout << "Usage: xml_editor <command> [options]\n";
        return 0;
    }

    string command = argv[1];

    string inputFile, outputFile;
    bool fix = false;
    int id = -1;
    vector<int> ids;
    string word, topic;

    for (int i = 2; i < argc; i++) {
        string arg = argv[i];

        if (arg == "-i") inputFile = getArg(i, argc, argv);
        else if (arg == "-o") outputFile = getArg(i, argc, argv);
        else if (arg == "-f") fix = true;
        else if (arg == "-id") id = stoi(getArg(i, argc, argv));
        else if (arg == "-ids") {
            string s = getArg(i, argc, argv);
            size_t pos = 0;
            while ((pos = s.find(',')) != string::npos) {
                ids.push_back(stoi(s.substr(0, pos)));
                s.erase(0, pos + 1);
            }
            ids.push_back(stoi(s));
        }
        else if (arg == "-w") word = getArg(i, argc, argv);
        else if (arg == "-t") topic = getArg(i, argc, argv);
    }

    /* ===== COMMAND DISPATCH ===== */

    if (command == "verify") {
        bool valid = checkXMLConsistency(inputFile);
        cout << (valid ? "XML is valid\n" : "XML is invalid\n");
    }
    else if (command == "format") {
        string xmlcontent = readXMLFile_withSpaces(inputFile);
        XMLFormatter formatter;
        string formatString  = formatter.formatXML(xmlcontent);
        if (!outputFile.empty()) {
            ofstream out(outputFile);
            out << formatString;
            out.close();
        } else {
            cout << formatString;
        }
    }
    else if (command == "json") {
        SimpleXmlToJson converter;
        string xmlcontent = readXMLFile_withSpaces(inputFile);
        string jsoncontent = converter.convert(xmlcontent);
        if (!outputFile.empty()) {
            ofstream out(outputFile);
            out << jsoncontent;
            out.close();
        } else {
            cout << jsoncontent;
        }
    }
    else if (command == "mini") {
        XMLMinifier minifier;
        string xmlcontent = readXMLFile_withSpaces(inputFile);
        string minifiedString  = minifier.minifyXML(xmlcontent);
        if (!outputFile.empty()) {
            ofstream out(outputFile);
            out << minifiedString;
            out.close();
        } else {
            cout << minifiedString;
        }
    }
    else if (command == "compress") {
        compressor comp;
        comp.compress_file(inputFile, outputFile);
    }
    else if (command == "decompress") {
        decompressor decomp;
        decomp.decompress_file(inputFile, outputFile);
    }
    else if (command == "draw") {
        graph<string> g(100);
        g.convertXMLtoGraph(inputFile);
        g.addEdgesFromFollowers();
        g.exportToDot(outputFile);
    }
    else if (command == "most_active") {
        
    }
    else if (command == "most_influencer") {
    }
    else if (command == "mutual") {
    }
    else if (command == "suggest") {
    }
    else if (command == "search") {
    }
    else {
        cerr << "Unknown command: " << command << endl;
    }

    return 0;
}
