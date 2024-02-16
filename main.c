// Program 7 Outlast the Baddies & Avoid the Abyss
// FALL 2023, CS211, UIC
// System: zybooks

//gameboard.h

// Starter code written by Prof. Scott Reckinger
// Univeristy of Illinois, Chicago
// Edited by Jason Merino-Garcia


#ifndef _GAMEBOARD_H
#define _GAMEBOARD_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <stdexcept>

#include "boardcell.h"
#include "grid.h"

using namespace std;

class GameBoard {
	private: 
	    Grid<BoardCell*> board;
        size_t numRows;
        size_t numCols;
        size_t HeroRow; // Hero's position row
	    size_t HeroCol; // Hero's position column
        int numMonsters;
        int numSuperMonsters;
        int numAbysses;
        int numBats;
        bool wonGame; // false, unless the Hero reached the exit successfully

		
	public: 
		/* default constructor */
        GameBoard() {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 50;
            numBats = 2;
            wonGame = false;
            
            this -> numRows = 15;
            this -> numCols = 40;
            
            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;
            
            blankBoard();
        }
        
        /* param constructor */
        GameBoard(size_t numRows, size_t numCols) {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 20;
            numBats = 3;
            wonGame = false;
            
            this -> numRows = numRows;
            this -> numCols = numCols;
            
            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;
            
            blankBoard();
        }
        
