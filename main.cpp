#include <stdio.h>
#include <Windows.h>
#include <SrRestorePtApi.h>
#include <atlconv.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>

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
		if (strcmp(aStr, "3") == 0) flag = 3;
		if (strcmp(aStr, "4") == 0) flag = 4;
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

	case 3: { 
		      if (!system("lib.vbs")) { 
		          printf("Please Wait dialog box! \nIf the dialog box does not appear, it means no system restore point! \n -----------------------------------------"); 
	          } else {
		          printf("--- File lib.vbs not found! ---");
	          }
		      return 0;
	        } break;

	case 4: {
		     int validLine = 22;
			 int validSize = 824;
			 int validC = 20;
		     char out_s;
			 string buf;
			 char file[80] = "C:\\Windows\\System32\\drivers\\etc\\hosts";
			 ifstream i(file);
			 int countLine = 0;
			 int countC = 0;
			 if (i){
				 while (i.good()) {
					 i.get(out_s);
				     if (out_s == '\n') countLine++;
				 }
				 i.clear();
				 i.seekg(0);
				 while (i.good()) {
					 getline(i, buf);
					 if (!i.eof()) {
						 if (buf[0] == '#') countC++;
					 } else break;
				 }
			 }

			 else { printf("File not exist!\n"); }
			 i.close();
			 struct stat fi;
			 stat(file, &fi);
			 if (fi.st_size == validSize && countC == validC && countLine == validLine) printf("File hosts IS ORIGINAL! IS GOOD!\n");
			 else printf("File hosts NOT ORIGINAL! IS BAD!\n");
	        } break;

	case 0: {
				printf(".....................................................................\n");
				printf("System Restore Utility v0.4 by Merzlyakov Daniil, Krylov Anton, IU8-93, 2015\n");
				printf("\n-------------------------------------------------------------------\n");
				printf("syntax: SystemRestore N [p]\n");
				printf("\n-------------------------\n");
				printf("1 - Create Restore Point, p - name restore point\n");
				printf("2 - Remove Restore Point, p - number restore point. HINT: Number restore point's can be obtained using argument 3 (systemrestore.exe 3)\n");
				printf("3 - Get a list of restore points\n");
				printf("4 - Check the original of the file host\n");
				return 0;
	} break;
	}
	return 0;
	
}
