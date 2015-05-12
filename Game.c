//Made by Alex, Ruby, Louis and Steph
//Game.c 

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "Game-1.07.h"

//Possible movements via path:
#define LEFT 'L'
#define RIGHT 'R'
#define BACK 'B' 
#define NUM_DIRECTIONS 6
#define COORD 7 //Number of coordinates in x, y, z plane, -3to3
#define REGIONCOORD 6

//Maximum values of items
#define MAX_KPIPTS 150
#define MAX_G08S 8
#define MAX_STU_TYPES 6
#define NO_PLAYERS 4

//Initial number of students the player starts with
#define INITIAL_THD 0
#define INITIAL_BPS 3
#define INITIAL_BQN 3
#define INITIAL_MTV 1
#define INITIAL_MJ 1
#define INITIAL_MMONEY 1

//KPI Value of each move:
#define CAMPUSPTS 10
#define G08PTS 10    //Only 10 points added to previous 10 pts of campus
#define ARCPTS 2
#define SPINOFFPTS 0 //Will have effect only if pub/ip.
#define PUBPTS 0     //Only prestige, no actual points
#define IPPTS 10
#define RETRAINPTS 0 //No KPI points will be earned
#define MOSTARCPTS 10
#define MOSTPUBPTS 10 

//Paths to different retraining centres from top Campus A
#define TV1 "R"			//MTV 1 retraining centre	
#define TV2 "RR"                //MTV 2 retraining centre
#define MONEY1 "LR"		//MMONEY2 retraining centre
#define MONEY2 "LRL"            //MMONEY2 retraining centre
#define ENGINEER1 "RLRRLLRLR"	//BPS1 retraining centre
#define ENGINEER2 "RLRRLLRLRL"  //BPS2 retraining centre
#define SCIENCE1 "RLLRLRLR"     //BQN1 retraining centre
#define SCIENCE2 "RLLRLRLRR"    //BQN2 retraining centre
#define JOB1 "RLLRLRRLRL"       //MJOBS1 retraining centre
#define JOB2 "RLLRLRRLRLR"       //MJOBS2 retraining centre
   

#define DEFAULT_DISCIPLINES {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, \
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN,    \
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ,     \
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS }

#define DEFAULT_DICE {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5}

typedef struct _boardCoord {
	//Coordinates of the board
	int direction; // Range 0-5, initially determined by campus
	//Coordinate ranges are from -3 to 3. 
	//Anything else is within the sea.
	//Observation also all coordinates of vertexes add to +/-2
	//All region coordinates add up to 0
	//Two adjacent coordinates add up to 0 too
	int x;
	int y;
	int z;
} coord;

typedef struct _boardEdge {
	//Edge defined as line between two coordinates
	coord pointA;
	coord pointB;
} edge;

typedef struct _board {
	//Stores campus player code
	//Regular campuses are 1, 2, 3 but G08s are 4, 5, 6
	int campus[COORD][COORD][COORD];
	int region[REGIONCOORD][REGIONCOORD][REGIONCOORD];
	//Stores arc player code
	//Each arc will have two copies, since an arc is between two
	//points, hence points A - B and B - A
	int arc[COORD][COORD][COORD][COORD][COORD][COORD];
    //not needed anymore
    //int retraining [COORD][COORD][COORD][COORD][COORD][COORD];
} board;

typedef struct _player {
    int numArcs;
    int numCmps;
    int numG08s;
    int numPubs;
    int numIPs;
    int numStudents[MAX_STU_TYPES];
} player;

typedef struct _game {
    int currentTurn; //should start at -1
    int mostArcs;
    int mostPubs;
    int numRegions; 
    int regionDiscipline[NUM_REGIONS];
    int regionDice[NUM_REGIONS];
    board gameBoard; 
    player uni[NUM_UNIS];       
} game;

int cmpsConditions (Game g, action a, int player);
int G08Conditions (Game g, action a, int player);
int arcConditions (Game g, action a, int player);
int spinoffConditions (Game g, action a, int player);
int retrainConditions (Game g, action a, int player);
void initializeBoard (Game g);
void makeRegion (Game g, int discipline[]);
void makeArc (Game g, char * path, int player);
void makeCampus (Game g, char * path, int player);
void makeG08 (Game g, char * path, int player);
void addStudents (int regionID);
int rollDice (void);
coord pathMovement (char * path);
coord movement (coord point, char move); //single movements only

int main (int argc, char *argv []) {
	printf ("Entering Main!");
	return EXIT_SUCCESS;
}

