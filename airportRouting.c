/*
*   FILE            :	cA4a.c
*   PROJECT         :	PROG 1347-1  Assignment #4a
*   PROGRAMMER      :	Geun Young Gil (6944920)
*   FIRST VERSION   :	Nov. 14. 2014
*   DESCRIPTION     :	The purpose of this program is to find the fastest routes from starting airport to ending airport.
*						Actually this program finds all fastest routes for each airport from starting airport.
*						But displayPath() function prints only the fastest routes between starting airport and ending airport.
*/

#include <stdio.h>
#include <stdlib.h>

//Constant
#define	kToronto	1
#define	kAtlanta	2
#define	kAustin		3
#define kSantaFe	4
#define	kDenver		5
#define kChicago	6
#define	kBuffalo	7
#define kMaxCityNum	7
#define kMaxCityNameLen	10

//Declaration of functions
int getNum(void);
void findFastestRoute(int startingTime, int startingCity, const int kFlightScheduleTable[][kMaxCityNum][18][2], struct layOverAirport* pFastestPathToAirport[], const char kCityTimeZone[]);
void freeLinkedNodes(struct layOverAirport* pNode);
struct layOverAirport* copyLinkedNodes(struct layOverAirport* pOriginalLinkedNodes);
struct layOverAirport* findLastNode(struct layOverAirport* pNode);
struct layOverAirport* makeStructLayOverAirport(int arrivalTimeInPresentAirport, int departureAirport, int nextAirport, const int kFlightScheduleTable[][kMaxCityNum][18][2], struct layOverAirport* pFastestPathToAirport[], const char kCityTimeZone[]);
int convertTimeToArrivalTimeZone(int departureAirport, int departureTime, int flyingTime, int arrivalAirport, const char kCityTimeZone[]);
int getWaitTimeInAirport(int arrivalTimeInPresentAirport, int presentAirport, int nextAirport, const int kFlightScheduleTable[][kMaxCityNum][18][2]);
int findNextSchedule(int arrivalTimeInPresentAirport, int presentAirport, int nextAirport, const int kFlightScheduleTable[][kMaxCityNum][18][2]);
int convertBtwHrMinAndMin(char presentTimeFormat, int time);
void printTimeZone(const char timeZone);
void printTime(int time);
void displayPath(int endingAirport, int startingTime, struct layOverAirport* pFastestPathToAirport[], const char kAirportName[][kMaxCityNameLen], const char kCityTimeZone[]);


//Struct type declaration
/*
*	This program have two kinds of time formats.
*	1st: HHMM	(HH:MM, e.g. 1728 means 17:28(17 hours and 28 minutes))
*	2nd: MMMM	(MMMM, e.g. 980 means 980 minutes, only display in minutes)
*/
typedef struct layOverAirport{
	int departureAirport;
	int departureTime;							//The time zone of departure airport.(HHMM)
	int flyingTime;								//HHMM
	int arrivalAirport;
	int arrvalTime;								//The time zone of arrval airport. (HHMM)
	int totalTravelTime;						//MMMM
	struct layOverAirport* pNextAirport;		//It is for linked nodes.
};


