#ifndef __AI2TV__CONSTANTS__
#define __AI2TV__CONSTANTS__


/**********
*	GLOBAL SHUTDOWN VARIABLE
************/


// Dov was born on Nov. 28, so 1128 is just a unique, rarely used number

// port number for udp server client
#define AI2TV_UDP_PORT			11280

// constants used for buttons/events and the UDP communication

#define AI2TV_PLAY_PRESSED		11280001	// Pressed Play Button
#define AI2TV_PAUSE_PRESSED		11280002	// Pressed Pause Button


#define AI2TV_PLAY_ANNOUNCE		11280011	// Pressed Play Button
#define AI2TV_PAUSE_ANNOUNCE	11280012	// Pressed Pause Button

#define AI2TV_PLAY_CONFIRM		11280003	// Reply from other clients
#define AI2TV_PAUSE_CONFIRM		11280004	// Reply from other clients

#define AI2TV_PLAY_FINAL		11280005	// Authoritative Play
#define AI2TV_PAUSE_FINAL		11280006	// Authoritative Pause

#define AI2TV_NEXT_PRESSED		11280007	// clicked Next Button
#define AI2TV_TIME_BROADCAST	11280008	// just a sync broadcast
#define AI2TV_NOTE				11280009	// comment for client to display

#define AI2TV_WAITING			11280010	// waiting...

// other buttons
#define AI2TV_NET_PRESSED		11280013	// Pressed network Button
#define AI2TV_TIME_PRESSED		11280014	// Pressed time Button
#endif