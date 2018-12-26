/* Krishna Mani - 2018
 * Implementation of the Tremaux Maze solving algorithm
 * https://github.com/kmani314/
*/

#include "micromouseserver.h"

struct coordinateSystem {
    int xpos;
    int ypos;
};

static struct coordinateSystem position = {0, 0}; // create a coordinate system that starts at 0,0

static int markers[20][20] = {{0}}; // path markers

static int orientation = 0; // Track Orientation in order to know how to increment the coordinate system

static void (microMouseServer::*turnPointer)();
static int intent;
static int increment;

static int behind;
static int forward;
static int left;
static int right;
static int possibilites[3];

int orientConstrain(int x) { // ensure the orientation is between 0 and 3
    if(x < 0) {
        return x + 4;
    }
    if(x > 3) {
        return x - 4;
    }
    return x;
}

bool twoOfThree(bool x, bool y, bool z) {
    return x ? (y || z) : (y && z);
}

static int xTable[3][4] = {{0, 1, 0, -1}, // Lookup tables to know how to do operations on the coordinate system
                           {1, 0, -1, 0},
                           {-1, 0, 1, 0}};

static int yTable[3][4] = {{1, 0, -1, 0},
                           {0, -1, 0, 1},
                           {0, 1, 0, -1}};

void microMouseServer::studentAI() {
    moveForward();

    position.xpos += xTable[0][orientation]; // use the lookup table to find out how to increment the x/y coords
    position.ypos += yTable[0][orientation];

    turnRight();
    behind = markers[position.xpos + xTable[1][orientConstrain(orientation + 1)]][position.ypos + yTable[1][orientConstrain(orientation + 1)]];
    turnLeft();
    if(isWallForward() && (isWallLeft() ^ isWallRight())) { // Are we at a turn?

        turnPointer = !isWallLeft() ? &microMouseServer::turnLeft : &microMouseServer::turnRight; // Generalized logic using function pointers
        intent = !isWallLeft() ? 2 : 1;
        increment = !isWallLeft() ? -1 : 1;

        if(markers[position.xpos + xTable[intent][orientation]][position.ypos + yTable[intent][orientation]] == 0) { // Is this turn unmarked?
            (this->*turnPointer)();
            orientation = orientConstrain(orientation + increment);
        } else if(behind == 1) { // If not, is the path we just took only marked once?
            turnRight();
            turnRight();
            orientation = orientConstrain(orientation + 2);
        } else { // If the path we just took is marked twice and the other path has been taken once, take it.
            (this->*turnPointer)();
            orientation = orientConstrain(orientation + increment);
        }

    } else if(twoOfThree(!isWallLeft(), !isWallRight(), !isWallForward())) { // Are we at a 3/4 way junction?
        forward = isWallForward() ? 0 : markers[position.xpos + xTable[0][orientation]][position.ypos + yTable[0][orientation]];
        left = isWallLeft() ? 0 : markers[position.xpos + xTable[2][orientation]][position.ypos + yTable[2][orientation]];
        right = isWallRight() ? 0 : markers[position.xpos + xTable[1][orientation]][position.ypos + yTable[1][orientation]];

        if(forward == 0 && (left == 0 && right == 0)) {
            if(!isWallRight()) {
                turnRight();
                orientation = orientConstrain(orientation + 1);
            } else if(!isWallLeft()) {
                turnLeft();
                orientation = orientConstrain(orientation - 1);
            } // Otherwise don't turn because something's screwed up for sure

        } else if(behind == 1) {
            turnRight();
            turnRight();
            orientation = orientConstrain(orientation + 2);
        } else {
            // take the least taken path
            if((right <= forward && (right <= left)) && !isWallRight()) {
                turnRight();
                orientation = orientConstrain(orientation + 1);
            } else if ((left <= forward && (left <=right)) && !isWallLeft()) {
                turnLeft();
                orientation = orientConstrain(orientation - 1);
            }
        }
    } else {
        markers[position.xpos][position.ypos] += 1;
        if(isWallLeft() && isWallRight() && isWallForward()) {
            turnRight();
            turnRight();
            orientation = orientConstrain(orientation + 2);
        }
    }
}
