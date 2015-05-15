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

#include "Game-1.07.h"

#define LEFT 'L'
#define RIGHT 'R'
#define BACK 'B'

#define NUM_UNIS 3
/*
// player ID of each university
#define NO_ONE 0
#define UNI_A 1
#define UNI_B 2
#define UNI_C 3
// contents of an ARC
#define VACANT_ARC 0
#define ARC_A 1
#define ARC_B 2
#define ARC_C 3
// contents of a VERTEX
#define VACANT_VERTEX 0
#define CAMPUS_A 1
#define CAMPUS_B 2
#define CAMPUS_C 3
#define GO8_A 4
#define GO8_B 5
#define GO8_C 6
// action codes
#define PASS 0
#define BUILD_CAMPUS 1
#define BUILD_GO8 2
#define OBTAIN_ARC 3
#define START_SPINOFF 4
#define OBTAIN_PUBLICATION 5
#define OBTAIN_IP_PATENT 6
#define RETRAIN_STUDENTS 7
// disciplines
#define STUDENT_THD 0
#define STUDENT_BPS 1
#define STUDENT_BQN 2
#define STUDENT_MJ  3
#define STUDENT_MTV 4
#define STUDENT_MMONEY 5
#define NUM_REGIONS 19
#define PATH_LIMIT 150
#define TRUE 1
#define FALSE 0
*/



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
	testThrowDice ();
	testGetMostARCS ();
	testGetCampus ();
	testGetMostPublications ();
	testGetTurnNUmber ();
	testGetWhoseTurn ();
	testIsLegalAction ();

	//Failed:
   	testgetStudents ();
	testMakeAction ();
	testGetKPIpoints ();
	testgetExchangeRate ();
	testgetPublications ();
	testGetARC (); 
	testgetIPs (); 
	testgetCampuses ();
	testGetGO8s ();

	//Infinite loops:
	testGetDiscipline ();
	testGetDiceValue (); 

   printf ("\n~~**All tests passed!**~~\n");

   return EXIT_SUCCESS;
}

void testMakeAction (void) {

	char *path = NULL;

	int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV,\
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,\
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,\
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS };
	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

   printf ("\n~~testing makeAction()~~\n");

   printf ("testing action code 1: PASS...");
   Game g = newGame (disciplines, dice);
   action a;
   throwDice (g, 10);
   a.actionCode = PASS;
   makeAction (g, a);
   assert (getWhoseTurn (g) == UNI_B);
   disposeGame (g);
   printf ("...passed!\n");

   g = newGame (disciplines, dice);
   throwDice (g, 10);
   printf ("testing action code 3: OBTAIN_ARC...");
   a.actionCode = OBTAIN_ARC;
   path = "RL";
   strcpy (a.destination, path);
   makeAction (g, a);
   assert (getARCs (g, UNI_A) == 3);
   printf ("...passed!\n");
   printf ("testing action code 1: BUILD_CAMPUS...");
   a.actionCode = BUILD_CAMPUS;
   makeAction (g, a);
   assert (getCampuses (g, UNI_A) == 3);
   printf ("...passed!\n");
   printf ("testing action code 2: BUILD_GO8...");
   a.actionCode = BUILD_GO8;

   makeAction (g, a);
   assert (getGO8s (g, UNI_A) == 1);
   printf ("...passed!\n");
   printf ("testing action code 5: START_PUBLICATION...");
   a.actionCode = OBTAIN_PUBLICATION;
   makeAction (g, a);
   assert (getPublications (g, UNI_A) == 1);
   printf ("...passed!\n");
   printf ("testing action code 6: OBTAIN_IP_PATENT...");
   a.actionCode = OBTAIN_IP_PATENT;
   makeAction (g, a);
   assert (getIPs (g, UNI_B) == 1);
   printf ("...passed!\n");
   printf ("testing action code 7: RETRAIN_STUDENTS...");
   // testing actionCode 'RETRAIN_STUDENTS'
   a.actionCode = RETRAIN_STUDENTS;
   a.disciplineFrom = STUDENT_BQN;
   a.disciplineTo = STUDENT_BPS;
   makeAction (g, a);
   assert (getStudents (g, UNI_A, STUDENT_BPS) == 1);
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
   printf ("testing turn 1...");
   throwDice (g, 10);
   assert (getWhoseTurn (g) == UNI_A);
   printf ("...passed!\n");
   printf ("testing turn 2...");
   throwDice (g, 10);
   assert (getWhoseTurn (g) == UNI_B);
   printf ("...passed!\n");
   printf ("testing turn 3...");
   throwDice (g, 10);
   assert (getWhoseTurn (g) == UNI_C);
   printf ("...passed!\n");
   disposeGame (g);

   printf ("\n~~all tests for throwDice() passed!~~\n");

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
   }
   printf ("...passed!\n");
   disposeGame (g);

   printf ("\n~~all tests for getDiscipline() passed!~~\n");

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
   }
   printf ("...passed!\n");
   disposeGame (g);

   printf ("\n~~getDiceValue has passed!~~\n");

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
	a.disciplineFrom = 0;
	a.disciplineTo = 0;

	//Tests that it's vacant at the beginning
	assert (getARC (g, a.destination) == FALSE);
	strcpy (a.destination, "LRLR");
	assert (getARC (g, a.destination) == FALSE);

	//Tests that it will return an Arc Code
	player = getWhoseTurn(g);
	a.actionCode = OBTAIN_ARC;
	assert (getARC (g, a.destination) != FALSE);

	disposeGame(g);
	printf ("... All tests for getARC() passed.\n");

}

