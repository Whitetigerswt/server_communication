#include "stdafx.h"
#include "SocketHandler.h"

struct dataAddr {
	const char* file;
	DWORD addr;
};

static dataAddr DataFileAddresses[] =
{
	{"WEAPON", 0x086AA28},
	{"surfinfo", 0x0864DB4},
	{"HANDLING", 0x086A964},
	{"PEDGRP", 0x086A910},
	{"CARGRP", 0x086A924},
	{"melee", 0x086AAB4},
	{"ANIMGRP", 0x086A7F4}
};

SocketHandler::SocketHandler(string ip, int port) : ServerIP(ip), portNumber(port), m_sock(ip, port) {
	//m_sock = SocketClient(ip, port);
}

SocketHandler::~SocketHandler() {

}

bool SocketHandler::RequestDataFile(char* dataFile, int idx) {
	char buf[128];

	sprintf_s(buf, sizeof(buf), "Requesting data file: %s", dataFile);

	m_sock.send(buf, sizeof(buf));
	
	dataFiles[idx] = '\0';

	this->RecieveDataFile(dataFile);
	return true;
}

void SocketHandler::SendReturnValue(char* value) {
	m_sock.send(value, 256);
}

void SocketHandler::AddDataFile(string dataFile) {
	Sleep(100);
	for(int i=0; i < MAX_DATA_FILES; ++i) {
		if(dataFiles[i].empty() || dataFiles[i].compare(dataFile)) {
			dataFiles[i] = dataFile;
			this->RequestDataFile((char*)dataFile.c_str(), i); 
			break;
		} else continue;
	}
	
}

void SocketHandler::RecieveDataFile(char* dataFile) {
	char newbuf[512] = "";

	bool setdatafilemem = true;

	if(!strcmp(dataFile, "HANDLING")) {
		sprintf_s(newbuf, sizeof(newbuf), "DATA\\%s.aaa", dataFile); 
	} else {
		sprintf_s(newbuf, sizeof(newbuf), "CUST\\%s.aaa", dataFile); 
	}
	newbuf[0] = '\0';
	int ticks = 0;
	while(true) {
		m_sock.recv(newbuf, sizeof(newbuf));
		//sprintf_s(newbuf, sizeof(newbuf), "%s", m_sock.ReceiveLine().c_str());

		
		if(strlen(newbuf) > 0) {

			if(!strcmp(newbuf, "NO DATA FILE\n")) {
				break;
			}
			if(!strcmp(newbuf, "ENDED\n")) {
				break;
			}
		} else {
			ticks++;
			if(ticks >= 30) {
				// download failed
				// request data file again
				this->AddDataFile(dataFile);
				return;
			}
		}
	}

	if(!strcmp(dataFile, "HANDLING") ||
	   !strcmp(dataFile, "PEDGRP") ||
	   !strcmp(dataFile, "CARGRP")) {
		sprintf_s(newbuf, sizeof(newbuf), "%s.aaa", dataFile); 

		//vp(0x086A91D, 4);
		//memcpy((void*)0x086A91D, "CUST", 4);

		for(int i=0; i < sizeof(DataFileAddresses); ++i) {

			if(!strcmp(dataFile, DataFileAddresses[i].file)) {
				this->SetDataFileMemory(DataFileAddresses[i].addr, dataFile, false);
				setdatafilemem = false;
				break;
			}
		}
	} 

	if(strlen(dataFile) > 0 && setdatafilemem) {
		for(int i=0; i < sizeof(DataFileAddresses); ++i) {

			if(!strcmp(dataFile, DataFileAddresses[i].file)) {
				this->SetDataFileMemory(DataFileAddresses[i].addr, dataFile, true);
				break;
			}
		}
		
	}
}


void SocketHandler::SetDataFileMemory(DWORD address, char* dataFile, bool inccust) {
	char newbuf[50];
	if(inccust) {
		sprintf_s(newbuf, sizeof(newbuf), "CUST\\%s.aaa", dataFile);
	} else {
		sprintf_s(newbuf, sizeof(newbuf), "%s.aaa", dataFile);
	}
	vp(address, strlen(newbuf));
	memcpy((void*)address, newbuf, strlen(newbuf));
}

void SocketHandler::WaitForServerResponse() {
	char newbuf[512] = "";

	//sprintf_s(newbuf, sizeof(newbuf), "%s", m_sock.ReceiveLine().c_str());
	m_sock.recv(newbuf, sizeof(newbuf));
	if(strlen(newbuf) > 0) {
		Log *zzirlog = new Log("asd.txt");
		zzirlog->Write("Complete string: ");
		zzirlog->Write(newbuf);
		stringstream ss(newbuf); // Insert the string into a stream

		newbuf[0] = '\0';

		while (ss >> newbuf) {
			if(strlen(newbuf) > 0) {
				
				zzirlog->Write(newbuf);
				
			}
		}
		delete zzirlog;
	}
}