// Creates a new game and intialises the game board
// and player resources
Game newGame (int discipline[], int dice[]) {

    // sets memory aside for the game
    Game g = malloc (sizeof(game));

    // initialise turn to -1
    g->currentTurn = -1;

    // initialise game board
    int regionID;
    regionID = 0;
    while (regionID < NUM_REGIONS) {
        g->regionDiscipline[regionID] = discipline[regionID];
        g->regionDice[regionID] = dice[regionID];
        regionID++;
    }

    // initialise prestige awards
    g->mostArcs = NO_ONE;
    g->mostPubs = NO_ONE;

    // initialise players
    // 1<=player<=3 but array is from 0 to 2 so minus 1 to rectify
    int curPlayer = UNI_A-1;
    while (curPlayer < NUM_UNIS) {
        // initialises a player's students resources
        g->uni[curPlayer].numStudents[STUDENT_THD] = INITIAL_THD;
        g->uni[curPlayer].numStudents[STUDENT_BPS] = INITIAL_BPS;
        g->uni[curPlayer].numStudents[STUDENT_BQN] = INITIAL_BQN;
        g->uni[curPlayer].numStudents[STUDENT_MJ] = INITIAL_MJ;
        g->uni[curPlayer].numStudents[STUDENT_MTV] = INITIAL_MTV;
        g->uni[curPlayer].numStudents[STUDENT_MMONEY] = INITIAL_MMONEY;

        // initialises a player's resources and points
        g->uni[curPlayer].numArcs = 0;
        g->uni[curPlayer].numCmps = 2;
        g->uni[curPlayer].numG08s = 0;
        g->uni[curPlayer].numPubs = 0;
        g->uni[curPlayer].numIPs = 0;

        curPlayer++;
    }

    //Initialize the gameboard:
    initializeBoard(g);

    return g;
}

//Initializes the board for newGame()
void initializeBoard (Game g) {
	makeRegion();
	//For player 1 UNI_A
	g->gameBoard.campus[2][5][0] = UNI_A;
	char * path;
	path = "L";
	makeArc (g, path, UNI_A);
	path = "R";
	makeArc (g, path, UNI_A);
	g->gameBoard.campus[4][1][6] = UNI_A;
	g->gameBoard.arc[4][1][6][3][0][4] = UNI_A;
	g->gameBoard.arc[3][0][4][4][1][6] = UNI_A;
	g->gameBoard.arc[4][1][6][2][0][5] = UNI_A;
	g->gameBoard.arc[2][0][5][4][1][6] = UNI_A;

	//For player 2 UNI_B
	g->gameBoard.campus[1][6][4] = UNI_B;
	g->gameBoard.arc[1][6][4][0][4][3] = UNI_B;
	g->gameBoard.arc[0][4][3][1][6][4] = UNI_B;
	g->gameBoard.arc[1][6][4][0][5][2] = UNI_B;
	g->gameBoard.arc[0][5][2][1][6][4] = UNI_B;
	g->gameBoard.campus[5][0][2] = UNI_B;
	g->gameBoard.arc[5][0][2][6][2][3] = UNI_B;
	g->gameBoard.arc[6][2][3][5][0][2] = UNI_B;
	g->gameBoard.arc[5][0][2][6][1][4] = UNI_B;
	g->gameBoard.arc[6][1][4][5][0][2] = UNI_B;

	//For player 3 UNI_C
	path = "LRLRL";
	makeCampus (g, path, UNI_C);
	g->gameBoard.arc[6][4][1][4][3][0] = UNI_C;
	g->gameBoard.arc[4][3][0][6][4][1] = UNI_C;;
	g->gameBoard.arc[6][4][1][5][2][0] = UNI_C;;
	g->gameBoard.arc[5][2][0][6][4][1] = UNI_C;;
	g->gameBoard.campus[0][2][5] = UNI_C;
	g->gameBoard.arc[0][2][5][2][3][6] = UNI_C;
	g->gameBoard.arc[2][3][6][0][2][5] = UNI_C;
	g->gameBoard.arc[0][2][5][1][4][6] = UNI_C;
	g->gameBoard.arc[1][4][6][0][2][5] = UNI_C;
/*
	//Initializing retraining center
	g->gameBoard.retraining[1][1][5][2][3][6] = STUDENT_BPS;
	g->gameBoard.retraining[2][3][6][1][1][5] = STUDENT_BPS;
	g->gameBoard.retraining[5][1][1][6][2][3] = STUDENT_BQN;
	g->gameBoard.retraining[6][2][3][5][1][1] = STUDENT_BQN;
	g->gameBoard.retraining[5][1][5][4][1][3] = STUDENT_MJ;
	g->gameBoard.retraining[4][1][3][5][1][5] = STUDENT_MJ;
	g->gameBoard.retraining[1][5][1][3][6][2] = STUDENT_MTV;
	g->gameBoard.retraining[3][6][2][1][5][1] = STUDENT_MTV;
	g->gameBoard.retraining[5][5][1][3][4][0] = STUDENT_MMONEY;
	g->gameBoard.retraining[3][4][0][5][5][1] = STUDENT_MMONEY;
*/
}


