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

#define MAX_DISCIPLINE 6

int isActionCostMet (int actionCode, int *numDiscipline);
int canRetrain (Game g, int actionCode, int *numDiscipline);
int determineRetrainTo (int actionCode, int *numDiscipline);
int determineRetrainFrom (Game g, int actionCode, int *numDiscipline);

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
	int numDiscipline[MAX_DISCIPLINE];
	while (curDiscipline < MAX_DISCIPLINE) {
		numDiscipline[curDiscipline] = getStudents (g, player, curDiscipline);
		curDiscipline++;
	}

	// default action
    action nextAction;
    nextAction.actionCode = PASS;

	if (numCampus > 5) {
		printf ("\n---In makeGO8()--\n");
		// make GO8
		if (isActionCostMet ( BUILD_GO8, numDiscipline)) {
		    //while loop to try to find possible campus to build GO8 on
			action isValidGO8;
			isValidGO8.actionCode = BUILD_GO8;
		    while (i < NUM_EDGES) {
			strcpy (isValidGO8.destination, allPaths[i]);
		        if (isLegalAction (g, isValidGO8)) {
		            nextAction = isValidGO8;
					i = NUM_EDGES;
		        } else {
		        	i++;
				}
		    }
		} else if (canRetrain (g, BUILD_GO8, numDiscipline)) {
			action isValidRetrain;
			isValidRetrain.actionCode = RETRAIN_STUDENTS;
			isValidRetrain.disciplineTo = determineRetrainTo (BUILD_GO8, numDiscipline);
			isValidRetrain.disciplineFrom = determineRetrainFrom (g, BUILD_GO8, numDiscipline);
			if (isLegalAction (g, isValidRetrain)) {
				nextAction = isValidRetrain;
			}
		}
	} else if (numVacantARC == 0) {
		printf ("\n---In makeCampus()--\n");
		// make Campuses
		if (isActionCostMet (BUILD_CAMPUS, numDiscipline)){
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
		} else if (canRetrain (g, BUILD_CAMPUS, numDiscipline)) {
			action isValidRetrain;
			isValidRetrain.actionCode = RETRAIN_STUDENTS;
			isValidRetrain.disciplineTo = determineRetrainTo (BUILD_CAMPUS, numDiscipline);
			isValidRetrain.disciplineFrom = determineRetrainFrom (g, BUILD_CAMPUS, numDiscipline);
			if (isLegalAction (g, isValidRetrain)) {
				nextAction = isValidRetrain;
			}
		}
	} else if (numVacantARC != 0) {
		printf ("\n---In makeARC()--\n");
		// make arcs
		if (isActionCostMet (OBTAIN_ARC, numDiscipline)) {
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
		} else if (canRetrain (g, OBTAIN_ARC, numDiscipline)) {
			action isValidRetrain;
			isValidRetrain.actionCode = RETRAIN_STUDENTS;
			isValidRetrain.disciplineTo = determineRetrainTo (OBTAIN_ARC, numDiscipline);
			isValidRetrain.disciplineFrom = determineRetrainFrom (g, OBTAIN_ARC, numDiscipline);
			if (isLegalAction (g, isValidRetrain)) {
				nextAction = isValidRetrain;
			}
		}
	}
	return nextAction;
}

