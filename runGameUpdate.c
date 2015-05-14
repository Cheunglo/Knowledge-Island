/*
 * Tutorial tuebanjo15 Bernice Chen
 * Name: 4Stack
 *      Stephanie Chua, Louis Cheung, Alex Trinh, Ruby Yu
 * runGame.c
 * Runs the game with interactive player instead of an AI
 *
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <assert.h>
 #include "Game-1.07.h"


//Maximum values of items
#define MAX_KPIPTS 150
#define MAX_G08S 8
#define MAX_STU_TYPES 6
#define NO_PLAYERS 3

action actionPrompt (Game g);
static int rollDice (void);
static void actionOptions (void);
static void retrainOptions (void);

int main (int argc, char * argv[]) {

	int disciplines[19] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
		STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, \
		STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, \
		STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, \
		STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS};

	int dice[19] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

	Game g = newGame (disciplines, dice);
	action a;
//	int score = 0; //This stores the KPI points of current player 
	int curPlayer = NO_ONE; 
	int curTurn = 0; 
	int playerKPI = 0;
	int numStudents[MAX_STU_TYPES];
	int legal = 0;
	int diceScore = 0; 
	int spinoffDice = 0;
	//int discipline = 0;
	int counter = 0; 

	char *degrees[] = \
		  {"THDs", "BPSs", "BQNs", "MJs", "MTVs", "MMONEYs"};
	


	printf ("New game created, variables are initialized.\n");
	printf ("Let the game begin!\n\n");

	while (playerKPI < MAX_KPIPTS) {
		
		//To simulate an actual real life dice, we throw two dice 
		diceScore = rollDice();
		throwDice (g, diceScore);

		curTurn = getTurnNumber (g);
		curPlayer = getWhoseTurn (g);
		printf ("Turn number: %d\n", curTurn);
		printf ("Player %d's turn. Rolling dice.\n", curPlayer);
		printf ("The two dice rolled a %d! \n", diceScore);

		printf ("You currently have: \n");
		counter = 0;
		while (counter < MAX_STU_TYPES) {
			numStudents[counter] = getStudents (g, curPlayer, counter);
			printf ("    %d: %s\n", numStudents[counter],\
								    degrees[counter]);
			counter ++;
		}
		actionOptions ();
		a = actionPrompt (g);
		legal = isLegalAction (g, a);

		while (a.actionCode != PASS) {
			while (legal == FALSE) {
				printf ("The chosen action is illegal, try again.\n");
				actionPrompt (g);
				legal = isLegalAction (g, a);
			}

			if (a.actionCode == START_SPINOFF) {
				//Ensures dice returns 1, 2 or 3.
				spinoffDice = (rand()%3 + 1);
				if (spinoffDice == 1) {
					a.actionCode = OBTAIN_IP_PATENT;
				} else {
					a.actionCode = OBTAIN_PUBLICATION;
				}
			}

			makeAction (g, a);
			printf("Action made!");

			printf ("Your current stats:\n");
			printf ("    ARC Grants: %d\n", getARCs (g, curPlayer));
			printf ("    No of Campuses: %d\n", getCampuses (g, curPlayer));
			printf ("    No of G08s: %d\n", getGO8s (g, curPlayer));
			printf ("    No of Pubs: %d\n", getPublications (g, curPlayer));
			printf ("    No of IP Patents: %d\n", getIPs (g, curPlayer));

			playerKPI = getKPIpoints (g, curPlayer);
			printf ("Player %d KPI Score: %d\n\n", curPlayer, playerKPI);
		}

		printf ("Next player's turn.\n\n");

	}

	printf ("Congratulations, player %d won.\n", curPlayer);
	printf ("Game completed. Disposing game.\n");
	
	disposeGame (g);

	return EXIT_SUCCESS;
}

static void actionOptions (void) {
	printf ("Choose an action from the following:\n");
	printf ("Enter 0 - PASS\n");
	printf ("Enter 1 - Build a Campus\n");
	printf ("Enter 2 - Upgrade to a G08!\n");
	printf ("Enter 3 - Get an ARC.\n");
	printf ("Enter 4 - Start a business spinoff~\n");
	printf ("Enter 7 - Retrain your students.\n Input: ");
}

static void retrainOptions (void) {
	printf ("Retraining Students initiated!\n");
	printf ("Enter 0 - THD......\n");
	printf ("Enter 1 - BPS\n");
	printf ("Enter 2 - BQN\n");
	printf ("Enter 3 - MJ\n");
	printf ("Enter 4 - MTV\n");
	printf ("Enter 7 - MONEY $$$\n Input: ");	
}
action actionPrompt (Game g) {

	char *positionInput = malloc (100);
	action a;
	int counter = 0;
	char *degrees[] = \
		  {"THDs", "BPSs", "BQNs", "MJs", "MTVs", "MMONEYs"};
	
	scanf ("%d", &a.actionCode);
	printf ("You chose %d\n", a.actionCode);

	if (a.actionCode == OBTAIN_ARC 
		|| a.actionCode == BUILD_CAMPUS
		|| a.actionCode == BUILD_GO8) {
		printf ("Please input the position of your structure: ");
		scanf ("%s", positionInput);
		char *position = malloc (sizeof (char) * \
						(strlen(positionInput) + 1));
		while (counter < strlen (positionInput)) {
			position[counter] = positionInput[counter];
			counter ++;
		}
		position[strlen(positionInput)] = '\0';
		strcpy (a.destination, position);
	} else if (a.actionCode == RETRAIN_STUDENTS) {
		retrainOptions ();
		printf ("Retraining from: ");
		scanf ("%d\n", &a.disciplineFrom);
		printf ("i.e. %s\n", degrees[(int)a.disciplineFrom]); 
		printf ("Retraining to: ");
		scanf ("%d\n", &a.disciplineTo);
		printf ("i.e. %s\n", degrees[(int)a.disciplineTo]);
	}
	return a;
}

//Function to roll two dices
static int rollDice (void) {

	int diceValue = 0;

	//Ensures that the dice will be from 1 to 6
	diceValue = (rand()%6 + 1) + (rand()%6 + 1); 

	return diceValue;
}

