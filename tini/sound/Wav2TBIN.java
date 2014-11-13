import java.io.*;
import java.util.*;

public class Wav2TBIN
{
	static void printUsage()
	{
		System.out.println("Wav2TBIN");
		System.out.println("");
		System.out.println("Converts one or more WAV files to a TBIN formatted");
		System.out.println("archive.");
		System.out.println("");
		System.out.println("Usage: java Wav2TBIN TBINFILENAME ADDRESS WAVFILE1 WAVFILE2 ...");
		System.out.println("where");
		System.out.println("   TBINFILENAME Name of the TBIN file to be written");
		System.out.println("   ADDRESS      Hex starting address for the TBIN file");
		System.out.println("   WAVFILEN     One or more WAV files that will be added");
		System.out.println("                to the TBIN file.");

	}

	public static void main(String[] args) throws Exception
	{
		if (args.length < 3)
		{
			printUsage();
			return;
		}

		String tbinfilename = args[0];
		int starting_address = Integer.parseInt(args[1], 16);
		WAVFile[] waves = new WAVFile[args.length - 2];
		
		for (int i=0;i<waves.length;i++)
		{
			waves[i] = new WAVFile(args[i+2]);

			waves[i].printStructure();
		}

		// first write the number of wav files in this archive
		FileOutputStream fileout = new FileOutputStream(tbinfilename);
		TBINOutputStream tout = new TBINOutputStream(fileout, starting_address);
		tout.write(waves.length >> 8);
		tout.write(waves.length);

		int address = starting_address + 2;
		// now write the address offsets.  note 3 bytes for an address and 3 for a length per entry
		address += (waves.length * 6);

		for (int i=0;i<waves.length;i++)
		{
			tout.write(address >> 16);
			tout.write(address >> 8);
			tout.write(address);
			int thislength = waves[i].getAudioData().length;
			address += thislength;
			// we want the number of byte pairs
			thislength = thislength >> 1;
			tout.write(thislength >> 16);
			tout.write(thislength >> 8);
			tout.write(thislength);
		}

		// now write the audio data
		for (int i=0;i<waves.length;i++)
		{
			byte[] audiodata = waves[i].getAudioData();
			tout.write(audiodata, 0, audiodata.length);
		}

		tout.flush();
		tout.close();
	}
}

