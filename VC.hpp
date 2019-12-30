#pragma once

#include <iostream>
#include <vector>
#include <minisat/core/SolverTypes.h>
#include <minisat/core/Solver.h>

using namespace std;

inline void PrintVertex(vector<int> vertexpath) {
  for (auto v : vertexpath) {
    cout << v << " ";
  }
  cout << endl;
}
class VC {
private:
  int vertices;
  vector< pair<int,int> > edges;

  Minisat::Var ConvertToVar(int r, int c, int x);
  bool isInputValid(vector< pair<int,int> > edges);

  void AddClauseforMinCover(Minisat::Solver& solver, int x);
  void AddClauseforSeparateVertices(Minisat::Solver& solver, int x);
  void AddClauseforSinglePosCover(Minisat::Solver& solver, int x);
  void AddClauseforEachEdge(Minisat::Solver& solver, int x);

  bool SolveGraph(Minisat::Solver& solver, int x);
  vector<int> GetVertexPath(Minisat::Solver& solver, int x);

public:
  VC (int v = 0);
  int GetVertices() const;
  void AddClauses(Minisat::Solver& solver, int x);
  void AddEdges(vector< pair<int,int> > edges);
  void getMinimum();
  static void PrintErrorMessage(string em);

};
