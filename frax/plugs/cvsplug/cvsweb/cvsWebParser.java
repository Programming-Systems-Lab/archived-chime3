import java.net.*;
import java.io.*;

public class cvsWebParser
{
    public static void main(String args[]) throws Exception
    {
        try
        {
	    // Check to see that a command parameter was entered
            if (args.length != 1)
            {
                 // Print message, pause, then exit
                 System.err.println ("Invalid command parameters");
                 System.in.read();
                 System.exit(0);
            }
            // Create an URL instance
            URL url = new URL(args[0]);

            // Get an input stream for reading
            InputStream in = url.openStream();

            // Create a buffered input stream for efficency
            BufferedInputStream bufIn = new BufferedInputStream(in);

            // Repeat until end of file
            String dataStr = new String("");
            for (;;)
            {
                int data = bufIn.read();

                // Check for EOF
                if (data == -1)
                    break;
                else
                    //System.out.print ( (char) data);
                    dataStr += (char)data;
            }
            int start = 0, end = 0;
            while(true){
                start = dataStr.indexOf("dir.gif", start);
                if(start != -1){
                        System.out.println("Directory");
                        start = dataStr.indexOf("<A HREF=", start);
                        end = dataStr.indexOf(">", start);
                        System.out.println(dataStr.substring(start + 9, end - 1));
                        System.out.println();
                        start = end;
                        
                }
                else
                        break;
            }
             start = 0;
             end = 0;            
             while(true){
                start = dataStr.indexOf("text.gif", start);
                if(start != -1){
                        System.out.println("File");
                        start = dataStr.indexOf("<A HREF=", start);
                        end = dataStr.indexOf(">", start);
                        System.out.println("File : " + dataStr.substring(start + 9, end - 1));
                        start = end;
                        start = dataStr.indexOf("<td", start);
                        start = dataStr.indexOf("<b>", start);
                        end = dataStr.indexOf("</b>", start);
                        System.out.println("Revision : " + dataStr.substring(start + 3, end));
                        start = end;
                        start = dataStr.indexOf("<td", start);
                        start = dataStr.indexOf("<i>", start);
                        end = dataStr.indexOf("</i>", start);
                        System.out.println("Age : " + dataStr.substring(start + 3, end));
                        start = end;
                        start = dataStr.indexOf("<td>&nbsp;", start);
                        end = dataStr.indexOf("</td>", start);
                        System.out.println("Author : " + dataStr.substring(start + 10, end));
                        start = end;
                        start = dataStr.indexOf("<td>&nbsp;", start);
                        start = dataStr.indexOf("<font size=-1>", start); 
                        end = dataStr.indexOf("</font>", start);
                        System.out.println("Comments : " + dataStr.substring(start + 14, end));
                        System.out.println();
                        start = end;
                        
                }
                else
                        break;
            }
   
                
            //System.out.print(dataStr);
        }
        catch (MalformedURLException mue)
        {
            System.err.println ("Invalid URL");
        }
        catch (IOException ioe)
        {
            System.err.println ("I/O Error - " + ioe);
        }
    }
}

