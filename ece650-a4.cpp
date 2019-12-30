#include <minisat/core/SolverTypes.h>
#include <minisat/core/Solver.h>
#include <regex>
#include <vector>
#include <iostream>
#include "VC.hpp"

using namespace std;

vector<string>ExtractEdgeValues(string str, string del){
  vector<string> edges;

  auto start = 0U;
  auto end = str.find(del);
  while (end != string::npos)
  {
    edges.push_back(str.substr(start, end - start));
    start = end + del.length();
    end = str.find(del, start);
  }

  edges.push_back(str.substr(start, end));
  return edges;
}

vector< pair<int,int> > GetEdges(string s){

  pair<int, int> edge;
  vector< pair<int,int> > result;

  try {
    vector<string> edge_list = ExtractEdgeValues(s, "{<");
    edge_list = ExtractEdgeValues(edge_list[1], ">}");
    edge_list = ExtractEdgeValues(edge_list[0], ">,<");

    for (unsigned int i = 0; i < edge_list.size(); i++) {
      vector<string> edgeVector = ExtractEdgeValues(edge_list[i], ",");
      int v1 = stoi(edgeVector[0]);
      int v2 = stoi(edgeVector[1]);
      edge.first = v1;
      // cout << edge.first;
      edge.second = v2;
      // cout << edge.second;

      result.push_back(edge);
    }

  }catch (...){
    // cout << "Error: Invalid Input" << '\n';
    result.clear();
  }
  return result;
}

void TakeInput(){
  char cmd;
  int vertices;
  string edge_string;
  VC* obj = new VC(0);

  while (cin >> cmd) {
    if(cmd == 'V' || cmd == 'v'){
      cin >> vertices;
      if (vertices >= 0) {
        delete obj;
        obj = new VC(vertices);
      }
      else {
        // VC::PrintErrorMessage("Error: Invalid vertex value");
      }
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

    }

    else if(cmd == 'E' || cmd == 'e'){

      cin >> edge_string;
      obj->AddEdges(GetEdges(edge_string));
      obj->getMinimum();
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

    }
    else{
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

      // VC::PrintErrorMessage("Error: Invalid Command");

    }
  }
  delete obj;
  return;
}

int main() {

  TakeInput();
  return 0;
}
