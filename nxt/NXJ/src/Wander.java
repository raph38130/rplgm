/* ultrasonic demo */


import icommand.nxt.*;
import icommand.nxt.Motor;
import icommand.nxt.comm.*;
import java.io.*;

public class Wander {
  
   private boolean running;
   
   public Wander(){
      
      System.out.println("Wandering...");
      
      running = true;
      NXTCommand.open();
      
      Thread runloop = new Thread(){
         public void run(){
            
            UltrasonicSensor sensor =  new UltrasonicSensor(SensorPort.S4);
            int distance;
            
            while(isRunning()){
               distance = sensor.getDistance();
               // Manual conversion of distances
               // 63 is approximately 25 cm.
               if (distance < 63) {
                  Motor.B.setSpeed(25);
                  Motor.C.setSpeed(25);
                  Motor.B.forward();
                  Motor.C.backward();
               }
               else {
                  Motor.B.setSpeed(75);
                  Motor.C.setSpeed(75);
                  Motor.B.forward();
                  Motor.C.forward();
               }
            }
         }
      }; // end thread
      
      runloop.start();
      System.out.println("\nPress the return key to quit.");
      
      try{
         BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
         br.readLine();
         
      } catch(Exception e){ 
         e.printStackTrace();
      }
      
      running = false;
      
      try{
         runloop.join();   
      } catch(Exception e){
         e.printStackTrace();
      }
      
      stopRun();
   }
   
   public boolean isRunning(){
      return running;
   }
   
   public void stopRun(){
      System.out.println("Shutting Down!");
      Motor.B.stop();
      Motor.C.stop();
      NXTCommand.close();
   }
   
   public static void main(String[] args){
      
      new Wander();
   }
}