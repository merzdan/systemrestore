#include <stdio.h>
#include <Windows.h>
#include <SrRestorePtApi.h>
#include <atlconv.h>
#include <tchar.h>


using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	RESTOREPOINTINFO RstPt;
	STATEMGRSTATUS MgrStat;
	int flag = 0;
	int flag1 = 0;
	char* aStr1 = "";
	USES_CONVERSION;
	char* aStr = (char*) T2CA(argv[1]);
	if (argv[1]) {
		if (argv[2]) aStr1 = (char*)T2CA(argv[2]);
	}
	else {
		printf("Please enter argument! Argument for help \"?\"");
		return 1;
	}
	if (aStr){
		if (strcmp(aStr, "?") == 0) flag = 0;
		if (strcmp(aStr, "1") == 0) flag = 1;
		if (strcmp(aStr, "2") == 0) flag = 2;
	}
 
	if (argv[1]) if (argv[2]) if (aStr1) flag1 = atoi(aStr1);
	

	switch (flag){

	case 1: {   printf("Restore...wait\n");
				RstPt.dwEventType = BEGIN_SYSTEM_CHANGE;
				RstPt.dwRestorePtType = APPLICATION_INSTALL;
				if (argv[1]) {
					if (argv[2]) _tcscpy_s(RstPt.szDescription, argv[2]);
					else _tcscpy_s(RstPt.szDescription, _T("Restore  Point"));
				}
				//-------------------
				if (!SRSetRestorePoint(&RstPt, &MgrStat))
				{
					printf("Unable to set restore point. Error %ld\n",
						MgrStat.nStatus);
					if (MgrStat.nStatus == 5) printf("!!! RUN PROGRAM AS ADMINISTRATOR !!!\n");
					return 1;
				}
				//--------------------------
				printf("Restore point created. Sequence %ld\n", MgrStat.llSequenceNumber);

				RstPt.dwEventType = END_SYSTEM_CHANGE;
				RstPt.llSequenceNumber = MgrStat.llSequenceNumber;
				//---------------------------
				if (!SRSetRestorePoint(&RstPt, &MgrStat))
				{
					printf("Unable to set end of restore point. Error %ld\n",
						MgrStat.nStatus);
					
				
					SRRemoveRestorePoint((DWORD)MgrStat.llSequenceNumber);
					printf("Restore point removed\n");
					return 1;
				}
				//-------------------------
				printf("Restore point completed\n");
				return 0; } break;


				
		
	case 2: { MgrStat.llSequenceNumber = flag1;
				
			SRRemoveRestorePoint((DWORD)MgrStat.llSequenceNumber);
			printf("Restore point removed\n");
			return 0;

		} break;

	case 0: {
				printf(".....................................................................\n");
				printf("System Restore Utility v0.1 by Merzlyakov Daniil, Krylov Anton, IU8-63, 2014\n");
				printf("\n-------------------------------------------------------------------\n");
				printf("syntax: SystemRestore N [p]\n");
				printf("\n-------------------------\n");
				printf("1 - Create Restore Point, p - name restore point\n");
				printf("2 - Remove Restore Point, p - number restore point\n");
				return 0;
	} break;
	}
	return 0;
	
}
