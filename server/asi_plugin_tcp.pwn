#include <a_samp>
#include <socket>
#include <strlib>
#include <zcmd>

new Socket:mainSocket; // to recieve data
//new Socket:playerSockets[MAX_PLAYERS]; // to send

public OnFilterScriptInit() {
	mainSocket = socket_create(TCP);
	if(is_socket_valid(mainSocket)) {
		socket_set_max_connections(mainSocket, MAX_PLAYERS);
		socket_listen(mainSocket, 7667);
	}
}

public OnPlayerConnect(playerid) {
	socket_sendto_remote_client(mainSocket, 0, "ReadProcMem 0x012345 15");
}


CMD:test2(playerid, params[]) {
	socket_sendto_remote_client(mainSocket, 0, "ReadProcMem 0x012345 15");
}

public onSocketReceiveData(Socket:id, remote_clientid, data[], data_len) {
	printf(data);
	
	if(strlen(data) > 0) {
	    new i = strfind(data, "Requesting data file: ", true);
		if(i == 0) {
		    new file[50] = "custom/";
		    strcatmid(file, data, i+22, strlen(data));
		    strcat(file, ".dat");
		    
		    LoadAndSendDataFile(file, id, remote_clientid);
		    
		}
		/*if(!strcmp(data, "Requesting data files", true)) {
			LoadAndSendDataFile("custom/weapon.dat", id, remote_clientid);
			printf("Requested data file.");
		}*/
	}
}

public onUDPReceiveData(Socket:id, data[], data_len, remote_client_ip[], remote_client_port) {
	printf("UDP: %s", data);
}

public onSocketRemoteConnect(Socket:id, remote_client[], remote_clientid) {
	printf("%d started listening", remote_clientid);
	//LoadAndSendDataFile("custom/weapon.dat", id, remote_clientid);
}

public onSocketRemoteDisconnect(Socket:id, remote_clientid) {
	printf("%d stopped listening", remote_clientid);
}

stock LoadAndSendDataFile( file[], Socket:id, remote_clientid ) {

	printf(file);
	if(!fexist(file)) {
		socket_sendto_remote_client(id, remote_clientid, "NO DATA FILE\n");
		return 1;
	}
 	new File:f = fopen(file, io_read); 
 	new str[512];
 	
 	
 	if(f) {
	    while(fread(f, str)) 
	    {
	        /*if(strfind(str, "\n", true) != strlen(str)-3) {
	        	strcat(str, "\n");
	        }*/

	        if(strlen(str) > 0) {
	            print(str);
				socket_sendto_remote_client(id, remote_clientid, str);
			}
	    }
	    // tell the client we're done sending data

		socket_sendto_remote_client(id, remote_clientid, "ENDED\n");

        printf("sent ended");

		fclose(f);
	}
	return 1;
}
