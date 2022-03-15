/*
*	FILE			: common.h
*	PROJECT			: Multi Process Communication
*	PROGRAMMER		: Colby Taylor
*	FIRST VERSION		: 2022-02-21
*	DESCRIPTION		: This include file contains the common function prototypes for the application 
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

#pragma warning(disable:4996)
#pragma warning(disable:6262)

#define MAX_DC_ROLES 10

typedef struct tagDCInfo
{
	pid_t dcProcessID;
	int status;
	time_t lastTimeHeardFrom;
} DCInfo;

typedef struct tagMasterList
{
	int msgQueueID;
	int numberOfDCs;
	DCInfo dc[MAX_DC_ROLES];
} MasterList;

#define TRUE  1
#define FALSE 0

char* getStatusMessage(int status)
{
	char* msg = NULL;

	switch(status)
	{
		case 0:
			msg = "Everything is OKAY";
			break;
		case 1:
			msg = "Hydraulic Pressure Failure";
			break;
		case 2:
			msg = "Safety Button Failure";
			break;
		case 3:
			msg = "No Raw Material in the Process";
			break;
		case 4:
			msg = "Operating Temperature Out of Range";
			break;
		case 5:
			msg = "Operator Error";
			break;
		case 6:
			msg = "Machine is Off-line";
			break;
		default:
			msg = "Status not found";
			break;
	}

	return msg;
}


