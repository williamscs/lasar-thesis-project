/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.example.android.BluetoothChat;

import java.util.Calendar;
import android.app.Activity;
import android.app.Dialog;
import android.app.TimePickerDialog;
import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.inputmethod.EditorInfo;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;
import android.widget.TimePicker;
import android.widget.Toast;
import android.widget.ToggleButton;

/**
 * This is the main Activity that displays the current chat session.
 */
public class LASARControl extends Activity implements OnSeekBarChangeListener
{
	// Debugging
	private static final String TAG = "BluetoothChat";
	private static final boolean D = true;
	// Message types sent from the BluetoothChatService Handler
	public static final int MESSAGE_STATE_CHANGE = 1;
	public static final int MESSAGE_READ = 2;
	public static final int MESSAGE_WRITE = 3;
	public static final int MESSAGE_DEVICE_NAME = 4;
	public static final int MESSAGE_TOAST = 5;
	
	
	//Update Constants
	private static final int BRIGHTNESS = 3;
	private static final int BLINDS = 2;
	private static final int CLOCK = 0;

	//Visibility Constants
	public static final int GONE = 8;
	public static final int VISIBLE = 0;

	// Key names received from the BluetoothChatService Handler
	public static final String DEVICE_NAME = "device_name";
	public static final String TOAST = "toast";


	private static final int REQUEST_ENABLE_BT = 3;

	// Layout Views
	private TextView mTitle;
	private ListView mConversationView;
	private EditText mOutEditText;

	// Button Variables
	private Button mSendButton;
	private Button mPickTime;
	private ToggleButton toggleButton1;

	// SeekBar Variables 
	private SeekBar dimSeek;
	private SeekBar blindsSeek;
	TextView mProgressText;
	TextView mProgressText2;

	// Variable Integers For Settings
	int dim = 50;
	int blinds = 50;

	// Time Setting Variables
	private TextView mTimeDisplay;
	private TextView mUpdateTime;
	
	//Values to be saved into microcontroller's
	// rudimentary Real-time Clock
	private int mHour;
	private int mMinute;
	private int mSecond;
	static final int TIME_DIALOG_ID = 0;
	
	//Values to be saved into microcontroller's
	// Preferences settings
	private String sHour;
	private String sMinute;
	private String sSecond;
	String SavedTime;

	//Bluetooth MAC ADDRESSES
	//DEBUGX [RN42-D5CC]: 00066645D5CC
	//ROOM A [RN42-D5CC]:   
	//ROOM B [RN42-D5CC]: 
	//ROOM C [RN42-D5CC]: 

	//[SAT2] = 00066645D5CA

	// Name of the connected device
	private String mConnectedDeviceName = null;
	// Array adapter for the conversation thread
	private ArrayAdapter<String> mConversationArrayAdapter;
	// String buffer for outgoing messages
	private StringBuffer mOutStringBuffer;
	// Local Bluetooth adapter
	private BluetoothAdapter mBluetoothAdapter = null;
	// Member object for the chat services
	private BluetoothChatService mChatService = null;


	@Override
	public void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		if(D) Log.e(TAG, "+++ ON CREATE +++");

