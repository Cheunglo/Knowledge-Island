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
action buildOrPass (Game g, int actionCode, path *allPaths);
action retrainOrPass (Game g, int actionCode, int *numDiscipline);
int determineRetrainTo (int actionCode, int *numDiscipline);
int determineRetrainFrom (Game g, int actionCode, int *numDiscipline);

action decideAction (Game g) {

	// all possible paths in the board
	path allPaths[NUM_EDGES] = {
		"L","R","RR","RRL","RRLR","RRLRL","RRLRLL","RRLRLLR","RRLRLLRL","RRLRLLRLR",
		"RRLRLLRLRL","RRLRLLRLRLL","RRLRLLRLRLLR","RRLRLLRLRLLRL","RRLRLLRLRLLRLR",
		"RRLRLLRLRLLRLRL","LR","LRL","LRLR","LRLRL","LRLRLR","LRLRLRR","LRLRLRRL",
		"LRLRLRRLR","LRLRLRRLRL","LRLRLRRLRLR","LRLRLRRLRLRR","LRLRLRRLRLRRL",
		"LRLRLRRLRLRRLR","LRLRLRRLRLRRLRL","RL","RRLL","RRLLRR","RRLLRLRR","RRLLRLRLR",
		"RLRLRLRLR","LRRLRLRLRL","LRLRRLRLRL","LRLRRLRLL","LRLRRLL","LRLRR","LRR",
		"RLL","RLR","RRLLL","RRLLR","RRLLRL","RRLLRLR","RRLLRLRL","RRLLRLRLL",
		"RLRLRLRL","RLRLRLRLL","LRRLRLRLR","LRRLRLRLL","LRLRRLRLR","LRLRRLRL",
		"LRLRRLR","LRLRRL","LRRLL","LRRL","RLRL","RRLLRLL","RLRLRLR","LRRLRLRL",
		"LRRLRL","LRRLR","RLRLL","RLRLR","RLRLRL","RLRLRLL","LRRLRLR","LRRLRL",
	};

	// arbitrary counter
	int i = 0;

	// get player information
	int player = getWhoseTurn (g);

	// get ARCs information
	//int numARC = getARCs (g, player);
	int numVacantARC = 0;
	while (i < NUM_EDGES) {
		if (getARC (g, allPaths[i]) == VACANT_ARC) {
			numVacantARC++;
		}
		i++;
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
		printf ("\n---Attempting to BUILD_GO8---\n");
		if (isActionCostMet ( BUILD_GO8, numDiscipline)) {
			nextAction = buildOrPass (g, BUILD_GO8, allPaths);
		} else {
			nextAction = retrainOrPass (g, BUILD_GO8, numDiscipline);
		}
	} else if (numVacantARC == 0) {
		printf ("\n---Attempting to BUILD_CAMPUS---\n");
		if (isActionCostMet (BUILD_CAMPUS, numDiscipline)) {
			nextAction = buildOrPass (g, BUILD_CAMPUS, allPaths);
		} else {
			nextAction = retrainOrPass (g, BUILD_CAMPUS, numDiscipline);
		}
	} else if (numVacantARC != 0) {
		printf ("\n---Attemping to OBTAIN_ARC---\n");
		if (isActionCostMet (OBTAIN_ARC, numDiscipline)) {
			nextAction = buildOrPass (g, OBTAIN_ARC, allPaths);
		} else {
			nextAction = retrainOrPass (g, OBTAIN_ARC, numDiscipline);
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

action buildOrPass (Game g, int actionCode, path *allPaths) {

	action determineAction;
	determineAction.actionCode = PASS;

	action isValidBuild;
	isValidBuild.actionCode = actionCode;

	int i = 0;
	while (i < NUM_EDGES){
		strcpy (isValidBuild.destination, allPaths[i]);
		if (isLegalAction (g, isValidBuild)){
			determineAction = isValidBuild;
			i = NUM_EDGES;
		} else {
			i++;
		}
	}

	return determineAction;
}

action retrainOrPass (Game g, int actionCode, int *numDiscipline) {

	action determineAction;
	determineAction.actionCode = PASS;

	action isValidRetrain;
	isValidRetrain.actionCode = RETRAIN_STUDENTS;

	isValidRetrain.actionCode = RETRAIN_STUDENTS;
	isValidRetrain.disciplineTo = determineRetrainTo (actionCode, numDiscipline);
	isValidRetrain.disciplineFrom = determineRetrainFrom (g, actionCode, numDiscipline);

	if (isLegalAction (g, isValidRetrain)) {
		determineAction = isValidRetrain;
	}

	return determineAction;
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
