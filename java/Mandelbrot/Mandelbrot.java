/* Mandelbrot.java
 *
 * L'Ensemble Fractal de Mandelbrot
 *
 * RP Sat Mar 31 20:57:21 CEST 2001
 *	- swing app
 *
 * RP août 1996
 *	- initial revision (applet)
 *
 */

import javax.swing.*;
import java.awt.*;

class Mandelbrot extends JFrame implements Runnable
{
//coordonnées de la première image
        double X1 = -2;
        double X2 = 2;
        double Y1 = -2;
        double Y2 = 2;
        double a;
        double b;
        double x1;
        double y1;
        double x2;
        double y2;
//pour le zoom
        boolean first_point = true;
        int xsouris1;
        int xsouris2;
        int ysouris1;
        int ysouris2;
        boolean bouge = false;
//nombre d'itérations
        private int N = 50;
//taille des bandes dessinées à chaque passage
        private int step = 20;
//couleurs
        private Color palette[] = {
	Color.black,Color.orange,Color.cyan,Color.red, Color.green,Color.blue,Color.yellow,Color.gray,Color.lightGray,Color.white,Color.magenta,Color.pink
				};
        Thread proc;
        Image bufferImage;
        Graphics bufferGraphic;

private int conv(double c1, double c2)
{
        int i;
        double  x1 = 0,
        xaux,
        y1 = 0;

        for (i=0; i<N; i++) {
                xaux = x1;
                x1 = x1*x1 - y1*y1 + c1;
                y1 = 2*xaux*y1 + c2;
                if ((x1*x1 + y1*y1)>4) return i;
        	}

        return N-1;
}
public boolean mouseDown(Event e, int x, int y) 
        {
        //premier point : coin sup gauche
        if (first_point == true) {
                first_point=false;
                xsouris1=x;ysouris1=y;
                x1=x*a+X1;
                                y1=y*b+Y2;
                System.out.println("premier point " + x1 + " " + y1 + "\n");
        }
        else    {
                //second point : définit un carré avec le précédent
                first_point=true;
                x2=x*a+X1;
                y2=y1+x1-x2;
                System.out.println("second point " + x2 + " " + y2 + "\n");
                X1=x1; X2=x2; Y1=y2; Y2=y1;
                a = (X2-X1)/size().width; b = (Y1-Y2)/size().height;
                if (proc.isAlive()==true) { 
                        proc.stop();
                        proc=null;
                }
                proc = new Thread(this);
                proc.setPriority(Thread.MIN_PRIORITY);
                proc.start();
        }
        return  true;
}

public boolean mouseMove(Event e, int x, int y)
        {
        if (first_point==true) return true;
        bouge=true;
        xsouris2=x;ysouris2=y;
        return true;
}
Mandelbrot()
{
setSize(600,600);
setResizable(false);
show();
System.out.println("Initialisation...\n");
        bufferImage=createImage(size().width,size().height);
        bufferGraphic=bufferImage.getGraphics();
        a = (X2-X1)/size().width; b = (Y1-Y2)/size().height;
        //mise en tache de fond
        if (proc==null) {
                proc = new Thread(this);
                proc.setPriority(Thread.MIN_PRIORITY);
                proc.start();
        }

}
public void run() 
{
        int n;

        for (int orig=0; orig < step; orig++) 
                for (int xvar = orig; xvar < size().width; xvar+=step) {
                repaint();
                proc.yield();
                for (int yvar = 0; yvar < size().height; yvar++) {
                        n = (conv(xvar*a + X1,yvar*b + Y2) * palette.length) / N; 
                        bufferGraphic.setColor(palette[n]);
                        bufferGraphic.drawLine(xvar, yvar,xvar+step-orig-1,yvar);
                }
        }
}
public void paint(Graphics g)
{
        g.drawImage(bufferImage,0,0,this);
}

public static void main(String[]a)
{
new Mandelbrot();        

}
}
