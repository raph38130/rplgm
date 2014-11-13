import org.eclipse.swt.widgets.Composite;
/*
 * Created on 16 mars 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */

/**
 * @author raph
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class WebStepper extends Composite {

	public static void main(String[] args) {
		/* Before this is run, be sure to set up the following in the launch configuration 
		 * (Arguments->VM Arguments) for the correct SWT library path. 
		 * The following is a windows example:
		 * -Djava.library.path="installation_directory\plugins\org.eclipse.swt.win32_3.0.0\os\win32\x86"
		 */
		org.eclipse.swt.widgets.Display display = org.eclipse.swt.widgets.Display.getDefault();		
		org.eclipse.swt.widgets.Shell shell = new org.eclipse.swt.widgets.Shell(display);
		shell.setLayout(new org.eclipse.swt.layout.FillLayout());
		shell.setSize(new org.eclipse.swt.graphics.Point(300,200));
		WebStepper thisClass = new WebStepper(shell, org.eclipse.swt.SWT.NONE);
		shell.open();
		
		while (!shell.isDisposed()) {
			if (!display.readAndDispatch()) display.sleep ();
		}
		display.dispose();		
	}
	public WebStepper(Composite parent, int style) {
		super(parent, style);
		initialize();
	}
	private void initialize() {
		setSize(new org.eclipse.swt.graphics.Point(300,200));
	}
}
