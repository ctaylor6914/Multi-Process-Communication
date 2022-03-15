/*
*	FILE			: dataCorruptor.c
*	PROJECT			: SP_A03
*	PROGRAMMER		: Mita Das 8685078 & Colby Taylor 8466914
*	FIRST VERSION	: 2022-02-21
*	DESCRIPTION		: This is the main entry point for data corruptor operation
*					: this file checks to see if there is shared memory and will wait for 
*					: 10 seconds x 100 loops to wait for shared mem. Once connected it 	
*					: will randomly choose a number from 0-20 and corrupt the DC and DR 's
*					: once the msgQ is no longer detected the DX will end
*/

#include "../inc/dataCorruptor.h"

int main(int argc, char* argv[])
{

	key_t shmKey, message_key;
	int tries = 1;
	int sleepTime = 0;
	int pick = 0;
	int mid = 0;
	int shmid = 0;
	int qID = 0;
	int retcode = 0;
	int dcNums = 0;
	int procID = 0;
	int exitCorruptor = FALSE;
	int killProcess = FALSE;
	int whichProcess = 0;
	int didItKill = 0;
	
	time_t localTimeInServer;
	char timeString[20] = {0};
	
	MasterList *p = NULL;
	FILE *fptr = NULL;
	
	shmKey = ftok ("/tmp", 16535);		//get shared memory key
	if (shmKey == -1) 			//if it cannot get key return;
	{ 
		printf("(DX) Cant get shared mem key");
	 	return 1;
	}
	//check if sharedmem exists
	while ((shmid = shmget (shmKey, sizeof (MasterList), 0)) == -1) 
	{
		// not available yet ... check in 10 seconds
		sleep (10);
		tries++;
		if(tries >= 100)
		{
			printf("(DX) No shared mem found - quit\n");
			return 1;
		}
	}
	p = (MasterList *)shmat (shmid, NULL, 0); //try to attach to shared memory 
	if(p==NULL)
	{
		printf("(DX) No shared mem found - quit\n");
		return 1;
		//cannot attach to shared memory
	}
	//open log file
	fptr = fopen("/tmp/dataCorruptor.log", "w");

	if(fptr == NULL)// if file doesn open
	{
		printf("(DX) Cannot Open File\n");
		return 1;
	}
	if(fclose(fptr))//if cannot close file
	{
		printf("(DX) Problem closing file\n");
		return 1;
	}
	// when shared memory exists and it is attached enter main loop
	while(exitCorruptor == FALSE)
	{
		killProcess = FALSE;
		whichProcess = 0;
		pick = 0;
		sleepTime = rand() % 31;
		didItKill = 0;
		if(sleepTime < 10)
		{
			sleepTime +=10;
		}
		sleep (sleepTime);
		
		fptr = fopen("/tmp/dataCorruptor.log", "a");//open for appened
		
		if(fptr == NULL)// if file doesnt assign to fptr
		{
			fclose(fptr);
			fptr = fopen("/tmp/dataCorruptor.log", "w");
			if(fptr == NULL)
			{
				printf("(DX) Cannot Open File - Inside main loop\n");
				return 1;
			}
		}
		
		//get local time for logging purposes 
		time_t t = time(NULL);
  		struct tm tm = *localtime(&t);
		localTimeInServer = mktime(&tm);

		// parsing time to string
		strftime(timeString, sizeof(timeString), "%Y-%m-%e %H:%M:%S", &tm);
		
		
		message_key = ftok ("/tmp", 16535);
		if (message_key == -1) 
		{ 
			// detach from shared memory and exit
			fprintf(fptr, "[%s] : DX Cannot get Message Queue Key\n", timeString);
			exitCorruptor = TRUE;
			break;
		}
		if((qID = msgget(message_key, 0)) == -1)
		{
			// not available yet assume hoochamacallit system down
			//log DX expected msgq is gone - assuming DR/DCs done
			fprintf(fptr, "[%s] : DX detected that msgQ is gone - Assuming DR/DCs done\n", timeString);
			exitCorruptor = TRUE;
			break;
		}

		// pick a random number under 21
		srand((unsigned) time(&t));
		pick = rand() % 21;
		
		switch (pick) //spin the WHEEL OF DEATH
		{
			case 0:
			{
				//do nothin
				fprintf(fptr, "[%s] : WOD Action 00 randomly selected - Doing Nothing\n", timeString);
				break;
			}
			case 1:
			{
				//kill DC-01 if exists
				killProcess = TRUE;
				whichProcess = 0;
				break;
			}
			case 2:
			{
				//kill DC-03 if exists
				killProcess = TRUE;
				whichProcess = 2;
				break;
			}
			case 3:
			{
				//kill DC-02 if exists
				killProcess = TRUE;
				whichProcess = 1;
				break;
			}
			case 4:
			{
				//kill DC-01 if exists
				killProcess = TRUE;
				whichProcess = 0;
				break;
			}
			case 5:
			{
				//kill DC-03 if exists
				killProcess = TRUE;
				whichProcess = 2;
				break;
			}
			case 6:
			{
				//kill DC-02
				killProcess = TRUE;
				whichProcess = 1;
				break;
			}
			case 7:
			{
				//Kill DC-04
				killProcess = TRUE;
				whichProcess = 3;
				break;
			}
			case 8:
			{
				//do nothing
				fprintf(fptr, "[%s] : WOD Action 08 randomly selected - Doing Nothing\n", timeString);
				break;
			}
			case 9:
			{
				//kill DC-05
				killProcess = TRUE;
				whichProcess = 4;
				break;
			}
			case 10:
			{
				//delete msgQ being used by DCs and DR
				fprintf(fptr, "[%s] : WOD Action 10 randomly selected - Deleting msgQ\n", timeString);
				retcode = msgctl(qID, IPC_RMID, NULL);
				break;
			}
			case 11:
			{
				//kill DC-01
				killProcess = TRUE;
				whichProcess = 0;
				break;
			}
			case 12:
			{
				//kill DC-06
				killProcess = TRUE;
				whichProcess = 5;
				break;
			}
			case 13:
			{
				//kill DC-02
				killProcess = TRUE;
				whichProcess = 1;
				break;
			}
			case 14:
			{
				//Kill DC-07
				killProcess = TRUE;
				whichProcess = 6;
				break;
			}
			case 15:
			{
				//kill DC-03
				killProcess = TRUE;
				whichProcess = 2;
				break;
			}
			case 16:
			{
				//kill DC-08
				killProcess = TRUE;
				whichProcess = 7;
				break;
			}
			case 17:
			{
				//delete msgq being used by DCs and DR
				fprintf(fptr, "[%s] : WOD Action 17 randomly selected - Deleting msgQ\n", timeString);
				retcode = msgctl(qID, IPC_RMID, NULL);
				break;
			}
			case 18:
			{
				//kill DC-09
				killProcess = TRUE;
				whichProcess = 8;
				break;
			}
			case 19:
			{
				// do nothing
				fprintf(fptr, "[%s] : WOD Action 19 randomly selected - Doing Nothing\n", timeString);
				break;
			}
			case 20:
			{
				//kill DC-10
				killProcess = TRUE;
				whichProcess = 9;
				break;
			}
		
		}
		//gathering information from shared memory
		dcNums = p->numberOfDCs; // how many datacreators
		procID = p->dc[whichProcess].dcProcessID; // specific process id

		if(killProcess == TRUE)// if a kill process pick has been selected 
		{
			if(whichProcess <= dcNums - 1)//if process selected is less than total data creators
			{
				didItKill = kill(procID, 01);// kill process
				if(didItKill == -1)
				{
					fprintf(fptr, "[%s] : WOD Action %d randomly selected - DC-%d pID %d NOT-TERMINATED\n", timeString, pick, whichProcess+1, procID);
					//printf("[%s] : WOD Action %d randomly selected - DC-%d pID %d NOT-TERMINATED\n", timeString, pick, whichProcess+1, procID);
				}
				else
				{
					fprintf(fptr, "[%s] : WOD Action %d randomly selected - DC-%d pID %d TERMINATED\n", timeString, pick, whichProcess+1, procID);
					//printf("[%s] : WOD Action %d randomly selected - DC-%d pID %d TERMINATED\n", timeString, pick, whichProcess+1, procID);
				}
			}
			else
			{
				fprintf(fptr, "[%s] : WOD Action %d randomly selected - DC-%d DID NOT EXIST\n", timeString, pick, whichProcess+1);
				//printf("[%s] : WOD Action %d randomly selected - DC-%d DID NOT EXIST\n", timeString, pick, whichProcess+1);
			}
		}
		fclose(fptr);// close file
	}
	shmdt(p);//detach from shared memory
	return 0;
}

