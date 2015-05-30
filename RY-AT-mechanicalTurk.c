/*
 *  Knowledge_Island: mechanicalTurk
 *
 *  Ruby Yu and Alex Trinh
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Game.h"
#include "mechanicalTurk.h"

#define TRUE 1
#define FALSE 0

#define NUM_EDGES 72

#define NUM_DISCIPLINES 6

int isActionCostMet (int *numDiscipline, int actionCode);
action retrainStudents (Game g, int actionCode, int retrainTo, int retrainFrom);

action decideAction (Game g) {

	// all possible paths in the board
	path allPaths[NUM_EDGES] = {
    "L","R","RR","RRL","RRLR","RRLRL","RRLRLL","RRLRLLR","RRLRLLRL","RRLRLLRLR","RRLRLLRLRL","RRLRLLRLRLL",
    "RRLRLLRLRLLR","RRLRLLRLRLLRL","RRLRLLRLRLLRLR","RRLRLLRLRLLRLRL","LR","LRL","LRLR","LRLRL","LRLRLR",
    "LRLRLRR","LRLRLRRL","LRLRLRRLR","LRLRLRRLRL","LRLRLRRLRLR","LRLRLRRLRLRR","LRLRLRRLRLRRL",
    "LRLRLRRLRLRRLR","LRLRLRRLRLRRLRL","RL","RRLL","RRLLRR","RRLLRLRR","RRLLRLRLR","RLRLRLRLR","LRRLRLRLRL",
    "LRLRRLRLRL","LRLRRLRLL","LRLRRLL","LRLRR","LRR","RLL","RLR","RRLLL","RRLLR","RRLLRL","RRLLRLR",
    "RRLLRLRL","RRLLRLRLL","RLRLRLRL","RLRLRLRLL","LRRLRLRLR","LRRLRLRLL","LRLRRLRLR","LRLRRLRL","LRLRRLR",
    "LRLRRL","LRRLL","LRRL","RLRL","RRLLRLL","RLRLRLR","LRRLRLRL","LRRLRL","LRRLR","RLRLL","RLRLR","RLRLRL",
    "RLRLRLL","LRRLRLR","LRRLRL",
    };
	
	// arbitrary counters
    int i = 0;
	int counter = 0;

    // get player information
    int player = getWhoseTurn (g);

    // get ARCs information
    //int numARC = getARCs (g, player);
	int numVacantARC = 0;
	while (counter < NUM_EDGES) {
		if (getARC (g, allPaths[counter]) == VACANT_ARC) {
			numVacantARC++;
		}
		counter++;
	}

    int numCampus = getCampuses (g, player);
    //int numGo8 = getGO8s (g,player);

	// number of students for each discipline
	int curDiscipline = 0;
	int numDiscipline[NUM_DISCIPLINES];
	while (curDiscipline < NUM_DISCIPLINES) {
		numDiscipline[curDiscipline] = getStudents (g, player, curDiscipline);
		curDiscipline++;
	}

    // default action
    action nextAction;
    nextAction.actionCode = PASS;
	int retrainFrom = STUDENT_BPS;
	int retrainTo = STUDENT_BPS;

	if (numCampus > 5) {
		printf ("\n---In makeGO8()--\n");
		// make GO8
		if (isActionCostMet (numDiscipline, BUILD_GO8)){
		    //while loop to try to find possible campus to build GO8 on
			action isValidGO8;
			isValidGO8.actionCode = BUILD_GO8;
		    while (i < NUM_EDGES){
				strcpy (isValidGO8.destination, allPaths[i]);
		        if (isLegalAction (g, isValidGO8)){
		            nextAction = isValidGO8;
					i = NUM_EDGES;
		        } else {
		        	i++;
				}
		    }
		} else {
			action isValidRetrain;
			isValidRetrain.actionCode = RETRAIN_STUDENTS;
			retrainTo = STUDENT_MJ;
			while (retrainTo < NUM_DISCIPLINES) {
				if ((retrainTo != STUDENT_MTV) && (numDiscipline[retrainTo] < 1)) {
					while (retrainFrom < NUM_DISCIPLINES) {
						if (retrainFrom == STUDENT_MJ || retrainFrom == STUDENT_MMONEY) {
							if (numDiscipline[retrainFrom] >=
								getExchangeRate (g, player, retrainFrom, retrainTo)+3) {
								isValidRetrain.disciplineTo = retrainTo;
								isValidRetrain.disciplineFrom = retrainFrom;
							}
						} else {
							isValidRetrain.disciplineTo = retrainTo;
							isValidRetrain.disciplineFrom = retrainFrom;
						}
						if (isLegalAction (g, isValidRetrain)) {
							nextAction = isValidRetrain;
						}
						retrainFrom++;
					}
				}
				retrainTo++;
			}
		}
	} else if (numVacantARC == 0) {
		printf ("\n---In makeCampus()--\n");
		// make Campuses
		if (isActionCostMet (numDiscipline, BUILD_CAMPUS)){
		    //while loop to try to find possible vertex to build campus on
			action isValidCampus;
			isValidCampus.actionCode = BUILD_CAMPUS;
		    while (i < NUM_EDGES){
				strcpy (isValidCampus.destination, allPaths[i]);
		        if (isLegalAction (g, isValidCampus)){
		            nextAction = isValidCampus;
					i = NUM_EDGES;
		        } else {
		        	i++;
				}
		    }
		} else {
			action isValidRetrain;
			isValidRetrain.actionCode = RETRAIN_STUDENTS;
			while (retrainTo < STUDENT_MMONEY) {
				if (numDiscipline[retrainTo] < 1) {
					while (retrainFrom < NUM_DISCIPLINES) {
						if (retrainFrom == STUDENT_BPS || retrainFrom == STUDENT_BQN ||
							retrainFrom == STUDENT_MJ || retrainFrom == STUDENT_MTV) {
							if (numDiscipline[retrainFrom] >=
								getExchangeRate (g, player, retrainFrom, retrainTo)+1) {
								isValidRetrain.disciplineTo = retrainTo;
								isValidRetrain.disciplineFrom = retrainFrom;
							}
						} else {
							isValidRetrain.disciplineTo = retrainTo;
							isValidRetrain.disciplineFrom = retrainFrom;
						}
						if (isLegalAction (g, isValidRetrain)) {
							nextAction = isValidRetrain;
						}
						retrainFrom++;
					}
				}
				retrainTo++;
			}
		}
	} else if (numVacantARC != 0) {
		printf ("\n---In makeARC()--\n");
		// make arcs
		if (isActionCostMet (numDiscipline, OBTAIN_ARC)){
		    //while loop to try to find possible path to build arc on
			action isValidARC;
			isValidARC.actionCode = OBTAIN_ARC;
		    while (i < NUM_EDGES){
				strcpy (isValidARC.destination, allPaths[i]);
		        if (isLegalAction (g, isValidARC)){
		            nextAction = isValidARC;
					i = NUM_EDGES;
		        } else {
		        	i++;
				}
		    }
		} else {
			action isValidRetrain;
			isValidRetrain.actionCode = RETRAIN_STUDENTS;
			while (retrainTo < STUDENT_MJ) {
				if (numDiscipline[retrainTo] < 1) {
					while (retrainFrom < NUM_DISCIPLINES) {
						if (retrainFrom == STUDENT_BPS || retrainFrom == STUDENT_BQN) {
							if (numDiscipline[retrainFrom] >=
								getExchangeRate (g, player, retrainFrom, retrainTo)+1) {
								isValidRetrain.disciplineTo = retrainTo;
								isValidRetrain.disciplineFrom = retrainFrom;
							}
						} else {
							isValidRetrain.disciplineTo = retrainTo;
							isValidRetrain.disciplineFrom = retrainFrom;
						}
						if (isLegalAction (g, isValidRetrain)) {
							nextAction = isValidRetrain;
						}
						retrainFrom++;
					}
				}
				retrainTo++;
			}
		}
	}
	return nextAction;
}

int isActionCostMet (int *numDiscipline, int actionCode) {

	int isActionCostMet = FALSE;

	if (actionCode == OBTAIN_ARC && numDiscipline[STUDENT_BPS] >= 1 
		&& numDiscipline[STUDENT_BQN] >= 1) {
		isActionCostMet = TRUE;
	} else if (actionCode == BUILD_CAMPUS && numDiscipline[STUDENT_BPS] >= 1 
		&& numDiscipline[STUDENT_BQN] >= 1 && numDiscipline[STUDENT_MJ] >= 1
		&& numDiscipline[STUDENT_MTV] >= 1) {
		isActionCostMet = TRUE;
	} else if (actionCode == BUILD_GO8 && numDiscipline[STUDENT_MJ] >= 2 
		&& numDiscipline[STUDENT_MMONEY] >= 3) {
		isActionCostMet = TRUE;
	} else if (actionCode == START_SPINOFF && numDiscipline[STUDENT_MJ] >= 1
		&& numDiscipline[STUDENT_MTV] >= 1 && numDiscipline[STUDENT_MMONEY] >= 1) {
		isActionCostMet = TRUE;
	}
	
	return isActionCostMet;
}

/*action retrainStudents (Game g, int actionCode, int retrainTo, int retrainFrom) {

	int player = getWhoseTurn (g);
	int rate = 0;

	int numDiscipline[NUM_DISCIPLINES];
	int curDiscipline = 0;
	while (curDiscipline < NUM_DISCIPLINES) {
		numDiscipline[curDiscipline] = getStudents (g, player, curDiscipline);
		curDiscipline++;
	}

	

	return retrainStudents;
}*/
