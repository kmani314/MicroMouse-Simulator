/* Krishna Mani - 2018
* DFS Iterative Graph Traversal
* https://github.com/kmani314/
*/

#include "micromouseserver.h"
#include <iostream>
using namespace std;

struct coordinateSystem {
    int xpos;
    int ypos;
};

static struct coordinateSystem position = {0, 0}; // create a coordinate system that starts at 0,0

static bool markers[20][20] = {{false}}; // path markers

static int orientation = 0; // Track Orientation in order to know how to increment the coordinate system

static bool backtracking = false;
static vector<int> moves;

static int xTable[3][4] = {{0, 1, 0, -1}, {1, 0, -1, 0}, {-1, 0, 1, 0}};

static int yTable[3][4] = {{1, 0, -1, 0}, {0, -1, 0, 1}, {0, 1, 0, -1}};

int orientConstrain(int x) {
    if(x < 0) {
        return x + 4;
    }
    if(x > 3) {
        return x - 4;
    }
    return x;
}
int getDirection(int moveToTake, int moveTaken) {
    int x = moveTaken - moveToTake;
    if(x < 0) {
        return x + 3;
    }
    if(x > 2) {
        return x - 3;
    }
    return x;
}
bool twoOfThree(bool x, bool y, bool z) {
    return x ? (y || z) : (y && z);
}

void microMouseServer::studentAI() {
    moveForward();

    position.xpos += xTable[0][orientation];
    position.ypos += yTable[0][orientation];

    markers[position.xpos][position.ypos] = true;
    if(backtracking) {
        if(!isWallRight() && !markers[position.xpos + xTable[1][orientation]][position.ypos + yTable[1][orientation]]) {
            backtracking = false;
            turnRight();
            orientation = orientConstrain(orientation + 1);
            int x = moves.back();
            moves.pop_back();
            moves.push_back(getDirection(1, x));
        } else if(!isWallLeft() && !markers[position.xpos + xTable[2][orientation]][position.ypos + yTable[2][orientation]]) {
            backtracking = false;
            turnLeft();
            orientation = orientConstrain(orientation - 1);
            int x = moves.back();
            moves.pop_back();
            moves.push_back(getDirection(2, x));
        } else if(!isWallForward() && !markers[position.xpos + xTable[0][orientation]][position.ypos + yTable[0][orientation]]) {
            backtracking = false;
            int x = moves.back();
            moves.pop_back();
            moves.push_back(getDirection(0, x));
        } else {
            switch(moves.back()) {
            case(1):
                turnLeft();
                orientation = orientConstrain(orientation - 1);
                break;
            case(2):
                turnRight();
                orientation = orientConstrain(orientation + 1);
                break;
            }

            moves.pop_back();
        }
        return;
    }


    if (!isWallRight() && !markers[position.xpos + xTable[1][orientation]][position.ypos + yTable[1][orientation]]) { // right hand wall following
        turnRight();
        moves.push_back(1);
        orientation = orientConstrain(orientation + 1);
    } else if (!isWallForward() && !markers[position.xpos + xTable[0][orientation]][position.ypos + yTable[0][orientation]]) {
        // go forward
        moves.push_back(0);
    } else if (!isWallLeft() && !markers[position.xpos + xTable[2][orientation]][position.ypos + yTable[2][orientation]]) {
        turnLeft();
        orientation = orientConstrain(orientation - 1);
        moves.push_back(2);
    } else {
        turnRight(); // Backtrack if dead end hit
        turnRight();
        backtracking = true;
        orientation = orientConstrain(orientation + 2);
    }
}