// release all the memory malloced for the game
void disposeGame (Game g) {

    free (g);
}


// make the specified action for the current player and update the
// game state accordingly.
// The function may assume that the action requested is legal.
// START_SPINOFF is not a legal action here
void makeAction (Game g, action a) {

    // determine the current player
    int curPlayer;
    curPlayer = getWhoseTurn (g);
    int rate = 0;

    // peforms a particular action given the action code
    if (a.actionCode == PASS) {
        throwDice (g, rollDice());
    } else if (a.actionCode == BUILD_CAMPUS) {
        // deduct the resources from the player
        g->uni[curPlayer].numStudents[STUDENT_BPS] -= 1;
        g->uni[curPlayer].numStudents[STUDENT_BQN] -= 1;
        g->uni[curPlayer].numStudents[STUDENT_MJ] -= 1;
        g->uni[curPlayer].numStudents[STUDENT_MTV] -= 1;

        makeCampus (g, a.destination, curPlayer);
        g->uni[curPlayer].numCmps++;

    } else if (a.actionCode == BUILD_GO8) {
        // deduct the resources from the player
        g->uni[curPlayer].numStudents[STUDENT_MJ] -= 2;
        g->uni[curPlayer].numStudents[STUDENT_MMONEY] -= 3;

        makeG08 (g, a.destination, curPlayer);

        g->uni[curPlayer].numCmps--;
        g->uni[curPlayer].numG08s++;

    } else if (a.actionCode == OBTAIN_ARC) {
        // deduct the resources from the player
        g->uni[curPlayer].numStudents[STUDENT_BQN] -= 1;
        g->uni[curPlayer].numStudents[STUDENT_BPS] -= 1;

        makeArc (g, a.destination, curPlayer);
        g->uni[curPlayer].numArcs++;
		
		//change mostArcs
		if(g->mostArcs == NO_ONE){
			g->mostArcs = curPlayer;
		}
		else if(g->uni[curPlayer].numArcs > g->uni[g->mostArcs].numArcs){
			g->mostArcs = curPlayer;
		}

    } else if (a.actionCode == OBTAIN_PUBLICATION) {
        g->uni[curPlayer].numPubs++;
		
		//change mostPubs
		if(g->mostPubs == NO_ONE){
			g->mostPubs = curPlayer;
		}
		else if(g->uni[curPlayer].numPubs > g->uni[g->mostPubs].numPubs){
			g->mostPubs = curPlayer;
		}
		
    } else if (a.actionCode == OBTAIN_IP_PATENT) {
        g->uni[curPlayer].numIPs++;
    } else if (a.actionCode == RETRAIN_STUDENTS) {
        rate = getExchangeRate (g, curPlayer, a.disciplineFrom, a.disciplineTo);
        g->uni[curPlayer].numStudents[a.disciplineFrom] -= rate;
        g->uni[curPlayer].numStudents[a.disciplineTo]++;
    }
}

// advance the game to the next turn,
// assuming that the dice has just been rolled and produced diceScore
// the game starts in turn -1 (we call this state "Terra Nullis") and
// moves to turn 0 as soon as the first dice is thrown.
// determines which region contains the diceValue of the diceScore
// determines who gets what
void throwDice (Game g, int diceScore) {

    // advances the game to next turn
    g->currentTurn++;

    // to obtain the regionID/location of the diceScore/diceValue
    int i = 0;
    int regionID = 0;
    while (i < NUM_REGIONS) {
        if (diceScore == getDiceValue(g, i)) {
            regionID = i;
            addStudents (regionID);
        }
        i++;
    }

    // whenever a 7 is thrown, immediately after any new students are produced,
    // all MTV and MMONEY students of all universities decide to switch to ThD's.
    if (diceScore == 7) {
        // 1<=player<=3 but array is from 0 to 2 so minus 1 to rectify
		curPlayer = UNI_A-1;
        while (curPlayer < NUM_UNIS) {
            g->uni[curPlayer].numStudents[STUDENT_THD] += g->uni[curPlayer].numStudents[STUDENT_MTV] +
                                                          g->uni[curPlayer].numStudents[STUDENT_MMONEY];
            g->uni[curPlayer].numStudents[STUDENT_MTV] = 0;
            g->uni[curPlayer].numStudents[STUDENT_MMONEY] = 0;
            curPlayer++;
        }
    }
}


