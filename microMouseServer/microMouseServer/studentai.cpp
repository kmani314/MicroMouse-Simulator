#include "micromouseserver.h"
#include <iostream> // for debug

using namespace std;

struct coordinateSystem {
    int xpos;
    int ypos;
};

static struct coordinateSystem position = {0, 0}; //create a coordinate system that starts at 0,0 - This is to track which squares have been visited and which have not

static bool visited[20][20] = {{0}}; // Track all squares - set all squares to not visited (false)

// int orientation = 0; //Track Orientation in order to know how to increment the coordinate system

vector<string> moves;

void microMouseServer::studentAI() {
    moveForward(); // we know this will work because the mouse was oriented in the last run

   // moves.push_back("FORWARD"); //push this move to
    printUI("FORWARD");

    if (!isWallRight()) {
        turnRight();
     //   moves.push_back("RIGHT");
        printUI("RIGHT");
    } else if(!isWallForward()) {
        // no orientation to do here, since moveForward() is called on next run
    } else if(!isWallLeft()) {
        turnLeft();
       // moves.push_back("LEFT");
        printUI("LEFT");
    } else {
        printUI("U-TURN");
        turnRight();
        turnRight();
    }
}