void testIsLegalAction (void) {
	//Tests if func tests if action and retraining is legal, TRUE/FALSE

	printf ("Testing isLegalAction()...\n");

	int plyr = UNI_A;
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
	action a;
	a.actionCode = 0;
	a.disciplineFrom = 0;
	a.disciplineTo = 0;

	//Ensure that the game has already started
	assert (isLegalAction (g, a) == FALSE);
	throwDice (g, 3);
	assert (isLegalAction (g, a) == TRUE);

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

	a.actionCode = BUILD_CAMPUS; //Needs 1 of each but THD/M$
	if ((stuTypes[1] > 0) && (stuTypes[2] > 0) && (stuTypes[3] > 0) && (stuTypes[4] > 0)){
		if (getCampus (g, a.destination) == VACANT_VERTEX) {
			assert (isLegalAction (g, a) == TRUE);
		}
	} else {
		assert (isLegalAction (g, a) == FALSE);
	}

	a.actionCode = BUILD_GO8; //Needs a campus, 2MJs, 3M$ + one campus
	noGO8s = getGO8s (g, plyr);
	if ((stuTypes[3] >= 1) && (stuTypes[5] > 2) && (noCampus > 0) && (noGO8s < 8)){
		assert (isLegalAction (g, a) == TRUE);
	} else {
		assert (isLegalAction (g, a) == FALSE);
	}

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

	a.actionCode = START_SPINOFF;
	if ((stuTypes[3] >= 0) && (stuTypes[4] > 0) && (stuTypes[5] > 0)){
		assert (isLegalAction (g, a) == TRUE);
	} else {
		assert (isLegalAction (g, a) == FALSE);
	}

	//Illegal for players to OBTAIN_PUBLICATION and OBTAIN_IP_PATENT
	a.actionCode = OBTAIN_PUBLICATION;
	assert (isLegalAction (g, a) == FALSE);
	a.actionCode = OBTAIN_IP_PATENT;
	assert (isLegalAction (g, a) == FALSE);

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

	//Testing to ensure uni has enough students for retraining
	//Testing THDs that cannot be retrained
	a.disciplineTo = STUDENT_THD;
	a.disciplineFrom = STUDENT_THD;
	assert (isLegalAction (g, a) == FALSE);

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
	int move = 0;
	int player = 0;
	int kpiPts[3] = {0};
	int kpiValues[10] = {PASS, CAMPUS, G08, ARC, 0, 0, IP, 0, MOSTARC, MOSTPUB};

	//Tests all new games start with 0, inlcuding NO_ONE
	while (player < 4) {
		assert (getKPIpoints (g, player) == 0);
		player ++;
	}

	//Tests that players have correct KPI values for every move.
	while (move < 8) {
		player = getWhoseTurn (g);
		a.actionCode = move;
		makeAction (g, a);
		kpiPts[player] += kpiValues[move];
		assert (getKPIpoints (g, player) == kpiPts[player]);
	}

	//Tests prestige KPI Points for most ARCs & Publications
	player = getMostARCs (g);
	kpiPts[player] += MOSTARC;
	assert (getKPIpoints (g, player) == kpiPts[player]);

	player = getMostPublications (g);
	kpiPts[player] += MOSTPUB;
	assert (getKPIpoints (g, player) == kpiPts[player]);

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
	int player = 0;
	int numArcs[3] = {0};

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
	int numG08[3] = {0};

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

void testgetCampuses (void) {
   printf ("Testing getCampuses\n");
   printf ("It returns no. normal campuses a player has.\n");
   printf ("First set of tests:\n");
   printf ("Each player building a campus every turn");

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
   action a = {BUILD_CAMPUS, "L", 3, 1};
   int index = 0;
   assert (getCampuses (g, UNI_A) == 2);
   assert (getCampuses (g, UNI_B) == 2);
   assert (getCampuses (g, UNI_C) == 2);
   while (index < 10) {
         throwDice (g, 7);
         makeAction (g, a);
         assert (getCampuses (g, UNI_A) == index + 2);
         throwDice (g, 7);
         makeAction (g, a);
         assert (getCampuses (g, UNI_B) == index + 2);
         throwDice (g, 7);
         makeAction (g, a);
         assert (getCampuses (g, UNI_C) == index + 2);
      index ++;
   }
   printf ("Set 1 of tests passed\n Starting second set of tests\n");
   printf ("Building campuses.\n");
   printf ("PlayerA every turn, playerB every second and \
            playerC every third.\n");

   g = newGame (disciplines, dice);
   a.actionCode = PASS; 
   action b = {BUILD_CAMPUS, "L", 3, 1};
   int indexA = 0;
   int indexB = 0;
   int indexC = 0;

      while (indexC < 10) {
         throwDice (g, 7);
         makeAction (g, b);
         indexA ++;
         assert (getCampuses (g, UNI_A) == indexA + 2);
         throwDice (g, 7);
         if ((indexB % 2) == 0) {
            makeAction (g, b);
            indexB ++;
        } else {
            makeAction (g, a);
        }
         assert (getCampuses (g, UNI_B) == indexB + 2);
         throwDice (g, 7);
         if ((indexC % 3) == 0) {
            makeAction (g, b);
            indexC ++;
         } else {
            makeAction (g, a);
         }
         assert (getCampuses (g, UNI_C) == indexC + 2) ;
        }
   printf ("all tests passed\n");

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
   printf ("Testing getPublications\n");
   printf ("It returns no. Publications a player has.\n");
   printf ("First set of tests:\n");
   printf ("Each player getting a Publication every turn");

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
   action a = {OBTAIN_PUBLICATION, "L", 3, 1};
   int index = 0;
   assert (getPublications (g, UNI_A) == 0);
   assert (getPublications (g, UNI_B) == 0);
   assert (getPublications (g, UNI_C) == 0);
   while (index < 10) {
         throwDice (g, 7);
         makeAction (g, a);
         assert (getPublications (g, UNI_A) == index);
         throwDice (g, 7);
         makeAction (g, a);
         assert (getPublications (g, UNI_B) == index);
         throwDice (g, 7);
         makeAction (g, a);
         assert (getPublications (g, UNI_C) == index);
      index ++;
   }
   printf ("Set 1 of tests passed\n Starting second set of tests\n");
   printf ("Getting Publications.\n");
   printf ("PlayerA every turn, playerB every second and \
            playerC every third.\n");

   g = newGame (disciplines, dice);
   a.actionCode = PASS;
   action b = {OBTAIN_PUBLICATION, "L", 3, 1};
   int indexA = 0;
   int indexB = 0;
   int indexC = 0;

      while (indexC < 10) {
         throwDice (g, 7);
         makeAction (g, b);
         indexA ++;
         assert (getPublications (g, UNI_A) == indexA);
         throwDice (g, 7);
         if ((indexB % 2) == 0) {
            makeAction (g, b);
            indexB ++;
        } else {
            makeAction (g, a);
        }
         assert (getPublications (g, UNI_B) == indexB);
         throwDice (g, 7);
         if ((indexC % 3) == 0) {
            makeAction (g, b);
            indexC ++;
         } else {
            makeAction (g, a);
         }
         assert (getPublications (g, UNI_C) == indexC);
        }
   printf ("all tests passed\n");
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
   bJ.disciplineTo = STUDENT_THD

   c.actionCode  = OBTAIN_ARC;
   //random input
   c.disciplineFrom = STUDENT_MTV;
   c.disciplineTo = STUDENT_THD;
   
   r1.actionCode = RETRAN_STUDENTS;
   strcpy (r1.destination, " ");
   r1.disciplineFrom = STUDENT_BPS;
   r1.disciplineTo = STUDENT_MJ;
   
   r2.actionCode = RETRAN_STUDENTS;
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
