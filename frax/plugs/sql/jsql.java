import java.sql.*;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.IOException;
public class jsql{
        public static void main(String[] Args){
                String xml = "";
                try{
                        Class.forName("org.gjt.mm.mysql.Driver").newInstance();
                }
                catch(Exception E){
                        System.err.println("Unable to load driver.");
                        E.printStackTrace();
                }
                InputStreamReader reader = new InputStreamReader(System.in);
                BufferedReader console = new BufferedReader(reader);
                String databaseUrl = null, database = null, table = null, query = null;
                try{
                        System.out.println("Enter database url : ");
                        databaseUrl = console.readLine();
                        System.out.println("Enter database to use : ");
                        database = console.readLine();
                        
                        
                }
                catch(IOException e){
                        System.out.println(e);
                        System.exit(1);
                }
                try{
                        Connection C = DriverManager.getConnection("jdbc:mysql://" + databaseUrl + "/" + database);
                        Statement Stmt = C.createStatement();
                        ResultSet RS = Stmt.executeQuery("show tables");
                        System.out.println("\n-----------Available Tables------------");
                        int columns = 0, x = 1;
                        while(RS.next()){
                                System.out.println(x + ": " + RS.getString(1));
                                x++;
                        }
                        x = 1;
                        System.out.println("\nChoose a table to show information for : ");
                        try{
                                table = console.readLine();
                        }
                        catch(IOException e){
                                System.out.println(e);
                                System.exit(1);
                        }
                        System.out.println("\n-------------Columns---------------");
                        RS = Stmt.executeQuery("show columns from " + table);
                        while(RS.next()){
                                //System.out.println(x + ": " + RS.getString(1));
                                x++;
                                columns++;
                                xml += createXMLColumn(RS.getString(1));
                        }
                        
                        RS = Stmt.executeQuery("select Count(*) from " + table);
                        System.out.println("Number of rows for " + table + " : " + RS.getString(1));
                        
                        x = 1;
                        
                        System.out.println("\nEnter Query : ");
                        /*
                        try{
                                query = console.readLine();
                        }
                        catch(IOException e){
                                System.out.println(e);
                                System.exit(1);
                        }
                        RS = Stmt.executeQuery(query);
                        System.out.println();
                        while(RS.next()){
                                while(x <= columns){
                                        System.out.print(RS.getString(x) + "\t");
                                        x++;
                               }
                               x = 1;
                               System.out.println();
                       }
                       */
                        RS.close();
                        Stmt.close();
                        C.close();
                        System.out.println(xml);
                }
                catch(SQLException E){
                        System.out.println("SQLException: " + E.getMessage());
                        System.out.println("SQLSate: " + E.getSQLState());
                        System.out.println("VenorError: " + E.getErrorCode());
                }
        }
        public static String createXMLColumn(String column){
                String temp = "<Column>" + column + "</Column>\n";
                return temp;
                                      
        }
}
