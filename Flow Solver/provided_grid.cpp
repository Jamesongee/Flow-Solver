#include "provided_grid.h"
#include <cassert>
#include <algorithm>


// =============================================================================

// constructor loads the input puzzle board
Grid::Grid(const std::string &filename) {

  std::ifstream istr(filename);
  assert (istr.good());
  
  std::string s;
  while (istr >> s) { endpoints.push_back(s); }

  // initially all of the vertical and horizontal edges are blank!
  //
  // NOTE note that the dimensions are off by one of the endpoints.
  // We have 1 fewer row of vertical path edges and 1 fewer column of
  // horizontal path edges.
  verticals = std::vector<std::string>(height()-1,std::string(width(),' '));
  horizontals = std::vector<std::string>(height(),std::string(width()-1,' '));
}

// =============================================================================

void Grid::print() {

  // loop over the rows of the grid
  for (int j = 0; j < height(); j++) {

    // first print the horizontal cell boundaries
    for (int i = 0; i < width(); i++) {
      char bar = (j==0) ? '-' : ' ';
      char vertical = (j != 0) ? verticals[j-1][i] : bar;
      std::cout << "+" << bar << vertical << bar;
    }
    std::cout << "+" << std::endl;
    
    // then print 3 rows for the middle of each cell
    for (int k = 0; k < 3; k++) {

      for (int i = 0; i < width(); i++) {
        
        // what symbol should be printed in the center of this cell?
        char center = print_center(i,j);

        // what horizontal path edges run through this cell?
        char horizontal_left = (k == 1 && i != 0) ? horizontals[j][i-1] : ' ';
        char horizontal_right = (k == 1 && i+1 < width()) ? horizontals[j][i] : ' ';
        // the left edge of the cell
        char side = (i==0) ? '|' : horizontal_left;

        // do the printing
        if (k == 0) center = (j != 0) ? verticals[j-1][i] : ' ';
        if (k == 2) center = (j+1 < height()) ? verticals[j][i] : ' ';
        std::cout << side << horizontal_left << center << horizontal_right;
      }

      // the right edge of the whole grid
      std::cout << "|" << std::endl;
    }
  }

  // the lower edge of the whole grid
  for (int i = 0; i < width(); i++) {
    std::cout << "+" << std::string(3,'-');
  }
  std::cout << "+" << std::endl;
}

char Grid::print_center(int i, int j) {
  assert (i >= 0 && i < width());
  assert (j >= 0 && j < height());

  if (endpoints[j][i] != '.') { return endpoints[j][i]; }
  
  // if this is not an endpoint or bridge cell, look at the 4 edges
  // if any of them is a path, take that color for the center
  
  char up    = (j == 0)          ? ' ' : verticals  [j-1][i  ];
  char down  = (j == height()-1) ? ' ' : verticals  [j  ][i  ];
  char left  = (i == 0)          ? ' ' : horizontals[j  ][i-1];
  char right = (i == width()-1)  ? ' ' : horizontals[j  ][i  ];

  char center = up;
  if (center == ' ') center = down;
  if (center == ' ') center = right;
  if (center == ' ') center = left;
  return center;
}


// =============================================================================



/*
This is a simple function that returns a vector containing the locations of the 
color in the argument. This is achieved by going through every point in the 
puzzle and then inserting the points that are the colors into a vector.
*/
std::vector<int> Grid::findcolor(char color){
  std::vector<int> v;
  int startx,starty,endx,endy;
  bool start=false;
  for (int i=0; i<height();i++){
    for (int j=0;j<width();j++){
      if (start==false&&endpoints[i][j]==color){
        startx=i;
        starty=j;
        start=true;
      }
      else if(start==true&&endpoints[i][j]==color){
        endx=i;
        endy=j;
      }
      
    }
  }
  v.push_back(startx);
  v.push_back(starty);
  v.push_back(endx);
  v.push_back(endy);
  return v;
}

//==============================================================================
//==============================================================================

/*
This is another simple function that simply returns a vector containing each 
color. This is achieved by looping through the puzzle, making sure its a letter,
and then making sure its not already in the vector. If all of these are the 
case, it is added to the vector and returned.
*/
std::vector<char> Grid::returnchar(){
  std::vector<char> c;
  char letter;
  for (int i=0; i<height();i++){
    for (int j=0;j<width();j++){
        letter=endpoints[i][j];
      if(letter>64 && letter<91){
        if (std::find(c.begin(),c.end(),letter)!=c.end()){}
        else{
          c.push_back(letter);
        }

      }
    }
  }
  return c;
}