		// Set up the window layout
		requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);
		setContentView(R.layout.roomselection);
		//requestWindowFeature(Window.FEATURE_NO_TITLE);
	   // getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN);
		getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE, R.layout.custom_title);



		// Get local Bluetooth adapter
		mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

		// If the adapter is null, then Bluetooth is not supported
		if (mBluetoothAdapter == null) {
			Toast.makeText(this, "Bluetooth is not available", Toast.LENGTH_LONG).show();
			finish();
			return;
		}


	}

	@Override
	protected Dialog onCreateDialog(int id) {
		switch (id) {
		case TIME_DIALOG_ID:
			return new TimePickerDialog(this,
					mTimeSetListener, mHour, mMinute, false);
		}
		return null;
	}

	/**
	 * updates the time we display in the TextView
	 */
	private void updateDisplay() {
		mTimeDisplay.setText(
				new StringBuilder()
				.append(pad(mHour)).append(":")
				.append(pad(mMinute)).append(":")
				.append(pad(mSecond)));

		sHour= Integer.toString(mHour);
		sMinute = Integer.toString(mMinute);
		sSecond = Integer.toString(mSecond);
		SavedTime = sHour+":"+sMinute+":"+sSecond;
	}

	/**
	 * the callback received when the user "sets" the time in the dialog
	 */
	private TimePickerDialog.OnTimeSetListener mTimeSetListener =
			new TimePickerDialog.OnTimeSetListener() {
		public void onTimeSet(TimePicker view, int hourOfDay, int minute) {
			mHour = hourOfDay;
			mMinute = minute;
			updateDisplay();
		}
	};

	/**
	 * Pads the value, if needed with a preceding zero
	 * @param c Value that must be padded
	 * @return
	 */
	private static String pad(int c) {
		if (c >= 10)
			return String.valueOf(c);
		else
			return "0" + String.valueOf(c);
	}

	/***********************
	  Checks onStart and onResume
	-------------------------
	 * -Internal Modules
	 * 	onStart()
	 * 	onResume()
	 ***********************/
	@Override
	public void onStart() {
		super.onStart();
		if(D) Log.e(TAG, "++ ON START ++");

		// If BT is not on, request that it be enabled.
		// setupChat() will then be called during onActivityResult
		if (!mBluetoothAdapter.isEnabled()) 
		{
			Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
			startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
		}
		//        // Otherwise, setup the chat session
		//         else {
		//            if (mChatService == null) setupChat();
		//        }
	}

	@Override
	public synchronized void onResume() {
		super.onResume();
		if(D) Log.e(TAG, "+ ON RESUME +");

		// Performing this check in onResume() covers the case in which BT was
		// not enabled during onStart(), so we were paused to enable it...
		// onResume() will be called when ACTION_REQUEST_ENABLE activity returns.
		//        if (mChatService != null) {
		//            // Only if the state is STATE_NONE, do we know that we haven't started already
		//            if (mChatService.getState() == BluetoothChatService.STATE_NONE) {
		//              // Start the Bluetooth chat services
		//              mChatService.start();
		//            }
		//}
	}
	/***********************
	SeekBar Progress Checking
	-------------------------
	 * -Internal Modules
	 * 	onProgressChanged()
	 * 	onStartTrackingTouch()
	 * 	onStopTrackingTouch()
	 * -Inputs
	 * 	seekBar
	 * 	progress
	 * 	fromTouch
	 ***********************/

	public void onProgressChanged(SeekBar seekBar, int progress, boolean fromTouch)
	{
		switch(seekBar.getId()){
		case R.id.dimBar:
			updateDim(progress);
			mProgressText.setText(""+progress);
			break;

		case R.id.blindsBar:
			updateBlinds(progress);
			mProgressText2.setText(""+progress);
			break;
		}

	}
	
	/*
	 * (non-Javadoc)
	 * @see android.widget.SeekBar.OnSeekBarChangeListener#onStopTrackingTouch(android.widget.SeekBar)
	 */
	public void onStopTrackingTouch(SeekBar seekBar) {

		switch(seekBar.getId()){
		case R.id.dimBar:
			updateSatellite(BRIGHTNESS, '\0');
			break;

		case R.id.blindsBar:
			updateSatellite(BLINDS, '\0');
			break;
		}
	}


	/***********************
	Toggle Button Checking
	-------------------------
	 * -Internal Modules
	 * 	addListenerOnButton()
	 * -Inputs
	 * 	toggleButton1
	 * -Outputs
	 * 	result.toString()
	 ***********************/

	/**
	 * 
	 */
	public void addListenerOnButton(){
		toggleButton1=(ToggleButton) findViewById(R.id.toggleButton1);
		toggleButton1.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {
				// Send a turn off message to the device
				String result = (toggleButton1.getText()).toString();
				if(result.compareToIgnoreCase("ON") == 0)
				{
					updateDim(99);
				}
				else if(result.compareToIgnoreCase("OFF") == 0)
				{
					updateDim(-1);
				}
				updateSatellite(BRIGHTNESS, '\0');
			}
		});
	}

	/***********************
	Chat Setup
	-------------------------
	 * -Internal Modules
	 * 	setupChat()
	 * 	onClick()
	 * -Inputs
	 * 	mConversationView
	 * 	mOutEditText
	 * 	mSendButton
	 * -Outputs
	 * 	message
	 * 	mConversationArrayAdapter
	 ***********************/
	/**
	 * Initializes all communications with the Bluetooth module
	 */
	private void setupChat() {
		Log.d(TAG, "setupChat()");

		// Initialize the array adapter for the conversation thread
		mConversationArrayAdapter = new ArrayAdapter<String>(this, R.layout.message);
		mConversationView = (ListView) findViewById(R.id.in);
		mConversationView.setAdapter(mConversationArrayAdapter);

		// Initialize the compose field with a listener for the return key
		mOutEditText = (EditText) findViewById(R.id.edit_text_out);
		mOutEditText.setOnEditorActionListener(mWriteListener);

		mSendButton = (Button) findViewById(R.id.button_pump);
		mSendButton.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {
				// Send a message using content of the edit text widget
				TextView view = (TextView) findViewById(R.id.edit_text_out);
				String message = view.getText().toString();
				sendMessage(message);
			}
		});

		// Initialize the BluetoothChatService to perform bluetooth connections
		mChatService = new BluetoothChatService(this, mHandler);

		// Initialize the buffer for outgoing messages
		mOutStringBuffer = new StringBuffer("");

		/***********************
        Time Updates
        -------------------------
		 * -Internal Modules
		 * 		onClick()
		 * 		updateDisplay
		 * -Inputs
		 * 		mHour
		 * 		mMinute
		 * 		mSendButton
		 * -Outputs
		 * 		Calendar
		 * 		message
		 ***********************/        

		//SEND THE UPDATED TIME    
		mUpdateTime = (Button) findViewById(R.id.button1);
		mUpdateTime.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {
				// Send a turn off message to the device
				final Calendar c = Calendar.getInstance();
				mHour = c.get(Calendar.HOUR_OF_DAY);
				mMinute = c.get(Calendar.MINUTE);
				mSecond = c.get(Calendar.SECOND);
				// display the current date
				updateDisplay();
				updateSatellite(CLOCK, '\0');
			}
		});

	}

	@Override
	public synchronized void onPause() {
		super.onPause();
		if(D) Log.e(TAG, "- ON PAUSE -");
	}

	@Override
	public void onStop() {
		super.onStop();
		if(D) Log.e(TAG, "-- ON STOP --");
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		// Stop the Bluetooth chat services
		if (mChatService != null) mChatService.stop();
		if(D) Log.e(TAG, "--- ON DESTROY ---");
	}


	/**
	 * Sends a message.
	 * @param message  A string of text to send.
	 */
	private void sendMessage(String message) {
		// Check that we're actually connected before trying anything
		if (mChatService.getState() != BluetoothChatService.STATE_CONNECTED) {
			Toast.makeText(this, R.string.not_connected, Toast.LENGTH_SHORT).show();
			return;
		}

		// Check that there's actually something to send
		if (message.length() > 0) {
			// Get the message bytes and tell the BluetoothChatService to write
			String sent_data = ""+message+";";//Termination of a string indicated by "."
			byte[] send = sent_data.getBytes();
			mChatService.write(send);


			// Reset out string buffer to zero and clear the edit text field
			mOutStringBuffer.setLength(0);
			mOutEditText.setText(mOutStringBuffer);
		}
	}

	// The action listener for the EditText widget, to listen for the return key
	private TextView.OnEditorActionListener mWriteListener =
			new TextView.OnEditorActionListener() {
		public boolean onEditorAction(TextView view, int actionId, KeyEvent event) {
			// If the action is a key-up event on the return key, send the message
			if (actionId == EditorInfo.IME_NULL && event.getAction() == KeyEvent.ACTION_UP) {
				String message = view.getText().toString();
				sendMessage(message);
			}
			if(D) Log.i(TAG, "END onEditorAction");
			return true;
		}
	};

	/**
	 * The Handler that gets information back from the BluetoothChatService
	 */
	private final Handler mHandler = new Handler()
	{
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case MESSAGE_STATE_CHANGE:
				if(D) Log.i(TAG, "MESSAGE_STATE_CHANGE: " + msg.arg1);
				switch (msg.arg1) {
				case BluetoothChatService.STATE_CONNECTED:
					mTitle.setText(R.string.title_connected_to);
					mTitle.append(mConnectedDeviceName);
					mConversationArrayAdapter.clear();
					break;
				case BluetoothChatService.STATE_CONNECTING:
					mTitle.setText(R.string.title_connecting);
					break;
				case BluetoothChatService.STATE_LISTEN:
				case BluetoothChatService.STATE_NONE:
					mTitle.setText(R.string.title_not_connected);
					break;
				}
				break;
			case MESSAGE_WRITE:
				byte[] writeBuf = (byte[]) msg.obj;
				// construct a string from the buffer
				String writeMessage = new String(writeBuf);
				mConversationArrayAdapter.add("Me:  " + writeMessage);
				break;
			case MESSAGE_READ:
				byte[] readBuf = (byte[]) msg.obj;
				// construct a string from the valid bytes in the buffer
				String readMessage = new String(readBuf, 0, msg.arg1);
				if(readMessage.indexOf(';') >= 0)
				{
					mConversationArrayAdapter.add(mConnectedDeviceName+":  " + readMessage);
					getInfo(readMessage);
				}
				break;
			case MESSAGE_DEVICE_NAME:
				// save the connected device's name
				mConnectedDeviceName = msg.getData().getString(DEVICE_NAME);
				Toast.makeText(getApplicationContext(), "Connected to "
						+ mConnectedDeviceName, Toast.LENGTH_SHORT).show();
				break;
			case MESSAGE_TOAST:
				Toast.makeText(getApplicationContext(), msg.getData().getString(TOAST),
						Toast.LENGTH_SHORT).show();
				break;
			}
		}
	};

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.option_menu, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.wake:
			updateSatellite(BLINDS | BRIGHTNESS, 'W');
			return true;
		case R.id.leave:
			updateSatellite(BLINDS | BRIGHTNESS, 'L');
			return true;
		case R.id.returned:
			updateSatellite(BLINDS | BRIGHTNESS, 'R');
			return true;
		case R.id.sleep:
			updateSatellite(BLINDS | BRIGHTNESS, 'S');
			return true;

		}
		return false;
	}

	public static String EXTRA_DEVICE_ADDRESS = "device_address";

	/**
	 * @param view
	 */
	public void toMain(View view)
	{
		String address = "00:06:66:45:D5:C5";
		setContentView(R.layout.main);
		//SetupSeekBars
		final TextView tvRoom = (TextView) findViewById(R.id.roomname);
		tvRoom.setText(  view.getContentDescription());
		if(tvRoom.getText().toString().compareToIgnoreCase("Living Room") == 0)
		{
			address = "00:06:66:45:D5:CC";
		}
		else if(tvRoom.getText().toString().compareToIgnoreCase("Bed Room") == 0)
		{
			address = "00:06:66:45:D5:CA";
		}
		else if(tvRoom.getText().toString().compareToIgnoreCase("Kitchen") == 0)
		{
			address = "00:06:66:45:D5:C4";
		}
		// Cancel discovery because it's costly and we're about to connect

		setupChat();
		mChatService.connect(mBluetoothAdapter.getRemoteDevice(address));


		dimSeek = (SeekBar)findViewById(R.id.dimBar);
		mProgressText = (TextView)findViewById(R.id.textView1);
		dimSeek.setOnSeekBarChangeListener(this);

		blindsSeek = (SeekBar)findViewById(R.id.blindsBar);
		mProgressText2 = (TextView)findViewById(R.id.textView2);
		blindsSeek.setOnSeekBarChangeListener(this);

		//Setup Toggle Button
		addListenerOnButton();

		// Set up the custom title
		mTitle = (TextView) findViewById(R.id.title_left_text);
		mTitle.setText(R.string.app_name);
		mTitle = (TextView) findViewById(R.id.title_right_text);

		/***********************
      	Clock Functionality
    	-------------------------
		 * -Internal Modules
		 * 	Dialog()
		 * 	updateDisplay()
		 * 	TimePicker()
		 * 	Pad()
		 * -inputs
		 * 	mTimeDisplay
		 * 	mPickTime
		 * 	mHour	
		 * 	mMinute
		 ***********************/

		// capture our View elements
		mTimeDisplay = (TextView) findViewById(R.id.textView3);
		mPickTime = (Button) findViewById(R.id.button_cmd3);

		// add a click listener to the button
		mPickTime.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				showDialog(TIME_DIALOG_ID);
			}
		});
		// get the current time
		final Calendar c = Calendar.getInstance();
		mHour = c.get(Calendar.HOUR_OF_DAY);
		mMinute = c.get(Calendar.MINUTE);
		// display the current date
		updateDisplay();

		//    	while(mChatService.getState() != 3);
		//    	
		//    	sendMessage("X");

	}

	@Override
	public void onBackPressed() 
	{
		setContentView(R.layout.roomselection);
		mChatService.stop();
		// do something on back.
		return;
	}

	/**
	 * @param info
	 */
	public void getInfo( String info )
	{
		int i = 0;

		if(info.indexOf('D') >= 0  && info.indexOf('X') >= 0)
		{
			int j = info.indexOf('D');
			info = info.substring(j+2);
			int sp = info.indexOf(':');
			try
			{
				updateDim( 100-Integer.parseInt((info.substring(0, sp))));
				i = i + sp + 1;
				sp = info.lastIndexOf(';');
				updateBlinds(Integer.parseInt(info.substring(i,sp)));
			}
			catch(NumberFormatException e)
			{
				Toast.makeText(this, "Bad Connection: Please try again.", Toast.LENGTH_LONG).show();
				setContentView(R.layout.roomselection);
				mChatService.stop();
			}
		}
		if(info.indexOf('F') >= 0 && info.indexOf(':') > 0)
		{
			int j = info.lastIndexOf('F');
			info = info.substring(j+2, info.lastIndexOf(';'));
			float x = Float.parseFloat(info);
			double k = (2.239 * x * x) + (8.8824 * x) + 43.068;
			Toast.makeText(this, String.valueOf(k) + " lux" , Toast.LENGTH_LONG).show();
		}
	}

	/**
	 * @param view
	 */
	public void getFreq(View view)
	{
		sendMessage("F");
	}
	
	/**
	 * @param new_dim
	 */
	private void updateDim( int new_dim )
	{
		if(new_dim <= 0)
		{
			dim = -1;
			dimSeek.setProgress(0);
		}
		else
		{
			dim = new_dim;
			dimSeek.setProgress(dim);
		}
	}
	
	/**
	 * @param new_blinds
	 */
	private void updateBlinds( int new_blinds )
	{
		if( new_blinds > 0)
			blinds = new_blinds;
		else
			blinds = 0;

		blindsSeek.setProgress(blinds);
	}
	
	/**
	 * Sends values to be updated on Satellite device
	 * @param toChange 
	 * @param alarm
	 */
	private void updateSatellite(int toChange, char alarm)
	{
		Calendar c = Calendar.getInstance();
		int hour = c.get(Calendar.HOUR_OF_DAY);
		int min = c.get(Calendar.MINUTE);
		int sec = c.get(Calendar.SECOND);
		String out = "";
		if( (toChange % BLINDS) == 0)
		{
			out = out+"B:"+blinds+":";
		}
		if( (toChange % BRIGHTNESS) == 0)
		{
			out = out+"D:";
			if((100-dim) < 10)
			{
					out= out+"0"+(100-dim);
			}
			else
			{
				if(dim > 0)
					out= out +""+ (100-dim);
				else
					out= out +""+ dim;
			}
			out = out + ":";
		}
		
		
		if(alarm != 0)
		{

			out = out + "C:"+sHour+":"+sMinute+":"+sSecond + ":";
			if(alarm == 'W')
			{
				out = out+"W";
			}
			else if(alarm == 'L')
			{
				out = out+"L";
			}
			else if(alarm == 'R')
			{
				out = out+"R";
			}
			else if(alarm == 'S')
			{
				out = out+"S";
			}
			
		}
		else
		{
			out = out+"C:"+hour+":"+min+":"+sec;
		}

		
		sendMessage(out);
	}

	@Override
	public void onStartTrackingTouch(SeekBar arg0) {
		// Auto-generated method stub
		
	}
}