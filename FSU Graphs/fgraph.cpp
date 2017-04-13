/*
    fgraph.cpp
    03/19/17

    Tests input of graphs 
    Includes topological sort driver

    File format:

      Lines at the top of the file begining with '#' are file documentation.
      Once those lines are passed, the remainder of the file is data.

      The data in the file consists of unsigned integers. 
      The first number (after documentation) is the vertex count.
      Then there are numbers in pairs representing "from to" edges.
      The number of edges is limited only by the graph structures possible.
      The numbers (which represent vertices) must be in the range [0..VertexCount).

      For example:
        # 
        # yada dada
        # 
        3 
        1 2 1 0 
        2 0
        0 1 0 2
      represents a graph with 3 vertices (enumerated 0, 1, 2) and edges
      [1 2] [1 0] [2 0] [0 1] [0 2].

      The format of the file is irrelevant, but may be more readable of all
      edges from a given vertex are placed on a single line.

    Copyright 2014, R.C. Lacher
*/


#include <cstdlib>
#include <cstring>
#include <fstream>
#include <ansicodes.h>
#include <graph.h>
#include <graph_util.h> // ShowAL, FileSpec, OutDegreeFrequencyDistribution
#include <topsort.h>
#include <queue.h>

// choose the adjacency list representations
typedef size_t Vertex;
typedef fsu::ALUGraph<Vertex>  Ungraph;
typedef fsu::ALDGraph<Vertex>  Digraph;
// */

/* // choose the adjacency matrix representations
typedef size_t Vertex;
typedef fsu::IAMUGraph        Ungraph;
typedef fsu::IAMDGraph        Digraph;
// */

