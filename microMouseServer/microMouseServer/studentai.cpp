#include "micromouseserver.h"
#include <iostream> // for debug

using namespace std;

struct coordinateSystem {
    int xpos;
    int ypos;
};

static struct coordinateSystem position = {0, 0}; //create a coordinate system that starts at 0,0
static bool visited[20][20] = {{0}}; //set all squares to not visited (false)

void microMouseServer::studentAI() {
    //moveForward();
    //if (!isWallRight() && !visited[position.xpos + 1][position.ypos]) {

    //} else if(!isWallForward() && !visited[position.xpos][position.ypos + 1]) {
    //}

    /*
    * The following are the eight functions that you can call. Feel free to create your own fuctions as well.
    * Remember that any solution that calls moveForward more than once per call of studentAI() will have points deducted.
    *
    *The following functions return if there is a wall in their respective directions
    *bool isWallLeft();
    *bool isWallRight();
    *bool isWallForward();
    *
    *The following functions move the mouse. Move forward returns if the mouse was able to move forward and can be used for error checking
    *bool moveForward();
    *void turnLeft();
    *void turnRight();
    *
    * The following functions are called when you need to output something to the UI or when you have finished the maze
    * void foundFinish();
    * void printUI(const char *mesg);
    */

}
