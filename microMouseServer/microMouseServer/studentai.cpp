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
static int forward;
static int left;
static int right;
static int behind;

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
    moveForward(); // we know this will work because the mouse was oriented in the last run

    position.xpos += xTable[0][orientation]; // use the lookup table to find out how to increment the x/y coords
    position.ypos += yTable[0][orientation];


    if((isWallForward() && (isWallLeft() ^ isWallRight())) || !(twoOfThree(isWallLeft(), isWallRight(), isWallForward()))) { // junction / turn finding, these are the squares where decisions need to be made.

        forward = isWallForward() ? markers[position.xpos + xTable[0][orientation]][position.ypos + yTable[0][orientation]] : 0; // If there is a wall, it's not worth getting the value of the marker
        left = isWallLeft() ? markers[position.xpos + xTable[2][orientation]][position.ypos + yTable[2][orientation]] : 0;
        right = isWallRight() ? markers[position.xpos + xTable[1][orientation]][position.ypos + yTable[1][orientation]] : 0;
        turnRight();
        behind = isWallRight() ? markers[position.xpos + xTable[1][orientation]][position.ypos + yTable[1][orientation]] : 0; // Turn around to get the value of the path we were just on
        turnLeft();


    } else {
        markers[position.xpos][position.ypos] += 1;
    }
}
