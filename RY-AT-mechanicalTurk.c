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

action decideAction (Game g) {

    path pathVertex[54];
    pathVertex[0] = "R";
    pathVertex[1] = "RR";
    pathVertex[2] = "RRL";

    int player = getWhoseTurn (g);
    //get number of resources
    int numMJ = getStudents (g, player, STUDENT_MJ);
    int numMTV = getStudents (g, player, STUDENT_MTV);
    int numMMONEY = getStudents (g, player, STUDENT_MMONEY);
    int numTHD = getStudents(g,player,STUDENT_THD);
    int numBPS = getStudents(g,player,STUDENT_BPS);
    int numBQN = getStudents(g,player,STUDENT_BQN);
    
    
    action nextAction;
    nextAction.actionCode = PASS;
    //make arcs
    if (numBQN >= 1 && numBPS >= 1){
        int validArcFound = 0;
        int counter=0;
        action getArc;
        getArc.actionCode = OBTAIN_ARC;
        //while loop to try to find possible path to build arc on
        while (counter < 72){
            getArc.destination = allPaths[counter];
            if (isLegalAction(g, getArc)){
                validArcFound = counter;
            }
            
            counter++
        }
        
        if(validArcFound){
            nextAction.actionCode = OBTAIN_ARC;
            nextAction.destination = allPaths[validArcFound];
        }
    }
    
    //start spinoff
    if (numMJ > 0 && numMTV > 0 && numMMONEY > 0) {
        nextAction.actionCode = START_SPINOFF;
    }

    return nextAction;
}
