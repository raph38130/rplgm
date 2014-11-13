package fr.lgm.client;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.ClickListener;
import com.google.gwt.user.client.ui.FlexTable;
import com.google.gwt.user.client.ui.Grid;
import com.google.gwt.user.client.ui.Image;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.Widget;

/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class LedDisp implements EntryPoint {
	//Label l;

	class Digit extends FlexTable implements ClickListener {
		public void onClick(Widget sender) {
			//l.setText(((Button) sender).getText());
			if (((Button) sender).getStyleName().toString().charAt(0) == 'h') {
				if (((Button) sender).getStyleName().charAt(1) == '0')
					((Button) sender).setStyleName("h-Button");
				else
					((Button) sender).setStyleName("h0-Button");
			} else {
				if (((Button) sender).getStyleName().charAt(1)== '0')
					((Button) sender).setStyleName("v-Button");
				else
					((Button) sender).setStyleName("v0-Button");
			}
			
		}

		Digit(String name) {
			Button b0 = new Button(name+"0"), b1 = new Button(name+"1"), b2 = new Button(
					name+"2"), b3 = new Button(name+"3"), b4 = new Button(name+"4"), b5 = new Button(
							name+"5"), b6 = new Button(name+"6");
			b0.addClickListener(this);
			b1.addClickListener(this);
			b2.addClickListener(this);
			b3.addClickListener(this);
			b4.addClickListener(this);
			b5.addClickListener(this);
			b6.addClickListener(this);

			b0.setStyleName("h-Button");
			b1.setStyleName("v-Button");
			b2.setStyleName("v-Button");
			b3.setStyleName("h-Button");
			b4.setStyleName("v-Button");
			b5.setStyleName("v-Button");
			b6.setStyleName("h-Button");

			setWidget(0, 1, b0);
			setWidget(1, 0, b1);
			setWidget(1, 2, b2);
			setWidget(2, 1, b3);
			setWidget(3, 0, b4);
			setWidget(3, 2, b5);
			setWidget(4, 1, b6);
		}
	}

	public void onModuleLoad() {
		//l = new Label("status = ");
		Digit d1 = new Digit("1");		
		Digit d2 = new Digit("2");
		Digit d3 = new Digit("3");
		Digit d4 = new Digit("4");
		
		RootPanel.get("slot1").add(d1);
		RootPanel.get("slot2").add(d2);
		RootPanel.get("slot3").add(d3);
		RootPanel.get("slot4").add(d4);
		//RootPanel.get("slot1").add(l);
		
	}

}