int main()
{
	//[departing city][destination of departing city][the time schedule to each destination][departure time/flying time]
	const int kFlightScheduleTable[kMaxCityNum][kMaxCityNum][18][2] = { {
		    //Toronto airport schedule
		    { { kAtlanta, 0 }, { 625, 220 }, { 910, 450 }, { 1230, 415 }, { 1610, 610 }, { 2000, 215 }, { -1, -1 } },
			{ { kDenver,  0 }, { 730, 335 }, { 1500, 600}, {-1,-1} },
			{ { kChicago, 0 }, { 640, 120 }, { 740, 135 }, { 840, 135 }, { 940, 135 }, { 1040, 135 }, { 1140, 135 }, { 1240, 135 }, { 1340, 135 },
			{ 1440, 135 }, { 1530, 145 }, { 1630, 145 }, { 1730, 145 }, { 1830, 145 }, { 1930, 145 }, { 2100, 130 }, { 2200, 115 }, { -1, -1 } },
			{{-1, -1} }
			}, {
			//Atlanta airport schedule
			{ { kToronto, 0 }, { 710, 210 }, { 1030, 410 }, { 1500, 350 }, { 1710, 610 }, { 2100, 220 }, { -1, -1 } },
			{ { kAustin, 0 }, { 900, 210 }, { 1530, 250 }, { 2000, 230 }, { -1, -1 } },
			{ { kDenver, 0 }, { 600, 300 }, { 1320, 500 }, { 1710, 250 }, { -1, -1 } },
			{ { kChicago, 0 }, { 650, 210 }, { 750, 300 }, { 850, 300 }, { 950, 300 }, { 1050, 300 }, { 1150, 300 }, { 1250, 300 }, { 1350, 300 },
			{ 1450, 300 }, { 1550, 230 }, { 1650, 230 }, { 1750, 230 }, { 1850, 230 }, { 1950, 230 }, { 2030, 210 }, {-1,-1} },
			{ { -1, -1 } }
			}, {
			//Austin airport schedule
			{ { kAtlanta, 0 }, { 910, 220 }, { 1500, 220 }, { 2130, 230 }, { -1, -1 } },
			{ { kDenver, 0 }, { 1030, 220 }, { 1820, 220 }, { -1, -1 } },
			{ { kSantaFe, 0 }, { 1700, 55 }, { -1, -1 } },
			{ { -1, -1 } }
			}, {
			//Santa Fe airport schedule
			{ { kAustin, 0 }, { 1500, 45 }, { -1, -1 } },
			{ { -1, -1 } }
			}, {
			//Denver airport schedule
			{ { kToronto, 0 }, { 630, 410 }, { 1030, 520 }, { 1400, 500 }, { -1, -1 } },
			{ { kAtlanta, 0 }, { 600, 310 }, { 1300, 320 }, { 1500, 350 }, { -1, -1 } },
			{ { kAustin, 0 }, { 1200, 200 }, { 1500, 220 }, { -1, -1 } },
			{ { kChicago, 0 }, { 700, 220 }, { 800, 250 }, { 900, 250 }, { 1000, 250 }, { 1100, 250 }, { 1200, 250 }, { 1300, 250 }, { 1400, 250 },
			{ 1500, 250 }, { 1600, 250 }, { 1830, 240 }, { -1, -1 } },
			{ { -1, -1 } }
			}, {
			//Chicago airport schedule
			{ { kToronto, 0 }, { 740, 110 }, { 910, 230 }, { 1010, 230 }, { 1110, 230 }, { 1210, 230 }, { 1310, 230 }, { 1410, 230 }, { 1510, 230 },
			{ 1610, 230 }, { 1710, 230 }, { 1910, 200 }, { 2110, 210 }, { -1, -1 } },
			{ { kAtlanta, 0 }, { 650, 210 }, { 800, 240 }, { 900, 240 }, { 1000, 240 }, { 1100, 240 }, { 1200, 240 }, { 1300, 240 }, { 1400, 240 },
			{ 1500, 240 }, { 1600, 240 }, { 1700, 240 }, { 1800, 240 }, { 1900, 240 }, { 2000, 240 }, { 2150, 300 }, { -1, -1 } },
			{ { kDenver, 0 }, { 900, 210 }, { 1130, 220 }, { 1330, 220 }, { 1530, 220 }, { 1730, 220 }, { 2100, 250 }, { -1, -1 } },
			{ { kBuffalo, 0 }, { 1100, 200 }, { 1310, 150 }, { 1500, 230 }, { 1800, 210 }, { -1, -1 } },
			{ { -1, -1 } }
			}, {
			//Buffalo airport schedule
			{ { kChicago, 0 }, { 940, 140 }, { 1110, 150 }, { 1740, 240 }, { 2010, 220 }, { -1, -1 } },
			{ { -1, -1 } }
			} };
	const char kAirportName[kMaxCityNum][kMaxCityNameLen] = { "Toronto","Atlanta","Austin","Santa Fe","Denver","Chicago","Buffalo" };
	const char kCityTimeZone[kMaxCityNum] = { 'E','E','C','M','M','C','E' };		//E:EST, C:CST, M:MST
	struct layOverAirport *pFastestPathToAirport[kMaxCityNum] = {NULL};

	while (1)																		//Interface
	{
		int startingCity =	0;
		int endingCity =	0;
		int startingTime =	0;
		int exit = 1;

		printf("\n This program will find the fastest route from starting city to ending city.\n\n");
		printf(" If you want to exit this program, please push '0'.\n");
		printf(" Or if you keep going this, push any keys.\n");
		printf(" >> ");
		exit = getNum();

		if (exit == 0)																//Exit program if user enter 0
		{
			break;
		}

		for (int i = 0; i < kMaxCityNum; i++)
		{
			pFastestPathToAirport[i] = NULL;
		}

		system("cls");																//Clean window
		printf("\n Toronto: 1, Atlanta: 2, Austin: 3, Santa Fe: 4, \n");
		printf(" Denver: 5, Chicago: 6, Buffalo: 7 \n");


		printf("\n Starting city(1 to 7): ");										//Enter starting city
		startingCity = getNum();

		if ((startingCity < 1) || (startingCity>7))
		{
			printf("\n You entered a wrong number!!\n");
			continue;
		}


		printf("\n Ending city(1 to 7): ");											//Enter ending city
		endingCity = getNum();

		if ((endingCity < 1) || (endingCity > 7))
		{
			printf("\n You entered a wrong number!!\n");
			continue;
		}
		else if (startingCity == endingCity)
		{
			printf("\n You don't allow to enter the same city with starting city. \n");
			continue;
		}


		printf("\n Starting Time(e.g. 412, 2359): ");								//Enter starting time	
		startingTime = getNum();

		if (((startingTime / 100) < 0) || ((startingTime / 100) > 23))
		{
			printf("\n You inputted wrong hours.\n");
			continue;
		}
		else if (((startingTime%100)>59)||((startingTime%100)<0))
		{
			printf("\n You inputed wrong minutes.\n");
			continue;
		}

		//Find all fastest routes for each airport from starting
		findFastestRoute(startingTime, startingCity, kFlightScheduleTable, pFastestPathToAirport, kCityTimeZone);
		//Print the fastest routes from starting airport to ending airport 
		displayPath(endingCity, startingTime, pFastestPathToAirport, kAirportName, kCityTimeZone);
		break;
	}

	return 0;
}


