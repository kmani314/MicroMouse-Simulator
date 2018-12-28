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

static bool markers[20][20] = {{false}}; // path markers

static int orientation = 0; // Track Orientation in order to know how to increment the coordinate system

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

bool twoOfThree(bool x, bool y, bool z) {
    return x ? (y || z) : (y && z);
}

void microMouseServer::studentAI() {
    moveForward();

    position.xpos += xTable[0][orientation];
    position.ypos += yTable[0][orientation];
    // implement dfs. go right until a dead end, go back and go left, repeat.
    markers[position.xpos][position.ypos] = true;

    if (!isWallRight()) { // right hand wall following
        turnRight();
    } else if (!isWallForward()) {
        // go forward
    } else if (!isWallLeft()) {
        turnLeft();
    } else {
        turnRight(); // Backtrack if dead end hit
        turnRight();
    }
}
