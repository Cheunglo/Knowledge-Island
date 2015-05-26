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
    int numMJ = getStudents (g, player, STUDENT_MJ);
    int numMTV = getStudents (g, player, STUDENT_MTV);
    int numMMONEY = getStudents (g, player, STUDENT_MMONEY);

    action nextAction;

    if (numMJ > 0 && numMTV > 0 && numMMONEY > 0) {
        nextAction.actionCode = START_SPINOFF;
    } else {
        nextAction.actionCode = PASS;
    }

    makeAction (g, nextAction);

    return nextAction;
}
