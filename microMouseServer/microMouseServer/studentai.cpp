/* Krishna Mani - 2018
* Iterative Deepening DFS Graph Traversal
* https://github.com/kmani314/
*/
#include "micromouseserver.h"
#include <algorithm>
#include <iostream>
using namespace std;
struct coordinateSystem {
    int xpos;
    int ypos;
};

static struct coordinateSystem position = {0, 0}; // create a coordinate system that starts at 0,0

static bool markers[20][20] = {{false}}; // path markers for knowing which squares have been visited

static int orientation = 0; // Track Orientation in order to know how to increment the coordinate system

static bool backtracking = false;
static std::vector<int> moves;

static int xTable[3][4] = {{0, 1, 0, -1}, {1, 0, -1, 0}, {-1, 0, 1, 0}}; // lookup tables for getting right / left / forward values

static int yTable[3][4] = {{1, 0, -1, 0}, {0, -1, 0, 1}, {0, 1, 0, -1}};

static int dirTable[3][3] = {{0, 2, 1}, {2, 0, 0}, {1, 0, 0}}; // lookup table for path splicing

int orientConstrain(int x) { // constrain orientation so it is between 0 and 3 inclusive
    if(x < 0) {
        return x + 4;
    }
    if(x > 3) {
        return x - 4;
    }
    return x;
}
void microMouseServer::studentAI() {
    moveForward();
    if(moves.size() > 2 && !backtracking) {
        vector<int> solution;
        solution.push_back(2);
        solution.push_back(2);
        auto iterator = std::search(moves.end() - 2, moves.end(), solution.begin(), solution.end());
        if((iterator != moves.end()) && !isWallLeft()) foundFinish();
    }
    position.xpos += xTable[0][orientation];
    position.ypos += yTable[0][orientation];
    markers[position.xpos][position.ypos] = true;
    if(backtracking) { // if we hit a dead end in the previous call, start backtracking until a valid path is found
        if(!isWallRight() && !markers[position.xpos + xTable[1][orientation]][position.ypos + yTable[1][orientation]]) { // break out of backtrack if valid path found
            backtracking = false;
            turnRight();
            orientation = orientConstrain(orientation + 1);
            int x = moves.back();
            moves.pop_back();
            moves.push_back(dirTable[1][x]); // Add the move that would connect the two branches together to the moves vector
        } else if(!isWallLeft() && !markers[position.xpos + xTable[2][orientation]][position.ypos + yTable[2][orientation]]) {
            backtracking = false;
            turnLeft();
            orientation = orientConstrain(orientation - 1);
            int x = moves.back();
            moves.pop_back();
            moves.push_back(dirTable[2][x]);
        } else if(!isWallForward() && !markers[position.xpos + xTable[0][orientation]][position.ypos + yTable[0][orientation]]) {
            backtracking = false;
            int x = moves.back();
            moves.pop_back();
            moves.push_back(dirTable[0][x]);
        } else {
            switch(moves.back()) { // if we could not find another valid path, perform the last moves in the vector backwards
            case(1):
                turnLeft();
                orientation = orientConstrain(orientation - 1);
                break;
            case(2):
                turnRight();
                orientation = orientConstrain(orientation + 1);
                break;
            }
            moves.pop_back(); // delete the last move after doing it
        }
        return; // exit to make sure that the normal wall follower does not execute
    }
    if (!isWallRight() && !markers[position.xpos + xTable[1][orientation]][position.ypos + yTable[1][orientation]]) { // right hand wall following ensuring paths aren't visited twice
        turnRight();
        moves.push_back(1);
        orientation = orientConstrain(orientation + 1);
    } else if (!isWallForward() && !markers[position.xpos + xTable[0][orientation]][position.ypos + yTable[0][orientation]]) {
        moves.push_back(0);
    } else if (!isWallLeft() && !markers[position.xpos + xTable[2][orientation]][position.ypos + yTable[2][orientation]]) {
        turnLeft();
        orientation = orientConstrain(orientation - 1);
        moves.push_back(2);
    } else {
        turnRight(); // Turn around if no suitable path is found
        turnRight();
        backtracking = true; // enable backtracking
        orientation = orientConstrain(orientation + 2); // increment orientation
    }
}