/*
*   FUNCTION    :	findFastestRoute
*
*   DISCRIPTION :	Find the fastest routes for each airport from starting airport.
*
*   PARAMETERS  :	int						startingTime				:	The starting time (HHMM == HH:MM)
*					int						startingCity				:	The starting city
*					const int*				kFlightScheduleTable		:	Flight schedule table inculuding all airports
*					struct layOverAirport*	pFastestPathToAirport[]		:	Array to store fastest path for each airport.
*					char*					kCityTimeZone				:	Time zones of all city
*
*   RETURNS     :   void
*/
void findFastestRoute(int startingTime, int startingCity, const int kFlightScheduleTable[][kMaxCityNum][18][2],struct layOverAirport* pFastestPathToAirport[], const char kCityTimeZone[])
{
	int visitedAirport[kMaxCityNum] = { 0 };																	//Unvisited:0, visited:-1, airports plan to visit in next turn: 1
	int i = 0;
	int timeToArrival = 0;																						//The time that spend from departure to arrival of next airport
	struct layOverAirport* pTmp = NULL;
	struct layOverAirport* pTmp1 = NULL;

	visitedAirport[startingCity - 1] = 1;																		//Starting airport is visited.

	while (1)
	{
		for (i = 0; i < kMaxCityNum; i++)
		{
			if (visitedAirport[i] == 1)																			//Find next airport 
			{
				break;
			}
		}

		if (i > 6)																								//Exit this function if all routes have searched
		{
			break;
		}

		int j = 0;
		while (kFlightScheduleTable[i][j][0][0] != -1)															//find all destinations in present airport
		{
			pTmp = NULL;
			pTmp1 = NULL;
			timeToArrival = 0;
			int nextAirport = kFlightScheduleTable[i][j][0][0];

			if (nextAirport == startingCity)																	//If next airport is the same as starting airport,just skip.
			{
				j++;
				continue;
			}

			else if (i == (startingCity - 1))																	//If it stay in starting airport, just make the node for next airport 
			{                                                                                                   //Because it the is first node.
				pFastestPathToAirport[nextAirport - 1] = makeStructLayOverAirport(startingTime, startingCity, nextAirport, kFlightScheduleTable, pFastestPathToAirport, kCityTimeZone);
				visitedAirport[nextAirport - 1] = 1;															//Change the airport status to visit next turn.
			}

			else if (visitedAirport[nextAirport - 1] == 0)														//Unvisited airport
			{
				pFastestPathToAirport[nextAirport - 1] = copyLinkedNodes(pFastestPathToAirport[i]);				//Copy linked nodes of departure airport
				pTmp = findLastNode(pFastestPathToAirport[nextAirport - 1]);									//Find last node of departure airport's route
																												//Link the new node that made for next airport to last node
				pTmp->pNextAirport = makeStructLayOverAirport(pTmp->arrvalTime, i + 1, nextAirport, kFlightScheduleTable, pFastestPathToAirport, kCityTimeZone);
				visitedAirport[nextAirport - 1] = 1;
			}

			else if ((visitedAirport[nextAirport - 1] == 1) || (visitedAirport[nextAirport - 1] == -1))			//Visited aiport except starting airport
			{
				pTmp = findLastNode(pFastestPathToAirport[nextAirport - 1]);									//Last node of arrival airport
				pTmp1 = findLastNode(pFastestPathToAirport[i]);													//Last node of departure airport

				int k = 1;
				while (kFlightScheduleTable[i][j][k][0] != -1)													//Find flight for next airport
				{
					if (kFlightScheduleTable[i][j][k][0] == findNextSchedule(pTmp1->arrvalTime, i + 1, nextAirport, kFlightScheduleTable))
					{
						break;
					}
					k++;
				}

				timeToArrival = getWaitTimeInAirport(pTmp1->arrvalTime, i + 1, nextAirport, kFlightScheduleTable)//Total travel time for new route.
								+ convertBtwHrMinAndMin('H', kFlightScheduleTable[i][j][k][1])
								+ pTmp1->totalTravelTime;

				if (timeToArrival < pTmp->totalTravelTime)														//If it find faster route,
				{
					////////////////////////////////////////
					pTmp = copyLinkedNodes(pFastestPathToAirport[i]);											//Make new linked nodes 
					pTmp1 = findLastNode(pTmp);																	//Go to last node of new linked nodes
					pTmp1->pNextAirport = makeStructLayOverAirport(pTmp1->arrvalTime, pTmp1->arrivalAirport, nextAirport, kFlightScheduleTable, pFastestPathToAirport, kCityTimeZone);

					freeLinkedNodes(pFastestPathToAirport[nextAirport - 1]);									//Deletes nodes of previous route
					pFastestPathToAirport[nextAirport - 1] = pTmp;												//Register new route
					visitedAirport[nextAirport - 1] = 1;														//Airport to visit next time
				}

			}

			else
			{
				printf("/n!!error: function findFastestRoute()!!\n");
				break;
			}

			j++;																								//Find another airport to go from present airport	
		}

		visitedAirport[i] = -1;																					//Change to visited airport
	}
}