// what type of students are produced by the specified region?
// regionID is the index of the region in the newGame arrays (above)
// see discipline codes above
int getDiscipline (Game g, int regionID) {

    int discipline;
    discipline = g->regionDiscipline[regionID];
    return discipline;
}


// what dice value produces students in the specified region?
// 2..12
int getDiceValue (Game g, int regionID) {

    int diceValue;
    diceValue = g->regionDice[regionID];
    return diceValue;
}

int rollDice (void) {

    int diceValue = 0;
    diceValue = (rand()%6 + 1) + (rand()%6 + 1);

    return diceValue;
}


//Returns TRUE/FALSE on whether action is legal or not
int isLegalAction (Game g, action a) {

	int output = FALSE;
	int player = getWhoseTurn (g);

	//Ensures game has already started
	if (g->currentTurn < 0) {
		output = FALSE; 
	} 

	//Ensure action codes are legal
	if ((a.actionCode >= 0) && (a.actionCode < 8)) {
		output = FALSE; 
	}

	//Tests the conditions for building the 
	if (a.actionCode == PASS) {
		output = TRUE;
	} else if (a.actionCode == BUILD_CAMPUS) {
		output = cmpsConditions (g, a, player);
	} else if (a.actionCode == BUILD_GO8) {
		output = G08Conditions (g, a, player);
	} else if (a.actionCode == OBTAIN_ARC) {
		output = arcConditions (g, a, player);
	} else if (a.actionCode == START_SPINOFF) {
		output = spinoffConditions (g, a, player); 
	} else if (a.actionCode == OBTAIN_PUBLICATION) {
		//no player should be able to obtain publication freely
		output = FALSE;
	} else if (a.actionCode == OBTAIN_IP_PATENT) {
		//no player should be able to obtain IP patent freely
		output = FALSE;
	} else if (a.actionCode == RETRAIN_STUDENTS) {
		output = retrainConditions (g, a, player);
	}

	return output;
}

//Function returns the KPI points of specified player
int getKPIpoints (Game g, int player) {

	int kpiPoints = 0;
	
	//Add KPI points from every object player owns
	kpiPoints += g->uni[player].numArcs * ARCPTS;
	kpiPoints += g->uni[player].numCmps * CAMPUSPTS;
	kpiPoints += g->uni[player].numG08s * G08PTS;
	kpiPoints += g->uni[player].numIPs * IPPTS;

	//Conditions for prestige KPI points
	if (player == g->mostArcs) {
		kpiPoints += MOSTARCPTS;
	} 

	if (player == g->mostPubs) {
		kpiPoints += MOSTPUBPTS;
	}

	return kpiPoints;
}

//Returns the number of arcs of the player
int getARCs (Game g, int player) {

	return g->uni[player].numArcs;
}

//Returns number of G08s of specified player
int getGO8s (Game g, int player) {
	//Ensures that there is less than 8G08s the player owns, else error
	assert (g->uni[player].numG08s <= MAX_G08S);
	return g->uni[player].numG08s;
}

//Returns true/false, tests conditions for campus,
// which needs 1 of each but THD or M$,
// and no two campuses can be on adjacent vertexes
int cmpsConditions (Game g, action a, int player) {

	int answer = FALSE;

	if ((g->uni[player].numStudents[STUDENT_BPS] >= 1) 
		&& (g->uni[player].numStudents[STUDENT_BQN] >= 1) 
		&& (g->uni[player].numStudents[STUDENT_MJ] >= 1) 
		&& (g->uni[player].numStudents[STUDENT_MTV] >= 1) 
		&& (getCampus (g, a.destination) == VACANT_VERTEX)
		&& (getARC (g, a.destination) != VACANT_ARC)) {

		answer = TRUE;
	} else {
		answer = FALSE;
	}

	return answer;
}

//Tests conditions for G08, which needs a campus + 2MJs + 3M$ 
int G08Conditions (Game g, action a, int player) {

	int answer = FALSE;

	if ((g->uni[player].numStudents[STUDENT_MJ] >= 2) 
		&& (g->uni[player].numStudents[STUDENT_MMONEY] >= 3) 
		&& (g->uni[player].numCmps > 0) 
		&& (g->uni[player].numG08s < MAX_G08S) 
		&& (getCampus (g, a.destination) > 3)) {

		answer = TRUE;
	} else {
		answer = FALSE;
	}

	return answer;
}

