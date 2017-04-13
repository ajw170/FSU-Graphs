/* 
    fbfsurvey_ug.cpp
    Chris Lacher
    08/30/16

    Copyright 2016, R.C. Lacher
*/

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <graph.h>
#include <graph_util.h>  // Load, GraphTypeName, FileSpec
#include <bfsurvey.h>
#include <survey_util.h> // Arguments, Writedata, Levelorder

// undirected adjacency list representation
typedef size_t                  Vertex;
typedef fsu::ALUGraph <Vertex>  GraphType;
const char typecode [3] = {'U','L'};  // Undirected adjacency List
// */

/* // directed adjacency list representation
typedef size_t                  Vertex;
typedef fsu::ALDGraph <Vertex>  GraphType;
const char typecode [3] = {'D','L'};  // Directed adjacency List
// */

int main( int argc , char* argv[] )
{
  if (argc < 5)
  {
    Arguments(std::cout);
    FileSpec(std::cout); 
    return (EXIT_FAILURE);
  }

  // argv[1] is the graph file
  std::cout << "\nBegin BFSurvey functionality test\n\n"
	    << "graph type: " << GraphTypeName(typecode) << '\n';
  char* inFile(nullptr);
  inFile = argv[1];

  // argv[2] determines full survey or single search
  bool survey = 1;
  if ('s' == argv[2][0])
  {
    survey = 0;
  }

  // argv[3] is the start vertex 
  size_t start = atoi(argv[3]);

  // argv[4] is the trace flag
  bool traceQue = atoi(argv[4]);

  // create graph
  GraphType g;
  if (!Load (inFile,g))
  {
    std::cout << " ** Unable to load file " << inFile << '\n'
	      << "    check file  and filename and try again";
    FileSpec(std::cout);
    return (EXIT_FAILURE);
  }
  else 
  {
    std::cout << " Load complete\n";
    std::cout << " Input file: " << argv[1] << '\n';
    std::cout << "  VrtxSize = " << g.VrtxSize() << '\n'
	      << "  EdgeSize = " << g.EdgeSize() << '\n';
  }

  // perform bfsurvey
  fsu::BFSurvey <GraphType> bfs(g,start);
  bfs.traceQue = traceQue;
  if (survey) // do complete survey
  {
    bfs.Search();
  }
  else // search from start only
  {
    if (traceQue) bfs.ShowQueSetup(std::cout);
    if (traceQue) bfs.ShowQue(std::cout);
    bfs.Reset();
    bfs.Search(start);
  }

  WriteData(bfs,std::cout);
  std::cout << '\n';
  Levelorder(bfs,std::cout);
  std::cout << "\nEnd BFSurvey functionality test\n";
}