        /* destructor */
        virtual ~GameBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    delete board(row, col);
                }
            }
        }

        void blankBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    board(row, col) = new Nothing(row,col);
                }
            }
        }

        char getCellDisplay(size_t r, size_t c) {
            return board(r,c)->display();
        }

        void setCell(BoardCell* myCell, size_t r, size_t c) {
            board(r,c) = myCell;
        }
    
        void freeCell(size_t r, size_t c) {
            delete board(r,c);
        }

        // fills board with by randomly placing...
        //  - Hero (H) in the first three columns
        //  - EscapeLadder (*) in last three columns
        //  - 3 vertical Walls (+), each 1/2 of board height, in middle segment
        //  - Abyss cells (#), quantity set by numAbysses, in middle segment
        //  - Baddies [Monsters (m), Super Monsters (M), & Bats (~)] in middle segment;
        //    number of Baddies set by numMonsters, numSuperMonsters, & numBats
        void setupBoard(int seed) {
            srand(seed);
            size_t r,c;

            r = rand() % numRows;
            c = rand() % 3;
            delete board(r,c);
            board(r,c) = new Hero(r,c);
            HeroRow = r;
            HeroCol = c;

            r = rand() % numRows;
            c = numCols - 1 - (rand() % 3);
            delete board(r,c);
            board(r,c) = new EscapeLadder(r,c);
            
            int sizeMid = numCols - 6;

            c = 3 + (rand() % sizeMid);
            for (r = 0; r < numRows/2; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }
            size_t topc = c;

            while (c == topc || c == topc-1 || c == topc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows-1; r > numRows/2; --r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);           
            }
            size_t botc = c;

            while (c == topc || c == topc-1 || c == topc+1 || c == botc || c == botc-1 || c == botc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows/4; r < 3*numRows/4; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }

            for (int i = 0; i < numMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c);  
                board(r,c)->setPower(1);        
            }

            for (int i = 0; i < numSuperMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c); 
                board(r,c)->setPower(2);               
            }

            for (int i = 0; i < numBats; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Bat(r,c); 
            }

            for (int i = 0; i < numAbysses; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Abyss(r,c);              
            }
        }

        // neatly displaying the game board 
		void display( ) {
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            for (size_t row = 0; row < board.numrows(); row++) {
                cout << '|';
                for (size_t col = 0; col < board.numcols(row); col++) {
                    cout << board(row,col)->display();
                }
                cout << '|';
                cout << endl;
            }
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            
        }
		
        bool getWonGame() {
            return wonGame;
        }
        
        // distributing total number of monsters so that 
        //  ~1/3 of num are Super Monsters (M), and
        //  ~2/3 of num are Regular Monsters (m)
        void setNumMonsters(int num) {
            numSuperMonsters = num/3;
            numMonsters = num - numSuperMonsters;
        }

        void setNumAbysses(int num) {
            numAbysses = num;
        }

        void setNumBats(int num) {
            numBats = num;
        }

        size_t getNumRows() {
            return numRows;
        }

        size_t getNumCols() {
            return numCols;
        }

        
        //---------------------------------------------------------------------------------
        // void getHeroPosition(size_t& row, size_t& col)
        //
        // getter for Hero's position, which are private data members
        //      int HeroRow;
	    //      int HeroCol;
        // note: row and col are passed-by-reference
        //---------------------------------------------------------------------------------
        void getHeroPosition(size_t& row, size_t& col) {

            row = HeroRow;  
            col = HeroCol;  
        }

        
        //---------------------------------------------------------------------------------
        // void setHeroPosition(size_t row, size_t col)
        //
        // setter for Hero's position, which are private data members
        //      int HeroRow;
	    //      int HeroCol;
        //---------------------------------------------------------------------------------
        void setHeroPosition(size_t row, size_t col) {
            HeroRow = row;
            HeroCol = col;
            
        }

        
        //---------------------------------------------------------------------------------
        // findHero()
        //
        // updater for Hero's position, which are private data members
        //      int HeroRow;
	    //      int HeroCol;
        // this function should find Hero in board and update
        //      HeroRow and HeroCol with the Hero's updated position;
        // if Hero cannot be found in board, then set Hero's position to (-1,-1)
        //---------------------------------------------------------------------------------
        void findHero() {
            for (size_t i = 0; i < numRows; i++){
                for (size_t j = 0; j < numCols; j++){
                    if (board(i,j)->display() == 'H'){
                        setHeroPosition(i,j);
                        setHeroPosition(i,j);
                        return;
                    }
                }
            }

            setHeroPosition(-1,-1);
        }


        //makeMoves moves the hero based off of userinput passed in as the parameter HeroNextMove
        //All baddies are then moved in response to the heros movement.
        //Returns:
        // False: If the player (Hero) is no longer on the board
        // True: If the player (Hero) is still on the board
        bool makeMoves(char HeroNextMove) {

            for(size_t i = 0; i < numRows; i++){ //Resets all board elements moved to false
                for(size_t j = 0; j < numCols; j++){
                    board(i,j)->setMoved(false);
                }
            }

            // determine where hero proposes to move to
            size_t newR, newC;
            board(HeroRow,HeroCol)->setNextMove(HeroNextMove);
            board(HeroRow,HeroCol)->attemptMoveTo(newR,newC,HeroRow,HeroCol);
            try {
                // hero attempts to move out-of-bounds in rows
                if (newR < 0 || newR >= numRows) { 
                    throw runtime_error("Hero trying to move out-of-bounds with an invalid row");
                } 
            }
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                newR = HeroRow;
                cout << "Changing row for Hero position to stay in-bounds" << endl;
            }

            try {
                // hero attempts to move out -of- bounds in cols
                if (newC < 0 || newC >= numCols){
                    throw runtime_error("Hero trying to move out-of-bounds with invalid col");
                }
            }
            catch (runtime_error& excpt){
                cout << excpt.what() << endl;
                newC = HeroCol;
                cout << "Changing row for Hero position to stay in-bounds" << endl;
            }

            try {
                // hero attempts to space occupied by wall
                if (board(newR, newC)->display() == '+'){
                    throw runtime_error("Hero trying to move into spaced occupied by wall");
                }
            }
            catch (runtime_error& excpt){
                cout << excpt.what() << endl;
                cout << "Changing row for Hero position to stay in-bounds" << endl;
                if (board(newR, HeroCol)->display() != '+'){ //Ignore horizontal movement
                    newC = HeroCol;
                } else if (board(HeroRow, newC)->display() != '+'){ //Ignore vertical movement
                    newR = HeroRow;
                } else { //Ignore both the horizontal and vertical movement
                    newR = HeroRow;
                    newC = HeroCol;
                }
            }

            try {
                // hero moves into abyss
                if (board(newR, newC)->display() == '#'){
                    throw runtime_error("Hero fell into the abyss, womp womp");
                }
            }
            catch (runtime_error& excpt){
                cout << excpt.what() << endl;
                delete board(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                return false;
            }

            if (board(newR, newC)->display() == '*'){
                wonGame = true;
                delete board(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                return false;
            }

            if (board(newR, newC)->isBaddie()){
                wonGame = false;
                delete board(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                return false;
            }


            if (HeroNextMove != 's' || HeroNextMove != 'S'){ //Checks if the hero was supposed to move at all
                delete board(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                delete board(newR, newC);
                board(newR, newC) = new Hero(newR, newC);
                HeroRow = newR;
                HeroCol = newC;
            }
            // etc.

            for (size_t r = 0; r < numRows; r++ ){
                for (size_t c = 0; c < numCols; c++){
                    if (board(r,c)->isBaddie() && board(r,c)->getMoved() == false){ //Don't want to effect Hero again
                        board(r,c)->attemptMoveTo(newR, newC, HeroRow, HeroCol);                                       
                    
                        try {
                            // Baddie attempts to move out-of-bounds in rows
                            if (newR < 0 || newR >= numRows) { 
                                throw runtime_error("Baddie trying to move out-of-bounds with an invalid row");
                            } 
                        }
                        catch (runtime_error& excpt) {
                            cout << excpt.what() << endl;
                            newR = r;
                            cout << "Changing row for Baddie position to stay in-bounds" << endl;
                        }

                        try {
                            // Baddie attempts to move out -of- bounds in cols
                            if (newC < 0 || newC >= numCols){
                                throw runtime_error("Baddie trying to move out-of-bounds with invalid col");
                            }
                        }
                        catch (runtime_error& excpt){
                            cout << excpt.what() << endl;
                            newC = c;
                            cout << "Changing row for Baddie position to stay in-bounds" << endl;
                        }

                        try {
                            // baddie attempts to space occupied by wall
                            if (board(newR, newC)->display() == '+' || board(newR, newC)->display() == '*'){
                                throw runtime_error("Baddie trying to move into blocked space");
                            }
                        }
                        catch (runtime_error& excpt){
                            cout << excpt.what() << endl;
                            cout << "Changing row for Baddie position to stay in-bounds" << endl;
                            if (board(newR, c)->display() != '+' || board(newR, c)->display() == '*'){ //Ignore horizontal movement
                                newC = c;
                            } else if (board(r, newC)->display() != '+' || board(r, newC)->display() == '*'){ //Ignore vertical movement
                                newR = r;
                            } else { //Ignore both the horizontal and vertical movement
                                newR = r;
                                newC = c;
                            }
                        }

                        try {
                            // baddie attempts to space occupied by wall
                            if (board(newR, newC)->display() == '*'){
                                throw runtime_error("Baddie trying to move into exit");
                            }
                        }
                        catch (runtime_error& excpt){
                            cout << excpt.what() << endl;
                            cout << "Changing row for Baddie position to stay in-bounds" << endl;
                            if (board(newR, c)->display() != '*'){ //Ignore horizontal movement
                                newC = c;
                            } else if (board(r, newC)->display() != '*'){ //Ignore vertical movement
                                newR = r;
                            } else { //Ignore both the horizontal and vertical movement
                                newR = r;
                                newC = c;
                            }
                        }

                        try {
                            // baddie moves into abyss
                            if (board(newR, newC)->display() == '#'){
                                throw runtime_error("Baddie fell into the abyss, womp womp");
                            }
                        }
                        catch (runtime_error& excpt){
                            cout << excpt.what() << endl;
                            delete board(r, c);
                            board(r, c) = new Nothing(r, c);
                        }
                    
                        if(board(newR, newC)->isBaddie()){ //Collision with another baddie
                            newR = r;
                            newC = c;
                        }

                        if (newR == r && newC == c){
                            continue;
                        } else {
                            if (board(r,c)->display() == 'm' || board(r,c)->display() == 'M'){//Setting movement for a monster
                                delete board(newR, newC);
                                board(newR, newC) = new Monster(newR, newC);
                                board(newR,newC)->setPower(board(r,c)->getPower());
                                board(newR,newC)->setMoved(true);
                                delete board(r,c);
                                board(r,c) = new Nothing(r,c);
                            } else if (board(r,c)->display() == '~'){ //setting movement for a bat
                                delete board(r,c);
                                board(r,c) = new Nothing(r,c);
                                delete board(newR, newC);
                                board(newR, newC) = new Bat(r,c);
                                board(newR,newC)->setMoved(true);                            }
                        }
                    }
                }
            }

            findHero();
            if (HeroCol == -1 && HeroRow == -1){//The hero is no longer on the board
                return false;
            } else {
                return true;
            }
        }

    
};

#endif //_GAMEBOARD_H