int main( int argc , char* argv[] )
{
  // std::cout << ANSI_BOLD_BLUE;
  std::cout << "\n  Welcome to fgraph - a graph functionality test\n";
  // std::cout << ANSI_RESET_ALL;
  char* inFile(0);
  char* logFile(0);

  if (argc < 2)
  {
    std::cout << ANSI_BOLD_RED
              << "  Arguments expected:\n"
              << "                      1: input filename (required)\n"
              << "                      2: log filename   (optional)\n";
    FileSpec(std::cout);
    std::cout << ANSI_RESET_ALL;
    exit (EXIT_FAILURE);
  }
  else if (argc == 2)
  {
    std::cout << "  Input file: " << argv[1] << '\n'
	      << "    No log file\n";
    inFile = argv[1];
  }
  else
  {
    std::cout << "  Input file: " << argv[1] << '\n'
	      << "    Log file: " << argv[2] << '\n';
    inFile = argv[1];
    logFile = argv[2];
  }

  Ungraph unGraph;
  Digraph diGraph;

  std::ifstream inStream;
  std::ofstream logStream;
  Vertex v, w; // general purpose vertex variables
  char ch;     // general purpose file read variable

  inStream.open(inFile);
  if (inStream.fail())
  {
    std::cout << "Cannot open file " << inFile << ". Please try again.\n";
    FileSpec(std::cout);
    exit (EXIT_FAILURE);
  }
 
  if (logFile)
  {
    logStream.open(logFile);
    if (logStream.fail())
    {
      std::cerr << "Cannot open file " << logFile << ". Please try again\n";
      exit (EXIT_FAILURE);
    }
  }

  // skip documentation -- '#' in column 1
  ch = inStream.peek();
  while (ch == '#')
  {
    ch = inStream.get();
    while ((ch != '\n') && (!inStream.eof()))
      ch = inStream.get();
    ch = inStream.peek();
  }

  if (logFile)
  {
    logStream << logFile << '\n';
    for (size_t i = 0; i < std::strlen(logFile); ++i)
      logStream << '-';
    logStream << "\n\n";
  }

  std::cout << "Loading from file " << inFile << "..." << std::flush;

  if (logFile)
    logStream << "input from file " << inFile << ":\n";
  inStream >> v;
  if (logFile)
    logStream << v;
  // std::cout << "\n setting vertex sizes\n";
  // unGraph.SetVrtxSize(v - 1);
  // unGraph.PushVertex();
  // diGraph.SetVrtxSize(v - 1);
  // diGraph.PushVertex();
  unGraph.SetVrtxSize(v);
  diGraph.SetVrtxSize(v);

  while (inStream >> v >> w)
  {
    // std::cout << " adding undirected edge [" << v << ',' << w << "]\n";
    unGraph.AddEdge(v,w);
    // std::cout << " adding   directed edge (" << (size_t)v << ',' << (size_t)w << ")\n";
    diGraph.AddEdge(v,w);
    if (logFile)
      logStream << ' ' << v << ' ' << w;
  }
  if (logFile)
    logStream << '\n';
  inStream.close();
  std::cout << "\nLoad complete.\n" << std::flush;

  std::cout << "unGraph.VrtxSize(): " << unGraph.VrtxSize() << '\n';
  std::cout << "unGraph.EdgeSize(): " << unGraph.EdgeSize() << '\n';
  std::cout << "diGraph.VrtxSize(): " << diGraph.VrtxSize() << '\n';
  std::cout << "diGraph.EdgeSize(): " << diGraph.EdgeSize() << std::endl;

  // testing Reverse()
  Digraph reverse = diGraph;
  diGraph.Reverse(reverse);
  std::cout << "diGraph.Dump():\n";
  diGraph.Dump(std::cout);
  std::cout << "reverse.Dump():\n";
  reverse.Dump(std::cout);
  // */

  if (logFile)
  {
    logStream << "\nungraph data:\n"
	      << "\nunGraph.VrtxSize(): " << unGraph.VrtxSize() << '\n'
	      << "unGraph.EdgeSize(): " << unGraph.EdgeSize() << '\n'
	      << "unGraph.Dump(): \n";
    unGraph.Dump(logStream);
    for (size_t i = 0; i < unGraph.VrtxSize(); ++i)
    {
      logStream << "unGraph.InDegree(" << i << ")  == " << unGraph.InDegree(i) << '\n'
		<< "unGraph.OutDegree(" << i << ") == " << unGraph.OutDegree(i) << '\n';
    }
    
    logStream << "\ndigraph data:\n"
	      << "\ndiGraph.VrtxSize(): " << diGraph.VrtxSize() << '\n'
	      << "diGraph.EdgeSize(): " << diGraph.EdgeSize() << '\n'
	      << "diGraph.Dump(): \n";
    diGraph.Dump(logStream);
    for (size_t i = 0; i < diGraph.VrtxSize(); ++i)
    {
      logStream << "diGraph.InDegree(" << i << ")  == " << diGraph.InDegree(i) << '\n'
		<< "diGraph.OutDegree(" << i << ") == " << diGraph.OutDegree(i) << '\n';
    }
  }

  fsu::Queue<int> result;
  bool success =  fsu::TopSort(diGraph, result);

  if (success)
  {
    std::cout << "Top Sort: " ;
    if (logFile)
      logStream << "Top Sort: " ;
  }
  else
  {
    std::cout << "No Top Sort: digraph has cycle\n";
    std::cout << "Partial Top Sort: " ;
    if (logFile)
    {
      logStream << "No Top Sort: digraph has cycle\n";
      logStream << "Partial Top Sort: " ;
    }
  }

  result.Display(std::cout, ' ');
  std::cout << '\n';
 
  std::cout << " Undirected Graph:\n";
  OutDegreeFrequencyDistribution(unGraph);
  std::cout << " Directed Graph:\n";
  OutDegreeFrequencyDistribution(diGraph);

  if (logFile)
  {
    result.Display(logStream, ' ');
    logStream << '\n';
    logStream.close();
  }

  if (logFile)
    std::cout << "See file " << logFile << " for complete test data\n";

  return EXIT_SUCCESS;
}