/*
*   FUNCTION    :	freeLinkedNodes
*
*   DISCRIPTION :	Free linked nodes
*
*   PARAMETERS  :	struct layOverAirport*	pNode	:	The first node of linked nodes
*
*   RETURNS     :   void
*/
void freeLinkedNodes(struct layOverAirport* pNode)
{
	struct layOverAirport* pTmp = NULL;

	while (pNode != NULL)
	{
		pTmp = pNode;
		pNode = pTmp->pNextAirport;					//Get next node pointer
		free(pTmp);									//Free present node
	}
}

/*
*   FUNCTION    :	copyLinkedNodes
*
*   DISCRIPTION :	Copy linked nodes from original linked nodes.
*					Recursive function.
*
*   PARAMETERS  :	struct layOverAirport*	pOriginalLinkedNodes	:	Array to store fastest path for each airport.
*
*   RETURNS     :   struct layOverAirport*							:	Pointer of the first node of copied nodes
*/
struct layOverAirport* copyLinkedNodes(struct layOverAirport* pOriginalLinkedNodes)
{
	struct layOverAirport* pTmp				= NULL;
	struct layOverAirport* pNewLinkedNode	= NULL;

	pTmp = pOriginalLinkedNodes;

	if (pTmp == NULL)
	{
		return NULL;
	}
	
	
	if ((pNewLinkedNode = malloc(sizeof(struct layOverAirport))) == NULL)			//Make new node
	{
		printf("\n!!error: function copyLinkedNodes(). Out of Memory!!\n");
		return NULL;
	}