int isActionCostMet (int actionCode, int *numDiscipline) {

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

int canRetrain (Game g, int actionCode, int *numDiscipline) {

	int canRetrain = FALSE;
	int player = getWhoseTurn (g);

	// exchange rates for retraining based on retraining centres
	// we are only concerned with disciplineFrom
	// thus the last argument, disciplineTo is unimportant
	int rateBPS = getExchangeRate (g, player, STUDENT_BPS, STUDENT_MMONEY);
	int rateBQN = getExchangeRate (g, player, STUDENT_BQN, STUDENT_MMONEY);
	int rateMJ = getExchangeRate (g, player, STUDENT_MJ, STUDENT_MMONEY);
	int rateMTV = getExchangeRate (g, player, STUDENT_MTV, STUDENT_MMONEY);
	int rateMMONEY = getExchangeRate (g, player, STUDENT_MMONEY, STUDENT_MMONEY);

	int numBPS = numDiscipline[STUDENT_BPS];
	int numBQN = numDiscipline[STUDENT_BQN];
	int numMJ = numDiscipline[STUDENT_MJ];
	int numMTV = numDiscipline[STUDENT_MTV];
	int numMMONEY = numDiscipline[STUDENT_MMONEY];

	if (actionCode == OBTAIN_ARC) {
		if (numBPS > rateBPS || numBQN > rateBQN || numMJ >= rateMJ ||
			numMTV >= rateMTV || numMMONEY >= rateMMONEY) {
			canRetrain = TRUE;
		}
	} else if (actionCode == BUILD_CAMPUS) {
		if (numBPS > rateBPS || numBQN > rateBQN || numMJ > rateMJ ||
			numMTV > rateMTV || numMMONEY >= rateMMONEY) {
			canRetrain = TRUE;
		}
	} else if (actionCode == BUILD_GO8)	{
		if (numBPS >= rateBPS || numBQN >= rateBQN || numMJ > rateMJ+1 ||
			numMTV >= rateMTV || numMMONEY > rateMMONEY+2) {
			canRetrain = TRUE;
		}
	} else if (actionCode == START_SPINOFF) {
		if (numBPS >= rateBPS || numBQN >= rateBQN || numMJ > rateMJ ||
			numMTV > rateMTV || numMMONEY > rateMMONEY) {
			canRetrain = TRUE;
		}
	}

	return canRetrain;
}

int determineRetrainTo (int actionCode, int *numDiscipline) {

	int retrainTo = STUDENT_BPS;
	int retrainTo_MAX = STUDENT_BPS;
	int curDiscipline = STUDENT_BPS;

	if (actionCode == BUILD_GO8) {
		if (numDiscipline[STUDENT_MJ] < 2) {
			retrainTo = STUDENT_MJ;
		} else if (numDiscipline[STUDENT_MMONEY] < 3) {
			retrainTo = STUDENT_MMONEY;
		}
	} else {
		if (actionCode == OBTAIN_ARC) {
			curDiscipline = STUDENT_BPS;
			retrainTo_MAX = STUDENT_MJ;
		} else if (actionCode == BUILD_CAMPUS) {
			curDiscipline = STUDENT_BPS;
			retrainTo_MAX = STUDENT_MMONEY;
		} else if (actionCode == START_SPINOFF) {
			curDiscipline = STUDENT_MJ;
			retrainTo_MAX = MAX_DISCIPLINE;
		}
		while (curDiscipline < retrainTo_MAX) {
			if (numDiscipline[curDiscipline] < 1) {
				retrainTo = curDiscipline;
			}
			curDiscipline++;
		}
	}

	return retrainTo;
}

int determineRetrainFrom (Game g, int actionCode, int *numDiscipline) {

	int player = getWhoseTurn (g);
	int retrainFrom = STUDENT_BPS;
	int curDiscipline = STUDENT_BPS;
	int rate = 0;

	if (actionCode == OBTAIN_ARC) {
		while (curDiscipline < MAX_DISCIPLINE) {
			rate = getExchangeRate (g, player, curDiscipline, STUDENT_MMONEY);
			if (curDiscipline == STUDENT_BPS || curDiscipline == STUDENT_BQN) {
				if (numDiscipline[curDiscipline] > rate) {
					retrainFrom = curDiscipline;
				}
			} else {
				if (numDiscipline[curDiscipline] >= rate) {
					retrainFrom = curDiscipline;
				}
			}
			curDiscipline++;
		}
	} else if (actionCode == BUILD_CAMPUS) {
		while (curDiscipline < MAX_DISCIPLINE) {
			rate = getExchangeRate (g, player, curDiscipline, STUDENT_MMONEY);
			if (curDiscipline == STUDENT_MMONEY) {
				if (numDiscipline[curDiscipline] >= rate) {
					retrainFrom = curDiscipline;
				}
			} else {
				if (numDiscipline[curDiscipline] > rate) {
					retrainFrom = curDiscipline;
				}
			}
			curDiscipline++;
		}
	} else if (actionCode == BUILD_GO8) {
		while (curDiscipline < MAX_DISCIPLINE) {
			rate = getExchangeRate (g, player, curDiscipline, STUDENT_MMONEY);
			if (curDiscipline == STUDENT_MJ) {
				if (numDiscipline[curDiscipline] > rate+2) {
					retrainFrom = curDiscipline;
				}
			} else if (curDiscipline == STUDENT_MMONEY){
				if (numDiscipline[curDiscipline] > rate+3) {
					retrainFrom = curDiscipline;
				}
			} else {
				if (numDiscipline[curDiscipline] >= rate) {
					retrainFrom = curDiscipline;
				}
			}
			curDiscipline++;
		}
	} else if (actionCode == START_SPINOFF) {
		while (curDiscipline < MAX_DISCIPLINE) {
			rate = getExchangeRate (g, player, curDiscipline, STUDENT_MMONEY);
			if (curDiscipline == STUDENT_BPS || curDiscipline == STUDENT_BQN) {
				if (numDiscipline[curDiscipline] >= rate) {
					retrainFrom = curDiscipline;
				}
			} else {
				if (numDiscipline[curDiscipline] > rate) {
					retrainFrom = curDiscipline;
				}
			}
			curDiscipline++;
		}
	}

	return retrainFrom;
}
