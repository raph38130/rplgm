package rcx;

import josx.platform.rcx.Motor;
import josx.platform.rcx.Sensor;
import josx.platform.rcx.SensorListener;
import josx.platform.rcx.TextLCD;

public class Voiture17 {

	/**
	 * @param args
	 * @throws InterruptedException 
	 */
	public static void main(String[] args) throws InterruptedException {
		
		//motor init
		Motor.A.setPower(7);
		Motor.C.setPower(7);
		
		//sensors init S1,S3 : 3 = LIGHT 0x80 = PERCENT
		Sensor.S1.setTypeAndMode(3,0x80);
		Sensor.S3.setTypeAndMode(3,0x80);
		
		//sensors init S2 : 1 = TOUCH 0x20 = BOOL
		Sensor.S2.setTypeAndMode(1,0x20);
		/*Sensor.S2.addSensorListener(
				//une instance de classe implémentant SensorListener
				new SensorListener()
				{
					public void stateChanged(Sensor arg0, int arg1, int arg2)  {
						try {
							TextLCD.print("hit");
							//stop car 5s
							Motor.A.stop();
							Motor.C.stop();
							Thread.sleep(5000);
							//rotate 5s
							Motor.A.backward();
							Motor.C.forward();
							Thread.sleep(5000);
							TextLCD.print("----");
						} catch (InterruptedException e) {TextLCD.print("***");}
					}
				}
		
								);*/
		Sensor.S1.activate();
		Sensor.S2.activate();
		Sensor.S3.activate();
		
		TextLCD.print("p4");
		Motor.A.forward();
		Motor.C.forward();
		while (true) {
			//turn right ?
            if (Sensor.S1.readValue() < 45) {
                 Motor.A.backward();           
            	 Motor.C.forward();
            }
            //turn left ?
            if (Sensor.S3.readValue() < 45) {
                 Motor.C.backward();           
            	 Motor.A.forward();  
            }
            //hit ?
            if (Sensor.S2.readBooleanValue()) {
            	Motor.A.stop();
				Motor.C.stop();
				Thread.sleep(5000);
				Motor.C.backward();
				Motor.A.forward();
				Thread.sleep(2500);
            }
            
		}
	}

}