	//Copy to new node
	pNewLinkedNode->arrivalAirport		= pTmp->arrivalAirport;
	pNewLinkedNode->arrvalTime			= pTmp->arrvalTime;
	pNewLinkedNode->departureAirport	= pTmp->departureAirport;
	pNewLinkedNode->departureTime		= pTmp->departureTime;
	pNewLinkedNode->flyingTime			= pTmp->flyingTime;
	pNewLinkedNode->totalTravelTime		= pTmp->totalTravelTime;
	pNewLinkedNode->pNextAirport		= copyLinkedNodes(pTmp->pNextAirport);		//Recursive function for link

	return pNewLinkedNode;
}

/*
*   FUNCTION    :	findLastNode
*
*   DISCRIPTION :	Find the last node in linked nodes
*
*   PARAMETERS  :	struct layOverAirport*	pNode	:	Pointer of linked nodes
*
*   RETURNS     :   struct layOverAirport*			:	Return last node pointer.
*														If there are no link, return NULL.
*/
struct layOverAirport* findLastNode(struct layOverAirport* pNode)
{
	struct layOverAirport* pTmp = NULL;

	if (pNode == NULL)
	{
		return NULL;
	}

	pTmp = pNode;

	while (pTmp->pNextAirport != NULL)			//Loop to find the last node
	{
		pTmp = pTmp->pNextAirport;
	}

	return pTmp;
}


/*
*   FUNCTION    :	makeStructLayOverAirport
*
*   DISCRIPTION :	make struct layOverAirport for arrival airport node.
*
*   PARAMETERS  :	int						arrivalTimeInPresentAirport	:	Arrival time in present airport (HHMM == HH:MM)
*					int						departureAirport			:	Airport to depart
*					int						nextAirport					:	Next airport
*					const int*				kFlightScheduleTable		:	Flight schedule table inculuding all airports
*					struct layOverAirport*	pFastestPathToAirport[]		:	Array to store fastest path for each airport.	
*					char*					kCityTimeZone				:	Time zones of all city
*
*   RETURNS     :   struct layOverAirport*								:	Return address of struct, pNextAirport has NULL.
*/
struct layOverAirport* makeStructLayOverAirport(int arrivalTimeInPresentAirport, int departureAirport, int nextAirport, const int kFlightScheduleTable[][kMaxCityNum][18][2], struct layOverAirport* pFastestPathToAirport[], const char kCityTimeZone[])
{
	struct layOverAirport* pTemp = NULL;
	struct layOverAirport* pTemp2 = NULL;


	if ((pTemp = malloc(sizeof(struct layOverAirport))) == NULL)					//Make new node
	{
		printf("\n Out of Memory!!\n");
		return NULL;
	}

	pTemp->departureAirport =	departureAirport;
	pTemp->departureTime	=	findNextSchedule(arrivalTimeInPresentAirport, departureAirport, nextAirport, kFlightScheduleTable);		
	pTemp->arrivalAirport	=	nextAirport;

	int i = 0;
	while (i < kMaxCityNum)														
	{
		if (kFlightScheduleTable[departureAirport - 1][i][0][0] == nextAirport)		//Find the schedule for next airport
		{
			break;
		}
		i++;
	}

	int j = 1;
	while (kFlightScheduleTable[departureAirport - 1][i][j][0] != -1)				//Get flying time to next airport
	{
		if (pTemp->departureTime == kFlightScheduleTable[departureAirport - 1][i][j][0])
		{
			pTemp->flyingTime = kFlightScheduleTable[departureAirport - 1][i][j][1];
			break;
		}
		j++;
	}

	pTemp->arrvalTime	=	convertTimeToArrivalTimeZone(departureAirport, pTemp->departureTime, pTemp->flyingTime, nextAirport, kCityTimeZone);
	pTemp->pNextAirport =	NULL;