//==============================================================================
//==============================================================================
/*
This function is one of the main recursive functions used in this program. This 
function takes in a filename, a start location, a vector of a vector of ints to 
keep track of visited locations, a count to keep track of how many solutions 
are printed and valid, a vector that contains the locations of a color, 
the color we are currently solving for, a vector of characters that holds all of
the colors, an index to know which color we should be solving for, a total to 
keep track of if the board is completley covered or not, and three booleans that
are true or false depending on what has been typed into the command line. This 
function works by checking next locations and making sure they are valid to move
there. If they are, this function is then called but with different start values
depending on which location was checked. If the next location is valid, the 
puzzle display will be updated and the program will mark that the location has 
been visited. If the funcion recurses to a point where it can't go any further, 
it will go back until it has a next destintation that is valid. This function 
also works with bridges and makes sure to check that a bridge is valid before 
traversing it. If it then reaches the correct color, the program will begin the 
same process with another color until all the colors are connected. As the 
recursion backtracks, it will change the display of the grid back to what it was
and will make it so that the space says it wasn't visited. Depending on what was
inputed in the command line, the program will either print 1 solution, all 
solutions, or all solutions that cover the entire board.
*/
void Grid::allsolutions(const std::string &filename,int x,int y,
  std::vector<std::vector<int>>& visited,int &count,std::vector<int> v,
  char color,std::vector<char> c,int index,int total,bool cover,bool& base,
  bool all){
  if((!base&&!all)||all==true){
    total++;
    if (x==v[2] && y==v[3]){
      if (color!=c[c.size()-1]){
        index++;
        std::vector<int> next;
        next=findcolor(c[index]);
        allsolutions(filename,next[0],next[1],visited,count,next,c[index],c
          ,index,total,cover,base,all);
      }
      else{
        base=true;
        if (cover){
          if (total==width()*height()){
            print();
            std::cout<<std::endl;
            count++;
          }
        }
        else{
          count++;
          print();
          if (all){
            std::cout<<std::endl;
          }
          return;
        }
      }
    }
    bool bridgel=false;
    bool bridger=false;
    bool bridgeu=false;
    bool bridged=false;
    bool bridgel2=false;
    bool bridger2=false;
    bool bridgeu2=false;
    bool bridged2=false;
    bool bridgel3=false;
    bool bridger3=false;
    bool bridgeu3=false;
    bool bridged3=false;

    visited[x][y]+=1;

    char lower=tolower(color);

    //Moving to the left
    if (y-1>=0){
      if((endpoints[x][y-1]=='.' || endpoints[x][y-1]==color) 
        && visited[x][y-1]<1){
        if (y-1<width()-1){horizontals[x][y-1]=lower;}
        allsolutions(filename,x,y-1,visited,count,v,color,c,index,total,cover,
          base,all);
        horizontals[x][y-1]=' ';
      }

      else if(endpoints[x][y-1]=='#' && y-2>=0 && visited[x][y-2]<1 
        && visited[x][y-1]<2&&(endpoints[x][y-2]=='.'||endpoints[x][y-2]==color)
        &&verticals[x][y-1]!=lower){

        visited[x][y-1]+=1;
        if (visited[x][y-1]==2){total++;}
        bridgel=true;
        if (y-1<width()-1){
          horizontals[x][y-1]=lower;
          horizontals[x][y-2]=lower;
        }
        allsolutions(filename,x,y-2,visited,count,v,color,c,index,total,cover,
          base,all);
        horizontals[x][y-1]=' ';
        horizontals[x][y-2]=' ';
      }

      else if(endpoints[x][y-1]=='#' && endpoints[x][y-2]=='#' && y-3>=0 
        && visited[x][y-1]<2 && visited[x][y-2]<2&&visited[x][y-3]<1
        &&(endpoints[x][y-3]=='.'||endpoints[x][y-3]==color)
        &&verticals[x][y-1]!=lower&&verticals[x][y-2]!=lower){

        visited[x][y-1]+=1;
        visited[x][y-2]+=1;
        if (visited[x][y-1]==2){total++;}
        if (visited[x][y-2]==2){total++;}
        bridgel2=true;
        if (y-2<width()-1){
          horizontals[x][y-1]=lower;
          horizontals[x][y-2]=lower;
          horizontals[x][y-3]=lower;
        }
        allsolutions(filename,x,y-3,visited,count,v,color,c,index,total,cover,
          base,all);
        horizontals[x][y-1]=' ';
        horizontals[x][y-2]=' ';
        horizontals[x][y-3]=' ';
      }

      else if(endpoints[x][y-1]=='#' && endpoints[x][y-2]=='#' 
        && endpoints[x][y-3]=='#'&& y-4>=0 && visited[x][y-1]<2 
        && visited[x][y-2]<2&&visited[x][y-3]<2&&visited[x][y-4]<1
        &&endpoints[x][y-4]=='.'&&verticals[x][y-1]!=lower
        &&verticals[x][y-2]!=lower&&verticals[x][y-3]!=lower){

        visited[x][y-1]+=1;
        visited[x][y-2]+=1;
        visited[x][y-3]+=1;
        if (visited[x][y-1]==2){total++;}
        if (visited[x][y-2]==2){total++;}
        if (visited[x][y-3]==2){total++;}
        bridgel3=true;
        if (y-3<width()-1){
          horizontals[x][y-1]=lower;
          horizontals[x][y-2]=lower;
          horizontals[x][y-3]=lower;
          horizontals[x][y-4]=lower;
        }
        allsolutions(filename,x,y-4,visited,count,v,color,c,index,total,cover,
          base,all);
        horizontals[x][y-1]=' ';
        horizontals[x][y-2]=' ';
        horizontals[x][y-3]=' ';
        horizontals[x][y-4]=' ';
      }
    }

    //Moving to the right
    if (y+1<width()){
      if((endpoints[x][y+1]=='.'|| endpoints[x][y+1]==color)&&visited[x][y+1]<1){
        if (y>=0 &&y<width()-1){horizontals[x][y]=lower;}
        allsolutions(filename,x,y+1,visited,count,v,color,c,index,total,cover,
          base,all);
        if (y>=0 &&y<width()-1){horizontals[x][y]=' ';}
      }
    
      else if(endpoints[x][y+1]=='#' && y+2<width() && visited[x][y+2]<1
       && visited[x][y+1]<2&&(endpoints[x][y+2]=='.'||endpoints[x][y+2]==color)
       &&verticals[x][y+1]!=lower){

        visited[x][y+1]+=1;
        if (visited[x][y+1]==2){total++;}
        bridger=true;
        if (y>=0&&y+1<width()-1){
          horizontals[x][y]=lower;
          horizontals[x][y+1]=lower;
        }
        allsolutions(filename,x,y+2,visited,count,v,color,c,index,total,cover,
          base,all);
        if (y>=0 &&y<width()-1){
          horizontals[x][y]=' ';
          horizontals[x][y+1]=' ';
        }
      }

      else if(endpoints[x][y+1]=='#' && endpoints[x][y+2]=='#'&& y+3<width() 
        && visited[x][y+3]<1 && visited[x][y+1]<2 && visited[x][y+2]<2 
        &&(endpoints[x][y+3]=='.'||endpoints[x][y+3]==color)
        &&verticals[x][y+1]!=lower&&verticals[x][y+2]!=lower){

        visited[x][y+1]+=1;
        visited[x][y+2]+=1;
        if (visited[x][y+1]==2){total++;}
        if (visited[x][y+2]==2){total++;}
        bridger2=true;
        if (y>=0&&y+2<width()-1){
          horizontals[x][y]=lower;
          horizontals[x][y+1]=lower;
          horizontals[x][y+2]=lower;
        }
        allsolutions(filename,x,y+3,visited,count,v,color,c,index,total,cover,
          base,all);
        if (y>=0 &&y<width()-1){
          horizontals[x][y]=' ';
          horizontals[x][y+1]=' ';
          horizontals[x][y+2]=' ';
        }
      }

      else if(endpoints[x][y+1]=='#' && endpoints[x][y+2]=='#' 
        && endpoints[x][y+3]=='#'&& y+4<width() && visited[x][y+3]<2 
        && visited[x][y+1]<2 && visited[x][y+2]<2
        && visited[x][y+4]<1&&endpoints[x][y+4]=='.'&&verticals[x][y+1]!=lower
        &&verticals[x][y+2]!=lower&& verticals[x][y+3]!=lower){

        visited[x][y+1]+=1;
        visited[x][y+2]+=1;
        visited[x][y+3]+=1;
        if (visited[x][y+1]==2){total++;}
        if (visited[x][y+2]==2){total++;}
        if (visited[x][y+3]==2){total++;}
        bridger3=true;
        if (y>=0&&y+3<width()-1){
          horizontals[x][y]=lower;
          horizontals[x][y+1]=lower;
          horizontals[x][y+2]=lower;
          horizontals[x][y+3]=lower;
        }
        allsolutions(filename,x,y+4,visited,count,v,color,c,index,total,cover,
          base,all);
        if (y>=0 &&y<width()-1){
          horizontals[x][y]=' ';
          horizontals[x][y+1]=' ';
          horizontals[x][y+2]=' ';
          horizontals[x][y+3]=' ';
        }
      }
    }
    //Moving Up
    if (x-1>=0){
      if((endpoints[x-1][y]=='.'|| endpoints[x-1][y]==color)
        &&visited[x-1][y]<1){
        if (x-1>=0 &&x-1<height()-1){verticals[x-1][y]=lower;}
        allsolutions(filename,x-1,y,visited,count,v,color,c,index,total,cover,
          base,all);
        if (x-1>=0 &&x-1<height()-1){verticals[x-1][y]=' ';}
      }

      else if(endpoints[x-1][y]=='#' && x-2>=0 && visited[x-2][y]<1 
        &&visited[x-1][y]<2
        &&(endpoints[x-2][y]=='.'||endpoints[x-2][y]==color)
        &&horizontals[x-1][y]!=lower){

        visited[x-1][y]+=1;
        if (visited[x-1][y]==2){total++;}
        bridgeu=true;
        if (x>=0 && x-2<height()-1){
          verticals[x-1][y]=lower;
          verticals[x-2][y]=lower;
        }
        allsolutions(filename,x-2,y,visited,count,v,color,c,index,total,cover,
          base,all);
        if (x>=0 && x-2<height()-1){
          verticals[x-1][y]=' ';
          verticals[x-2][y]=' ';
        }
      }

      else if(endpoints[x-1][y]=='#' &&endpoints[x-2][y]=='#'&& x-3>=0 
        && visited[x-2][y]<2 && visited[x-1][y]<2 && visited[x-3][y]<1
        &&(endpoints[x-3][y]=='.'||endpoints[x-3][y]==color)
        &&horizontals[x-1][y]!=lower && horizontals[x-2][y]!=lower){

        visited[x-1][y]+=1;
        visited[x-2][y]+=1;
        if (visited[x-1][y]==2){total++;}
        if (visited[x-2][y]==2){total++;}
        bridgeu2=true;
        if (x>=0 && x-3<height()-1){
          verticals[x-1][y]=lower;
          verticals[x-2][y]=lower;
          verticals[x-3][y]=lower;
        }
        allsolutions(filename,x-3,y,visited,count,v,color,c,index,total,cover,
          base,all);
        if (x>=0 && x-3<height()-1){
          verticals[x-1][y]=' ';
          verticals[x-2][y]=' ';
          verticals[x-3][y]=' ';
        }
      }

      else if(endpoints[x-1][y]=='#' &&endpoints[x-2][y]=='#'&& endpoints[x-3][y]=='#'
        && x-4>=0 && visited[x-2][y]<2 && visited[x-1][y]<2 && visited[x-3][y]<2 
        && visited[x-4][y]<1&&endpoints[x-4][y]=='.'&&horizontals[x-1][y]!=lower 
        && horizontals[x-2][y]!=lower&&horizontals[x-3][y]!=lower){

        visited[x-1][y]+=1;
        visited[x-2][y]+=1;
        visited[x-3][y]+=1;
        if (visited[x-1][y]==2){total++;}
        if (visited[x-2][y]==2){total++;}
        if (visited[x-3][y]==2){total++;}
        bridgeu3=true;
        if (x>=0 && x-4<height()-1){
          verticals[x-1][y]=lower;
          verticals[x-2][y]=lower;
          verticals[x-3][y]=lower;
          verticals[x-4][y]=lower;
        }
        allsolutions(filename,x-4,y,visited,count,v,color,c,index,total,cover,
          base,all);
        if (x>=0 && x-4<height()-1){
          verticals[x-1][y]=' ';
          verticals[x-2][y]=' ';
          verticals[x-3][y]=' ';
          verticals[x-4][y]=' ';
        }
      }
    }

    //Moving down
    if (x+1<height()){
      if((endpoints[x+1][y]=='.'|| endpoints[x+1][y]==color)&&visited[x+1][y]<1){
        if (x>=0 &&x<height()-1){verticals[x][y]=lower;}
        allsolutions(filename,x+1,y,visited,count,v,color,c,index,total,cover,
          base,all);
        if (x>=0 &&x<height()-1){verticals[x][y]=' ';}
      }

      else if(endpoints[x+1][y]=='#' && x+2<height() && visited[x+2][y]<1 
        && visited[x+1][y]<2&&(endpoints[x+2][y]=='.'||endpoints[x+2][y]==color)
        &&horizontals[x+1][y]!=lower){

        visited[x+1][y]+=1;
        if (visited[x+1][y]==2){total++;}
        bridged=true;
        if (x>=0 && x+1<height()-1){
          verticals[x][y]=lower;
          verticals[x+1][y]=lower;
        }
        allsolutions(filename,x+2,y,visited,count,v,color,c,index,total,cover,
          base,all);
        if (x>=0 && x-2<height()-1){
          verticals[x][y]=' ';
          verticals[x+1][y]=' ';
        }
      }

      else if(endpoints[x+1][y]=='#' && endpoints[x+2][y]=='#'&& x+3<height() 
        && visited[x+2][y]<2 && visited[x+1][y]<2&& visited[x+3][y]<1 
        &&(endpoints[x+3][y]=='.'||endpoints[x+3][y]==color)
        &&horizontals[x+1][y]!=lower&& horizontals[x+2][y]!=lower){

        visited[x+1][y]+=1;
        visited[x+2][y]+=1;
        if (visited[x+1][y]==2){total++;}
        if (visited[x+2][y]==2){total++;}
        bridged2=true;
        if (x>=0 && x+2<height()-1){
          verticals[x][y]=lower;
          verticals[x+1][y]=lower;
          verticals[x+2][y]=lower;
        }
        allsolutions(filename,x+3,y,visited,count,v,color,c,index,total,cover,
          base,all);
        if (x>=0 && x-1<height()-1){
          verticals[x][y]=' ';
          verticals[x+1][y]=' ';
          verticals[x+2][y]=' ';
        }
      }

      else if(endpoints[x+1][y]=='#' && endpoints[x+2][y]=='#'&& endpoints[x+3][y]=='#'
        &&x+4<height() && visited[x+2][y]<2 && visited[x+1][y]<2&& visited[x+3][y]<2 
        &&visited[x+4][y]<1 &&endpoints[x+4][y]=='.'&&horizontals[x+1][y]!=lower 
        && horizontals[x+2][y]!=lower&&horizontals[x+3][y]!=lower){

        visited[x+1][y]+=1;
        visited[x+2][y]+=1;
        visited[x+3][y]+=1;
        if (visited[x+1][y]==2){total++;}
        if (visited[x+2][y]==2){total++;}
        if (visited[x+3][y]==2){total++;}
        bridged3=true;
        if (x>=0 && x+3<height()-1){
          verticals[x][y]=lower;
          verticals[x+1][y]=lower;
          verticals[x+2][y]=lower;
          verticals[x+3][y]=lower;
        }
        allsolutions(filename,x+4,y,visited,count,v,color,c,index,total,cover,
          base,all);
        if (x>=0 && x<height()-1){
          verticals[x][y]=' ';
          verticals[x+1][y]=' ';
          verticals[x+2][y]=' ';
          verticals[x+3][y]=' ';
        }
      }
    }

    if (bridged){visited[x+1][y]-=1;}
    else if (bridgeu){visited[x-1][y]-=1;}
    else if (bridgel){visited[x][y-1]-=1;}
    else if (bridger){visited[x][y+1]-=1;}
    else if (bridged2){
      visited[x+1][y]-=1;
      visited[x+2][y]-=1;
    }
    else if (bridgeu2){
      visited[x-1][y]-=1;
      visited[x-2][y]-=1;
    }
    else if (bridgel2){
      visited[x][y-1]-=1;
      visited[x][y-2]-=1;
    }
    else if (bridger2){
      visited[x][y+1]-=1;
      visited[x][y+2]-=1;
    }
    else if (bridged3){
      visited[x+1][y]-=1;
      visited[x+2][y]-=1;
      visited[x+3][y]-=1;
    }
    else if (bridgeu3){
      visited[x-1][y]-=1;
      visited[x-2][y]-=1;
      visited[x-3][y]-=1;
    }
    else if (bridgel3){
      visited[x][y-1]-=1;
      visited[x][y-2]-=1;
      visited[x][y-3]-=1;
    }
    else if (bridger3){
      visited[x][y+1]-=1;
      visited[x][y+2]-=1;
      visited[x][y+3]-=1;
    }
    visited[x][y]-=1;
  }
}

