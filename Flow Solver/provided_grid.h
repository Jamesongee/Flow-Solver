#include <fstream>
#include <iostream>
#include <string>
#include <vector>


class Grid {

public:

  // constructor loads the input puzzle board
  Grid (const std::string &filename);

  // simple accessors
  int width() { return endpoints[0].size(); }
  int height() { return endpoints.size(); }

  // print an ASCII art representation of the board
  void print();


  std::vector<int> findcolor(char color);
  std::vector<char> returnchar();

  void onecolor(const std::string &filename,int x,int y,
    std::vector<std::vector<int>>& visited,int &count,std::vector<int> v,char color);

  void allsolutions(const std::string &filename,int x,int y,
    std::vector<std::vector<int>>& visited,int &count,std::vector<int> v,char color,
    std::vector<char> c,int index,int total,bool cover,bool& base,bool all);


  friend class Color;
  
private:

  // a helper function for print
  char print_center(int i, int j);
  
  // a simple representation of the input puzzle
  std::vector<std::string> endpoints;

  // a representation of the solution paths
  //
  // NOTE note that the dimensions are off by one of the endpoints.
  // We have 1 fewer row of vertical path edges and 1 fewer column of
  // horizontal path edges.
  std::vector<std::string> verticals;
  std::vector<std::string> horizontals;
};


