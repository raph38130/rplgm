import josx.platform.rcx.*;

public class HelloWorld
{
  public static void main (String[] aArg)
  throws Exception
  {
     LCD.clear();
     TextLCD.print ("hello");
     Thread.sleep(2000);
     TextLCD.print ("world");
     Thread.sleep(2000);
     // drive forward
     Motor.A.forward();

     // just run until RUN button is pressed again
     Button.RUN.waitForPressAndRelease();


  }
}
