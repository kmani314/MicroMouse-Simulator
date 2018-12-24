#include "micromouseserver.h"

struct coordinateSystem {
    int xpos;
    int ypos;
};

struct pathMarker { // my implementation of the well known pathfinder Tremaux's Algorithm - except instead of marking the beginning and end, it marks every square
    int crossed;
};

static struct coordinateSystem position = {0, 0}; //create a coordinate system that starts at 0,0 - This is to track which squares have been visited and which have not

static pathMarker markers[20][20] = {{{0}}};

static int orientation = 0; //Track Orientation in order to know how to increment the coordinate system
static int xDir = 1; // Track how the orientation manifests into looking for blocks that have been travelled
static int yDir = 1;

int orientConstrain(int x) {
    if(x < 0) {
        return x + 4;
    }
    if(x > 3) {
        return x - 4;
    }
    return x;
}

void microMouseServer::studentAI() {
    moveForward(); // we know this will work because the mouse was oriented in the last run

    switch(orientation) {
        case(0):
            position.ypos += 1;
            yDir = 1;
            break;
        case(1):
            position.xpos += 1;
            xDir = 1;
            break;
        case(2):
            position.ypos -= 1;
            yDir = -1;
            break;
        case(3):
            position.xpos -= 1;
            xDir = -1;
    }

    markers[position.xpos][position.ypos].crossed += 1;

    if (!isWallRight()) {
        turnRight();
        orientation = orientConstrain(orientation + 1);

    } else if(!isWallForward()) {
        // no orientation to do here, since moveForward() is called on next run

    } else if(!isWallLeft()) {
        turnLeft();
       // moves.push_back("LEFT");
        orientation = orientConstrain(orientation - 1);

    } else {
        turnRight();
        turnRight();
        orientation = orientConstrain(orientation + 2);
    }
}
