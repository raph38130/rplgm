/*
 * PlayPostedAudio.java - KLA, 03/25/2004
 */

/***************************************************************************
 *
 * Copyright (C) 2004 Dallas Semiconductor Corporation.
 * All rights Reserved. Printed in U.S.A.
 * This software is protected by copyright laws of
 * the United States and of foreign countries.
 * This material may also be protected by patent laws of the United States
 * and of foreign countries.
 * This software is furnished under a license agreement and/or a
 * nondisclosure agreement and may only be used or copied in accordance
 * with the terms of those agreements.
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Dallas Semiconductor retains all ownership rights.
 *
 ***************************************************************************/

import java.io.*;
import java.util.*;
import com.dalsemi.tininet.http.*;

public class PlayPostedAudio implements PostScript
{
  public static final int AUDIO_FLASH_ADDRESS = 0x480000;

  // how many canned audio messages do we have in flash?
  public static native int getNumberOfAudios(int address);

  // get the address of the indexed audio record 
  public static native int getAudioAddress(int address, int index);

  // get the length of the indexed audio record
  public static native int getAudioLength(int address, int index);

  // start playing the audio file at 'address' for 'length' bytes
  public static native int startAudio(int address, int length);

  // check to see if anything is playing right now
  public static native boolean isAudioPlaying();



  public void handlePost( Vector data, OutputStream out, HTTPWorker work ) throws IOException
  {
    // Inform user that we are handling the POST request
    System.out.println("Handling POST Request");

    StringBuffer strBuff;

    // Now create the response page

    // Write out the HTML headers and basic title text here
    strBuff = new StringBuffer ( "<HTML><title>TINI CANNED AUDIO PLAYER</title>\r\n"
                                 + "<BODY><CENTER><H1>TINI CANNED AUDIO PLAYER</H1>\r\n");


    PostElement pe = (PostElement)data.elementAt(0);
    int cannedindex = -1;
    try
    {
      cannedindex = Integer.parseInt(pe.value);
    }
    catch(Exception e)
    {
    }
 
    if (cannedindex==-1)
    {
       strBuff.append("Error: Posted value is not a valid number: "+pe.value+"<br>");
    }
    else
    {
      int num_files = getNumberOfAudios(AUDIO_FLASH_ADDRESS);
      if (cannedindex >= num_files)
      {
        strBuff.append("Sorry, there are only "+num_files+" audio files in my archive and you have selected to play number "+cannedindex);
        strBuff.append("<br><br>That wasn't very nice of you.<br>");
      }
      else
      { 
        if (isAudioPlaying())
        {
          strBuff.append("Sorry, but there is currently audio playing.<br>");
          strBuff.append("It would be rude of you to go now.  Wait your turn.<br>");
        }
        else
        {
          int audioaddress = getAudioAddress(AUDIO_FLASH_ADDRESS, cannedindex);
          //System.out.println("Audio file address: "+Integer.toHexString(audioaddress));

          int audiolength = getAudioLength(AUDIO_FLASH_ADDRESS, cannedindex);
          //System.out.println("Audio length: "+audiolength);

          startAudio(audioaddress, audiolength);
          strBuff.append("Playing canned audio sample #"+pe.value);
          //System.out.println("Started the audio");
          //if (!isAudioPlaying())
          //{
          //  System.out.println("Something is wrong, the audio is not playing!");
          //}
        }
      }
    }

    // Create HTML footers
    strBuff.append( "<br><a href=\"index.html\">BACK</a></CENTER></BODY></HTML>" );

    // Use HTTPWorker's encode body function to generate the URL encoded response
    out.write( work.encodeBody( HTTPServer.HTTP_OK, "OK", HTTPServer.MIME_TEXT_HTML, strBuff.length() ) );

    // Send out the bytes of our response page
    out.write( strBuff.toString().getBytes() );

    // Ensure that our data is written imediatly
    out.flush();

    out.close();

    // Inform user that we are done handling the POST request
    System.out.println("Finished handling POST request");
  }

}
