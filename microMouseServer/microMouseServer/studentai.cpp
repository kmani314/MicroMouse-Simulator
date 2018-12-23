#include "micromouseserver.h"
#include <iostream> // for debug

using namespace std;

struct coordinateSystem {
    int xpos;
    int ypos;
};

static struct coordinateSystem position = {0, 0}; //create a coordinate system that starts at 0,0 - This is to track which squares have been visited and which have not

static bool visited[20][20] = {{0}}; // Track all squares - set all squares to not visited (false)

int orientation = 0; //Track Orientation in order to know how to increment the coordinate system

//vector<string> moves;

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

    printUI("FORWARD");

    switch(orientation) {
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


