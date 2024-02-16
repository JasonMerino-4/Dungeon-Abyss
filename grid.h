// Program 7 Outlast the Baddies & Avoid the Abyss
// FALL 2023, CS211, UIC
// System: zybooks

/*grid.h*/

// 
// Starter code written by Prof. Scott Reckinger
// Univeristy of Illinois, Chicago
// Edited by Jason Merino-Garcia

//
// Grid
//
// The Grid class creates a 2D grid
// All elements are initialized to the default value for the given template type

#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm>

using namespace std;

template<typename T>
class Grid {
private:
  struct ROW {
    T*  Cols;     // array of column elements
    size_t NumCols;  // total # of columns (0..NumCols-1)
  };

  ROW* Rows;     // array of ROWs
  size_t  NumRows;  // total # of rows (0..NumRows-1)
    
public:
  //
  // default constructor:
  //
  // Called automatically by to construct a 4x4 Grid. 
  // All elements initialized to default value of T.
  //
  Grid() {
    Rows = new ROW[4];  // 4 rows
    NumRows = 4;
  
    // initialize each row to have 4 columns:
    for (size_t r = 0; r < NumRows; ++r) {
      Rows[r].Cols = new T[4];
      Rows[r].NumCols = 4;

      // initialize the elements to their default value:
      for (size_t c = 0; c < Rows[r].NumCols; ++c) {
        Rows[r].Cols[c] = T();  // default value for type T:
      }
    }
  }

  //
  // parameterized constructor:
  //
  // Called automatically to construct a Grid 
  // with R rows, where each row has C columns. 
  // All elements initialized to default value of T.
  //
  Grid(size_t R, size_t C) {

    if (R < 1) { //makes sure valid number of rows is inputted
      throw out_of_range("Grid::constructor: invalid # of rows");
    }
    if (C < 1) { //makes sure valid number of colunmns is inputted
      throw out_of_range("Grid::constructor: invalid # of cols");
    }

    Rows = new ROW[R];
    NumRows = R;

    for (size_t i = 0; i < NumRows; ++i){//Allocated col for each row
        Rows[i].Cols = new T[C];
        Rows[i].NumCols = C;

        //initialize row and element elements with their default type value
        for (size_t j = 0; j < Rows[i].NumCols; ++j){
            Rows[i].Cols[j] = T();
        }
    }

  }
    
  //
  // destructor:
  //
  // Called automatically to free memory for this Grid.
  //
  virtual ~Grid() {

    for (size_t i = 0; i < NumRows; i++){//Frees each rows cols
        delete[] Rows[i].Cols;
    }

    delete[] Rows;//frees rows

  }


  //
  // copy constructor:
  //
  // Called automatically to construct a Grid that contains a
  // copy of an existing Grid.  Example: this occurs when passing
  // Grid as a parameter by value
  //
  //   void somefunction(Grid<int> G2)  <--- G2 is a copy:
  //   { ... }
  //
  Grid(const Grid<T>& other) {

    NumRows = 0;
    Rows = new ROW[other.NumRows];
    NumRows=  other.NumRows;

    for (size_t i = 0; i < other.NumRows; ++i){//Allocated colunns to each row
        Rows[i].Cols = new T[other.Rows[i].NumCols];
        Rows[i].NumCols = other.Rows[i].NumCols;

        for (size_t j = 0; j < other.Rows[i].NumCols; j++){//Copies over value from other's row and col index
            Rows[i].Cols[j] = other.Rows[i].Cols[j];
        }
    }
  
  }
    
  //
  // copy operator=
  //
  // Called when one Grid is assigned into another, i.e. this = other;
  //
  Grid& operator=(const Grid& other) {

    for (size_t i = 0; i < NumRows; i++){ //frees old grid
        delete[] Rows[i].Cols;
    }

    delete[] Rows; //frees old grid

    NumRows = 0;

    Rows = new ROW[other.NumRows];
    NumRows=  other.NumRows;

    for (size_t i = 0; i < other.NumRows; ++i){ //Allocated new rows and columns with size matching other's rows and cols.
        Rows[i].Cols = new T[other.Rows[i].NumCols];
        Rows[i].NumCols = other.Rows[i].NumCols;

        for (size_t j = 0; j < other.Rows[i].NumCols; j++){//Copies over value from index at that row
            Rows[i].Cols[j] = other.Rows[i].Cols[j];
        }
    }
  
    return *this;  // TODO: update this, it is only here so code compiles.
      
  }


  //
  // numrows
  //
  // Returns the # of rows in the Grid.  
  // The indices for these rows are 0..numrows-1.
  //
  size_t numrows() const {    
    return NumRows;  // TODO:  update this, it is only here so code compiles.
  }
  

  //
  // numcols
  //
  // Returns the # of columns in row r.  
  // The indices for these columns are 0..numcols-1.  
  // For now, each row has the same number of columns.
  //
  size_t numcols(size_t r) const {
    
    if (r < 0 || r >= NumRows) { //makes sure valid number of rows is inputted
      throw out_of_range("Grid::constructor: invalid # for row");
    }
      
    return Rows[r].NumCols;  // TODO: update this, it is only here so code compiles.
  }


  //
  // size
  //
  // Returns the total # of elements in the Grid.
  //
  size_t size() const {
      size_t size = Rows[0].NumCols * NumRows;
      return size;
  }


  //
  // ()
  //
  // Returns a reference to the element at location (r, c);
  // this allows you to access or assign the element:
  //
  //    grid(r, c) = ...
  //    cout << grid(r, c) << endl;
  //
  T& operator()(size_t r, size_t c) {

    if (r < 0 || r >= NumRows) { //makes sure valid number of rows is inputted
      throw out_of_range("Grid::constructor: invalid # for row");
    }
    if (c < 0 || Rows[r].NumCols <= c) { //makes sure valid number of colunmns is inputted
      throw out_of_range("Grid::constructor: invalid # for col");
    }

    return Rows[r].Cols[c];
  }

  //
  // _output
  //
  // Outputs the contents of the grid; for debugging purposes.  
  // This is not tested.
  //
  void _output() {
      
  }

};
