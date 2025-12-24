#include <iostream>
#include "SimpleXmlToJson.h"
#include "compression_decompression.h"
#include "formatter.h"
#include "graph.h"
#include <fstream>
#include "minifier.h"
#include "xmlCheck.h"
#include <map>
#include <string>
#include <vector>

using namespace std;
// xml_editor mini -i input_file.xml -o output_file.xml

string getArg(int &i, int argc, char *argv[]) {
  if (i + 1 >= argc) {
    cerr << "Missing argument after " << argv[i] << endl;
    exit(1);
  }
  return argv[++i];
}

/* ===== MAIN ===== */

int main(int argc, char *argv[]) {

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

    if (arg == "-i")
      inputFile = getArg(i, argc, argv);
    else if (arg == "-o")
      outputFile = getArg(i, argc, argv);
    else if (arg == "-f")
      fix = true;
    else if (arg == "-id")
      id = stoi(getArg(i, argc, argv));
    else if (arg == "-ids") {
      string s = getArg(i, argc, argv);
      size_t pos = 0;
      while ((pos = s.find(',')) != string::npos) {
        ids.push_back(stoi(s.substr(0, pos)));
        s.erase(0, pos + 1);
      }
      ids.push_back(stoi(s));
    } else if (arg == "-w"){
      word = getArg(i, argc, argv);
    }
    else if (arg == "-t"){
      topic = getArg(i, argc, argv);
    }
  }



  if (command == "verify") {
    bool valid = checkXMLConsistency(inputFile);
    cout << (valid ? "XML is valid\n" : "XML is invalid\n");
  } else if (command == "format") {
    string xmlcontent = readXMLFile_withSpaces(inputFile);
    XMLFormatter formatter;
    string formatString = formatter.formatXML(xmlcontent);
    if (!outputFile.empty()) {
      ofstream out(outputFile);
      out << formatString;
      out.close();
    }
      cout << formatString;

  } else if (command == "json") {
    SimpleXmlToJson converter;
    string xmlcontent = readXMLFile_withSpaces(inputFile);
    string jsoncontent = converter.convert(xmlcontent);
    if (!outputFile.empty()) {
      ofstream out(outputFile);
      out << jsoncontent;
      out.close();
    }
      cout << jsoncontent;
    
  } else if (command == "mini") {
    XMLMinifier minifier;
    string xmlcontent = readXMLFile_withSpaces(inputFile);
    string minifiedString = minifier.minifyXML(xmlcontent);
    if (!outputFile.empty()) {
      ofstream out(outputFile);
      out << minifiedString;
      out.close();
    }
      cout << minifiedString;
    
  } else if (command == "compress") {
      compressor comp;
      comp.compress_file(inputFile, outputFile);
  } else if (command == "decompress") {
      decompressor decomp;
      decomp.decompress_file(inputFile, outputFile);
  } else if (command == "draw") {
      graph<string> g(100);
      g.convertXMLtoGraph(inputFile);
      g.addEdgesFromFollowers();
      g.exportToDot("output.dot");
      convertDotToJpg("output.dot", "output_file.jpg");
  } else if (command == "most_active") {
      graph<string> g(100);
      g.convertXMLtoGraph(inputFile);
      g.addEdgesFromFollowers();
      pair<string, string> mostActive = g.Most_Active();
      cout << "Most Active User: ID = " << mostActive.first << ", Name = " << mostActive.second << endl;
  } else if (command == "most_influencer") {
      graph<string> g(100);
      g.convertXMLtoGraph(inputFile);
      g.addEdgesFromFollowers();
      pair<string, string> mostInfluencer = g.Most_Influencer();
      cout << "Most Influencer User: ID = " << mostInfluencer.first << ", Name = " << mostInfluencer.second << endl;
  } else if (command == "mutual") {
      graph<string> g(100);
      g.convertXMLtoGraph(inputFile);
      g.addEdgesFromFollowers();
      vector<string> userIDs;
      for (int uid : ids) {
          userIDs.push_back(to_string(uid));
      }
      vector<pair<string, string>> mutualFollowers = g.MutualFollowers(userIDs);
      cout << "Mutual Followers:\n";
      for (const auto& p : mutualFollowers) {
          cout << "ID = " << p.first << ", Name = " << p.second << endl;
      }
  } else if (command == "suggest") {
      graph<string> g(100);
      g.convertXMLtoGraph(inputFile);
      g.addEdgesFromFollowers();
      vector<pair<string, string>> suggestions = g.Suggest(to_string(id));
      cout << "Suggestions for User ID " << id << ":\n";
      for (const auto& p : suggestions) {
          cout << "ID = " << p.first << ", Name = " << p.second << endl;
      }
  } else if (command == "search") {
    graph<string> g(100);
    g.convertXMLtoGraph(inputFile);
    g.addEdgesFromFollowers();
    vector<pair<string, string>> results;
    for (const auto& pair : g.ID_MAP) {
      const UserData& user = pair.second;
      bool match = false;

      if (!word.empty()) {
          if (user.name.find(word) != string::npos)
              match = true;
      }

      if (!topic.empty()) {
          for (const postData& post : user.posts) {
              for (const string& t : post.topics) {
                  if (t.find(topic) != string::npos) { // partial match
                      match = true;
                      break;
                  }
              }
              if (match) break;
          }
      }

      if (match) {
        results.push_back({user.id, user.name});
      }
    }
    cout << "Search Results:\n";
    for (const auto& p : results) {
      cout << "ID = " << p.first << ", Name = " << p.second << endl;
    }
  }
  else if(command == "correct") {
      string xmlcontent = correctMismatchedTags(readXMLFile_withSpaces(inputFile));
      if (!outputFile.empty()) {
        ofstream out(outputFile);
        out << xmlcontent;
        out.close();
      }
        cout << xmlcontent;
  }
  else {
    cerr << "Unknown command: " << command << endl;
  }

  return 0;
}
