import icommand.navigation.Pilot;
import icommand.nxt.Motor;
import icommand.nxt.comm.NXTCommand;


public class Demo1 {

	public static void main(String[] args) throws InterruptedException {
		NXTCommand.open();
		
		Pilot sc = new Pilot(2.1f,4.4f,Motor.B, Motor.C,true);
        sc.setSpeed(320);
        sc.travel(-20);
        sc.rotate(+180);
        sc.travel(-20);

        while(sc.isMoving())Thread.yield();
        sc.stop();
	}
}