//Tests conditions for a spinoff, which is MJ+MTV+MMNY
int spinoffConditions (Game g, action a, int player) {

	int answer = FALSE;

	if ((g->uni[player].numStudents[STUDENT_MJ] >= 1) 
		&& (g->uni[player].numStudents[STUDENT_MTV] >= 1) 
		&& (g->uni[player].numStudents[STUDENT_MMONEY] >= 1)){

		answer = TRUE;
	} else {
		answer = FALSE;
	}
	return answer;
}

//Tests conditions for path, which needs 1 BPS and 1 BQN
//Ensure paths are connected to other paths & are legal paths
//Does not leave into the sea, and path is vacant
int arcConditions (Game g, action a, int player) {

	int answer = FALSE;

	if ((g->uni[player].numStudents[STUDENT_BPS] >= 1) 
		&& (g->uni[player].numStudents[STUDENT_BQN] >= 1) 
		&& (a.destination[0] != BACK)  //player doesn't move back to sea
		&& (sizeof (a.destination) < PATH_LIMIT) 
		&& (getARC (g, a.destination) == VACANT_ARC)) {

		answer = TRUE;
	} else {
		answer = FALSE;
	}
	return answer;
}

//Returns true/false based on whether it suits retraining conditions
//Ensures valid discipline numbers used
//Ensures enough students for retraining
//THDs cannot be retrained, (code 0)
int retrainConditions (Game g, action a, int player) {
	
	int answer = FALSE;
	int rate = 0;
	int numStudents = 0;

	rate = getExchangeRate (g, player, a.disciplineFrom, a.disciplineTo);
	numStudents = getStudents (g, player, a.disciplineFrom);

	if ((a.disciplineTo >= 0) && (a.disciplineFrom > 0)
		&& (a.disciplineTo < MAX_STU_TYPES)
		&& (a.disciplineFrom < MAX_STU_TYPES)
		&& (numStudents >= rate)) {

		answer = TRUE;
	} else {
		answer = FALSE;
	}
	
	return answer;
}


//Checks to see if there is an arc in the edge
int getARC (Game g, path pathToEdge) {
	
	int arcCode = 0;
	char *path = pathToEdge;
	int xPtA = 0;
	int yPtA = 0;
	int zPtA = 0;
	int xPtB = 0;
	int yPtB = 0;
	int zPtB = 0;

	edge arc; //Edge to be checked
	//Assert that they will all add to +-2 to be a vertex, else region
	arc.pointA = pathMovement (path);
	assert (abs(arc.pointA.x+arc.pointA.y+arc.pointA.z) == 2);
	arc.pointB = movement (arc.pointA, BACK);
	assert (abs(arc.pointB.x+arc.pointB.y+arc.pointB.z) == 2);

	//Sum of adjacent coordinates add to 0
	assert (arc.pointA.x+arc.pointA.y+arc.pointA.z+arc.pointB.z == 0);

	xPtA = arc.pointA.x + 3;
	yPtA = arc.pointA.y + 3;
	zPtA = arc.pointA.z + 3;
	xPtB = arc.pointB.x + 3;
	yPtB = arc.pointB.y + 3;
	zPtB = arc.pointB.z + 3;

	arcCode = g->gameBoard.arc[xPtA][yPtA][zPtA][xPtB][yPtB][zPtB];
	
	return arcCode;
}

//Creates an ARC in given board
void makeArc (Game g, char * path, int player) {

	int xPtA = 0;
	int yPtA = 0;
	int zPtA = 0;
	int xPtB = 0;
	int yPtB = 0;
	int zPtB = 0;
	edge arc;
	arc.pointA = pathMovement (path);
	arc.pointB = movement (arc.pointA, BACK);

	xPtA = arc.pointA.x + 3;
	yPtA = arc.pointA.y + 3;
	zPtA = arc.pointA.z + 3;
	xPtB = arc.pointB.x + 3;
	yPtB = arc.pointB.y + 3;
	zPtB = arc.pointB.z + 3;
	g->gameBoard.arc[xPtA][yPtA][zPtA][xPtB][yPtB][zPtB] = player;
	g->gameBoard.arc[xPtB][yPtB][zPtB][xPtA][yPtA][zPtA] = player;
}

//Checks to see if there is a campus 
int getCampus (Game g, path pathToVertex) {
	
	char *path = pathToVertex;
	int campusCode = 0;
	int xArray = 0;
	int yArray = 0;
	int zArray = 0;

	coord vertex = pathMovement (path); //Vertex to be checked
	//Ensure that it is a vertex, that it adds to +-2
	assert (abs(vertex.x+vertex.y+vertex.z) == 2);

	xArray = vertex.x + 3;
	yArray = vertex.y + 3;
	zArray = vertex.z + 3;

	//Returns whatever code inside (player or 0)
	campusCode = g->gameBoard.campus[xArray][yArray][zArray];

	return campusCode;
}

