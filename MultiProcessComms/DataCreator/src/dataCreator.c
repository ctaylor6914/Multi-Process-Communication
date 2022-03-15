/*
*	FILE			: dataCreator.c
*	PROJECT			: SP_A03
*	PROGRAMMER		: Mita Das & Colby Taylor
*	FIRST VERSION	: 2022-02-21
*	DESCRIPTION		: This is the main entry point for data creator operation.
*					  In this program, data creator sends msg to data reader.
*					  It writes to respective log file.
*/

#include "../inc/dataCreator.h"

int main(int argc, char* argv[])
{
	key_t message_key;
	pid_t myPID;
	int mid;
	int status;
	int	sleepTime;
	int	exitClient = FALSE;
	int	rc;
	char timeString[20] = {0};

	DCInfo msg;
	time_t t;

	// file pointer for writing
	FILE *fptr;

	// this client needs to know their own PID
	myPID = getpid();

	// open the same message key/queue as server
	message_key = ftok ("/tmp", 16535);
	if (message_key == -1) 
	{ 
	  	printf ("(DC) Cannot allocate key\n");
	  	return 1;
	}

	while ((mid = msgget (message_key, 0)) == -1) 
	{
		// not available yet ... check in 10 seconds
		sleep (10);
	}

	status = 0;
	while (exitClient == FALSE)
	{
		fptr = fopen("/tmp/dataCreator.log", "a");

		// get the local time in server
		time_t t = time(NULL);
  		struct tm tm = *localtime(&t);

		// update the message to be sent to client
		msg.dcProcessID = myPID;
		msg.status = status;
		msg.lastTimeHeardFrom = mktime(&tm);

		// parsing time to string
		strftime(timeString, sizeof(timeString), "%Y-%m-%e %H:%M:%S", &tm);

		// write to the file
		// printf("[%s] : DC [%d] - MSG SENT - Status %d (%s)\n", timeString, msg.dcProcessID, msg.status, getStatusMessage(status));
		fprintf(fptr, "[%s] : DC [%d] - MSG SENT - Status %d (%s)\n", timeString, msg.dcProcessID, msg.status, getStatusMessage(status));

		// Send the message
		rc = msgsnd (mid, (void *)&msg, sizeof (DCInfo), 0);
		if (rc == -1)
		{
			//return 4;
		}

		// Exit client if status is 6
		if (status == 6)
		{
			exitClient = TRUE;
		}

		// Generate random status
		srand((unsigned) time(&t));
		status = rand() % 7;

		// Getting the sleep time
		srand((unsigned) time(&t));
		sleepTime = rand() % 31 + 10;
		if (sleepTime > 30)
		{
			sleepTime = 30;
		}

		fclose(fptr);

		sleep(sleepTime);
	}

	return 0;
}