	//Total travel calculation
	if (pFastestPathToAirport[departureAirport - 1] == NULL)						//If present airport is starting airport, it don't need to calculate previous travel time.
	{
		pTemp->totalTravelTime = getWaitTimeInAirport(arrivalTimeInPresentAirport, departureAirport, nextAirport, kFlightScheduleTable)
								 + convertBtwHrMinAndMin('H', pTemp->flyingTime);
		return pTemp;
	}

	pTemp2 = findLastNode(pFastestPathToAirport[departureAirport - 1]);
	pTemp->totalTravelTime = pTemp2->totalTravelTime
							 + getWaitTimeInAirport(arrivalTimeInPresentAirport, departureAirport, nextAirport, kFlightScheduleTable)
							 + convertBtwHrMinAndMin('H', pTemp->flyingTime);

	return pTemp;
}  


/*
*   FUNCTION    :	convertTimeToArrivalTimeZone
*
*   DISCRIPTION :	Convert the time of time zone in arrival city
*
*   PARAMETERS  :	int		departureAirport	:	Departure airport
*					int		departureTime		:	Departure time from airport (HHMM == HH:MM)
*					int		flyingTime			:	Flying time to next airport (HHMM)
*					int		arrivalAirport		:	Next airport
*					char*	kCityTimeZone		:	Time zones of all city
*
*   RETURNS     :   int							:	The time of time zone in arrival city (HHMM)
*													-1 means that there is no match with time zones
*/
int convertTimeToArrivalTimeZone(int departureAirport, int departureTime, int flyingTime, int arrivalAirport, const char kCityTimeZone[])
{
	const int	kEst		= 60 * 5;								//Time gap from UTC
	const int	kCst		= 60 * 6;
	const int	kMst		= 60 * 7;
	int			arrivalTime = 0;				

	arrivalTime = convertBtwHrMinAndMin('H', departureTime) + convertBtwHrMinAndMin('H', flyingTime);
	
	if (kCityTimeZone[departureAirport-1] == 'E')					//Convert the time to UTC from the time of departure city
	{
		arrivalTime += kEst;
	}
	else if (kCityTimeZone[departureAirport - 1] == 'C')
	{
		arrivalTime += kCst;
	}
	else if (kCityTimeZone[departureAirport - 1] == 'M')
	{
		arrivalTime += kMst;
	}
	else
	{
		printf("\n !!error: function convertTimeToArrivalTimeZone()!!\n");
		return -1;
	}

	
	if (kCityTimeZone[arrivalAirport - 1] == 'E')					//Convert the time in time zone of arrival city from the UTC
	{
		arrivalTime -= kEst;
	}
	else if (kCityTimeZone[arrivalAirport - 1] == 'C')
	{
		arrivalTime -= kCst;
	}
	else if (kCityTimeZone[arrivalAirport - 1] == 'M')
	{
		arrivalTime -= kMst;
	}
	else
	{
		printf("\n !!error: function convertTimeToArrivalTimeZone()!!\n");
		return -1;
	}

	if (arrivalTime < 0)											//If the time in arrival city is still a day ago than departure city.
	{
		arrivalTime += 24 * 60; 
	}

	return convertBtwHrMinAndMin('M', arrivalTime);					//Change time format to HHMM
}

                             
/*
*   FUNCTION    :	getWaitTimeInAirport
*
*   DISCRIPTION :	Get waiting time in present airport for next flight 
*
*   PARAMETERS  :	int			arrivalTimeInPresentAirport	:	The time that arrived at the present airport
*					int			presentAirport				:	Present airport
*					int			nextAirport					:	The airport to go next
*					const int*	kFlightScheduleTable		:	Flight schedule table inculuding all airports
*
*   RETURNS     :   int										:	The minutes that spend for next flight in the airport (time format: MM (e.g. 90 == 1:30))
*/
int getWaitTimeInAirport(int arrivalTimeInPresentAirport, int presentAirport, int nextAirport, const int kFlightScheduleTable[][kMaxCityNum][18][2])
{
	int deptTime = 0;

	deptTime = findNextSchedule(arrivalTimeInPresentAirport, presentAirport, nextAirport, kFlightScheduleTable);

if (deptTime >= arrivalTimeInPresentAirport)				//Today flight
{
	return convertBtwHrMinAndMin('H', deptTime) - convertBtwHrMinAndMin('H', arrivalTimeInPresentAirport);
}
else														//Tommorow flight
{
	return ((24 * 60) - convertBtwHrMinAndMin('H', arrivalTimeInPresentAirport)) + convertBtwHrMinAndMin('H', deptTime);
}

}


