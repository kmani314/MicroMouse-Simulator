#include "micromouseserver.h"

struct coordinateSystem {
    int xpos;
    int ypos;
};

struct pathMarker { // my implementation of the well known pathfinder Tremaux's Algorithm - except instead of marking the beginning and end, it marks every square
    int crossed;
};

static struct coordinateSystem position = {0, 0}; // create a coordinate system that starts at 0,0 - This is to track which squares have been visited and which have not

static pathMarker markers[20][20] = {{{0}}};

static int orientation = 0; // Track Orientation in order to know how to increment the coordinate system

int orientConstrain(int x) {
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
static int xTable[3][4] = {{0, 1, 0, 1}, // Forward, Right, Left
                           {1, 0, -1, 0},
                           {-1, 0, 1, 0}};

static int yTable[3][4] = {{1, 0, -1, 0},
                           {0, -1, 0, 1},
                           {0, 1, 0, -1}};
void microMouseServer::studentAI() {
    moveForward(); // we know this will work because the mouse was oriented in the last run

    switch(orientation) { //figure out how to increment the x/y values for a certain orientation
        case(0):
            position.ypos += 1; 
            break;
        case(1):
            position.xpos += 1;
            break;
        case(2):
            position.ypos -= 1;
            break;
        case(3):
            position.xpos -= 1;
    }

    if(twoOfThree(isWallLeft(), isWallRight(), isWallForward())) {
        markers[position.xpos][position.ypos].crossed += 1; // add a mark to this path
    }

    if (!isWallRight() && markers[position.xpos + xTable[1][orientation]][position.ypos + yTable[1][orientation]].crossed < 2) {
        turnRight();
        orientation = orientConstrain(orientation + 1);

    } else if(!isWallForward() && markers[position.xpos + xTable[0][orientation]][position.ypos + yTable[0][orientation]].crossed < 2) {
        // no orientation to do here, since moveForward() is called on next run

    } else if(!isWallLeft() && markers[position.xpos + xTable[2][orientation]][position.ypos + yTable[2][orientation]].crossed < 2) {
        turnLeft();
        orientation = orientConstrain(orientation - 1);

    } else {
        turnRight(); // make a u-turn if there is nowhere to go
        turnRight();
        orientation = orientConstrain(orientation + 2);
    }
}
