/*
*	FILE			: dataReader.c
*	PROJECT			: SP_A03
*	PROGRAMMER		: Mita Das & Colby Taylor
*	FIRST VERSION		: 2022-02-21
*	DESCRIPTION		: This is the main entry point for data reader operation.
*					  In this function, DR waitis for 15 secs before entering into the
*					  loop for reading msg from msgqueue. It will stop is it does not
*					  find any message. I will wait for 1.5 secs before msgqueue again.
*					  It writes to respective log file.
*/

#include "../inc/dataReader.h"

int main(int argc, char* argv[])
{
	key_t message_key;
	int mid = -1;
	int rc;
	int exitServer = FALSE;
	int	dcNums = 0;
	int shmid = 0;
	key_t shmem_key;
	time_t localTimeInServer;
	char timeString[20] = {0};

	MasterList	*p;
	MasterList	masterList;
	DCInfo 		msg;

	// file pointer for writing
	FILE *fptr;

	// create the key
	message_key = ftok ("/tmp", 16535);
	if (message_key == -1) 
	{ 
		printf ("(DR) Cannot allocate key\n");
		return 1;
	}

	// if message queue exists, use it, or allocate a new one
	if ((mid = msgget (message_key, 0)) == -1) 
	{
		// nope, let's create one (user/group read/write perms)
		mid = msgget (message_key, IPC_CREAT | 0660);
		if (mid == -1) 
		{
			printf ("(DR) Cannot allocate a new queue!\n");
			return 2;
		}
	}

	// create the shared memory
	if ((shmid = shmget (message_key, sizeof (MasterList), 0)) == -1) 
	{
		shmid = shmget (message_key, sizeof (MasterList), IPC_CREAT | 0660);
		if(shmid ==-1)
		{
			printf("(DR) Cannot allocate new memory");
			return 3;
		}
	}

	// now attach to our shared memory
	p = (MasterList *)shmat (shmid, NULL, 0);
	if (p == NULL) 
	{
	  	printf ("(DR) Cannot attach to Shared-Memory!\n");
	  	return 4;
	}

	// setting the message id and num of clients in the masterlist, updating client details with default values
	p->msgQueueID = mid;
	p->numberOfDCs = dcNums;
	for (int i = 0; i < MAX_DC_ROLES; i++)
	{
		p->dc[i].dcProcessID = -1;
		p->dc[i].status = -1;
		p->dc[i].lastTimeHeardFrom = -1;
	}
	
	// Open the files in write mode so it creates a fresh file for the run
	fptr = fopen("/tmp/dataCreator.log", "w");
	fptr = fopen("/tmp/dataMonitor.log", "w");

	// Sleep for 15 seconds
	sleep (15);

	// Message receive loop for server
	while (exitServer == FALSE)
	{
		int isFound = FALSE;
		fptr = fopen("/tmp/dataMonitor.log", "a");

		// receive message with no-wait
		msgrcv (mid, (void *)&msg, sizeof (DCInfo), 0, IPC_NOWAIT);

		// get the local time in server for comparison
		time_t t = time(NULL);
  		struct tm tm = *localtime(&t);
		localTimeInServer = mktime(&tm);

		// parsing time to string
		strftime(timeString, sizeof(timeString), "%Y-%m-%e %H:%M:%S", &tm);

		// updating client details in masterlist for exiting clients
		for (int i = 0; i < MAX_DC_ROLES; i++)
		{
			if (p->dc[i].dcProcessID == msg.dcProcessID && msg.dcProcessID >= 0)
			{
				isFound = TRUE;
				p->dc[i].status = msg.status;
				p->dc[i].lastTimeHeardFrom = msg.lastTimeHeardFrom;

				// write to the file
				// printf("[%s] : DC-%02d [%d] updated in the master list - MSG RECEIVED - Status %d (%s)\n", timeString, i+1, msg.dcProcessID, msg.status, getStatusMessage(msg.status));
				fprintf(fptr, "[%s] : DC-%02d [%d] updated in the master list - MSG RECEIVED - Status %d (%s)\n", timeString, i+1, msg.dcProcessID, msg.status, getStatusMessage(msg.status));
				break;
			}
		}

		// add the client if its not in the list
		if (isFound == FALSE && msg.dcProcessID >= 0)
		{
			p->dc[dcNums].dcProcessID = msg.dcProcessID;
			p->dc[dcNums].status = msg.status;
			p->dc[dcNums].lastTimeHeardFrom = msg.lastTimeHeardFrom;

			// increasing dc nums for new addition
			dcNums += 1;

			// write to the file
			// printf("[%s] : DC-%02d [%d] added to the master list - NEW DC - Status %d (%s)\n", timeString, dcNums, msg.dcProcessID, msg.status, getStatusMessage(msg.status));
			fprintf(fptr, "[%s] : DC-%02d [%d] added to the master list - NEW DC - Status %d (%s)\n", timeString, dcNums, msg.dcProcessID, msg.status, getStatusMessage(msg.status));
		}

		// loop till the exiting client no so that we can check the time difference for each client
		for (int i = 0; i < dcNums; i++)
		{
			double diff = difftime(localTimeInServer, p->dc[i].lastTimeHeardFrom);

			// set the client details to default for time not between 0 and 35 or status is 6
			if (diff < 0 || diff > 35 || p->dc[i].status == 6)
			{
				// writing to the log
				if(diff < 0 || diff > 35)
				{
					// printf("[%s] : DC-%02d [%d] removed from master list - NON-RESPONSIVE\n", timeString, i+1 , p->dc[i].dcProcessID);
					fprintf(fptr, "[%s] : DC-%02d [%d] removed from master list - NON-RESPONSIVE\n", timeString, i+1 , p->dc[i].dcProcessID);
				}
				else if (p->dc[i].status == 6)
				{
					// printf("[%s] : DC-%02d [%d] has gone OFFLINE - removing from master-list\n", timeString, i+1 , p->dc[i].dcProcessID);
					fprintf(fptr, "[%s] : DC-%02d [%d] has gone OFFLINE - removing from master-list\n", timeString, i+1 , p->dc[i].dcProcessID);
				}

				// resetting the values
				p->dc[i].dcProcessID = -1;
				p->dc[i].status = -1;
				p->dc[i].lastTimeHeardFrom = -1;

				// rearranging the masterlist by copying values from the next item
				for (int j = i; j < MAX_DC_ROLES; j++)
				{					
					p->dc[j].dcProcessID = p->dc[j + 1].dcProcessID;
					p->dc[j].status = p->dc[j + 1].status;
					p->dc[j].lastTimeHeardFrom = p->dc[j + 1].lastTimeHeardFrom;
				}

				// decreasing dc nums for deletion
				dcNums -= 1;
			}
		}

		// updating the number of clients in masterlist
		p->numberOfDCs = dcNums;

		// reset the msg values
		msg.dcProcessID = -1;
		msg.status = -1;
		msg.lastTimeHeardFrom = -1;

		// exit client if no dcs present
		if(dcNums == 0)
		{
			// printf("[%s] : All DCs have gone offline or terminated - DR TERMINATING\n", timeString);
			fprintf(fptr, "[%s] : All DCs have gone offline or terminated - DR TERMINATING\n", timeString);
			exitServer = TRUE;

			// detach and clean up our resources

			// Detaching from Shared-Memory
			// printf("(SERVER) Detaching from Shared-Memory\n");
			shmdt (p);

			// Removing the Shared-Memory resource
			// printf("(SERVER) Removing the Shared-Memory resource\n");
			shmctl (shmid, IPC_RMID, 0);
	  
			// done with the message queue - so clean-up
			// printf ("(SERVER) Removing Message QUEUE resource\n");
			msgctl (mid, IPC_RMID, NULL);
		}

		fclose(fptr);

		// sleep for 1.5 secs
		sleep (1.5);
	}

	return 0;
}

