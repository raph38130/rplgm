/* lcd2Display.java
 *
 * Simple test of the I2C :  uses LCD LPH3827-1 on Philipps OM5027 evaluation Board
 *
 * TODO init
 *
 *
 * Thu Jun 24 12:59:29 CEST 200 RP
 *	- initial rev
 *
APPLI=lcd2Display
JDK_HOME=/home/raph/j2sdk1.5.0/
TINI_HOME=/home/raph/TP_TINI/tini1.13/
${JDK_HOME}/bin/javac -classpath \
${TINI_HOME}/bin/modules.jar:${TINI_HOME}/bin/tiniclasses.jar ${APPLI}.java ;\
${JDK_HOME}/bin/java -classpath ${TINI_HOME}/bin/tini.jar BuildDependency \
-add I2C400 -x ${TINI_HOME}/bin/owapi_dep.txt -p ${TINI_HOME}/bin/modules.jar \
-d ${TINI_HOME}/bin/tini.db -f ${APPLI}.class -o ${APPLI}.tini
 *
 *
 *
 * NOTICE : this application needs PWM from 80LC580 micro-P
 * switch GND VDD must be ON, SCL SDA OFF
 */
/*
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
;The following is a collection of procedures for the control of the LPH3827-1 ;
; code from LCD2.INC (OM5027 PROM)
;LCD module.                                                                  ;
;                                                                             ;
;LCD2_init         initialise LCD after powerup                               ;
;LCD2_clrdisplay   clear display                                              ;
;LCD2_WrCodeStr    send string from code space pointed to by dptr, sentinal 04;
;LCD2_WrDataStr    send string from IIC buffer starting at IICbuff+1, "  04   ;
;LCD2_singledata   send single ascii char from IICbuff at IICbuff+1           ;
;LCD2_home         home                                                       ;
;LCD2_newline      line no.in iicbuff+1                                       ;
;LCD2_indent       indents cursor, no. of spaces to indent in IICbuff+1       ;
;LCD2_clrline      clears present line and sets address to 1st space on line  ;
;readbf            delays until LCD not busy. Returns current DDRAM address   ;
;send1Cmnd         sends a single command                                     ;
;LCD2_shift        shifts display left (iicbuff+1 = 0), right (iicbuff+1 = 1) ;
;                              visible address range                          ;
;                      line 1:       00 to 13h                                ;
;                      line 2:       20 to 33h                                ;
;                      line 3:       40 to 53h                                ;
;                      line 4:       60 to 73h  (only visible when scrolled)  ;
;                                                                             ;
;lcd2_wrScr            writes a page of text (3 lines)  see below             ;
;lcd2_wrPage           "         "               "       "    "               ;
;lcd2_wrPageStr                                                               ;
;delayms                                                                      ;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
*/
import com.dalsemi.system.*;

/**
 * Simple test of the I2C :  uses LCD LPH3827-1 on Philipps OM5027 evaluation Board
 */
public class lcd2Display
{
final static byte initCmnd= 0x30;	//00110000b       ;initilise command
final static byte fnsetCmnd = 0x34;	//00110100b       ;function set command
final static byte dispsetCmnd = 0x0E;	//00001110b       ;display set command
final static byte readbfCtrl = 0x20;	//00100000b       ;read busy flag
final static byte dispOffCmnd = 0x08;	//00001000b       ;display off command
final static byte dispClrCmnd = 0x01;	//00000001b       ;clear display command

final static byte EntryCmnd = 0x06;	//00000110b       ;
final static byte homeCmnd = 0x02;	//00000010b       ;home cursor command
final static byte wrdataCtrl = 0x40;	//01000000b       ;write data
final static byte clrdispcmnd = 0x01;	//00000001b       ;


private I2CPort port;
private byte[] buffer;

lcd2Display()
{
    buffer = new byte[25];
    port = new I2CPort();
    port.setClockDelay((byte)2);
    port.setAddress((byte)(0x74 >> 1)); 	//CHIP-ON-GLASS 2116A DRIVER 	OK
}


private void send1Comnd(int n)
{
//buffer[0]= control byte
//buffer[1]=; command byte
try {
    port.write(buffer,0,n);
    }
catch (IllegalAddressException e){System.out.println("Illegal Address on Memory mapped I2C"+e);}
}

void LCD2_clrdisplay()
{
buffer[0]=0;buffer[1]=clrdispcmnd;
send1Comnd(2);
}
void LCD2_home()
{
buffer[0]=0;buffer[1]=homeCmnd;
send1Comnd(2);
}

void LCD2_singledata(char c)
{
buffer[0]=wrdataCtrl;buffer[1]=(byte) c;
send1Comnd(2);
}

void LCD2_WrCodeStr(String s)
{
buffer[0]=wrdataCtrl;
for (int i=0;i<s.length();i++)
	buffer[i+1]=(byte)s.charAt(i);
send1Comnd(s.length()+1);
}

//LCD2 = 3 lines x 13 chars
void LCD2_newline(int line)
{
buffer[0]=0;
switch (line) {
	case 1 : buffer[1]=(byte)0x91;
		break;
	case 2 : buffer[1]=(byte)0xa0;
		break;
	case 3 : buffer[1]=(byte)0xb4;
		break;
	default: return;
	}
send1Comnd(2);
}

void demo()
{
LCD2_clrdisplay();
LCD2_home();
LCD2_singledata('h');LCD2_singledata('e');LCD2_singledata('l');LCD2_singledata('l');LCD2_singledata('o');
LCD2_home();
LCD2_newline(3);
LCD2_WrCodeStr("apsii LGM");
int i=0;
while(true) {
	LCD2_newline(2);
	LCD2_WrCodeStr("  "+Integer.toString(i));
	try {Thread.sleep(500);} catch (InterruptedException e) {}
	i++;
	}
}

static void main(String[] args)
{ 
    System.out.println("Lcd2 Display V1.0 RP/LGM\nPhilips eval.board 0M5027");
    new lcd2Display().demo();
}
}