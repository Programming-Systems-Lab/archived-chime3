/**
 * MonitorConnection monitors the time a connection is up with client. 
 * If the client is <b>idle</b> for IDLE_CONNECTION_TIME 
 * MonitorConnection interrupt the 
 * FRAXUserInterfaceServerConnection and causes it to disconect the client.
 */

 package frax.FRAXUserInterface.server;
 import java.util.Timer;
 import java.util.TimerTask;

 class MonitorConnection extends TimerTask implements FUIConstants{
	//a reference to the FRaxUserInterfaceServerConnection object
	FRAXUserInterfaceServerConnection fuisc = null;

	MonitorConnection( FRAXUserInterfaceServerConnection fuisc ){
		this.fuisc = fuisc;
	}

	public void run(){
		//check if the timer for this object is suspended
		if( !fuisc.isSuspended() ){
			long currentTime = System.currentTimeMillis();
			//check for idle connection 
			if( (currentTime - fuisc.getLastConnection() ) >= IDLE_CONNECTION_TIME ){
				//stop the scheduling of this task
				cancel(); 
				fuisc.interrupt();
			}
		}
	}
}
		
