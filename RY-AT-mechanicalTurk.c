/*
 *  Knowledge_Island: mechanicalTurk
 *
 *  Ruby Yu and Alex Trinh
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Game.h"
#include "mechanicalTurk.h"

#define NUM_EDGES 72

action decideAction (Game g) {

    path allPaths[NUM_EDGES] = {
    "L","R","RR","RRL","RRLR","RRLRL","RRLRLL","RRLRLLR","RRLRLLRL","RRLRLLRLR","RRLRLLRLRL","RRLRLLRLRLL",
    "RRLRLLRLRLLR","RRLRLLRLRLLRL","RRLRLLRLRLLRLR","RRLRLLRLRLLRLRL","LR","LRL","LRLR","LRLRL","LRLRLR",
    "LRLRLRR","LRLRLRRL","LRLRLRRLR","LRLRLRRLRL","LRLRLRRLRLR","LRLRLRRLRLRR","LRLRLRRLRLRRL",
    "LRLRLRRLRLRRLR","LRLRLRRLRLRRLRL","RL","RRLL","RRLLRR","RRLLRLRR","RRLLRLRLR","RLRLRLRLR","LRRLRLRLRL",
    "LRLRRLRLRL","LRLRRLRLL","LRLRRLL","LRLRR","LRR","RLL","RLR","RRLLL","RRLLR","RRLLRL","RRLLRLR",
    "RRLLRLRL","RRLLRLRLL","RLRLRLRL","RLRLRLRLL","LRRLRLRLR","LRRLRLRLL","LRLRRLRLR","LRLRRLRL","LRLRRLR",
    "LRLRRL","LRRLL","LRRl","RLRL","RRLLRLL","RLRLRLR","LRRLRLRL","LRRLRL","LRRLR","RLRLL","RLRLR","RLRLRL",
    "RLRLRLL","LRRLRLR","LRRLRL",
    }

    int counter = 0;

    // get player code for AI
    int player = getWhoseTurn (g);
    // get number of students resources
    /*int numMJ = getStudents (g, player, STUDENT_MJ);
    int numMTV = getStudents (g, player, STUDENT_MTV);
    int numMMONEY = getStudents (g, player, STUDENT_MMONEY);
    int numTHD = getStudents(g,player,STUDENT_THD);*/
    int numBPS = getStudents(g,player,STUDENT_BPS);
    int numBQN = getStudents(g,player,STUDENT_BQN);
    // get number of campuses/arcs/Go8s
    int numARC = getARCs (g,player);
    int numCampus = getCampuses (g,player);
    //int numGo8 = getGO8s (g,player);

    // default action
    action nextAction;
    nextAction.actionCode = PASS;

    // make arcs
    if ((numBQN > 0 && numBPS > 0) && (numARC <= 3 * numCampus)){
        int validArcFound = 0;
        action getArc;
        getArc.actionCode = OBTAIN_ARC;
        //while loop to try to find possible path to build arc on
        while (counter < NUM_EDGES){
            getArc.destination = allPaths[counter];
            if (isLegalAction(g, getArc)){
                validArcFound = counter;
            }
            counter++
        }
        if (validArcFound){
            nextAction.actionCode = OBTAIN_ARC;
            nextAction.destination = allPaths[validArcFound];
        }
    }

    // make campuses
    /*if (numBPS > 0 && numBQN > 0 && numMJ > 0 && numMTV > 0) {
        while (counter < 72) {

        }
    }*/

    // start spinoff
    /*if (numMJ > 0 && numMTV > 0 && numMMONEY > 0) {
        nextAction.actionCode = START_SPINOFF;
    }*/

    return nextAction;
}
