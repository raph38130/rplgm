/* SqlRequest.java
 *
 *
 * Mon Feb 24 14:50:48 CET 2003 RP
 *	- with mysql-server debian
# The skip-networkin option will no longer be set via debconf menu.
# You have to manually change it if you want networking i.e. the server
# listening on port 3306. The default is "disable" - for security reasons.
#skip-networking
 *
 * Thu Feb 14 13:15:13 CET 2002 RP
 *	-initial revision (tested with mm.mySQL jdbc driver)
 */
import java.sql.*;

class SqlRequest
{

static public void main(String[]args)
{
System.out.println("SQL Req v0.0");
try {
	/* load mySQL driver */
	Class.forName("org.gjt.mm.mysql.Driver");
	
	/* open connection */
	Connection con =
	DriverManager.getConnection("jdbc:mysql://172.16.0.1:3306/tango?user=root&passwd=");
	
	/* */
	Statement stmt = con.createStatement();
	ResultSet rs = stmt.executeQuery("SHOW TABLES");
	while (rs.next())
             System.out.println(rs.getString("Tables_in_tango"));
	rs = stmt.executeQuery("DESCRIBE device");     
	while (rs.next())
             System.out.println(rs.getString("Field"));
    }
catch(ClassNotFoundException e1){System.out.println(e1);}
catch(SQLException e2){System.out.println(e2);}
System.out.println("end");
}
}
