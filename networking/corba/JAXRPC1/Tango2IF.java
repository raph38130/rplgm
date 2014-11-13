/* $Id$
 *
 *
 */
 
package lgm;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Tango2IF extends Remote {
    public String myFunc1(int n) throws RemoteException;
    
    /* Tango2 Interface */
    public String[] getserverlist() throws RemoteException;
    public String[] commandlistquery(String device) throws RemoteException;
    public String[] commandinout(String device, String cmd, String args) throws RemoteException;
}