//Stores campus data in gameboard 
void makeCampus (Game g, char * path, int player) {

	int xArray = 0;
	int yArray = 0;
	int zArray = 0;
	coord movedPoint;

	movedPoint = pathMovement (path);
	assert (abs(movedPoint.x+movedPoint.y+movedPoint.z) == 2);
	//Save the player code into the array
	xArray = movedPoint.x + 3;
	yArray = movedPoint.y + 3;
	zArray = movedPoint.z + 3;
	g->gameBoard.campus[xArray][yArray][zArray] = player;

}

//Stores G08 data in gameboard 
void makeG08 (Game g, char * path, int player) {

	int xArray = 0;
	int yArray = 0;
	int zArray = 0;
	coord movedPoint;

	movedPoint = pathMovement (path);
	//Save the player code into the array
	xArray = movedPoint.x + 3;
	yArray = movedPoint.y + 3;
	zArray = movedPoint.z + 3;
	//Since it's 4, 5, 6 for a GO8
	g->gameBoard.campus[xArray][yArray][zArray] = player+3;
}

coord pathMovement (char * path) {

	int move = 0;
	int i = 0;
	coord movedPoint;
	coord start;
	//Always start from upper Campus A
	start.direction = 3;
	start.x = -1;
	start.y = 2;
	start.z = -3;

	movedPoint = start;
	//Continuously searches string for movements and make moves to get to point
	while (path[i] != 0) {
		move = path[i];
		movedPoint = movement (movedPoint, move);
		i ++;
	}

	return movedPoint;
}

// which university currently has the prestige award for the most ARCs?
// this is NO_ONE until the first arc is purchased after the game 
// has started.  
int getMostARCs (Game g){
	return g->mostArcs;
}

// which university currently has the prestige award for the most pubs?
// this is NO_ONE until the first publication is made.
int getMostPublications (Game g){
	return g->mostPubs;
}

// return the current turn number of the game -1,0,1, ..
int getTurnNumber (Game g){
	return g->currentTurn;
}

// return the player id of the player whose turn it is 
// the result of this function is NO_ONE during Terra Nullis
int getWhoseTurn (Game g){
	int whoseTurn = UNI_C;
    //special case for terra nullis
    if (g->currentTurn == -1) {
        whoseTurn = UNI_C;
    } 
	//mod turn number by 3 as the turn rotates between 3 unis
	else if (g->currentTurn % 3 == 0) {
        whoseTurn = UNI_A;
    } 
	else if (g->currentTurn % 3 == 1) {
        whoseTurn = UNI_B;
    } 
	else if (g->currentTurn % 3 == 2) {
        whoseTurn = UNI_C;
    }

    return whoseTurn;
}


/* Movement Function 
 *
 * About the directions:
 * Six directions, three of them positive and other three negative.
 * In actual Game.c, the direction is predetermined by campus location
 *         and subsequently updated each time a move is made.
 * Possible direction values: 0, 1, 2, 3, 4, 5 (in a ring)
 * To visualize, draw a ring that changes every Pi/3.
 * 
 * Coordinate System: 3D x-y-z plane to identify each vertex and region
 * For more info: http://www.redblobgames.com/grids/hexagons/
 * 
 * Movement Algorithm:
 + x, y, z will add/subtract 1 or 2 depending on the movement direction.
 * If a value is added, the rest will be added too 
 *            and vice versa with subtraction.
 * Only one value (x/y/z) can be +2, and the rest +1.
 *
 * Every L = Direction - 1
 * Every R = Direction + 1
 * Every B = Direction + 3 (180 Deg turn)
 * 
 * Corresponding directions and the side added/subtracted 2
 * Directions 0, 3 | L: x | R: z | B: y
 * Directions 1, 4 | L: y | R: x | B: z
 * Directions 2, 5 | L: z | R: y | B: x
 * 
 * The sign is also determined by the movement direction:
 * Addition directions: 1, 3, 5
 * Subtraction directions: 0, 2, 4
 */
