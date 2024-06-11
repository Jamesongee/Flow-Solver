#include <iostream>
#include <string>
#include <cassert>
#include "provided_grid.h"
#include <ctime>


int main(int argc, char* argv[]) {
  //clock_t t;
  //t = clock();
  std::string filename;

  // default behavior
  bool one_solution = true;

  // other options
  char color = ' ';
  bool one_color = false;
  bool all_paths = false;
  bool all_solutions = false;
  bool covers_board = false;

  
  // =======================================
  // FIRST:  Parse the arguments
  for (int i = 1; i < argc; i++) {
    if (std::string(argv[i]) == std::string("--one_color")) {
      // the next argument is the color
      i++;
      assert (i < argc);
      assert (std::string(argv[i]).size() == 1);
      color = argv[i][0];
      one_color = true;
      one_solution = false;
      assert (all_paths == false && all_solutions == false && covers_board == false);
    } else if (std::string(argv[i]) == std::string("--all_paths")) {
      one_solution = false;
      all_paths = true;
      assert (one_color == false && all_solutions == false && covers_board == false);
    } else if (std::string(argv[i]) == std::string("--all_solutions")) {
      one_solution = false;
      all_solutions = true;
      assert (one_color == false && all_paths == false);
    } else if (std::string(argv[i]) == std::string("--covers_board")) {
      covers_board = true;
      assert (one_color == false && all_paths == false);
    } else {
      // must be the name of the file
      assert (filename == "");
      filename = argv[i];
    }
  }

  
  // =======================================
  // NEXT:  Load the puzzle file
  if (filename == "") {
    std::cerr << "ERROR!  Missing the filename" << std::endl;
    exit(0);
  }
  Grid g(filename);

  // =======================================
  // FINALLY:  SOLVE & PRINT -- this is placeholder code
  

  /*
  This case covers if only the puzzle file is inputed with no other specificaction.
  If there is any amount of solutions, it will print one solution. However if there
  are no solutions, the original board will be printed and 'no solutions' will 
  be displayed
  */
  
  if (one_solution){
    std::vector<int> v;
    std::vector<char> c;
    std::vector<std::vector<int>> visited;
    visited.resize(g.height(), std::vector<int>(g.width()));
    for(int i=0;i<g.height();i++){
      for (int j=0;j<g.width();j++){
        visited[i][j]=0;
      }
    }
    int total=0;
    int pathcount=0;
    c=g.returnchar();
    v=g.findcolor(c[0]);
    int index=0;
    bool cover=false;
    bool base=false;
    bool all=false;
    g.allsolutions(filename,v[0],v[1],visited,pathcount,v,c[0],c,index,total,cover,base,all);
    if (base==false){
      g.print();
      std::cout <<std::endl<<"No Solutions"<< std::endl;
    }
  }
  
  /*
  If '--one_color' is specified along with the color and puzzle, this part will
  print all of the possible paths for the color in that puzzle and will display
  the total number of paths found for that color. If no paths are found, it will
  display that there were no paths for that color
  */

  if (one_color){
    std::vector<int> v;
    v=g.findcolor(color);
    std::vector<std::vector<int>> visited;
    visited.resize(g.height(), std::vector<int>(g.width()));
    for(int i=0;i<g.height();i++){
      for (int j=0;j<g.width();j++){
        visited[i][j]=0;
      }
    }
    int pathcount=0;
    g.onecolor(filename,v[0],v[1],visited,pathcount,v,color);
    if (pathcount>0){
      std::cout <<"Found "<<pathcount <<" Path(s) for Color '"<<color<<"'"<< std::endl;
    }
    else{
      std::cout << "No Paths for Color '" << color << "'" << std::endl;
    }
  }

  /*
  If --all_paths is specified along with the puzzle, the program will output all
  paths for each color and then display how many total paths there are. If there
  are no paths, it will output that there are no paths.
  */

  if (all_paths){
    std::vector<int> v;
    std::vector<char> c;
    std::vector<std::vector<int>> visited;
    visited.resize(g.height(), std::vector<int>(g.width()));
    for(int i=0;i<g.height();i++){
      for (int j=0;j<g.width();j++){
        visited[i][j]=0;
      }
    }
    int pathcount=0;
    c=g.returnchar();
    for (int i=0;i<(int)c.size();i++){
      v.clear();
      v=g.findcolor(c[i]);
      g.onecolor(filename,v[0],v[1],visited,pathcount,v,c[i]);
    }
    if (pathcount>0){
      std::cout <<"Found "<<pathcount <<" Path(s)"<< std::endl;
    }
    else{
      std::cout << "No Paths"<< std::endl;
    }
  }

  /*
  if --all_solutions is specified by itself with a puzzle, the program will display
  every single solution to the puzzle and then say how many solutions there are
  to said puzzle according to the function. If there are no solutions to the 
  puzzle, 'no solutions' will be displayed
  */

  if(all_solutions&&!covers_board){
    std::vector<int> v;
    std::vector<char> c;
    std::vector<std::vector<int>> visited;
    visited.resize(g.height(), std::vector<int>(g.width()));
    for(int i=0;i<g.height();i++){
      for (int j=0;j<g.width();j++){
        visited[i][j]=0;
      }
    }
    int total=0;
    int pathcount=0;
    c=g.returnchar();
    v=g.findcolor(c[0]);
    int index=0;
    bool cover=false;
    bool base=true;
    bool all=true;
    g.allsolutions(filename,v[0],v[1],visited,pathcount,v,c[0],c,index,total,cover,base,all);
    if (pathcount>0){
      std::cout <<"Found "<<pathcount <<" Solution(s)"<< std::endl;
    }
    else{
      std::cout << "No Solutions"<< std::endl;
    } 
  }

  /*
  If --all_solutions and --covers_board are specified with a puzzle, the program
  will dispay all solutions that cover the entire board. Covering the board means
  that every space is filled and that if there is a bridge, it must have been 
  crossed twice by two different colors.
  */

  if(all_solutions&&covers_board){
    std::vector<int> v;
    std::vector<char> c;
    std::vector<std::vector<int>> visited;
    visited.resize(g.height(), std::vector<int>(g.width()));
    for(int i=0;i<g.height();i++){
      for (int j=0;j<g.width();j++){
        visited[i][j]=0;
      }
    }
    int total=0;
    int pathcount=0;
    c=g.returnchar();
    v=g.findcolor(c[0]);
    int index=0;
    bool cover=true;
    bool base=true;
    bool all=true;
    g.allsolutions(filename,v[0],v[1],visited,pathcount,v,c[0],c,index,total,cover,base,all);
    if (pathcount>0){
      std::cout <<"Found "<<pathcount <<" Solution(s)"<< std::endl;
    }
    else{
      std::cout << "No Solutions"<< std::endl;
    } 
  }

  //t = clock() - t;
  //double time_taken = ((double)t)/CLOCKS_PER_SEC;
  //std::cout<<time_taken<<std::endl;
}