/*
*   FUNCTION    :	findNextSchedule
*
*   DISCRIPTION :	Find departure time for next airport in present airport
*
*   PARAMETERS  :	int			arrivalTimeInPresentAirport	:	The time that arrived at the present airport
*					int			presentAirport				:	Present airport
*					int			nextAirport					:	The airport to go next
*					const int*	kFlightScheduleTable		:	Flight schedule table inculuding all airports
*
*   RETURNS     :   int			:	The departure time to next airport (time format: HHMM (e.g. 1730 == 17:30))
*/
int findNextSchedule(int arrivalTimeInPresentAirport, int presentAirport, int nextAirport, const int kFlightScheduleTable[][kMaxCityNum][18][2])
{
	int i =0, j = 1;

	while (i < kMaxCityNum)
	{
		if (kFlightScheduleTable[presentAirport - 1][i][0][0] == nextAirport)		//Find the schedule for next airport
		{
			break;
		}
		i++;
	}

	while (kFlightScheduleTable[presentAirport-1][i][j][0] != -1)					//Loop until end of schedule.
	{
		if (kFlightScheduleTable[presentAirport-1][i][j][0] >= arrivalTimeInPresentAirport)
		{
			return kFlightScheduleTable[presentAirport-1][i][j][0];					//Return the time of next flight just after arrival
		}
		j++;
	}

	return kFlightScheduleTable[presentAirport-1][i][1][0];							//If it can't find present day, return the time of first flight. 
}



/*
*   FUNCTION    :	convertBtwHrMinAndMin
*
*   DISCRIPTION :	Convert between Hours Minutes format and Minutes. (e.g. 140(1:40) <-> 100 minutes)
*
*   PARAMETERS  :	char	presentTimeFormat	:	Entered time format, 'H' Hours and Minutes (HHMM) e.g. 1234 (12:34)
*																		 'M' Minutes e.g. 70 mins (1:10)
*					int		time				:	The time corresponded with presentTimeFormat
*
*   RETURNS     :   int							:	If presentTimeFormat is 'H', returns 0 ~ the end positive number of integer
*														  "        "        'M', retruns 0(00:00) ~ 2359(23:59)
*													But if presentTimeFormat has wrong input, return -1
*/
int convertBtwHrMinAndMin(char presentTimeFormat, int time)
{
	if (presentTimeFormat == 'H')							//Convert from Hours Minutes format to only Minutes format (e.g. 140(1:40) -> 100)
	{
		return ((time / 100) * 60) + (time % 100);
	}

	else if (presentTimeFormat == 'M')						//Convert from Minutes to Hours Minutes (e.g. 200 -> 320(3:20)) 	
	{
		if (time >= (24 * 60))                              //If minutes are over 24hours, substract 24hours.
		{
			time -= 24 * 60;
		}
		return ((time / 60) * 100) + (time % 60);
	}

	return -1;												//Return -1 if time format is not matched
}

/*
*   FUNCTION    :	printTimeZone
*
*   DISCRIPTION :	Print time zone 
*
*   PARAMETERS  :	const char			timeZone	:	'E'== EST / 'C' == CST / 'M' == MST
*
*   RETURNS     :   void			
*/
void printTimeZone(const char timeZone)
{
	if (timeZone == 'E')
	{
		printf(" EST");
	}

	else if (timeZone == 'C')
	{
		printf(" CST");
	}

	else if (timeZone == 'M')
	{
		printf(" MST");
	}
}

