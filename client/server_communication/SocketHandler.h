#include "PracticalSocket.h"
#include "log.h"
#include "misc.h"
#include <vector>
#include <string>
#include <sstream>

using namespace std;

#define		MAX_DATA_FILES		10

class SocketHandler {

public:
	SocketHandler(string ip, int port);
	~SocketHandler();
	void SendReturnValue(char* val);
	void AddDataFile(string dataFile);
	void RecieveDataFile(char* dataFile);
	void WaitForServerResponse();

private:
	bool RequestDataFile(char* dataFile, int idx);
	void SetDataFileMemory(DWORD address, char* dataFile, bool inccust);
	const string ServerIP;
	const int portNumber;
	TCPSocket m_sock;
	string dataFiles[MAX_DATA_FILES];

	//bool g_bWaiting;
protected:
	
};