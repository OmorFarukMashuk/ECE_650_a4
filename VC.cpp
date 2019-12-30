#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <chrono>
#include <cmath>
#include "VC.hpp"
#include <minisat/core/SolverTypes.h>
#include <minisat/core/Solver.h>

using namespace std;

VC::VC( int v ):
vertices(v) {

}

void InitializeVars(int vertices, int x, Minisat::Solver& solver){
  for (int r = 0; r < vertices; r++) {
    for (int c = 0; c < x; c++) {
      solver.newVar();
    }
  }
}

void VC::PrintErrorMessage(string em){
  // std::cerr << em << endl;
  return;
}
Minisat::Var VC::ConvertToVar(int r, int c, int x) {
  int columns = x;
  int newvar = r * columns + c;
  return newvar;
}


bool VC::isInputValid(vector< pair<int,int> > edges) {
  for ( auto& e : edges) {
    if (this->vertices <= e.first || this->vertices <= e.second || e.first < 0 || e.second < 0) {
      // PrintErrorMessage("Error: vertex does not exist");
      return false;
    }
  }
  return true;
}

void VC::AddClauseforMinCover(Minisat::Solver& solver, int x){
  int i = 0;
  while( i < x){
    Minisat::vec<Minisat::Lit> clause;
    int n = 0;
    while( n < this->vertices){
      clause.push(Minisat::mkLit(ConvertToVar(n,i,x)));
      n++;
    }
    solver.addClause(clause);
    i++;
  }
}


void VC::AddClauseforSeparateVertices(Minisat::Solver& solver, int x){

  int i = 0;
  while(i < this->vertices){
    int z = 0;
    while(z < x){
      int y = 0;
      while(y < z){
        solver.addClause( ~Minisat::mkLit(ConvertToVar(i,y,x)), ~Minisat::mkLit(ConvertToVar(i,z,x)));
        y++;
      }
      z++;
    }
    i++;
  }
}


void VC::AddClauseforSinglePosCover(Minisat::Solver& solver, int x){

  int i = 0;
  while(i < x){
    int z = 0;
    while(z < this->vertices){
      int y = 0;
      while(y < z){
        solver.addClause( ~Minisat::mkLit(ConvertToVar(y,i,x)), ~Minisat::mkLit(ConvertToVar(z,i,x)));
        y++;
      }
      z++;
    }
    i++;
  }
}


void VC::AddClauseforEachEdge(Minisat::Solver& solver, int x){

  for(auto& e : edges) {
    Minisat::vec<Minisat::Lit> literals;
    int i = 0;
    while(i < x){
      literals.push(Minisat::mkLit(ConvertToVar(e.first, i, x)));
      literals.push(Minisat::mkLit(ConvertToVar(e.second, i, x)));
      i++;
    }
    solver.addClause(literals);
  }
}

void VC::AddClauses(Minisat::Solver& solver, int x) {

  AddClauseforMinCover(solver, x);
  AddClauseforSeparateVertices(solver, x);
  AddClauseforSinglePosCover(solver, x);
  AddClauseforEachEdge(solver, x);

  return;
}

bool VC::SolveGraph(Minisat::Solver& solver, int x) {

  InitializeVars(vertices,x, solver);

  AddClauses(solver, x);
  auto sat = solver.solve();

  return sat;
}

vector<int> VC::GetVertexPath(Minisat::Solver& solver, int x) {

  vector<int> path;

  int r = 0;
  while(r < this->vertices){
    // for (int r = 0; r < vertices; r++) {
    int c = 0;
    while(c < x){
      // for (int c = 0; c < x; c++) {
      if (solver.modelValue(ConvertToVar(r,c,x)) == Minisat::l_True) {
        path.push_back(r);
      }
      c++;
    }
    r++;
  }
  sort(path.begin(), path.end());
  return path;
}

int VC::GetVertices() const {
  return this->vertices;
}

void VC::AddEdges(vector< pair<int,int> > e) {
  if ( isInputValid(e) ) {
    edges = e;
  }
  return;
}


void VC::getMinimum() {
  if (edges.empty()) {
    return;
  }

  int l = 0;
  int r = this->vertices;
  int m;

  int pth[this->vertices];
  vector<int> vertexpath[this->vertices];
  fill_n(pth, vertices, -1);

  bool found = false;

  while(l == r || r > l){
    m = ceil((r+l)/2);


    Minisat::Solver solver;

    pth[m] = SolveGraph(solver, m);

    if (pth[m]) {
      vertexpath[m] = GetVertexPath(solver, m);
    }

    // If SAT and result[x-1] are UNSAT, the minimum is found
    if (pth[m] == 1 && pth[m-1] == 0 && m != 0) {
      PrintVertex(vertexpath[m]);

      found = true;
      break;
    }

    // If UNSAT and result[x+1] are SAT, the minimum is found
    if (pth[m] == 0 && pth[m+1] == 1 && m != this->vertices) {
      PrintVertex(vertexpath[m+1]);

      found = true;
      break;
    }

    if (pth[m]) {

      // cout << "if result[m]: " << m << '\n';
      r = m - 1;
      // cout << "if r: " << r << '\n';
    }
    else {
      // cout << "else result[m]: " << m << '\n';
      l = m + 1;
      // cout << "else l: " << l << '\n';

    }
  }

  if(!found){
    std::cerr << "Error: UNSAT" << endl;
    return;
  }
  return;

}