coord movement (coord point, char move) {

	assert (point.direction >= 0);
	assert (point.direction < NUM_DIRECTIONS);

	coord newMove;
	int sign = 0;
	int dir = point.direction;

	//The direction determines movement will be an addition/subtraction
	//Addition directions: 1, 3, 5
	//Subtraction directions: 0, 2, 4
	if (dir%2 != 0) {
		sign = 1;
	} else if (dir%2 == 0) {
		sign = -1;
	}

	//At first, everything must be added/subtracted by minimum 1:
	newMove.x = point.x + sign;
	newMove.y = point.y + sign;
	newMove.z = point.z + sign;

	//Another extra +/-1 explained on direction movement algorithm 
	if (point.direction == 1 || point.direction == 4) {
		if (move == LEFT) {
			newMove.direction = point.direction - 1;
			newMove.y += sign;
		} else if (move == RIGHT) {
			newMove.direction = point.direction + 1;
			newMove.x += sign;
		} else if (move == BACK) {
			newMove.direction = point.direction + 3;
			newMove.z += sign;
		}
	} else if (point.direction == 2 || point.direction == 5) {
		if (move == LEFT) {
			newMove.direction = point.direction - 1;
			newMove.z += sign;
		} else if (move == RIGHT) {
			newMove.direction = point.direction + 1;
			newMove.y += sign;
		} else if (move == BACK) {
			newMove.direction = point.direction + 3;
			newMove.x += sign;
		}
	} else if (point.direction == 3 || point.direction == 0 ) {
		if (move == LEFT) {
			newMove.direction = point.direction - 1;
			newMove.x += sign;
		} else if (move == RIGHT) {
			newMove.direction = point.direction + 1;
			newMove.z += sign;
		} else if (move == BACK) {
			newMove.direction = point.direction + 3;
			newMove.y += sign;
		}
	}

	//Ensures that direction stays within the range 0-5
	if (newMove.direction < 0) {
		newMove.direction += NUM_DIRECTIONS;
	} else if (newMove.direction > 5) {
		newMove.direction -= NUM_DIRECTIONS;
	}

	return newMove;
}

//gets the number of normal campuses a player has
int getCampuses (Game g, int player) {

    //player 1 is position 0 and player 3 is position 2
    return g->uni[player - 1].numCmps;
}

//get the number of IPS a player has
int getIPs (Game g, int player) {

    //g is a pointer to a struct
    return g->uni[player - 1].numIPs;
}

//gets the number of Publications that a player has
int getPublications (Game g, int player) {

    //g is a pointer to a struct
    //NB player 1 is position 0 and player 3 is position 2 in the array
    return g->uni[player - 1].numPubs;
}

//gets the number of students a player has of a specific discipline
int getStudents (Game g, int player, int discipline) {
 
   //g is a pointer to a struct hence using "->" not "."
    // 1<=player<=3 but array is from 0 to 2 so minus 1 to rectify
    return g->uni[player - 1].numStudents[discipline];
}

//get the exchange rate for retraining a student
int getExchangeRate (Game g, int player,
                     int disciplineFrom, int disciplineTo) {

    //double check that isLegal is working
    assert (disciplineFrom != STUDENT_THD);

    //paths to different retraining centres
    path training1, training2;
    int rate;

    if (disciplineFrom == STUDENT_MTV) {
        strcpy (training1, TV1);
        strcpy (training2, TV2);
    } else if (disciplineFrom == STUDENT_MMONEY) {
        strcpy (training1, MONEY1);
        strcpy (training2, MONEY2);
    } else if (disciplineFrom == STUDENT_BPS) {
        strcpy (training1, ENGINEER1);
        strcpy (training2, ENGINEER2);
    } else if (disciplineFrom == STUDENT_BQN) {
        strcpy (training1, SCIENCE1);
        strcpy (training2, SCIENCE2);
    } else {
        strcpy (training1, JOB1);
        strcpy (training2, JOB2);
    }

    //NB player = ARC code e.g. player 1/(UNI_A) = ARC_A
    if ((getCampus (g, training1) == player) || \
    	(getCampus (g, training2) == player)) {
        rate = 2;
    } else {
        rate = 3;
    }
    return rate;
}

//Stores region data in gameboard
void makeRegion (Game g, int discipline[]) {

	g->gameBoard.region[1][5][3] = 0;
	g->gameBoard.region[1][4][4] = 1;
	g->gameBoard.region[1][3][5] = 2;

	g->gameBoard.region[2][5][2] = 3;
	g->gameBoard.region[2][4][3] = 4;
	g->gameBoard.region[2][3][4] = 5;
	g->gameBoard.region[2][2][5] = 6;

	g->gameBoard.region[3][5][1] = 7;
	g->gameBoard.region[3][4][2] = 8;
	g->gameBoard.region[3][3][3] = 9;
	g->gameBoard.region[3][2][4] = 10;
	g->gameBoard.region[3][1][5] = 11;

	g->gameBoard.region[4][4][1] = 12;
	g->gameBoard.region[4][3][2] = 13;
	g->gameBoard.region[4][2][3] = 14;
	g->gameBoard.region[4][1][4] = 15;

	g->gameBoard.region[5][3][1] = 16;
	g->gameBoard.region[5][2][2] = 17;
	g->gameBoard.region[5][1][3] = 18;
}

