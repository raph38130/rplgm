/* very simple demo */

import icommand.nxt.Motor;
import icommand.nxt.comm.NXTCommand;

public class Demo0 {

	public static void main(String[] args) throws InterruptedException {
		NXTCommand.open();
		
		Motor.B.setSpeed(75);
		Motor.C.setSpeed(75);

		Motor.C.forward();
		while (true) {
			
			Motor.B.forward();
			Thread.sleep(5000);
			
			Motor.B.backward();
			Thread.sleep(2500);
		}

	}

}
