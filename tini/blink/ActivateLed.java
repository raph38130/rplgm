package tini;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Vector;

import com.dalsemi.tininet.http.HTTPWorker;
import com.dalsemi.tininet.http.PostElement;
import com.dalsemi.tininet.http.PostScript;


public class ActivateLed implements PostScript {
	
	public void handlePost(Vector arg0, OutputStream arg1, HTTPWorker arg2)
			throws IOException {
		
		for (int i=0; i< arg0.size();i++) {
			PostElement pe= (PostElement)arg0.elementAt(i);
			if (pe.field.equals("tempo")) WebLed.duration=Integer.parseInt(pe.value);
		}
		
		if ( ((PostElement)arg0.elementAt(0) ).field.equals("led"))
			WebLed.b = false;	
		else WebLed.b = true;
		if (WebLed.b)
			WebLed.t.suspend();
		else
			WebLed.t.resume();
		
	}
}
