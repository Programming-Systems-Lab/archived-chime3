// AISyncController.h: interface for the AISyncController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AISYNCCONTROLLER_H__198FC127_7528_4944_A041_B43622146934__INCLUDED_)
#define AFX_AISYNCCONTROLLER_H__198FC127_7528_4944_A041_B43622146934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class ChimeSystemDriver;
class UDPServer;
class UDPClient;
class AISyncController
{
public:
	
	UDPServer *server;
	UDPClient *client;
	char** multicast_group;
	int *clientIDs; // list of client IDs to correspond to multicast group
	int num_clients;
	AISyncController(ChimeSystemDriver* drv);
	void setMultiCastGroup(int n_clients, char** client_addresses, int* ids);
	virtual ~AISyncController();
	void parseResponse(char* string);
	void listen();
	void send(char* msg);
};

#endif // !defined(AFX_AISYNCCONTROLLER_H__198FC127_7528_4944_A041_B43622146934__INCLUDED_)