//==============================================================================
//==============================================================================

/*
This function works the exact same as the allsolutions function except that this 
function doesn't keep track of all of the colors in the puzzle and instead prints
whenever the color that is inputed into the argument connects and makes a valid 
path. This function will print all paths for a sinle color and will keep track 
of the number of paths for said color.
*/
void Grid::onecolor(const std::string &filename,int x,int y,
  std::vector<std::vector<int>>& visited,int &count,std::vector<int> v,char color){
  if (x==v[2] && y==v[3]){
    count++;
    print();
    std::cout<<std::endl;
    return;
  } 
    bool bridgel=false;
    bool bridger=false;
    bool bridgeu=false;
    bool bridged=false;
    bool bridgel2=false;
    bool bridger2=false;
    bool bridgeu2=false;
    bool bridged2=false;
    bool bridgel3=false;
    bool bridger3=false;
    bool bridgeu3=false;
    bool bridged3=false;

    visited[x][y]+=1;

    char lower=tolower(color);

    //Moving to the left
    if (y-1>=0){
      if((endpoints[x][y-1]=='.' || endpoints[x][y-1]==color)&&visited[x][y-1]<1){
        if (y-1<width()-1){horizontals[x][y-1]=lower;}
        onecolor(filename,x,y-1,visited,count,v,color);
        horizontals[x][y-1]=' ';
      }

      else if(endpoints[x][y-1]=='#' && y-2>=0 && visited[x][y-2]<1 
        && visited[x][y-1]<2&&(endpoints[x][y-2]=='.'||endpoints[x][y-2]==color)
        &&verticals[x][y-1]!=lower){

        visited[x][y-1]+=1;
        bridgel=true;
        if (y-1<width()-1){
          horizontals[x][y-1]=lower;
          horizontals[x][y-2]=lower;
        }
        onecolor(filename,x,y-2,visited,count,v,color);
        horizontals[x][y-1]=' ';
        horizontals[x][y-2]=' ';
      }

      else if(endpoints[x][y-1]=='#' && endpoints[x][y-2]=='#' && y-3>=0 
        && visited[x][y-1]<2 && visited[x][y-2]<2&&visited[x][y-3]<1
        &&(endpoints[x][y-3]=='.'||endpoints[x][y-3]==color)&&verticals[x][y-1]!=lower
        &&verticals[x][y-2]!=lower){

        visited[x][y-1]+=1;
        visited[x][y-2]+=1;
        bridgel2=true;
        if (y-2<width()-1){
          horizontals[x][y-1]=lower;
          horizontals[x][y-2]=lower;
          horizontals[x][y-3]=lower;
        }
        onecolor(filename,x,y-3,visited,count,v,color);
        horizontals[x][y-1]=' ';
        horizontals[x][y-2]=' ';
        horizontals[x][y-3]=' ';
      }

      else if(endpoints[x][y-1]=='#' && endpoints[x][y-2]=='#' && endpoints[x][y-3]=='#'
        && y-4>=0 && visited[x][y-1]<2 && visited[x][y-2]<2&&visited[x][y-3]<2
        &&visited[x][y-4]<1&&endpoints[x][y-4]=='.'&&verticals[x][y-1]!=lower
        &&verticals[x][y-2]!=lower&&verticals[x][y-3]!=lower){

        visited[x][y-1]+=1;
        visited[x][y-2]+=1;
        visited[x][y-3]+=1;
        bridgel3=true;
        if (y-3<width()-1){
          horizontals[x][y-1]=lower;
          horizontals[x][y-2]=lower;
          horizontals[x][y-3]=lower;
          horizontals[x][y-4]=lower;
        }
        onecolor(filename,x,y-4,visited,count,v,color);
        horizontals[x][y-1]=' ';
        horizontals[x][y-2]=' ';
        horizontals[x][y-3]=' ';
        horizontals[x][y-4]=' ';
      }
    }

    //Moving to the right
    if (y+1<width()){
      if((endpoints[x][y+1]=='.'|| endpoints[x][y+1]==color)&&visited[x][y+1]<1){
        if (y>=0 &&y<width()-1){horizontals[x][y]=lower;}
        onecolor(filename,x,y+1,visited,count,v,color);
        if (y>=0 &&y<width()-1){horizontals[x][y]=' ';}
      }
    
      else if(endpoints[x][y+1]=='#' && y+2<width() && visited[x][y+2]<1
       && visited[x][y+1]<2&&(endpoints[x][y+2]=='.'||endpoints[x][y+2]==color)
       &&verticals[x][y+1]!=lower){

        visited[x][y+1]+=1;
        bridger=true;
        if (y>=0&&y+1<width()-1){
          horizontals[x][y]=lower;
          horizontals[x][y+1]=lower;
        }
        onecolor(filename,x,y+2,visited,count,v,color);
        if (y>=0 &&y<width()-1){
          horizontals[x][y]=' ';
          horizontals[x][y+1]=' ';
        }
      }

      else if(endpoints[x][y+1]=='#' && endpoints[x][y+2]=='#'&& y+3<width() 
        && visited[x][y+3]<1 && visited[x][y+1]<2 && visited[x][y+2]<2 
        &&(endpoints[x][y+3]=='.'||endpoints[x][y+3]==color)
        &&verticals[x][y+1]!=lower&&verticals[x][y+2]!=lower){

        visited[x][y+1]+=1;
        visited[x][y+2]+=1;
        bridger2=true;
        if (y>=0&&y+2<width()-1){
          horizontals[x][y]=lower;
          horizontals[x][y+1]=lower;
          horizontals[x][y+2]=lower;
        }
        onecolor(filename,x,y+3,visited,count,v,color);
        if (y>=0 &&y<width()-1){
          horizontals[x][y]=' ';
          horizontals[x][y+1]=' ';
          horizontals[x][y+2]=' ';
        }
      }

      else if(endpoints[x][y+1]=='#' && endpoints[x][y+2]=='#' && endpoints[x][y+3]=='#'
        && y+4<width() && visited[x][y+3]<2 && visited[x][y+1]<2 && visited[x][y+2]<2
         && visited[x][y+4]<1&&endpoints[x][y+4]=='.'&&verticals[x][y+1]!=lower
         &&verticals[x][y+2]!=lower&& verticals[x][y+3]!=lower){

        visited[x][y+1]+=1;
        visited[x][y+2]+=1;
        visited[x][y+3]+=1;
        bridger3=true;
        if (y>=0&&y+3<width()-1){
          horizontals[x][y]=lower;
          horizontals[x][y+1]=lower;
          horizontals[x][y+2]=lower;
          horizontals[x][y+3]=lower;
        }
        onecolor(filename,x,y+4,visited,count,v,color);
        if (y>=0 &&y<width()-1){
          horizontals[x][y]=' ';
          horizontals[x][y+1]=' ';
          horizontals[x][y+2]=' ';
          horizontals[x][y+3]=' ';
        }
      }
    }
    //Moving Up
    if (x-1>=0){
      if((endpoints[x-1][y]=='.'|| endpoints[x-1][y]==color)&&visited[x-1][y]<1){
        if (x-1>=0 &&x-1<height()-1){verticals[x-1][y]=lower;}
        onecolor(filename,x-1,y,visited,count,v,color);
        if (x-1>=0 &&x-1<height()-1){verticals[x-1][y]=' ';}
      }

      else if(endpoints[x-1][y]=='#' && x-2>=0 && visited[x-2][y]<1 
        && visited[x-1][y]<2
        &&(endpoints[x-2][y]=='.'||endpoints[x-2][y]==color)
        &&horizontals[x-1][y]!=lower){

        visited[x-1][y]+=1;
        bridgeu=true;
        if (x>=0 && x-2<height()-1){
          verticals[x-1][y]=lower;
          verticals[x-2][y]=lower;
        }
        onecolor(filename,x-2,y,visited,count,v,color);
        if (x>=0 && x-2<height()-1){
          verticals[x-1][y]=' ';
          verticals[x-2][y]=' ';
        }
      }

      else if(endpoints[x-1][y]=='#' &&endpoints[x-2][y]=='#'&& x-3>=0 
        && visited[x-2][y]<2 && visited[x-1][y]<2 && visited[x-3][y]<1
        &&(endpoints[x-3][y]=='.'||endpoints[x-3][y]==color)
        &&horizontals[x-1][y]!=lower && horizontals[x-2][y]!=lower){

        visited[x-1][y]+=1;
        visited[x-2][y]+=1;
        bridgeu2=true;
        if (x>=0 && x-3<height()-1){
          verticals[x-1][y]=lower;
          verticals[x-2][y]=lower;
          verticals[x-3][y]=lower;
        }
        onecolor(filename,x-3,y,visited,count,v,color);
        if (x>=0 && x-3<height()-1){
          verticals[x-1][y]=' ';
          verticals[x-2][y]=' ';
          verticals[x-3][y]=' ';
        }
      }

      else if(endpoints[x-1][y]=='#' &&endpoints[x-2][y]=='#'&& endpoints[x-3][y]=='#'
        && x-4>=0 && visited[x-2][y]<2 && visited[x-1][y]<2 && visited[x-3][y]<2 
        && visited[x-4][y]<1&&endpoints[x-4][y]=='.'&&horizontals[x-1][y]!=lower 
        && horizontals[x-2][y]!=lower&&horizontals[x-3][y]!=lower){

        visited[x-1][y]+=1;
        visited[x-2][y]+=1;
        visited[x-3][y]+=1;
        bridgeu3=true;
        if (x>=0 && x-4<height()-1){
          verticals[x-1][y]=lower;
          verticals[x-2][y]=lower;
          verticals[x-3][y]=lower;
          verticals[x-4][y]=lower;
        }
        onecolor(filename,x-4,y,visited,count,v,color);
        if (x>=0 && x-4<height()-1){
          verticals[x-1][y]=' ';
          verticals[x-2][y]=' ';
          verticals[x-3][y]=' ';
          verticals[x-4][y]=' ';
        }
      }
    }

    //Moving down
    if (x+1<height()){
      if((endpoints[x+1][y]=='.'|| endpoints[x+1][y]==color)&&visited[x+1][y]<1){
        if (x>=0 &&x<height()-1){verticals[x][y]=lower;}
        onecolor(filename,x+1,y,visited,count,v,color);
        if (x>=0 &&x<height()-1){verticals[x][y]=' ';}
      }

      else if(endpoints[x+1][y]=='#' && x+2<height() && visited[x+2][y]<1 
        && visited[x+1][y]<2&&(endpoints[x+2][y]=='.'||endpoints[x+2][y]==color)
        &&horizontals[x+1][y]!=lower){

        visited[x+1][y]+=1;
        bridged=true;
        if (x>=0 && x+1<height()-1){
          verticals[x][y]=lower;
          verticals[x+1][y]=lower;
        }
        onecolor(filename,x+2,y,visited,count,v,color);
        if (x>=0 && x-2<height()-1){
          verticals[x][y]=' ';
          verticals[x+1][y]=' ';
        }
      }

      else if(endpoints[x+1][y]=='#' && endpoints[x+2][y]=='#'&& x+3<height() 
        && visited[x+2][y]<2 && visited[x+1][y]<2&& visited[x+3][y]<1 
        &&(endpoints[x+3][y]=='.'||endpoints[x+3][y]==color)
        &&horizontals[x+1][y]!=lower&& horizontals[x+2][y]!=lower){

        visited[x+1][y]+=1;
        visited[x+2][y]+=1;
        bridged2=true;
        if (x>=0 && x+2<height()-1){
          verticals[x][y]=lower;
          verticals[x+1][y]=lower;
          verticals[x+2][y]=lower;
        }
        onecolor(filename,x+3,y,visited,count,v,color);
        if (x>=0 && x-1<height()-1){
          verticals[x][y]=' ';
          verticals[x+1][y]=' ';
          verticals[x+2][y]=' ';
        }
      }

      else if(endpoints[x+1][y]=='#' && endpoints[x+2][y]=='#'&& endpoints[x+3][y]=='#'
        &&x+4<height() && visited[x+2][y]<2 && visited[x+1][y]<2&& visited[x+3][y]<2 
        &&visited[x+4][y]<1 &&endpoints[x+4][y]=='.'&&horizontals[x+1][y]!=lower 
        && horizontals[x+2][y]!=lower&&horizontals[x+3][y]!=lower){

        visited[x+1][y]+=1;
        visited[x+2][y]+=1;
        visited[x+3][y]+=1;
        bridged3=true;
        if (x>=0 && x+3<height()-1){
          verticals[x][y]=lower;
          verticals[x+1][y]=lower;
          verticals[x+2][y]=lower;
          verticals[x+3][y]=lower;
        }
        onecolor(filename,x+4,y,visited,count,v,color);
        if (x>=0 && x<height()-1){
          verticals[x][y]=' ';
          verticals[x+1][y]=' ';
          verticals[x+2][y]=' ';
          verticals[x+3][y]=' ';
        }
      }
    }

    if (bridged){visited[x+1][y]-=1;}
    else if (bridgeu){visited[x-1][y]-=1;}
    else if (bridgel){visited[x][y-1]-=1;}
    else if (bridger){visited[x][y+1]-=1;}
    else if (bridged2){
      visited[x+1][y]-=1;
      visited[x+2][y]-=1;
    }
    else if (bridgeu2){
      visited[x-1][y]-=1;
      visited[x-2][y]-=1;
    }
    else if (bridgel2){
      visited[x][y-1]-=1;
      visited[x][y-2]-=1;
    }
    else if (bridger2){
      visited[x][y+1]-=1;
      visited[x][y+2]-=1;
    }
    else if (bridged3){
      visited[x+1][y]-=1;
      visited[x+2][y]-=1;
      visited[x+3][y]-=1;
    }
    else if (bridgeu3){
      visited[x-1][y]-=1;
      visited[x-2][y]-=1;
      visited[x-3][y]-=1;
    }
    else if (bridgel3){
      visited[x][y-1]-=1;
      visited[x][y-2]-=1;
      visited[x][y-3]-=1;
    }
    else if (bridger3){
      visited[x][y+1]-=1;
      visited[x][y+2]-=1;
      visited[x][y+3]-=1;
    }
    visited[x][y]-=1;
  }