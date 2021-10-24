//#define DEBUG
/*
TODO refactor to timers replacing  netwTimer, pingTimer

	 timers[NW_TIMER_BUSY]

	if(nodeId==rxData->msg.node) pingTimer = millis() + PING_TIMER;
	if(nodeId==rxData->msg.node) nextTimerMillis(NW_TIMER_PING, PING_TIMER);

	!! timeouts???

*/



#ifndef NetwSPI_H
#define NetwSPI_H

#include "NetwBase.h"

#define SPI_RESPONSE_SLEEP 50  // usec


#include "ArdUtils.h"
#include <SPI.h>

class NetwSPI: public NetwBase
{
public:

	bool isMaster=false;
	int slavePin = 9;
	int msgSize = 0;
	bool isSending = false;

	char writeBuf[PAYLOAD_LENGTH];
	int  writeBufFrom=0;
	int  writeBufTo=0;

    bool    verbose=true;
    bool    verboseSave=verbose;

	virtual ~NetwSPI(){}  // suppress warning

	NetwSPI( )
	{

	}
	NetwSPI(int nodeId )
	{
		this->nodeId=nodeId;
	}

	void handleInterrupt()
	{
	    byte c = SPDR;  // grab byte from SPI Data Register
	    SPDR = 0x00;    // next char

	    if(digitalRead(slavePin))
	    {
	    	pushChar(c);
	    	sleepTimer = millis()+1;
	        return;
	    }

	    if(c == 0x03)
	    {
	        if( msgSize > 0  )
	        {
	            SPDR = writeBuf[writeBufFrom++] ;
	            writeBufFrom = writeBufFrom % PAYLOAD_LENGTH;
	            msgSize--;
	        }
	    }
	}

	void setup( int pin, bool isServer);
	void loop();
	void print(char * text);
    bool isReady(void);
	bool isBusy(void);
	int write( RxData *rxData );
    void trace(char* id);
//	int bufOutSize()
//	{
//	    if(writeBufFrom == writeBufTo) return 0;
//	    int bufSize = writeBufTo-writeBufFrom;
//	    return  bufSize >0 ? bufSize : bufSize +  BUF_SIZE ;
//	}

private:

};

#endif
