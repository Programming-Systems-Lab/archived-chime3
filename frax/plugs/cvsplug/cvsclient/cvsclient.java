import org.netbeans.lib.cvsclient.*;
import org.netbeans.lib.cvsclient.admin.*;
import org.netbeans.lib.cvsclient.connection.*;
import org.netbeans.lib.cvsclient.command.*;
import org.netbeans.lib.cvsclient.command.status.*;
import org.netbeans.lib.cvsclient.connection.*;
import org.netbeans.lib.cvsclient.commandLine.BasicListener;
import org.netbeans.lib.cvsclient.commandLine.CommandFactory;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.File;

/**
 * Title:
 * Description:
 * Copyright:    Copyright (c) 2001
 * Company:
 * @author
 * @version 1.0
 */

public class cvsclient {

  public cvsclient() {
  }
  public static void main(String[] args) {
    PServerConnection c = new PServerConnection();
    c.setUserName("jz137");
    c.setEncodedPassword("A dy:4oA");
    c.setHostName("canal.psl.cs.columbia.edu");
    c.setRepository("/proj/psl-cvs");
    File file = new File("C:/pslroot/psl/chime/startServer.bat");
    try{
	c.open();
	Client client = new Client(c, new StandardAdminHandler());
	client.getEventManager().addCVSListener(new BasicListener());

        InputStreamReader reader = new InputStreamReader(System.in);
        BufferedReader console = new BufferedReader(reader);
        try{
          System.out.println("Enter local path : ");
          String localPath = console.readLine();
          client.setLocalPath(localPath);
        }
        catch(IOException e){
          System.out.println(e);
          System.exit(1);
        }
        //"C:/pslroot/psl/chime"

	StatusCommand command = new StatusCommand();
	//System.out.println(command.get);
        command.setRecursive(false);
        StatusBuilder builder = new StatusBuilder(client.getEventManager(), command);
        command.setBuilder(builder);
        GlobalOptions globaloptions = new GlobalOptions();
        globaloptions.setCVSRoot("/proj/psl-cvs");
        try{
          client.executeCommand(command, globaloptions);
          StatusInformation statusInfo = new StatusInformation();
          statusInfo.setFile(file);
          System.out.println(statusInfo.toString());
        }
        catch(CommandException ae){
          ae.printStackTrace();
        }

    }
    catch(AuthenticationException ae){
	ae.printStackTrace();
    }
  }
}

















