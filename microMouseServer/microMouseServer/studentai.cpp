/* Krishna Mani - 2018
* Maze Solving using Iterative Depth First Search
* https://github.com/kmani314/
*/
#include "micromouseserver.h"

struct coordinateSystem { // Struct that represents the current XY coordinates of the mouse
    int xpos;
    int ypos;
};

static struct coordinateSystem position = {0, 0}; // create a coordinate system that starts at 0,0

static bool markers[20][20] = {{false}}; // path markers for knowing which squares have been visited. Every square starts as unvisited

static int orientation = 0; // Track orientation - 0 = up, 1 = right, 2 = down, 3 = left

static bool backtracking = false;
static std::vector<int> moves; // Vector that tracks the moves that have been taken. 0 = Forward, 1 = Right, 2 = Left

static int xTable[3][4] = {{0, 1, 0, -1}, {1, 0, -1, 0}, {-1, 0, 1, 0}}; // Lookup tables for getting right / left / forward values for each orientation (matrix transformation)

static int yTable[3][4] = {{1, 0, -1, 0}, {0, -1, 0, 1}, {0, 1, 0, -1}};

static int dirTable[3][3] = {{0, 2, 1}, {2, 0, 0}, {1, 0, 0}}; // Lookup table for combining two branches of a path together

int orientConstrain(int x) { // Constrain orientation so it is between 0 and 3 inclusive
    if(x < 0) {
        return x + 4;
    }
    if(x > 3) {
        return x - 4;
    }
    return x;
}
void microMouseServer::studentAI() {
    moveForward(); // The moves vector does not represent every move that the mouse has taken. The vector represents the moves that come in between these forward commands. That is why a {2, 2} in the vector really means Left, Forward, Left, Forward.

    if(moves.size() > 2 && !backtracking) { // if the vector is of a valid size that a solution is possible and we are not backtracking, check whether we are in the solution
        std::vector<int> solution; // Vector representing the pattern the mouse would take in a solution structure - {2, 2} - Left, Forward, Left, Forward
        solution.push_back(2);
        solution.push_back(2);
        auto iterator = std::search(moves.end() - 2, moves.end(), solution.begin(), solution.end()); // search the last two values of the moves vector
        if((iterator != moves.end()) && !isWallLeft()) foundFinish(); // The solution is only valid if there is no wall to the left, preventing false solution triggering
    }
    position.xpos += xTable[0][orientation]; // Increment the coordinate system according to the x/y lookup tables
    position.ypos += yTable[0][orientation];

    markers[position.xpos][position.ypos] = true; // mark this square as visited

    if(backtracking) { // if we hit a dead end in the previous call, start backtracking until a valid path is found
        if(!isWallRight() && !markers[position.xpos + xTable[1][orientation]][position.ypos + yTable[1][orientation]]) { // Break out of backtracking loop if a path that is valid and not visited is found
            backtracking = false;
            turnRight();
            orientation = orientConstrain(orientation + 1); // Increment orientation and constrain it
            int x = moves.back(); // Get the move that resulted in this branch
            moves.pop_back(); // Remove it
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
            switch(moves.back()) { // If we could not find another valid path, perform the last moves in the vector backwards to backtrack - case 0 (Forward) is not included because we would not need to do anything
            case(1):
                turnLeft(); // perform the opposite move (1 = right, but we are turning left)
                orientation = orientConstrain(orientation - 1);
                break;
            case(2):
                turnRight();
                orientation = orientConstrain(orientation + 1);
                break;
            }
            moves.pop_back(); // delete the last move after running it
        }
        return; // exit to make sure that the normal wall follower does not execute
    }
    if (!isWallRight() && !markers[position.xpos + xTable[1][orientation]][position.ypos + yTable[1][orientation]]) { // if there isn't a wall to the right and it hasn't been visited, take it - right hand wall following with checks to prevent loops
        turnRight();
        moves.push_back(1); // Push a 1 (right) on to the vector
        orientation = orientConstrain(orientation + 1); // Increment orientation and constrain it
    } else if (!isWallForward() && !markers[position.xpos + xTable[0][orientation]][position.ypos + yTable[0][orientation]]) {
        moves.push_back(0); // Push a 0 (forward) on to the vector
    } else if (!isWallLeft() && !markers[position.xpos + xTable[2][orientation]][position.ypos + yTable[2][orientation]]) {
        turnLeft();
        orientation = orientConstrain(orientation - 1);
        moves.push_back(2); // Push a 2 (Left) on to the vector
    } else {
        turnRight(); // Turn around if no suitable path is found
        turnRight();
        backtracking = true; // Enable backtracking when there are no valid paths to take
        orientation = orientConstrain(orientation + 2); // Increment orientation for a U-Turn and constrain it
    }
}
