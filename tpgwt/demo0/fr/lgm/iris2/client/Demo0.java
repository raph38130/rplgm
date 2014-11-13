package fr.lgm.iris2.client;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.ClickListener; //import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.MultiWordSuggestOracle;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.SuggestBox;
import com.google.gwt.user.client.ui.SuggestionEvent;
import com.google.gwt.user.client.ui.SuggestionHandler;
import com.google.gwt.user.client.ui.TextArea;
import com.google.gwt.user.client.ui.Widget;

/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class Demo0 implements EntryPoint {
	boolean b;

	/**
	 * This is the entry point method.
	 */
	public void onModuleLoad() {
		final Button button = new Button("OK");
		final TextArea ta = new TextArea();
		MultiWordSuggestOracle oracle = new MultiWordSuggestOracle();  
		   oracle.add("Belfils");
		   oracle.add("Beurdeley");
		   oracle.add("Bouclier");
		   oracle.add("Cayreyre");
		   oracle.add("Cesmat");
		   oracle.add("Delac");
		   oracle.add("Grobet");
		   oracle.add("Jean");
		   oracle.add("Michalet");	   
		
        final SuggestBox sb=new SuggestBox(oracle);
        
		ta.setVisibleLines(10);
		button.addClickListener(new ClickListener() {
			public void onClick(Widget sender) {
				ta.setText(ta.getText() + " clic");
				if (b) {
				    button.addStyleDependentName("On");
				    button.setText("START");
				}
				else {
					 button.removeStyleDependentName("On");
					 button.setText("STOP");
				}
				b = !b;
			}
		});

		
		sb.addEventHandler(new SuggestionHandler(){

			public void onSuggestionSelected(SuggestionEvent event) {
				ta.setText(ta.getText()+event.getSelectedSuggestion().getReplacementString());
				
			}});
		// Assume that the host HTML has elements defined whose
		// IDs are "slot1", "slot2".  In a real app, you probably would not want
		// to hard-code IDs.  Instead, you could, for example, search for all 
		// elements with a particular CSS class and replace them with widgets.
		//
		RootPanel.get("slot1").add(button);
		RootPanel.get("slot3").add(sb);
		RootPanel.get("slot4").add(ta);
	}
}