//Returns a pointer to an array of three regions,
//Each with a student value supposedly.
int *checkCampRegion (Game g, path pathToVertex) {

	int xArray = 0;
	int yArray = 0;
	int zArray = 0;
	char *path = pathToVertex;
	int sign = 0;
	//When coordinate > 0, the others are -+1.
	int *region = malloc (3);
	//Since there are max 3 regions covering vertex
	coord movedPoint;

	movedPoint = pathMovement (path);
	assert (abs(movedPoint.x+movedPoint.y+movedPoint.z) == 2);

	xArray = movedPoint.x + 3;
	yArray = movedPoint.y + 3;
	zArray = movedPoint.z + 3;
	//Since it's 4, 5, 6 for a GO8

	//As logically sum of region coordinates = 0,
	//Thus to make it 0 you have to add the opposite to the
	//other coordinates. (opposite sign)
	if (movedPoint.x+movedPoint.y+movedPoint.z == -2) {
		sign = 1;
	} else {
		sign = -1;
	}

	//Now to determine if the board is at the edge
	//Since then it will only have two regions instead.
	//Returns the regionIDs, if regionID = 19, there is no region there
	if (abs (movedPoint.x) == 3) {
		region[0] = g->gameBoard.region[xArray+sign][yArray][zArray+sign];
		region[1] = g->gameBoard.region[xArray+sign][yArray+sign][zArray];
		region[2] = 19;
	} else if (abs (movedPoint.y) == 3) {
		region[0] = g->gameBoard.region[xArray+sign][yArray+sign][zArray];
		region[1] = g->gameBoard.region[xArray][yArray+sign][zArray+sign];
		region[2] = 19;
	} else if (abs (movedPoint.z) == 3) {
		region[0] = g->gameBoard.region[xArray][yArray+sign][zArray+sign];
		region[1] = g->gameBoard.region[xArray+sign][yArray][zArray+sign];
		region[2] = 19;
	} else if (abs (movedPoint.x*movedPoint.y*movedPoint.z) == 8) {
		if (movedPoint.x == movedPoint.y) {
			//Only one region in this few ones:
			region[0] = g->gameBoard.region[xArray+sign][yArray+sign][zArray];
			region[1] = 19;
			region[2] = 19;
		} else if (movedPoint.x == movedPoint.z) {
			region[0] = g->gameBoard.region[xArray+sign][yArray][zArray+sign];
			region[1] = 19;
			region[2] = 19;
		} else if (movedPoint.y == movedPoint.z) {
			region[0] = g->gameBoard.region[xArray][yArray+sign][zArray+sign];
			region[1] = 19;
			region[2] = 19;
		}
	} else {
		region[0] = g->gameBoard.region[xArray][yArray+sign][zArray+sign];
		region[1] = g->gameBoard.region[xArray+sign][yArray][zArray+sign];
		region[2] = g->gameBoard.region[xArray+sign][yArray+sign][zArray];
	}
	
	return region;
}

void addStudents (int regionID) {

    int i = 0;
    int x = 0;
    int y = 0;
    int z = 0;
    int curPlayer = UNI_A-1;
    int curVertex = g->gameBoard.campus[x][y][z];
    int *regionSurround = checkCampRegion (Game g, int x, int y, int z);
    while (x < 6) {
    	while (y < 6) {
    		while (z < 6) {
                curVertex = g->gameBoard.campus[x][y][z];
    			if (curVertex > 0) {
                    regionSurround = checkCampRegion (g, x, y, z);
                    i = 0;
                    while (i < 3 && regionSurround[i] == regionID) {
                        //mod curVertex by 4 to obtain value between 0-2
                        //since curVertex is 1-6
                        curPlayer = curVertex % 4;
                        if (curVertex >= 1 && curVertex <= 3) {
                            g->uni[curPlayer].numStudents[getDiscipline[regionID]]++;
                        } else if (curVertex >= 4 && curVertex <= 6) {
                            g->uni[curPlayer].numStudents[getDiscipline[regionID]] += 2;
                        }
                        i++;
                    }
                }
                z++;
    		}
            z = 0;
            y++;
    	}
        y = 0;
        x++;
    }
    free (regionSurround);
}
