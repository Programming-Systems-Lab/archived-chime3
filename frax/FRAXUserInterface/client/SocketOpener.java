/**
 * this class tries to open a socket. It is taking from "Core Java" by HorstMann and Cornell
 */



package frax.FRAXUserInterface.client;
import java.net.Socket;
import java.io.IOException;
import java.lang.InterruptedException;
import java.security.AccessControlException;


 public class SocketOpener implements Runnable{
   private String host;
   private int port;
   private Socket socket;

   public static Socket openSocket( String host, int port, int timeOut ){
      SocketOpener opener = new SocketOpener( host, port);
      Thread t = new Thread( opener );
      t.start();
      try
      {
	 t.join( timeOut );
      }
      catch( InterruptedException ie )
      {
      }

      //now return the socket
      return opener.getSocket();
   }

   public SocketOpener( String host, int port ){
      socket = null;
      this.host = host;
      this.port = port;
   }

   public void run(){
      try
      {
	 socket = new Socket( host, port );
      }
      catch( IOException ie )
      {
      }
      catch( AccessControlException ae )
      {
System.out.println("access control exc in openSocket");
      }
   }

   public Socket getSocket(){
      return socket;
   }
}