/*
*   FUNCTION    :	printTime
*
*   DISCRIPTION :	Print time having a.m. or f.m. formats
*
*   PARAMETERS  :	int						time	:	time format HHMM (==HH:MM)
*
*   RETURNS     :   void
*/
void printTime(int time)
{
	int hours = time / 100;
	int minutes = time % 100;

	if (hours < 12)							//a.m.
	{
		printf("%d:", hours);
		if (minutes > 9)					//10 ~ 59 minutes
		{
			printf("%d", minutes);
		}
		else								//Less than 10 minutes
		{
			printf("0%d", minutes);
		}
		printf(" a.m.");
	}

	else									//p.m.
	{
		if (hours > 12)
		{
			hours -= 12;
		}
		printf("%d:", hours);
		if (minutes > 9)					//10 ~ 59 minutes
		{
			printf("%d", minutes);
		}
		else								//Less than 10 minutes
		{
			printf("0%d", minutes);
		}
		printf(" p.m.");
	}
}

/*
*   FUNCTION    :	displayPath
*
*   DISCRIPTION :	Print the fastest path from starting airport to ending airport
*
*   PARAMETERS  :	int						endingAirport						:	Ending airport
*					int						startingTime						:	Starting time
*					struct layOverAirport*	pFastestPathToAirport[]				:	The array having the fastest routes for each airport
*					const char				kAirportName[][kMaxCityNameLen]		:	The array having airport names
*					const char				kCityTimeZone[]						:	Time zone for each cities	
*
*   RETURNS     :   void
*/
void displayPath(int endingAirport,int startingTime, struct layOverAirport* pFastestPathToAirport[],const char kAirportName[][kMaxCityNameLen],const char kCityTimeZone[])
{
	struct layOverAirport* pTmp = pFastestPathToAirport[endingAirport - 1];
	struct layOverAirport* pPreviousNode = NULL;

	printf("\n\n The result of fastest route.\n\n");
	printf(" Flyng from %s to %s.", kAirportName[pTmp->departureAirport - 1], kAirportName[endingAirport - 1]);
	printf("\n Starting from %s at ", kAirportName[pTmp->departureAirport - 1]);
	printTime(startingTime);
	printTimeZone(kCityTimeZone[pTmp->departureAirport - 1]);

	while (pTmp != NULL)														//Loop until the last node of linked nodes
	{
		printf("\n Leaving %s at ",kAirportName[pTmp->departureAirport - 1]);
		printTime(pTmp->departureTime);
		printTimeZone(kCityTimeZone[pTmp->departureAirport - 1]);

		if ((pPreviousNode == NULL) && (pTmp->departureTime < startingTime))	//Starting airport
		{
			printf(" next day");
		}

		else if (pPreviousNode != NULL)											//If there is no schedule for present day
		{
			if (pTmp->departureTime < pPreviousNode->arrvalTime)
			{
				printf(" next day");
			}

		}

		printf(" for %s.", kAirportName[pTmp->arrivalAirport - 1]);
		printf("\n Arriving in %s at ", kAirportName[pTmp->arrivalAirport - 1]);
		printTime(pTmp->arrvalTime);
		printTimeZone(kCityTimeZone[pTmp->arrivalAirport - 1]);

		pPreviousNode = pTmp;
		pTmp = pTmp->pNextAirport;												//Next node
	}

	printf("\n Total travel time: ");
	printf("%d:", pPreviousNode->totalTravelTime / 60);

	if ((pPreviousNode->totalTravelTime % 60) > 9)								//10 ~ 59 minutes
	{
		printf("%d\n", pPreviousNode->totalTravelTime % 60);
	}
	else																		//Less than 10 minutes
	{
		printf("0%d\n", pPreviousNode->totalTravelTime % 60);
	}
}


/*
*   FUNCTION    :   getNum
*
*   DISCRIPTION :   This function get the integer number from user.
*
*   PARAMETERS  :   void
*
*   RETURNS     :   int :   inputted number
*/
int getNum(void)
{
	/* the array is 121 bytes in size; we'll see in a later lecture how we can improve this code */
	char record[121] = { 0 }; /* record stores the string */
	int number = 0;
	/* NOTE to student: indent and brace this function consistent with your others */
	/* use fgets() to get a string from the keyboard */
	fgets(record, 121, stdin);
	/* extract the number from the string; sscanf() returns a number
	* corresponding with the number of items it found in the string */
	if (sscanf_s(record, "%d", &number) != 1)
	{
		/* if the user did not enter a number recognizable by
		* the system, set number to -1 */
		number = -1;
	}
	return number;
}