/*
 * Group: 4 Stack
 * Group Members: Stephanie Chua, Louis Cheung
 * Ruby Yu, Alex Trinh
 * Knowledge Island, testGame.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Game.h"

#define LEFT 'L'
#define RIGHT 'R'
#define BACK 'B'

//KPI Value of each move:
#define CAMPUS 10
#define G08 10    //Only 10 points added to previous 10 pts of campus
#define ARC 2
#define SPINOFF 0 //Will have effect only if pub/ip.
#define PUB 0     //Only prestige, no actual points
#define IP 10
#define RETRAIN 0 //No KPI points will be earned
#define MOSTARC 10
#define MOSTPUB 10

#define TV "RR"                //MTV  retraining centre
#define MONEY "LRL"            //MMONEY retraining centre
#define ENGINEER "RLRRLLRLRL"  //BPS retraining centre
#define SCIENCE "RLLRLRLRR"    //BQN retraining centre
#define JOB "RLLRLRRLRLR"       //MJOBS retraining centre

void testMakeAction (void);
void testThrowDice (void);
void testGetDiscipline (void);
void testGetDiceValue (void);
void testGetMostARCS (void);
void testGetMostPublications (void);
void testGetTurnNUmber (void);
void testGetWhoseTurn (void);
void testGetCampus (void);
void testGetARC (void);  //contents of given edge, is ARC?
void testIsLegalAction (void);
void testGetKPIpoints (void);
void testGetARCs (void); //returns no grants player has
void testGetGO8s (void);
void testgetCampuses (void);
void testgetIPs (void);
void testgetPublications (void);
void testgetStudents (void);
void testgetExchangeRate (void);

int main (int argc, char *argv[]) {

	//Tests that have passed:
	testGetARCs (); 
	testGetDiscipline ();
	testGetDiceValue (); 
	testThrowDice ();
	testGetMostARCS ();
	testGetCampus ();
	testGetMostPublications ();
	testGetTurnNUmber ();
	testGetWhoseTurn ();
	testIsLegalAction ();
	testGetGO8s ();
	testGetKPIpoints ();
	
	//Failed:
   	testgetStudents ();
	testMakeAction ();
	testgetExchangeRate ();
	testgetPublications ();
	testGetARC (); 
	testgetIPs (); 
	testgetCampuses ();


   printf ("\n~~**All tests passed!**~~\n");

   return EXIT_SUCCESS;
}

void testMakeAction (void) {

	int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,\
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS };
	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

   	printf ("\n~~testing makeAction()~~\n");
	Game g = newGame (disciplines, dice);
	action a;
	char *path = NULL;
	int preBPS_C = 0;

  	printf ("testing action code 0: PASS...");
   	throwDice (g, 10);
   	a.actionCode = PASS;
   	makeAction (g, a);
   	assert (getWhoseTurn (g) == UNI_B);
	a.actionCode = PASS;
	makeAction (g, a);
	assert (getWhoseTurn (g) == UNI_C);
	a.actionCode = PASS;
	makeAction (g, a);
	assert (getWhoseTurn (g) == UNI_A);
   	printf ("...passed!\n");

	printf ("testing action code 1: BUILD_CAMPUS...");
	a.actionCode = BUILD_CAMPUS;
	path = "RLRL";
	strcpy (a.destination, path);
	makeAction (g, a);
	assert (getCampuses (g, UNI_A) == 3);
	throwDice (g, 10);
	a.actionCode = BUILD_CAMPUS;
	path = "RLRLRL";
	strcpy (a.destination, path);
	makeAction (g, a);
	assert (getCampuses (g, UNI_B) == 3);
	printf ("...passed!\n");

	printf ("testing action code 2: BUILD_GO8...");
	a.actionCode = BUILD_GO8;
	path = "RLRLRL";
	strcpy (a.destination, path);
	makeAction (g, a);
	assert (getGO8s (g, UNI_B) == 1);
	printf ("...passed!\n");

   	printf ("testing action code 3: OBTAIN_ARC...");
   	a.actionCode = OBTAIN_ARC;
   	path = "RL";
   	strcpy (a.destination, path);
   	makeAction (g, a);
   	assert (getARCs (g, UNI_B) == 1);
	throwDice (g, 10);
	a.actionCode = OBTAIN_ARC;
	path = "RLLR";
	strcpy (a.destination, path);
	makeAction (g, a);
	assert (getARCs (g, UNI_C) == 1);
   	printf ("...passed!\n");

   	printf ("testing action code 5: START_PUBLICATION...");
   	a.actionCode = OBTAIN_PUBLICATION;
   	makeAction (g, a);
   	assert (getPublications (g, UNI_C) == 1);
   	printf ("...passed!\n");

   	printf ("testing action code 6: OBTAIN_IP_PATENT...");
   	a.actionCode = OBTAIN_IP_PATENT;
   	makeAction (g, a);
   	assert (getIPs (g, UNI_C) == 1);
   	printf ("...passed!\n");

   	printf ("testing action code 7: RETRAIN_STUDENTS..." );
	preBPS_C = getStudents (g, UNI_C, STUDENT_BPS);
   	a.actionCode = RETRAIN_STUDENTS;
   	a.disciplineFrom = STUDENT_BQN;
   	a.disciplineTo = STUDENT_BPS;
   	makeAction (g, a);
   	assert (getStudents (g, UNI_C, STUDENT_BPS) == preBPS_C + 1);
   	printf ("...passed!");

   	disposeGame (g);

   	printf ("\n~~all tests for makeAction() passed!~~\n");

}

void testThrowDice (void) {

	int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,\
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS };
	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

   	printf ("\n~~testing throwDice()~~\n");
	Game g = newGame (disciplines, dice);
	action a;
	char *path = NULL;
	int preBPS_B, preBQN_B, preMJ_A, preMJ_C, preMTV_A, preMTV_B, preMTV_C,
		preMMONEY_A, preMMONEY_B, preMMONEY_C, preTHD_A, preTHD_B, preTHD_C;

	printf ("testing turn 1 & diceScore 11...");
	preMTV_A = getStudents (g, UNI_A, STUDENT_MTV);
	throwDice (g, 11);
	assert (getWhoseTurn(g) == UNI_A);
	assert (getStudents (g, UNI_A, STUDENT_MTV) == preMTV_A + 1);
	//Builds a GO8 for UNI_A for later testing
	a.actionCode = BUILD_GO8;
	path = "LRRLRL";
	strcpy (a.destination, path);
	makeAction (g, a);
   	printf ("..passed!\n");

   	printf ("testing turn 2 & diceScore 9...");
	preBQN_B = getStudents (g, UNI_B, STUDENT_BQN);
	preMJ_A = getStudents (g, UNI_A, STUDENT_MJ);
	throwDice (g, 9);
	assert (getWhoseTurn(g) == UNI_B);
	assert (getStudents (g, UNI_B, STUDENT_BQN) == preBQN_B + 1);
	assert (getStudents (g, UNI_A, STUDENT_MJ) == preMJ_A + 2);
	//Builds a GO8 for UNI_A for later testing
	a.actionCode = BUILD_GO8;
	path = "RLRLR";
	strcpy (a.destination, path);
	makeAction (g, a);
   	printf ("...passed!\n");

   	printf ("testing turn 3 & diceScore 8...");
	preMJ_C = getStudents (g, UNI_C, STUDENT_MJ);
	preMTV_C = getStudents (g, UNI_C, STUDENT_MTV);
	throwDice (g, 8);
	assert (getWhoseTurn(g) == UNI_C);
	assert (getStudents (g, UNI_C, STUDENT_MJ) == preMJ_C + 1);
	assert (getStudents (g, UNI_C, STUDENT_MTV) == preMTV_C + 1);
   	printf ("...passed!\n");

	printf ("testing turn 4 & diceScore 5...");
	preBPS_B = getStudents (g, UNI_B, STUDENT_BPS);
	throwDice (g, 5);
	assert (getWhoseTurn(g) == UNI_A);
	assert (getStudents (g, UNI_B, STUDENT_BPS) == preBPS_B + 3);
	printf ("...passed!\n");

	printf ("testing turn 5 & diceScore 7...");
	preTHD_A = getStudents (g, UNI_A, STUDENT_THD);
	preTHD_B = getStudents (g, UNI_B, STUDENT_THD);
	preTHD_C = getStudents (g, UNI_C, STUDENT_THD);
	preMTV_A = getStudents (g, UNI_A, STUDENT_MTV);
	preMTV_B = getStudents (g, UNI_B, STUDENT_MTV);
	preMTV_C = getStudents (g, UNI_C, STUDENT_MTV);
	preMMONEY_A = getStudents (g, UNI_A, STUDENT_MMONEY);
	preMMONEY_B = getStudents (g, UNI_B, STUDENT_MMONEY);
	preMMONEY_C = getStudents (g, UNI_C, STUDENT_MMONEY);
	throwDice (g, 7);
	assert (getWhoseTurn(g) == UNI_B);
	assert (getStudents (g, UNI_A, STUDENT_MTV) == 0);
	assert (getStudents (g, UNI_B, STUDENT_MTV) == 0);
	assert (getStudents (g, UNI_C, STUDENT_MTV) == 0);
	assert (getStudents (g, UNI_A, STUDENT_MMONEY) == 0);
	assert (getStudents (g, UNI_B, STUDENT_MMONEY) == 0);
	assert (getStudents (g, UNI_C, STUDENT_MMONEY) == 0);
	assert (getStudents (g, UNI_A, STUDENT_THD) == preTHD_A + preMTV_A + preMMONEY_A + 2);
	assert (getStudents (g, UNI_B, STUDENT_THD) == preTHD_B + preMTV_B + preMMONEY_B);
	assert (getStudents (g, UNI_C, STUDENT_THD) == preTHD_C + preMTV_C + preMMONEY_C);
	printf ("...passed!\n");

	disposeGame (g);
   	printf ("~~all tests for throwDice() passed!~~\n");

}

void testGetDiscipline (void) {

   	printf ("\n~~testing getDiscipline()~~\n");

	int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,\
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS };
	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

   	Game g = newGame (disciplines, dice);
   	int i = 0;
   	printf ("testing all regions with a loop...");
   	while (i < NUM_REGIONS) {
      	assert (getDiscipline(g, i) == disciplines[i]);
		i++;
   	}
   	printf ("...passed!\n");
   	disposeGame (g);

	printf ("~~all tests for getDiscipline() passed!~~\n");

}

void testGetDiceValue (void) {

	printf ("\n~~testing getDiceValue()~~\n");

	int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ,\
			STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
			STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,\
			STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,\
			STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS };
	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

   	Game g = newGame (disciplines, dice);
   	int i = 0;
   	printf ("testing all regions with a loop...");
   	while (i < NUM_REGIONS) {
		assert (getDiceValue(g, i) == dice[i]);
		i++;
   	}
   	printf ("...passed!\n");
   	disposeGame (g);

   	printf ("~~getDiceValue has passed!~~\n");

}

void testGetMostARCS (void) {

	printf("testing getMostArcs()..\n");

	int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,\
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS };
	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

	Game g = newGame (disciplines, dice);

	printf("testing getMostArcs() returning NO_ONE if there are no arcs\n");
	assert(getMostARCs(g) == NO_ONE);

	printf("testing case 1: A has 1 arc, B and C have none\n");
	//give UNI A an arc
	throwDice(g,2);
	char * newpath = "LRR";
	action a;
	a.actionCode = OBTAIN_ARC;
	strcpy (a.destination, newpath);
	makeAction(g,a);

	//make sure now getmostarcs return UNI_A
	assert(getMostARCs(g) == UNI_A);

	printf("testing case 2: A has 1 arc, B has 2, C has 0\n");
	//give UNI B 2 arcs
	throwDice(g,5);

	newpath = "L";
	strcpy (a.destination, newpath);
	makeAction(g,a);

	newpath = "R";
	strcpy (a.destination, newpath);
	makeAction (g,a);

	//test new result
	assert(getMostARCs(g) == UNI_B);

	printf("All tests for GetMostARCS passed!\n");

}

// which university currently has the prestige award for the most pubs?
// this is NO_ONE until the first publication is made.
void testGetMostPublications(void){

	printf("testing getMostPublications...\n");

	int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,\
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS };
	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

	Game g = newGame (disciplines, dice);

	printf("testing if no one has publications\n");
	assert( getMostPublications(g) == NO_ONE);

	printf("testing case 1: C has 1, A and B have none\n");

	int counter = 0;
	while(counter < 3){
		throwDice(g,4);
		counter ++;
	}

	char *newpath = "L";
	action a;
	a.actionCode = OBTAIN_PUBLICATION;
	strcpy (a.destination, newpath);
   	makeAction(g,a);

	assert(getMostPublications(g)==UNI_C);

	printf("testing case 1: C has 1, B has 2, A has 0\n");

	throwDice(g,4);
	throwDice(g,4);

	newpath = "R";
	strcpy (a.destination, newpath);
	makeAction(g,a);

	newpath = "RLRL";
	strcpy (a.destination, newpath);
	makeAction(g,a);

	assert(getMostPublications(g) == UNI_B);

	printf("All tests for getMostPublications passed!\n");

}

// return the current turn number of the game -1,0,1, ..
//int getTurnNumber (Game g);
void testGetTurnNUmber(void){

	int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,\
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS };
	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

	Game g = newGame (disciplines, dice);

	printf("testing getTurnNumber...\n");

	printf("testing case 1 for terra nullis\n");
	assert(getTurnNumber(g) == -1);

	//throw dice to start game
	throwDice(g,4);

	printf("testing case 2: dice thrown once\n");
	assert(getTurnNumber(g) == 0);

	int counter = 0;
	while (counter < 10){
		throwDice(g,4);
		counter ++;
	}

	printf("testing case 2: dice thrown 11 times\n");
	assert(getTurnNumber(g)== 10);

	printf("All tests for getTurnNumber passed!\n");

}

// return the player id of the player whose turn it is
// the result of this function is NO_ONE during Terra Nullis
//t getWhoseTurn (Game g);
void testGetWhoseTurn (void){

	int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,\
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS };
	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

	Game g = newGame (disciplines, dice);

	printf("testing getWhoseTurn...\n");

	printf("testing case 1: terra nullis\n");
	assert(getWhoseTurn(g)==UNI_C);

	printf("testing case 2: first turn (A)\n");

	throwDice(g,4);
	assert(getWhoseTurn(g)==UNI_A);

	printf("testing case 3: 2nd turn (B)\n");
	throwDice(g,4);
	assert(getWhoseTurn(g)==UNI_B);

	printf("testing case 4: 8th turn (C)\n");
	int counter = 0;
	while(counter < 7){
		throwDice(g,4);
		counter ++;

	}

	assert(getWhoseTurn(g)==UNI_C);

	printf("All tests for getWhoseTurn passed!\n");

}

// return the contents of the given vertex (ie campus code or
// VACANT_VERTEX)
//int getCampus(Game g, path pathToVertex);

void testGetCampus(void){

	int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,\
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS };
	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

	Game g = newGame (disciplines, dice);
	char *path = "LRLR";

	throwDice(g,5); //start game
	action a;
	a.actionCode = BUILD_CAMPUS;
	strcpy (a.destination, path);

	makeAction(g,a); //give UNI A a campus at LRLR;

	printf("testing getCampus...\n");

	printf("testing case 1: no campus on vertex\n");
	assert(getCampus(g,"L") == VACANT_VERTEX);

	printf("testing case 2: campus on vertex\n");
	assert(getCampus(g,"LRLR") == UNI_A);

	printf("All tests for getCampus passed!\n");

}

void testGetARC (void) {
	//test if function test if edge has ARC

	printf ("Testing getARC()...\n");

	int player = 0;

	int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,\
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS };
	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

	Game g = newGame (disciplines, dice);
	action a;
	a.actionCode = 0;
	strcpy (a.destination, "L");
	a.disciplineFrom = 0;
	a.disciplineTo = 0;
	printf ("   Initializing tests complete.\n   Testing vacant ARCs...\n");
	//Tests that it's vacant at the beginning
	strcpy (a.destination, "L");
	assert (getARC (g, a.destination) == VACANT_ARC);
	strcpy (a.destination, "LRLR");
	assert (getARC (g, a.destination) == VACANT_ARC);
	printf ("   Vacant ARCs testing complete.\n");

	printf ("   Testing ARC building...\n");
	//Tests that it will return an Arc Code
	player = getWhoseTurn(g);
	a.actionCode = OBTAIN_ARC;
	makeAction (g, a);
	assert (getARC (g, a.destination) != VACANT_ARC);
	printf ("   ARC building complete.\n");

	disposeGame(g);
	printf ("... All tests for getARC() passed.\n");
}

void testIsLegalAction (void) {
	//Tests if func tests if action and retraining is legal, TRUE/FALSE

	printf ("Testing isLegalAction()...\n");

	int rate = 0;
	int counter = 0;
	int noCampus = 0;
	int noGO8s = 0;
	int noStudents = 0;
	int stuTypes[6];  //Stores number of students of each type

	int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,\
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS };
	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

	Game g = newGame (disciplines, dice);
	int plyr = getWhoseTurn (g);
	action a;
	a.actionCode = 0;
	char *path = "LR";
	strcpy (a.destination, path);
	a.disciplineFrom = 0;
	a.disciplineTo = 0;
	printf ("   New game initialized for testing.\n");

	//Ensure that the game has already started
	assert (isLegalAction (g, a) == FALSE);
	throwDice (g, 3);
	assert (isLegalAction (g, a) == TRUE);
	printf ("   Game has indeed already started.\n");

	//Ensure action codes are legal
	a.actionCode = -1;
	assert (isLegalAction (g, a) == FALSE);
	a.actionCode = -124;
	assert (isLegalAction (g, a) == FALSE);
	a.actionCode = 'f';
	assert (isLegalAction (g, a) == FALSE);
	a.actionCode = 34;
	assert (isLegalAction (g, a) == FALSE);
	a.actionCode = 8;
	assert (isLegalAction (g, a) == FALSE);
	a.actionCode = '?';
	assert (isLegalAction (g, a) == FALSE);
	printf ("   Action codes are legal.\n");

	noCampus = getCampuses (g, plyr);

	//Rolls dice so random number of students are generated
	while (counter < 13) {
		throwDice (g, counter);
		throwDice (g, counter);
		counter ++;
	}

	counter = 0; //to be reused in next loop
	while (counter < 6) {
		stuTypes[counter] = getStudents (g, plyr, counter);
		counter ++;
	}

	a.actionCode = PASS; //Always true when game has began
	assert (isLegalAction (g, a) == TRUE);
	printf ("   PASS action is definitely legal.\n");


	//Ensure paths are well formed & legal (legal direction & length)
	//Does not leave into the sea
	//Does it lead to vacant vertexr
	//Are they allowed to place a campus on vertex
	a.actionCode = OBTAIN_ARC;
	counter = 0; //to be reused in loop
	if ((stuTypes[1] >= 0) && (stuTypes[2] > 0)){
		if (a.destination[0] == BACK) {
			assert (isLegalAction (g, a) == FALSE);
		} else if (sizeof (a.destination) > PATH_LIMIT) {
			assert (isLegalAction (g, a) == FALSE);
		} else if (getARC (g, a.destination) == VACANT_ARC) {
			assert (isLegalAction (g, a) == TRUE);
		} else {
			assert (isLegalAction (g, a) == FALSE);
		}
	} else {
		assert (isLegalAction (g, a) == FALSE);
	}
	printf ("   ARC obtaining function is legally working.\n");
	makeAction (g, a); //Make the arc so that we can build campus next

	a.actionCode = BUILD_CAMPUS; //Needs 1 of each but THD/M$
	if ((stuTypes[1] > 0) && (stuTypes[2] > 0) && (stuTypes[3] > 0) && (stuTypes[4] > 0)){
		if (getCampus (g, a.destination) == VACANT_VERTEX) {
			assert (isLegalAction (g, a) == TRUE);
		}
	} else {
		assert (isLegalAction (g, a) == FALSE);
	}
	printf ("   Build Campus legal function is working.\n");

	a.actionCode = BUILD_GO8; //Needs a campus, 2MJs, 3M$ + one campus
	noGO8s = getGO8s (g, plyr);
	if ((stuTypes[3] >= 1) && (stuTypes[5] > 2) && (noCampus > 0) && (noGO8s < 8)){
		assert (isLegalAction (g, a) == TRUE);
	} else {
		assert (isLegalAction (g, a) == FALSE);
	}
	printf ("   Build GO8 legal function is working.\n");

	a.actionCode = START_SPINOFF;
	if ((stuTypes[3] >= 0) && (stuTypes[4] > 0) && (stuTypes[5] > 0)){
		assert (isLegalAction (g, a) == TRUE);
	} else {
		assert (isLegalAction (g, a) == FALSE);
	}
	printf ("    Starting spinoff function is legally working.\n");

	//Illegal for players to OBTAIN_PUBLICATION and OBTAIN_IP_PATENT
	a.actionCode = OBTAIN_PUBLICATION;
	assert (isLegalAction (g, a) == FALSE);
	a.actionCode = OBTAIN_IP_PATENT;
	assert (isLegalAction (g, a) == FALSE);
	printf ("    Publications/IP function is legally working.\n");

	//Testing for retraining action
	//Ensure valid discipline numbers used
	a.actionCode = RETRAIN_STUDENTS;
	a.disciplineTo = -124;
	assert (isLegalAction (g, a) == FALSE);
	a.disciplineTo = -1;
	assert (isLegalAction (g, a) == FALSE);
	a.disciplineTo = 'f';
	assert (isLegalAction (g, a) == FALSE);
	a.disciplineTo = 34;
	assert (isLegalAction (g, a) == FALSE);
	a.disciplineTo = 6;
	assert (isLegalAction (g, a) == FALSE);
	a.disciplineTo = '?';
	assert (isLegalAction (g, a) == FALSE);

	a.disciplineFrom = -124;
	assert (isLegalAction (g, a) == FALSE);
	a.disciplineFrom = 'f';
	assert (isLegalAction (g, a) == FALSE);
	a.disciplineFrom = 34;
	assert (isLegalAction (g, a) == FALSE);
	a.disciplineFrom = '?';
	assert (isLegalAction (g, a) == FALSE);
	printf ("    Basic discipline testing is all legal.\n");

	//Testing to ensure uni has enough students for retraining
	//Testing THDs that cannot be retrained
	a.disciplineTo = STUDENT_THD;
	a.disciplineFrom = STUDENT_THD;
	assert (isLegalAction (g, a) == FALSE);
	printf ("    All student retraining codes function is legal.\n");

	a.disciplineTo = 1;   //To avoid THDs (0) that cannot be retrained
	a.disciplineFrom = 1;

	while (a.disciplineFrom < 6) {
		while (a.disciplineTo < 6) {
			rate = getExchangeRate (g, plyr, a.disciplineFrom, a.disciplineTo);
			noStudents = getStudents (g, plyr, a.disciplineFrom);
			if (rate == noStudents) {
				assert (isLegalAction (g, a) == TRUE);
			} else if (rate != noStudents) {
				assert (isLegalAction (g, a) == FALSE);
			}
			a.disciplineTo ++;
		}
		a.disciplineFrom ++;
	}
	printf ("    Retraining function is legal.\n");

	printf ("Checks completed, game being disposed...\n");
	disposeGame (g); //Restarts the game for other testing.

	printf ("...isLegalAction() passed all tests.\n");
}

void testGetKPIpoints (void) {
	//Test if function returns KPI points of specified player

	printf ("Testing getKPIpoints()...\n");

	int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,\
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS };
	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

	Game g = newGame (disciplines, dice);
	action a;
	int player = UNI_A;
	int kpiPts[4] = {0};
	int kpiValues[10] = {0, CAMPUS, G08, ARC, 0, 0, IP, 0, MOSTARC, MOSTPUB};

	//Tests all new games start with 0, inlcuding NO_ONE
	printf ("Testing all initial conditions, KPI = 0.\n");
	while (player < 4) {
		assert (getKPIpoints (g, player) == 2*CAMPUS);
		kpiPts[player] += 2*CAMPUS;
		player ++;
	}
	printf ("All initial conditions are set, good.\n");


	//Tests that players have correct KPI values for every move.
	printf ("   Testing PASS points...\n");
	player = getWhoseTurn (g);
	printf ("      Player ID: %d\n", player);
	a.actionCode = PASS;
	makeAction (g, a);
	throwDice (g, 12);
	kpiPts[player] = kpiPts[player] + kpiValues[a.actionCode];
	assert (getKPIpoints (g, player) == kpiPts[player]);
	printf ("     PASS points passed test.\n");

	printf ("   Testing for ARC building + ARC Prestige Points...\n");
	player = getWhoseTurn (g);
	a.actionCode = OBTAIN_ARC;
	makeAction (g, a);
	throwDice (g, 12);
	kpiPts[player] = kpiPts[player] + kpiValues[a.actionCode];
	player = getMostARCs (g);
	kpiPts[player] += MOSTARC;
	assert (getKPIpoints (g, player) == kpiPts[player]);
	printf ("     ARC points score passed test.\n");

	//Tests that players have correct KPI values for making campuses.
	printf ("   Testing for Campuses building...\n");
	player = getWhoseTurn (g);
	printf ("      Player ID: %d\n", player);
	a.actionCode = BUILD_CAMPUS;
	makeAction (g, a);
	kpiPts[player] = kpiPts[player] + kpiValues[a.actionCode];
	assert (getKPIpoints (g, player) == kpiPts[player]);
	printf ("     Simple Campus points passed test.\n");
	a.actionCode = BUILD_GO8;
	makeAction (g, a);
	throwDice (g, 12);
	kpiPts[player] = kpiPts[player] + kpiValues[a.actionCode];
	assert (getKPIpoints (g, player) == kpiPts[player]);
	printf ("     GO8 points passed test.\n");
	printf ("   Campuses building points successful...\n");

	printf ("   Testing for Spinoffs building...\n");
	player = getWhoseTurn (g);
	a.actionCode = START_SPINOFF;
	makeAction (g, a);
	kpiPts[player] = kpiPts[player] + kpiValues[a.actionCode];
	assert (getKPIpoints (g, player) == kpiPts[player]);
	printf ("     Spinoff points passed test.\n");

	printf ("   Testing for obtaining IP Patents score...\n");
	a.actionCode = OBTAIN_IP_PATENT;
	makeAction (g, a);
	throwDice (g, 12);
	kpiPts[player] = kpiPts[player] + kpiValues[a.actionCode];
	assert (getKPIpoints (g, player) == kpiPts[player]);
	printf ("     IP points score passed test.\n");

	printf ("   Testing for obtaining Publications + Publication Prestige Points...\n");
	a.actionCode = OBTAIN_PUBLICATION;
	makeAction (g, a);
	throwDice (g, 12);
	kpiPts[player] = kpiPts[player] + kpiValues[a.actionCode];
	player = getMostPublications (g);
	kpiPts[player] += MOSTPUB;
	assert (getKPIpoints (g, player) == kpiPts[player]);
	printf ("     Publications points passed test.\n");

	printf ("   Testing for Retraining...\n");
	player = getWhoseTurn (g);
	a.actionCode = RETRAIN_STUDENTS;
	makeAction (g, a);
	throwDice (g, 12);
	kpiPts[player] = kpiPts[player] + kpiValues[a.actionCode];
	assert (getKPIpoints (g, player) == kpiPts[player]);
	printf ("     Retraining points passed test.\n");

	disposeGame(g);
	printf ("...All tests for getKPIpoints() passed.\n");
}

void testGetARCs (void) {
	//test if it returns number of ARC grants of specified player
	printf ("Testing getARCs()...\n");

	int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,\
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS };
	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

	Game g = newGame (disciplines, dice);
	action a;
	int player = UNI_A;
	int numArcs[4] = {0};

	//Tests all new games start with 0, inlcuding NO_ONE
	while (player < 4) {
		assert (getARCs (g, player) == 0);
		player ++;
	}

	//Tests that players will get ARC after making move
	player = getWhoseTurn (g);
	a.actionCode = OBTAIN_ARC;
	makeAction (g, a);
	numArcs[player] ++;
	assert (getARCs (g, player) == numArcs [player]);

	disposeGame(g);
	printf ("...All tests for getARCs() passed.\n");
}

void testGetGO8s (void) {
	//test if it returns number of G08 campuses of specified player
	printf ("Testing getG08s()...\n");

	int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,\
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS };
	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

	Game g = newGame (disciplines, dice);
	action a;
	int player = 0;
	int numG08[4] = {0};

	//Tests all new games start with 0, inlcuding NO_ONE
	while (player < 4) {
		assert (getGO8s (g, player) == 0);
		player ++;
	}

	//Tests that players will get G08 after making move
	player = getWhoseTurn (g);
	a.actionCode = BUILD_GO8;
	makeAction (g, a);
	numG08 [player] ++;
	assert (getGO8s (g, player) == numG08 [player]);

	disposeGame(g);
	printf ("...All tests for getG08s() passed.\n");
}

void testGetCampuses (void) {
	//test if it returns number of G08 campuses of specified player
	printf ("\nTesting getCampuses()...\n");

	int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,\
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS };
	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

	Game g = newGame (disciplines, dice);
	action a;
	int player = UNI_A;
	int numCmps[4] = {0, 2, 2, 2};

	//Tests all new games start with 0, inlcuding NO_ONE
	while (player < 4) {
		assert (getCampuses (g, player) == 2);
		player ++;
	}

	//Tests that players will get G08 after making move
	player = getWhoseTurn (g);
	a.actionCode = BUILD_CAMPUS;
	makeAction (g, a);
	numCmps[player] ++;
	assert (getCampuses (g, player) == numCmps[player]);

	disposeGame(g);
	printf ("...All tests for getCampuses() passed.\n\n");
}

void testgetIPs (void) {
   printf ("Testing getIPs\n");
   printf ("It returns no. IPs a player has.\n");
   printf ("First set of tests:\n");
   printf ("Each player getting an IP every turn");

   int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,\
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS };
	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

   Game g = newGame (disciplines, dice);
   //action {actionCode, destination, disciplineFrom, disciplineTo}
   //only the actionCode is needed for this test
   //the other components are just random
   action a = {OBTAIN_IP_PATENT, "L", 3, 1};
   int index = 0;
   assert (getIPs (g, UNI_A) == 0);
   assert (getIPs (g, UNI_B) == 0);
   assert (getIPs (g, UNI_C) == 0);
   while (index < 10) {
         throwDice (g, 7);
         makeAction (g, a);
         assert (getIPs (g, UNI_A) == index);
         throwDice (g, 7);
         makeAction (g, a);
         assert (getIPs (g, UNI_B) == index);
         throwDice (g, 7);
         makeAction (g, a);
         assert (getIPs (g, UNI_C) == index);
      index ++;
   }
   printf ("Set 1 of tests passed\n Starting second set of tests\n");
   printf ("Getting IPs.\n");
   printf ("PlayerA every turn, playerB every second and \
            playerC every third.\n");
   g = newGame (disciplines, dice);
   a.actionCode = PASS;

   action b = {OBTAIN_IP_PATENT, "L", 3, 1};
   int indexA = 0;
   int indexB = 0;
   int indexC = 0;

      while (indexC < 10) {
         throwDice (g, 7);
         makeAction (g, b);
         indexA ++;
         assert (getIPs (g, UNI_A) == indexA);
         throwDice (g, 7);
         if ((indexB % 2) == 0) {
            makeAction (g, b);
            indexB ++;
        } else {
            makeAction (g, a);
        }
         assert (getIPs (g, UNI_B) == indexB);
         throwDice (g, 7);
         if ((indexC % 3) == 0) {
            makeAction (g, b);
            indexC ++;
         } else {
            makeAction (g, a);
         }
         assert (getIPs (g, UNI_C) == indexC);
        }
   printf ("all tests passed\n");
}

void testgetPublications (void) {
   printf ("\nTesting getPublications\n");
   printf ("It returns no. Publications a player has.\n");
   printf ("First set of tests:\n");

   int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,\
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS };
	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

   Game g = newGame (disciplines, dice);
   //action {actionCode, destination, disciplineFrom, disciplineTo}
   //only the actionCode is needed for this test
   //the other components are just random
   action a;
   a.actionCode = OBTAIN_PUBLICATION;
   int player = UNI_A;

   printf ("   Testing initial conditions...\n");
   while (player < 4) {
   		assert (getPublications (g, player) == 0);
   		player ++;
   }
   printf ("   Initial publication settings correct.\n");

   printf ("   Testing obtaining publications...\n");
   throwDice (g, 5);
   player = getWhoseTurn (g);
   makeAction (g, a);
   assert (getPublications (g, player) == 1);

   throwDice (g, 5);
   player = getWhoseTurn (g);
   makeAction (g, a);
   assert (getPublications (g, player) == 1);

   throwDice (g, 5);
   player = getWhoseTurn (g);
   makeAction (g, a);
   assert (getPublications (g, player) == 1);

   printf ("All tests for getPublications() successful.\n\n");
}

void testgetStudents (void) {
   printf ("Testing getStudents\n");
   printf ("It returns no. Students of a specific class a player has.\n");
   printf ("First set of tests:\n");
   printf ("Each player getting and Student every turn");
   
   int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,\
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS };
	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

   Game g = newGame (disciplines, dice);
   int index = 0;
   int indexA1 = 0;
   int indexA2 = 0;
   int indexB1 = 0;
   int indexB2 = 0;
   int indexC1 = 0;
   int indexC2 = 0;
   
   assert (getStudents (g, UNI_A, STUDENT_THD) == 0);
   assert (getStudents (g, UNI_A, STUDENT_BPS) == 3);
   assert (getStudents (g, UNI_A, STUDENT_BQN) == 3);
   assert (getStudents (g, UNI_A, STUDENT_MJ) == 1);
   assert (getStudents (g, UNI_A, STUDENT_MTV) == 1);
   assert (getStudents (g, UNI_A, STUDENT_MMONEY) == 1);
   
   assert (getStudents (g, UNI_B, STUDENT_THD) == 0);
   assert (getStudents (g, UNI_B, STUDENT_BPS) == 3);
   assert (getStudents (g, UNI_B, STUDENT_BQN) == 3);
   assert (getStudents (g, UNI_B, STUDENT_MJ) == 1);
   assert (getStudents (g, UNI_B, STUDENT_MTV) == 1);
   assert (getStudents (g, UNI_B, STUDENT_MMONEY) == 1);           
   
   assert (getStudents (g, UNI_C, STUDENT_THD) == 0);
   assert (getStudents (g, UNI_C, STUDENT_BPS) == 3);
   assert (getStudents (g, UNI_C, STUDENT_BQN) == 3);
   assert (getStudents (g, UNI_C, STUDENT_MJ) == 1);
   assert (getStudents (g, UNI_C, STUDENT_MTV) == 1);
   assert (getStudents (g, UNI_C, STUDENT_MMONEY) == 1);   
   
   while (index < 12) {
      if ((index % 2) == 0) {
         throwDice (g, 11);
         indexA1 ++;
      } else {
         throwDice (g, 6);
         indexA2 ++;
      }
   
      assert (getStudents (g, UNI_A, STUDENT_THD) == 0);
      assert (getStudents (g, UNI_A, STUDENT_BPS) == 3);
      assert (getStudents (g, UNI_A, STUDENT_BQN) == 3);
      assert (getStudents (g, UNI_A, STUDENT_MJ) == indexA2 + 1);
      assert (getStudents (g, UNI_A, STUDENT_MTV) == indexA1 + 1);
      assert (getStudents (g, UNI_A, STUDENT_MMONEY) == 1);
   
      if ((index % 2) == 0) {
         throwDice (g, 9);
         indexB1 ++;
      } else {
         throwDice (g, 5);
         indexB2 ++;
      }
   
      assert (getStudents (g, UNI_B, STUDENT_THD) == 0);
      assert (getStudents (g, UNI_B, STUDENT_BPS) == indexB2 + 3);
      assert (getStudents (g, UNI_B, STUDENT_BQN) == indexB1 + 3);
      assert (getStudents (g, UNI_B, STUDENT_MJ) == 1);
      assert (getStudents (g, UNI_B, STUDENT_MTV) == 1);
      assert (getStudents (g, UNI_B, STUDENT_MMONEY) == 1);
   
      if ((index % 2) == 0) {
         throwDice (g, 8);
         indexC1 ++;
      } else {
         throwDice (g, 8);
         indexC2 ++;
      }
   
      assert (getStudents (g, UNI_C, STUDENT_THD) == 0);
      assert (getStudents (g, UNI_C, STUDENT_BPS) == 3);
      assert (getStudents (g, UNI_C, STUDENT_BQN) == 3);
      assert (getStudents (g, UNI_C, STUDENT_MJ) == indexC1 + 1);
      assert (getStudents (g, UNI_C, STUDENT_MTV) == indexC2 + 1);
      assert (getStudents (g, UNI_C, STUDENT_MMONEY) == 1);

      index ++;         
   }
   printf ("Set 1 of tests passed\n Starting second set of tests\n");
   printf ("Retraining Students.\n");
   
   action a1, a2, b1, b2, c1, c2;
   
   a1.actionCode  = RETRAIN_STUDENTS;
   a1.disciplineFrom = STUDENT_MTV;
   a1.disciplineTo = STUDENT_THD;
   
   a2.actionCode = RETRAIN_STUDENTS;
   a2.disciplineFrom = STUDENT_MJ;
   a2.disciplineTo =  STUDENT_MMONEY;

   b1.actionCode = RETRAIN_STUDENTS;
   b1.disciplineFrom = STUDENT_BQN;
   b1.disciplineTo =  STUDENT_MJ;

   b2.actionCode = RETRAIN_STUDENTS;
   b2.disciplineFrom = STUDENT_BPS;
   b2.disciplineTo =  STUDENT_MTV;

   c1.actionCode = RETRAIN_STUDENTS;
   c1.disciplineFrom = STUDENT_MJ;
   c1.disciplineTo =  STUDENT_BPS;
   
   c2.actionCode = RETRAIN_STUDENTS;
   c2.disciplineFrom = STUDENT_BPS;
   c2.disciplineTo =  STUDENT_MTV;

   index = 0;
   
   while (index < 4) {
   
      assert (getStudents (g, UNI_A, STUDENT_THD) == index);
      assert (getStudents (g, UNI_A, STUDENT_BPS) == 3);
      assert (getStudents (g, UNI_A, STUDENT_BQN) == 3);
      assert (getStudents (g, UNI_A, STUDENT_MJ) == indexA2 + 1);
      assert (getStudents (g, UNI_A, STUDENT_MTV) == indexA1 + 1);
      assert (getStudents (g, UNI_A, STUDENT_MMONEY) == index + 1);
   
      assert (getStudents (g, UNI_B, STUDENT_THD) == 0);
      assert (getStudents (g, UNI_B, STUDENT_BPS) == indexB2 + 3);
      assert (getStudents (g, UNI_B, STUDENT_BQN) == indexB1 + 3);
      assert (getStudents (g, UNI_B, STUDENT_MJ) == index + 1);
      assert (getStudents (g, UNI_B, STUDENT_MTV) == index + 1);
      assert (getStudents (g, UNI_B, STUDENT_MMONEY) == 1);
   
      assert (getStudents (g, UNI_C, STUDENT_THD) == 0);
      assert (getStudents (g, UNI_C, STUDENT_BPS) == index + 3);
      assert (getStudents (g, UNI_C, STUDENT_BQN) == index + 3);
      assert (getStudents (g, UNI_C, STUDENT_MJ) == indexC1 + 1);
      assert (getStudents (g, UNI_C, STUDENT_MTV) == indexC2 + 1);
      assert (getStudents (g, UNI_C, STUDENT_MMONEY) == 1);
   
      throwDice (g, 4);
      makeAction (g, a1);
      makeAction (g, a2);
      indexA1 -= 3;
      indexA2 -= 3;
      index ++;
   
      assert (getStudents (g, UNI_A, STUDENT_THD) == index);
      assert (getStudents (g, UNI_A, STUDENT_BPS) == 3);
      assert (getStudents (g, UNI_A, STUDENT_BQN) == 3);
      assert (getStudents (g, UNI_A, STUDENT_MJ) == indexA2 + 1);
      assert (getStudents (g, UNI_A, STUDENT_MTV) == indexA1 + 1);
      assert (getStudents (g, UNI_A, STUDENT_MMONEY) == index + 1);
   
      throwDice (g, 4);
      makeAction (g, b1);
      makeAction (g, b2);
      indexB1 -= 3;
      indexB2 -= 3;
   
      assert (getStudents (g, UNI_B, STUDENT_THD) == 0);
      assert (getStudents (g, UNI_B, STUDENT_BPS) == indexB2 + 3);
      assert (getStudents (g, UNI_B, STUDENT_BQN) == indexB1 + 3);
      assert (getStudents (g, UNI_B, STUDENT_MJ) == index + 1);
      assert (getStudents (g, UNI_B, STUDENT_MTV) == index + 1);
      assert (getStudents (g, UNI_B, STUDENT_MMONEY) == 1);
   
      throwDice (g, 4);
      makeAction (g, c1);
      makeAction (g, c2);
      indexB1 -= 3;
      indexB2 -= 3;
       
      assert (getStudents (g, UNI_C, STUDENT_THD) == 0);
      assert (getStudents (g, UNI_C, STUDENT_BPS) == index + 3);
      assert (getStudents (g, UNI_C, STUDENT_BQN) == index + 3);
      assert (getStudents (g, UNI_C, STUDENT_MJ) == indexC1 + 1);
      assert (getStudents (g, UNI_C, STUDENT_MTV) == indexC2 + 1);
      assert (getStudents (g, UNI_C, STUDENT_MMONEY) == 1);

      index ++;         
   }
   printf ("Testing that when a 7 is rolled, MMONEY and MTV \
            change into THD\n");
   assert (getStudents (g, UNI_A, STUDENT_THD) == index);
   assert (getStudents (g, UNI_A, STUDENT_BPS) == 3);
   assert (getStudents (g, UNI_A, STUDENT_BQN) == 3);
   assert (getStudents (g, UNI_A, STUDENT_MJ) == indexA2 + 1);
   assert (getStudents (g, UNI_A, STUDENT_MTV) == indexA1 + 1);
   assert (getStudents (g, UNI_A, STUDENT_MMONEY) == index + 1);

   assert (getStudents (g, UNI_B, STUDENT_THD) == 0);
   assert (getStudents (g, UNI_B, STUDENT_BPS) == indexB2 + 3);
   assert (getStudents (g, UNI_B, STUDENT_BQN) == indexB1 + 3);
   assert (getStudents (g, UNI_B, STUDENT_MJ) == index + 1);
   assert (getStudents (g, UNI_B, STUDENT_MTV) == index + 1);
   assert (getStudents (g, UNI_B, STUDENT_MMONEY) == 1);   

   assert (getStudents (g, UNI_C, STUDENT_THD) == 0);
   assert (getStudents (g, UNI_C, STUDENT_BPS) == index + 3);
   assert (getStudents (g, UNI_C, STUDENT_BQN) == index + 3);
   assert (getStudents (g, UNI_C, STUDENT_MJ) == indexC1 + 1);
   assert (getStudents (g, UNI_C, STUDENT_MTV) == indexC2 + 1);
   assert (getStudents (g, UNI_C, STUDENT_MMONEY) == 1);
   
   throwDice (g, 7);
   
   assert (getStudents (g, UNI_A, STUDENT_THD) == index + index + 2);
   assert (getStudents (g, UNI_A, STUDENT_BPS) == 3);
   assert (getStudents (g, UNI_A, STUDENT_BQN) == 3);
   assert (getStudents (g, UNI_A, STUDENT_MJ) == indexA2 + 1);
   assert (getStudents (g, UNI_A, STUDENT_MTV) == indexA1);
   assert (getStudents (g, UNI_A, STUDENT_MMONEY) == 0);

   assert (getStudents (g, UNI_B, STUDENT_THD) == index + 2);
   assert (getStudents (g, UNI_B, STUDENT_BPS) == indexB2 + 3);
   assert (getStudents (g, UNI_B, STUDENT_BQN) == indexB1 + 3);
   assert (getStudents (g, UNI_B, STUDENT_MJ) == index + 1);
   assert (getStudents (g, UNI_B, STUDENT_MTV) == index);
   assert (getStudents (g, UNI_B, STUDENT_MMONEY) == 0);
   
   assert (getStudents (g, UNI_C, STUDENT_THD) == indexC2 + 2);
   assert (getStudents (g, UNI_C, STUDENT_BPS) == index + 3);
   assert (getStudents (g, UNI_C, STUDENT_BQN) == index + 3);
   assert (getStudents (g, UNI_C, STUDENT_MJ) == indexC1 + 1);
   assert (getStudents (g, UNI_C, STUDENT_MTV) == 0);
   assert (getStudents (g, UNI_C, STUDENT_MMONEY) == 0);   
   printf ("all tests passed\n");
}

void testgetExchangeRate (void) {

	int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,\
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS };
	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};


   Game g = newGame (disciplines, dice);
   printf("Testing getExchangeRate.\n");
   int uni = 1; //player
   int i = 1; //disciplineFrom
   int j = 0; //disciplineTo
   while (uni < 4) {
      while (i < 6) {      
         while (j < 6 ) {
            if (i == j) {
               j ++;
            }
            assert (getExchangeRate (g, uni, i, j) == 3);
            j ++;
         }
      j = 0;
      i ++;
      }
      i = 1;
      uni ++;
   }      
   printf ("set 1 of tests passed\n");
   printf ("testing when there are retraining centres\n");
   action a, bT, bMON, bENG, bSCI, bJ, c, r1, r2;
   int k = 0;
   /*only UNI_C will build ARC to retraining centres as that player
     recieves either a BPS or a BQN which are needed to build ARC.
     also it will make the test less complicated an less prone to 
     error.
   */
   
   a.actionCode  = PASS;
   //random input
   a.disciplineFrom = STUDENT_MTV;
   a.disciplineTo = STUDENT_THD;
   
   bT.actionCode = BUILD_CAMPUS;
   strcpy (bT.destination, TV);
   bT.disciplineFrom = STUDENT_MTV;
   bT.disciplineTo = STUDENT_THD;
   
   bMON.actionCode = BUILD_CAMPUS;
   strcpy (bMON.destination, MONEY);
   bMON.disciplineFrom = STUDENT_MTV;
   bMON.disciplineTo = STUDENT_THD;
   
   bENG.actionCode = BUILD_CAMPUS;
   strcpy (bENG.destination, ENGINEER);
   bENG.disciplineFrom = STUDENT_MTV;
   bENG.disciplineTo = STUDENT_THD;
   
   bSCI.actionCode = BUILD_CAMPUS;
   strcpy (bSCI.destination, SCIENCE);
   bSCI.disciplineFrom = STUDENT_MTV;
   bSCI.disciplineTo = STUDENT_THD;
   
   bJ.actionCode = BUILD_CAMPUS;
   strcpy (bJ.destination, JOB);
   bJ.disciplineFrom = STUDENT_MTV;
   bJ.disciplineTo = STUDENT_THD;

   c.actionCode  = OBTAIN_ARC;
   //random input
   c.disciplineFrom = STUDENT_MTV;
   c.disciplineTo = STUDENT_THD;
   
   r1.actionCode = RETRAIN_STUDENTS;
   strcpy (r1.destination, " ");
   r1.disciplineFrom = STUDENT_BPS;
   r1.disciplineTo = STUDENT_MJ;
   
   r2.actionCode = RETRAIN_STUDENTS;
   strcpy (r2.destination, " ");
   r2.disciplineFrom = STUDENT_BQN;
   r2.disciplineTo = STUDENT_MTV;
   
   while (k < 99) {
      throwDice (g, 9);
      makeAction (g, a);
      k ++;
   }

   k = 0;

   while (k < 99 ) {
      throwDice (g, 5);
      makeAction (g, a);   
      k ++;
   }
   throwDice (g, 10);
   makeAction (g, a);
   throwDice (g, 10);
   makeAction (g, a);
   throwDice (g, 10);
   strcpy (c.destination, "R");
   makeAction (g, c);
   strcpy (c.destination, "RR");
   makeAction (g, c);
   strcpy (c.destination, "L");
   makeAction (g, c);
   strcpy (c.destination, "LR");
   makeAction (g, c);
   strcpy (c.destination, "LRL");
   makeAction (g, c);
   strcpy (c.destination, "RL");
   makeAction (g, c);
   strcpy (c.destination, "RLR");
   makeAction (g, c);
   strcpy (c.destination, "RLRR");
   makeAction (g, c);
   strcpy (c.destination, "RLRRL");
   makeAction (g, c);
   strcpy (c.destination, "RLRRLL");
   makeAction (g, c);
   strcpy (c.destination, "RLRRLLR");
   makeAction (g, c);
   strcpy (c.destination, "RLRRLLRL");
   makeAction (g, c);
   strcpy (c.destination, "RLRRLLRLR");
   makeAction (g, c);
   strcpy (c.destination, "RLRRLLRLRL");
   makeAction (g, c);
   strcpy (c.destination, "RLL");
   makeAction (g, c);
   strcpy (c.destination, "RLLR");
   makeAction (g, c);
   strcpy (c.destination, "RLLRL");
   makeAction (g, c);
   strcpy (c.destination, "RLLRLR");
   makeAction (g, c);
   strcpy (c.destination, "RLLRLRL");
   makeAction (g, c);
   strcpy (c.destination, "RLLRLRR");
   makeAction (g, c);
   strcpy (c.destination, "RLLRLRLR");
   makeAction (g, c);
   strcpy (c.destination, "RLLRLRRL");
   makeAction (g, c);
   strcpy (c.destination, "RLLRLRLRR");
   makeAction (g, c);
   strcpy (c.destination, "RLLRLRRLR");
   makeAction (g, c);
   strcpy (c.destination, "RLLRLRRLRL");
   makeAction (g, c);
   strcpy (c.destination, "RLLRLRRLRLR");
   makeAction (g, c);
   makeAction (g, bT);
   makeAction (g, bMON);
   makeAction (g, bENG);
   makeAction (g, bSCI);
   makeAction (g, bJ);
   
   i = 1;
   j = 0;

   while (i < 6) {      
      while (j < 6 ) {
         if (i == j) {
            j ++;
         }
         assert (getExchangeRate (g, uni, i, j) == 2);
         j ++;
      }
   j = 0;
   i ++;
   }

   printf ("all tests passed\n");
}
