// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "main.h"
#include "log.h"
#include "sockethandler.h"


static void WINAPI Load();

char buf[MAX_LENGTH_PACKET] = "";

using namespace std;

Log *zzlog = new Log("areallog.txt");



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	DisableThreadLibraryCalls(hModule);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		zzlog->Write("DLL_PROCESS_ATTACH");
		if ( CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)Load, NULL, 0, 0) == NULL ) {
			ExitProcess(GetLastError());
			return FALSE; 
		}
		zzlog->Write("Made it past new thread creation");
		//Sleep(500);
		//disableSampAC();
		//Load();
		break;
	}
	return TRUE;
}

/*

typedef void ( *Load_Dat_file)(char* filename);
typedef void ( *InitializeWeapon)(void);
typedef void ( *initalizeWeaponInfo)(void);
typedef void ( *LoadAll)(char* filename);

Load_Dat_file dat = (Load_Dat_file)0x005B9030;
InitializeWeapon o = (InitializeWeapon)0x0073A300;
initalizeWeaponInfo i = (initalizeWeaponInfo)0x005BF750;
LoadAll a = (LoadAll)0x0053BC80;*/

void vp(DWORD address, int s);

vector<string> split(const string& strValue, char separator)
{
    vector<string> vecstrResult;
    int startpos=0;
    int endpos=0;

    endpos = strValue.find_first_of(separator, startpos);
    while (endpos != -1)
    {       
        vecstrResult.push_back(strValue.substr(startpos, endpos-startpos)); // add to vector
        startpos = endpos+1; //jump past sep
        endpos = strValue.find_first_of(separator, startpos); // find next
        if(endpos==-1)
        {
            //lastone, so no 2nd param required to go to end of string
            vecstrResult.push_back(strValue.substr(startpos));
        }
    }

    return vecstrResult;
}


static void WINAPI Load() {

	string cmdline = GetCommandLineA();
	string ip;
	int port = NULL;
	std::vector<std::string> a;

	Log *log = new Log("log.txt");

	a = split(cmdline, ' ');

	log->Write((char*)cmdline.c_str());
	for (vector<string>::iterator i = a.begin(); i != a.end(); i++) {
		if (!i->compare("-h"))
		{
			advance(i, 1);
			ip = (*i);

		}
		else if (!i->compare("-p"))
		{
			advance(i, 1);
			port = atoi((*i).c_str());
		}
	}
	log->Write("before try");
	log->Write((char*)ip.c_str());
	try {
		SocketHandler s = SocketHandler(ip, 7667);
		log->Write("just connected");

		//s.AddDataFile("HANDLING");
		s.AddDataFile("WEAPON");
		s.AddDataFile("surfinfo");
		s.AddDataFile("melee");
		s.AddDataFile("ANIMGRP");
		
		disableSampAC();

		while(true) {
			s.WaitForServerResponse();
		}
	} catch (SocketException e) { 
		log->Write((char*)e.what());
		return;
	}
	//log->Write(buf);

	delete log;
	
}