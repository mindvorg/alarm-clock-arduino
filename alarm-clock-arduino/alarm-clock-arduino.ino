/***********************************************************************

Mini Clock v1.0, Jul 2014 by Nick Hall
Distributed under the terms of the GPL.

For help on how to build the clock see my blog:
http://123led.wordpress.com/

Tested on IDE v1.6.5

***********************************************************************/

//include libraries:
#include "LedControl.h"
#include <FontLEDClock.h>                // Font library
#include <Wire.h>                        // DS1307 clock
#include "RTClib.h"                      // DS1307 clock
#include <Button.h>                      // Button library by Alexander Brevig
//#include <game.h>
// Setup LED Matrix

LedControl lc = LedControl(11, 13, 9, 9); //sets the 3 pins as 12, 11 & 10 and then sets 4 displays (max is 8 displays)
//global variables
byte intensity = 7;                      // Default intensity/brightness (0-15)
byte clock_mode = 0;                     // Default clock mode. Default = 0 (basic_mode)
bool random_mode = 0;                    // Define random mode - changes the display type every few hours. Default = 0 (off)
byte old_mode = clock_mode;              // Stores the previous clock mode, so if we go to date or whatever, we know what mode to go back to after.
bool ampm = 0, music = 1;                           // Define 12 or 24 hour time. 0 = 24 hour. 1 = 12 hour
byte change_mode_time = 0;               // Holds hour when clock mode will next change if in random mode.
unsigned long delaytime = 500;           // We always wait a bit between updates of the display
int rtc[7];                              // Holds real time clock output
int alarmrtc[4] = { 99,99,99,99 };                         // Holds 
const int buzzer = 6;


int pinCS = 9;                               // ”казываем к какому выводу подключен контакт CS
int numberOfHorizontalDisplays = 1;          //  оличество матриц по горизонтали
int numberOfVerticalDisplays = 4;            //  оличество матриц по-вертикали
const int btnL = 5;
const int btnR = 3;
const int fire = 7;
int x = 3;
int tmp_x = x, tmp;
int y = 6;
int check, check_fire, posx;
int tmp_y = y;
int dead = 0;
int al_x = 1, al_y = 0;
bool start = 0, move_al = 0, lose = 0, the_end = 1;
const byte hf[8] = {
0b00111100,
0b01000010,
0b10100101,
0b10100101,
0b10000001,
0b01011010,
0b00100100,
0b00011000
};


const byte L[8] = { 0b00000000,0b01000000,0b01000000,0b01000000,0b01000000,0b01000000,0b01111110,0b00000000 };
const byte O[8] = { 0b00000000,0b01111110,0b01000010,0b01000010,0b01000010,0b01000010,0b01111110,0b00000000 };
const byte S[8] = { 0b00000000,0b01111110,0b01000000,0b01111110,0b0000010,0b00000010,0b01111110,0b00000000 };
const byte E[8] = { 0b00000000,0b01111110,0b01000000,0b01111110,0b01000000,0b01000000,0b01111110,0b00000000 };
const byte W[8] = { 0b00000000,0b01011010,0b01011010,0b01011010,0b01011010,0b00111100,0b00011000,0b00000000 };
const byte I[8] = { 0b00000000,0b00111100,0b00011000,0b00011000,0b00011000,0b00011000,0b00111100,0b00000000 };
const byte N[8] = { 0b00000000,0b01000110,0b01001010,0b01001010,0b01010010,0b01010010,0b01100010,0b00000000 };
bool life_aliens[5] = { true,true,true,true,true };


char days[7][4] = {
  "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
}; //day array - used in slide, basic_mode and jumble modes (The DS1307 outputs 1-7 values for day of week)
char daysfull[7][9] = {
  "Sunday", "Monday", "Tuesday", "Wed", "Thursday", "Friday", "Saturday"
};
char suffix[4][3] = {
  "st", "nd", "rd", "th"
};  //date suffix array, used in slide, basic_mode and jumble modes. e,g, 1st 2nd ...

//define constants
#define NUM_DISPLAY_MODES 1              // Number display modes (conting zero as the first mode)
#define NUM_SETTINGS_MODES 5             // Number settings modes = 6 (conting zero as the first mode)
#define SLIDE_DELAY 20                   // The time in milliseconds for the slide effect per character in slide mode. Make this higher for a slower effect
#define cls          clear_display       // Clear display

RTC_DS1307 ds1307;                              // Create RTC object

Button buttonA = Button(5, BUTTON_PULLUP);      // Setup button A (using button library)
Button buttonB = Button(7, BUTTON_PULLUP);      // Setup button B (using button library)
void midi() {
	tone(buzzer, 195, 346.1535);   delay(384.615);    tone(buzzer, 195, 173.07675);   delay(192.3075);    tone(buzzer, 293, 173.07675);    delay(192.3075);    tone(buzzer, 261, 346.1535);    delay(384.615);    tone(buzzer, 233, 346.1535);    delay(384.615);   tone(buzzer, 220, 346.1535);    delay(384.615);    tone(buzzer, 220, 173.07675);   delay(192.3075);    tone(buzzer, 220, 173.07675);    delay(192.3075);    tone(buzzer, 261, 346.1535);    delay(384.615);    tone(buzzer, 233, 173.07675);    delay(192.3075);   tone(buzzer, 220, 173.07675);    delay(192.3075);    tone(buzzer, 195, 346.1535);   delay(384.615);    tone(buzzer, 146, 173.07675);    delay(192.3075);    tone(buzzer, 233, 173.07675);    delay(192.3075);    tone(buzzer, 220, 173.07675);    delay(192.3075);   tone(buzzer, 233, 173.07675);    delay(192.3075);    tone(buzzer, 220, 173.07675);   delay(192.3075);    tone(buzzer, 233, 173.07675);    delay(192.3075);    tone(buzzer, 195, 346.1535);    delay(384.615);    tone(buzzer, 146, 173.07675);    delay(192.3075);   tone(buzzer, 233, 173.07675);    delay(192.3075);    tone(buzzer, 220, 173.07675);   delay(192.3075);    tone(buzzer, 233, 173.07675);    delay(192.3075);    tone(buzzer, 220, 173.07675);    delay(192.3075);    for (int i = 0; i < 5; i++) { tone(buzzer, 233, 173.07675);    delay(192.3075); } for (int i = 0; i < 4; i++) { tone(buzzer, 293, 173.07675); delay(192.3075); }  for (int i = 0; i < 4; i++) { tone(buzzer, 261, 173.07675);    delay(192.3075); }    for (int i = 0; i < 4; i++) { tone(buzzer, 349, 173.07675);   delay(192.3075); }       for (int i = 0; i < 8; i++) { tone(buzzer, 391, 173.07675);   delay(192.3075); }tone(buzzer, 783, 692.307);    delay(769.23);   tone(buzzer, 261, 173.07675);    delay(192.3075);    tone(buzzer, 233, 173.07675);   delay(192.3075);    tone(buzzer, 220, 173.07675);    delay(192.3075);    tone(buzzer, 174, 173.07675);    delay(192.3075);    tone(buzzer, 195, 346.1535);    delay(384.615);   tone(buzzer, 195, 173.07675);    delay(192.3075);    tone(buzzer, 293, 173.07675);   delay(192.3075);    tone(buzzer, 261, 346.1535);    delay(384.615);    tone(buzzer, 233, 346.1535);    delay(384.615);    tone(buzzer, 220, 346.1535);    delay(384.615);   tone(buzzer, 220, 173.07675);    delay(192.3075);    tone(buzzer, 220, 173.07675);   delay(192.3075);    tone(buzzer, 261, 346.1535);    delay(384.615);    tone(buzzer, 233, 173.07675);    delay(192.3075);    tone(buzzer, 220, 173.07675);    delay(192.3075);   tone(buzzer, 195, 346.1535);    delay(384.615);    tone(buzzer, 146, 173.07675);   delay(192.3075);    tone(buzzer, 233, 173.07675);    delay(192.3075);    tone(buzzer, 220, 173.07675);    delay(192.3075);    tone(buzzer, 233, 173.07675);    delay(192.3075);   tone(buzzer, 220, 173.07675);    delay(192.3075);    tone(buzzer, 233, 173.07675);   delay(192.3075);    tone(buzzer, 195, 346.1535);    delay(384.615);    tone(buzzer, 146, 173.07675);    delay(192.3075);    tone(buzzer, 233, 173.07675);    delay(192.3075);   tone(buzzer, 220, 173.07675);    delay(192.3075);    tone(buzzer, 233, 173.07675);   delay(192.3075);    tone(buzzer, 220, 173.07675);    delay(192.3075);    tone(buzzer, 233, 173.07675);    delay(192.3075);    tone(buzzer, 195, 346.1535);    delay(384.615);   tone(buzzer, 195, 173.07675);    delay(192.3075);    tone(buzzer, 293, 173.07675);   delay(192.3075);    tone(buzzer, 261, 346.1535);    delay(384.615);    tone(buzzer, 233, 346.1535);    delay(384.615);    tone(buzzer, 220, 346.1535);    delay(384.615);   tone(buzzer, 220, 173.07675);    delay(192.3075);    tone(buzzer, 220, 173.07675);   delay(192.3075);    tone(buzzer, 261, 346.1535);    delay(384.615);    tone(buzzer, 233, 173.07675);    delay(192.3075);    tone(buzzer, 220, 173.07675);    delay(192.3075);   tone(buzzer, 195, 346.1535);    delay(384.615);    tone(buzzer, 97, 173.07675);   delay(192.3075);    tone(buzzer, 233, 173.07675);    delay(192.3075);    tone(buzzer, 220, 173.07675);    delay(192.3075);    tone(buzzer, 233, 173.07675);    delay(192.3075);   tone(buzzer, 220, 173.07675);    delay(192.3075);    tone(buzzer, 233, 173.07675);   delay(192.3075);    tone(buzzer, 195, 346.1535);    delay(384.615);    tone(buzzer, 97, 173.07675);    delay(192.3075);    tone(buzzer, 233, 173.07675);    delay(192.3075);   tone(buzzer, 220, 173.07675);    delay(192.3075);    tone(buzzer, 233, 173.07675);   delay(192.3075);    tone(buzzer, 220, 173.07675);    delay(192.3075);    tone(buzzer, 233, 173.07675);    delay(192.3075);    tone(buzzer, 195, 346.1535);    delay(384.615);   tone(buzzer, 195, 173.07675);    delay(192.3075);    tone(buzzer, 293, 173.07675);   delay(192.3075);    tone(buzzer, 261, 346.1535);    delay(384.615);    tone(buzzer, 233, 346.1535);    delay(384.615);    tone(buzzer, 220, 346.1535);    delay(384.615);   tone(buzzer, 220, 173.07675);    delay(192.3075);    tone(buzzer, 220, 173.07675);   delay(192.3075);    tone(buzzer, 261, 346.1535);    delay(384.615);    tone(buzzer, 233, 173.07675);    delay(192.3075);    tone(buzzer, 220, 173.07675);    delay(192.3075);   tone(buzzer, 195, 346.1535);    delay(384.615);    tone(buzzer, 97, 173.07675);   delay(192.3075);    tone(buzzer, 466, 173.07675);
	music = 0;
}
void setup() {

	digitalWrite(5, HIGH);                 // turn on pullup resistor for button on pin 2
	digitalWrite(7, HIGH);                 // turn on pullup resistor for button on pin 3
	digitalWrite(4, HIGH);                 // turn on pullup resistor for button on pin 4
	pinMode(btnL, INPUT_PULLUP);
	pinMode(btnR, INPUT_PULLUP);
	pinMode(fire, INPUT_PULLUP);
	pinMode(buzzer, OUTPUT);
	Serial.begin(9600); //start serial

	//initialize the 4 matrix panels
	//we have already set the number of devices when we created the LedControl
	int devices = lc.getDeviceCount();
	//we have to init all devices in a loop
	for (int address = 0; address < devices; address++) {
		/*The MAX72XX is in power-saving mode on startup*/
		lc.shutdown(3 - address, false);
		/* Set the brightness to a medium values */
		lc.setIntensity(3 - address, intensity);
		/* and clear the display */
		lc.clearDisplay(3 - address);
	}

	//Setup DS1307 RTC
#ifdef AVR
	Wire.begin();
#else
	Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino
#endif
	ds1307.begin(); //start RTC Clock

	if (!ds1307.isrunning()) {
		Serial.println("RTC is NOT running!");
		ds1307.adjust(DateTime(__DATE__, __TIME__));  // sets the RTC to the date & time this sketch was compiled
	}
	//Show software version & hello message
	printver();

	//enable red led
	digitalWrite(13, HIGH);
}

void loop() {

	//    rtc[2] = now.hour();
   // rtc[1] = now.minute();
   //if (alarmrtc[1]==rtc[1]&&alarmrtc[2]==rtc[2])     Serial.println("1111111111111111111111111111111111"); else   Serial.println("000000000000000000000000000");
   // Serial.print("alarm1=");Serial.print(alarmrtc[1]);Serial.print("real=");Serial.print(rtc[1]);Serial.println(" ");
	//run the clock with whatever mode is set by clock_mode - the default is set at top of code.
	switch (clock_mode) {

	case 0://  if (alarmrtc[1]==rtc[1]&&alarmrtc[2]==rtc[2])     Serial.println("111111111"); else   Serial.println("00000000");
		basic_mode();
		break;
	case 1:
		small_mode();
		break;
	case 2:
		setup_menu();
		// slide(); 
		break;
		/* case 3:
		   word_clock(); //u can find another part of code in web but here's I need to add game sry guys
		   break;
		 case 4:

		   break;
		 //case 5:
		// set_alarm();
		// break;*/
	}
}


//plot a point on the display
void plot(byte x, byte y, byte val) {

	//select which matrix depending on the x coord
	byte address;
	if (x >= 0 && x <= 7) {
		address = 3;
	}
	if (x >= 8 && x <= 15) {
		address = 2;
		x = x - 8;
	}
	if (x >= 16 && x <= 23) {
		address = 1;
		x = x - 16;
	}
	if (x >= 24 && x <= 31) {
		address = 0;
		x = x - 24;
	}

	if (val == 1) {
		lc.setLed(address, y, x, true);
	}
	else {
		lc.setLed(address, y, x, false);
	}
}


//clear screen
void clear_display() {
	for (byte address = 0; address < 4; address++) {
		lc.clearDisplay(address);
	}
}

//fade screen down
void fade_down() {

	//fade from global intensity to 1
	for (byte i = intensity; i > 0; i--) {
		for (byte address = 0; address < 4; address++) {
			lc.setIntensity(address, i);
		}
		delay(30); //change this to change fade down speed
	}

	clear_display(); //clear display completely (off)

	//reset intentsity to global val
	for (byte address = 0; address < 4; address++) {
		lc.setIntensity(address, intensity);
	}
}



//power up led test & display software version number
void printver() {

	byte i = 0;
	char ver_a[8] = "MADE";
	char ver_b[8] = "IN";
	char ver_c[8] = "ETU";

	//test all leds.
	for (byte x = 0; x <= 32; x++) {
		for (byte y = 0; y <= 7; y++) {
			plot(x, y, 1);
		}
	}
	delay(300);
	fade_down();

	while (ver_a[i]) {
		puttinychar((i * 4), 1, ver_a[i]);
		delay(35);
		i++;
	}
	delay(500);
	fade_down();
	i = 0;
	while (ver_b[i]) {
		puttinychar((i * 4), 1, ver_b[i]);
		delay(35);
		i++;
	}
	delay(500);
	fade_down();
	i = 0;
	while (ver_c[i]) {
		puttinychar((i * 4), 1, ver_c[i]);
		delay(35);
		i++;
	}
	delay(500);
	fade_down();
}


// puttinychar
// Copy a 3x5 character glyph from the myfont data structure to display memory, with its upper left at the given coordinate
// This is unoptimized and simply uses plot() to draw each dot.
void puttinychar(byte x, byte y, char c)
{
	byte dots;
	if (c >= 'A' && c <= 'Z' || (c >= 'a' && c <= 'z')) {
		c &= 0x1F;   // A-Z maps to 1-26
	}
	else if (c >= '0' && c <= '9') {
		c = (c - '0') + 32;
	}
	else if (c == ' ') {
		c = 0; // space
	}
	else if (c == '.') {
		c = 27; // full stop
	}
	else if (c == ':') {
		c = 28; // colon
	}
	else if (c == '\'') {
		c = 29; // single quote mark
	}
	else if (c == '!') {
		c = 30; // single quote mark
	}
	else if (c == '?') {
		c = 31; // single quote mark
	}

	for (byte col = 0; col < 3; col++) {
		dots = pgm_read_byte_near(&mytinyfont[c][col]);
		for (char row = 0; row < 5; row++) {
			if (dots & (16 >> row))
				plot(x + col, y + row, 1);
			else
				plot(x + col, y + row, 0);
		}
	}
}



void putnormalchar(byte x, byte y, char c)
{

	byte dots;
	//  if (c >= 'A' && c <= 'Z' || (c >= 'a' && c <= 'z') ) {
	//    c &= 0x1F;   // A-Z maps to 1-26
	//  }
	if (c >= 'A' && c <= 'Z') {
		c &= 0x1F;   // A-Z maps to 1-26
	}
	else if (c >= 'a' && c <= 'z') {
		c = (c - 'a') + 41;   // A-Z maps to 41-67
	}
	else if (c >= '0' && c <= '9') {
		c = (c - '0') + 31;
	}
	else if (c == ' ') {
		c = 0; // space
	}
	else if (c == '.') {
		c = 27; // full stop
	}
	else if (c == '\'') {
		c = 28; // single quote mark
	}
	else if (c == ':') {
		c = 29; // clock_mode selector arrow
	}
	else if (c == '>') {
		c = 30; // clock_mode selector arrow
	}
	else if (c >= -80 && c <= -67) {
		c *= -1;
	}

	for (char col = 0; col < 5; col++) {
		dots = pgm_read_byte_near(&myfont[c][col]);
		for (char row = 0; row < 7; row++) {
			//check coords are on screen before trying to plot
			//if ((x >= 0) && (x <= 31) && (y >= 0) && (y <= 7)){

			if (dots & (64 >> row)) {   // only 7 rows.
				plot(x + col, y + row, 1);
			}
			else {
				plot(x + col, y + row, 0);
			}
			//}
		}
	}
}

//small_mode
//show the time in small 3x5 characters with seconds display

void small_mode() {

	char textchar[8]; // the 16 characters on the display
	byte mins = 100; //mins
	byte secs = rtc[0]; //seconds
	byte old_secs = secs; //holds old seconds value - from last time seconds were updated o display - used to check if seconds have changed

	cls();

	//run clock main loop as long as run_mode returns true
	while (run_mode()) {

		get_time();
		if (alarmrtc[1] == rtc[1] && alarmrtc[2] == rtc[2]) { obnulenie(); game(); }
		//check for button press
		if (buttonA.uniquePress()) {
			switch_mode();
			return;
		}
		if (buttonB.uniquePress()) {
			display_date();
			return;
		}

		//if secs changed then update them on the display
		secs = rtc[0];
		if (secs != old_secs) {

			//secs
			char buffer[3];
			itoa(secs, buffer, 10);

			//fix - as otherwise if num has leading zero, e.g. "03" secs, itoa coverts this to chars with space "3 ".
			if (secs < 10) {
				buffer[1] = buffer[0];
				buffer[0] = '0';
			}

			puttinychar(20, 1, ':'); //seconds colon
			puttinychar(24, 1, buffer[0]); //seconds
			puttinychar(28, 1, buffer[1]); //seconds
			old_secs = secs;
		}

		//if minute changes change time
		if (mins != rtc[1]) {

			//reset these for comparison next time
			mins = rtc[1];
			byte hours = rtc[2];
			if (hours > 12) {
				hours = hours - ampm * 12;
			}
			if (hours < 1) {
				hours = hours + ampm * 12;
			}


			//byte dow  = rtc[3]; // the DS1307 outputs 0 - 6 where 0 = Sunday0 - 6 where 0 = Sunday.
			//byte date = rtc[4];

			//set characters
			char buffer[3];
			itoa(hours, buffer, 10);

			//fix - as otherwise if num has leading zero, e.g. "03" hours, itoa coverts this to chars with space "3 ".
			if (hours < 10) {
				buffer[1] = buffer[0];
				//if we are in 12 hour mode blank the leading zero.
				if (ampm) {
					buffer[0] = ' ';
				}
				else {
					buffer[0] = '0';
				}
			}
			//set hours chars
			textchar[0] = buffer[0];
			textchar[1] = buffer[1];
			textchar[2] = ':';

			itoa(mins, buffer, 10);
			if (mins < 10) {
				buffer[1] = buffer[0];
				buffer[0] = '0';
			}
			//set mins characters
			textchar[3] = buffer[0];
			textchar[4] = buffer[1];

			//do seconds
			textchar[5] = ':';
			buffer[3];
			secs = rtc[0];
			itoa(secs, buffer, 10);

			//fix - as otherwise if num has leading zero, e.g. "03" secs, itoa coverts this to chars with space "3 ".
			if (secs < 10) {
				buffer[1] = buffer[0];
				buffer[0] = '0';
			}
			//set seconds
			textchar[6] = buffer[0];
			textchar[7] = buffer[1];

			byte x = 0;
			byte y = 0;

			//print each char
			for (byte x = 0; x < 6; x++) {
				puttinychar(x * 4, 1, textchar[x]);
			}
		}
		delay(50);
	}
	fade_down();
}


// basic_mode()
// show the time in 5x7 characters
void basic_mode()
{
	cls();

	char buffer[3];   //for int to char conversion to turn rtc values into chars we can print on screen
	byte offset = 0;  //used to offset the x postition of the digits and centre the display when we are in 12 hour mode and the clock shows only 3 digits. e.g. 3:21
	byte x, y;        //used to draw a clear box over the left hand "1" of the display when we roll from 12:59 -> 1:00am in 12 hour mode.

	//do 12/24 hour conversion if ampm set to 1
	byte hours = rtc[2];

	if (hours > 12) {
		hours = hours - ampm * 12;
	}
	if (hours < 1) {
		hours = hours + ampm * 12;
	}

	//do offset conversion
	if (ampm && hours < 10) {
		offset = 2;
	}

	//set the next minute we show the date at
	//set_next_date();

	// initially set mins to value 100 - so it wll never equal rtc[1] on the first loop of the clock, meaning we draw the clock display when we enter the function
	byte secs = 100;
	byte mins = 100;
	int count = 0;

	//run clock main loop as long as run_mode returns true
	while (run_mode()) {

		//get the time from the clock chip
		get_time();
		if (alarmrtc[1] == rtc[1] && alarmrtc[2] == rtc[2]) { obnulenie(); game(); }
		//check for button press
		if (buttonA.uniquePress()) {
			switch_mode();
			return;
		}
		if (buttonB.uniquePress()) {
			display_date();
			return;
		}

		//check whether it's time to automatically display the date
		//check_show_date();

		//draw the flashing : as on if the secs have changed.
		if (secs != rtc[0]) {

			//update secs with new value
			secs = rtc[0];

			//draw :
			plot(15 - offset, 2, 1); //top point
			plot(15 - offset, 5, 1); //bottom point
			count = 400;
		}

		//if count has run out, turn off the :
		if (count == 0) {
			plot(15 - offset, 2, 0); //top point
			plot(15 - offset, 5, 0); //bottom point
		}
		else {
			count--;
		}

		//re draw the display if button pressed or if mins != rtc[1] i.e. if the time has changed from what we had stored in mins, (also trigggered on first entering function when mins is 100)
		if (mins != rtc[1]) {

			//update mins and hours with the new values
			mins = rtc[1];
			hours = rtc[2];

			//adjust hours of ampm set to 12 hour mode
			if (hours > 12) {
				hours = hours - ampm * 12;
			}
			if (hours < 1) {
				hours = hours + ampm * 12;
			}

			itoa(hours, buffer, 10);

			//if hours < 10 the num e.g. "3" hours, itoa coverts this to chars with space "3 " which we dont want
			if (hours < 10) {
				buffer[1] = buffer[0];
				buffer[0] = '0';
			}

			//print hours
			//if we in 12 hour mode and hours < 10, then don't print the leading zero, and set the offset so we centre the display with 3 digits.
			if (ampm && hours < 10) {
				offset = 2;

				//if the time is 1:00am clear the entire display as the offset changes at this time and we need to blank out the old 12:59
				if ((hours == 1 && mins == 0)) {
					cls();
				}
			}
			else {
				//else no offset and print hours tens digit
				offset = 0;

				//if the time is 10:00am clear the entire display as the offset changes at this time and we need to blank out the old 9:59
				if (hours == 10 && mins == 0) {
					cls();
				}


				putnormalchar(1, 0, buffer[0]);
			}
			//print hours ones digit
			putnormalchar(7 - offset, 0, buffer[1]);


			//print mins
			//add leading zero if mins < 10
			itoa(mins, buffer, 10);
			if (mins < 10) {
				buffer[1] = buffer[0];
				buffer[0] = '0';
			}
			//print mins tens and ones digits
			putnormalchar(19 - offset, 0, buffer[0]);
			putnormalchar(25 - offset, 0, buffer[1]);
		}
	}
	fade_down();
}


/// scroll message - not used at present - too slow.
void scroll() {

	char message[] = { "Hello There " };

	cls();
	byte p = 6;      //current pos in string
	byte chara[] = { 0, 1, 2, 3, 4, 5 }; //chars from string
	int x[] = { 0, 6, 12, 18, 24, 30 }; //xpos for each char
	byte y = 0;                   //y pos

	// clear_buffer();

	while (message[p] != '\0') {

		//draw all 6 chars
		for (byte c = 0; c < 6; c++) {

			putnormalchar(x[c], y, message[chara[c]]);


			//draw a line of pixels turned off after each char,otherwise the gaps between the chars have pixels left in them from the previous char
			for (byte yy = 0; yy < 8; yy++) {
				plot(x[c] + 5, yy, 0);
			}

			//take one off each chars position
			x[c] = x[c] - 1;
		}

		//reset a char if it's gone off screen
		for (byte i = 0; i <= 5; i++) {
			if (x[i] < -5) {
				x[i] = 31;
				chara[i] = p;
				p++;
			}
		}
	}
}

//display_date - print the day of week, date and month with a flashing cursor effect
void display_date()
{

	cls();
	//read the date from the DS1307

	byte dow = rtc[3]; // day of week 0 = Sunday
	byte date = rtc[4];
	byte month = rtc[5] - 1;

	//array of month names to print on the display. Some are shortened as we only have 8 characters across to play with
	char monthnames[12][9] = {
	  "January", "February", "March", "April", "May", "June", "July", "August", "Sept", "October", "November", "December"
	};

	//print the day name

	//get length of text in pixels, that way we can centre it on the display by divindin the remaining pixels b2 and using that as an offset
	byte len = 0;
	while (daysfull[dow][len]) {
		len++;
	};
	byte offset = (31 - ((len - 1) * 4)) / 2; //our offset to centre up the text

	//print the name     
	int i = 0;
	while (daysfull[dow][i])
	{
		puttinychar((i * 4) + offset, 1, daysfull[dow][i]);
		i++;
	}
	delay(1000);
	fade_down();
	cls();

	// print date numerals
	char buffer[3];
	itoa(date, buffer, 10);
	offset = 10; //offset to centre text if 3 chars - e.g. 3rd

	// first work out date 2 letter suffix - eg st, nd, rd, th etc
	// char suffix[4][3]={"st", "nd", "rd", "th"  }; is defined at top of code
	byte s = 3;
	if (date == 1 || date == 21 || date == 31) {
		s = 0;
	}
	else if (date == 2 || date == 22) {
		s = 1;
	}
	else if (date == 3 || date == 23) {
		s = 2;
	}

	//print the 1st date number
	puttinychar(0 + offset, 1, buffer[0]);

	//if date is under 10 - then we only have 1 digit so set positions of sufix etc one character nearer
	byte suffixposx = 4;

	//if date over 9 then print second number and set xpos of suffix to be 1 char further away
	if (date > 9) {
		suffixposx = 8;
		puttinychar(4 + offset, 1, buffer[1]);
		offset = 8; //offset to centre text if 4 chars
	}

	//print the 2 suffix characters
	puttinychar(suffixposx + offset, 1, suffix[s][0]);
	puttinychar(suffixposx + 4 + offset, 1, suffix[s][1]);

	delay(1000);
	fade_down();

	//print the month name 

	//get length of text in pixels, that way we can centre it on the display by divindin the remaining pixels b2 and using that as an offset
	len = 0;
	while (monthnames[month][len]) {
		len++;
	};
	offset = (31 - ((len - 1) * 4)) / 2; //our offset to centre up the text
	i = 0;
	while (monthnames[month][i])
	{
		puttinychar((i * 4) + offset, 1, monthnames[month][i]);
		i++;
	}

	delay(1000);
	fade_down();
}


//dislpay menu to change the clock mode
void switch_mode() {

	//remember mode we are in. We use this value if we go into settings mode, so we can change back from settings mode (6) to whatever mode we were in.
	old_mode = clock_mode;

	char* modes[] = {
	  "Basic", "Small", "Setup"
	};

	byte next_clock_mode;
	byte firstrun = 1;

	//loop waiting for button (timeout after 35 loops to return to mode X)
	for (int count = 0; count < 35; count++) {

		//if user hits button, change the clock_mode
		if (buttonA.uniquePress() || firstrun == 1) {

			count = 0;
			cls();

			if (firstrun == 0) {
				clock_mode++;
			}
			if (clock_mode > NUM_DISPLAY_MODES + 1) {
				clock_mode = 0;
			}

			//print arrown and current clock_mode name on line one and print next clock_mode name on line two
			char str_top[9];

			//strcpy (str_top, "-");
			strcpy(str_top, modes[clock_mode]);

			next_clock_mode = clock_mode + 1;
			if (next_clock_mode > NUM_DISPLAY_MODES + 1) {
				next_clock_mode = 0;
			}

			byte i = 0;
			while (str_top[i]) {
				putnormalchar(i * 6, 0, str_top[i]);
				i++;
			}
			firstrun = 0;
		}
		delay(50);
	}
}


//run clock main loop as long as run_mode returns true
byte run_mode() {

	//if random mode is on... check the hour when we change mode.
	if (random_mode) {
		//if hour value in change mode time = hours. then reurn false = i.e. exit mode.
		if (change_mode_time == rtc[2]) {
			//set the next random clock mode and time to change it
			set_next_random();
			//exit the current mode.
			return 0;
		}
	}
	//else return 1 - keep running in this mode
	return 1;
}


//set the next hour the clock will change mode when random mode is on
void set_next_random() {

	//set the next hour the clock mode will change - current time plus 1 - 4 hours
	get_time();
	change_mode_time = rtc[2] + random(1, 5);

	//if change_mode_time now happens to be over 23, then set it to between 1 and 3am
	if (change_mode_time > 23) {
		change_mode_time = random(1, 4);
	}

	//set the new clock mode
	clock_mode = random(0, NUM_DISPLAY_MODES + 1);  //pick new random clock mode
}


//dislpay menu to change the clock settings
void setup_menu() {

	char* set_modes[] = {
	   "Rndom", "24 Hr","Set", "Brght", "Alarm","Exit" };
	if (ampm == 0) {
		set_modes[1] = ("12 Hr");
	}

	byte setting_mode = 0;
	byte next_setting_mode;
	byte firstrun = 1;

	//loop waiting for button (timeout after 35 loops to return to mode X)
	for (int count = 0; count < 35; count++) {

		//if user hits button, change the clock_mode
		if (buttonA.uniquePress() || firstrun == 1) {

			count = 0;
			cls();

			if (firstrun == 0) {
				setting_mode++;
			}
			if (setting_mode > NUM_SETTINGS_MODES) {
				setting_mode = 0;
			}

			//print arrown and current clock_mode name on line one and print next clock_mode name on line two
			char str_top[9];

			strcpy(str_top, set_modes[setting_mode]);

			next_setting_mode = setting_mode + 1;
			if (next_setting_mode > NUM_SETTINGS_MODES) {
				next_setting_mode = 0;
			}

			byte i = 0;
			while (str_top[i]) {
				putnormalchar(i * 6, 0, str_top[i]);
				i++;
			}

			firstrun = 0;
		}
		delay(50);
	}

	//pick the mode 
	switch (setting_mode) {
	case 0:
		set_random();
		break;
	case 1:
		set_ampm();
		break;
	case 2:
		set_time();
		break;
	case 3:
		set_intensity();
		break;
	case 4:
		set_alarm();
		break;
	}

	//change the clock from mode 6 (settings) back to the one it was in before 
	clock_mode = old_mode;
}


//toggle random mode - pick a different clock mode every few hours
void set_random() {
	cls();

	char text_a[9] = "Off";
	char text_b[9] = "On";
	byte i = 0;

	//if random mode is on, turn it off
	if (random_mode) {

		//turn random mode off
		random_mode = 0;

		//print a message on the display
		while (text_a[i]) {
			putnormalchar((i * 6), 0, text_a[i]);
			i++;
		}
	}
	else {
		//turn randome mode on. 
		random_mode = 1;

		//set hour mode will change
		set_next_random();

		//print a message on the display
		while (text_b[i]) {
			putnormalchar((i * 6), 0, text_b[i]);
			i++;
		}
	}
	delay(1500); //leave the message up for a second or so
}



//set 12 or 24 hour clock
void set_ampm() {

	// AM/PM or 24 hour clock mode - flip the bit (makes 0 into 1, or 1 into 0 for ampm mode)
	ampm = (ampm ^ 1);
	cls();
}


//change screen intensityintensity
void set_intensity() {

	cls();

	byte i = 0;
	char text[7] = "Bright";
	while (text[i]) {
		puttinychar((i * 4) + 4, 0, text[i]);
		i++;
	}

	//wait for button input
	while (!buttonA.uniquePress()) {

		levelbar(0, 6, (intensity * 2) + 2, 2);    //display the intensity level as a bar
		while (buttonB.isPressed()) {

			if (intensity == 15) {
				intensity = 0;
				cls();
			}
			else {
				intensity++;
			}
			//print the new value 
			i = 0;
			while (text[i]) {
				puttinychar((i * 4) + 4, 0, text[i]);
				i++;
			}

			//display the intensity level as a bar
			levelbar(0, 6, (intensity * 2) + 2, 2);

			//change the brightness setting on the displays
			for (byte address = 0; address < 4; address++) {
				lc.setIntensity(address, intensity);
			}
			delay(150);
		}
	}
}


// display a horizontal bar on the screen at offset xposr by ypos with height and width of xbar, ybar
void levelbar(byte xpos, byte ypos, byte xbar, byte ybar) {
	for (byte x = 0; x < xbar; x++) {
		for (byte y = 0; y <= ybar; y++) {
			plot(x + xpos, y + ypos, 1);
		}
	}
}


//set time and date routine
void set_time() {
	cls();
	//fill settings with current clock values read from clock
	get_time();
	byte set_min = rtc[1];
	byte set_hr = rtc[2];
	byte set_date = rtc[4];
	byte set_mnth = rtc[5];
	int  set_yr = rtc[6];
	//Set function - we pass in: which 'set' message to show at top, current value, reset value, and rollover limit.
	set_date = set_value(2, set_date, 1, 31);
	set_mnth = set_value(3, set_mnth, 1, 12);
	set_yr = set_value(4, set_yr, 2013, 2099);
	set_hr = set_value(1, set_hr, 0, 23);
	set_min = set_value(0, set_min, 0, 59);
	ds1307.adjust(DateTime(set_yr, set_mnth, set_date, set_hr, set_min));
	cls();
}
//used to set min, hr, date, month, year values. pass 
//message = which 'set' message to print, 
//current value = current value of property we are setting
//reset_value = what to reset value to if to rolls over. E.g. mins roll from 60 to 0, months from 12 to 1
//rollover limit = when value rolls over
int set_value(byte message, int current_value, int reset_value, int rollover_limit) {

	cls();
	char messages[6][17] = {
	  "Set Mins", "Set Hour", "Set Day", "Set Mnth", "Set Year" };

	//Print "set xyz" top line
	byte i = 0;
	while (messages[message][i])
	{
		puttinychar(i * 4, 1, messages[message][i]);
		i++;
	}

	delay(2000);
	cls();

	//print digits bottom line
	char buffer[5] = "    ";
	itoa(current_value, buffer, 10);
	puttinychar(0, 1, buffer[0]);
	puttinychar(4, 1, buffer[1]);
	puttinychar(8, 1, buffer[2]);
	puttinychar(12, 1, buffer[3]);
	delay(300);
	//wait for button input
	while (!buttonA.uniquePress()) {
		while (buttonB.isPressed()) {
			if (current_value < rollover_limit) {
				current_value++;
			}
			else {
				current_value = reset_value;
			}
			//print the new value
			itoa(current_value, buffer, 10);
			puttinychar(0, 1, buffer[0]);
			puttinychar(4, 1, buffer[1]);
			puttinychar(8, 1, buffer[2]);
			puttinychar(12, 1, buffer[3]);
			delay(150);
		}
	}
	return current_value;
}



void set_alarm() {
	cls();
	//fill settings with current clock values read from clock
	get_time();
	byte set_alarm_min = rtc[1];

	//alarmrtc[7]; 
	byte set_alarm_hr = rtc[2];
	//Set function - we pass in: which 'set' message to show at top, current value, reset value, and rollover limit.
	alarmrtc[2] = set_value_alarm(1, set_alarm_hr, 0, 23);
	alarmrtc[1] = set_value_alarm(0, set_alarm_min, 0, 59);
	cls();
	Serial.print("alarmrtc="); Serial.print(alarmrtc[1]);
}
//used to set min, hr, date, month, year values. pass 
//message = which 'set' message to print, 
//current value = current value of property we are setting
//reset_value = what to reset value to if to rolls over. E.g. mins roll from 60 to 0, months from 12 to 1
//rollover limit = when value rolls over
int set_value_alarm(byte message, int current_value, int reset_value, int rollover_limit) {
	cls();
	char messages[3][10] = {
	  "Set Mins", "Set Hour" };
	//Print "set xyz" top line
	byte i = 0;
	while (messages[message][i])
	{
		puttinychar(i * 4, 1, messages[message][i]);
		i++;
	}
	delay(2000);
	cls();
	//print digits bottom line
	char buffer[5] = "    ";
	itoa(current_value, buffer, 10);
	puttinychar(0, 1, buffer[0]);
	puttinychar(4, 1, buffer[1]);
	puttinychar(8, 1, buffer[2]);
	puttinychar(12, 1, buffer[3]);
	delay(300);
	//wait for button input
	while (!buttonA.uniquePress()) {
		while (buttonB.isPressed()) {
			if (current_value < rollover_limit) {
				current_value++; Serial.print("current_value="); Serial.print(current_value);
			}
			else {
				current_value = reset_value;
			}
			//print the new value
			itoa(current_value, buffer, 10);
			puttinychar(0, 1, buffer[0]);
			puttinychar(4, 1, buffer[1]);
			puttinychar(8, 1, buffer[2]);
			puttinychar(12, 1, buffer[3]);
			delay(150);
		}
	}
	return current_value;
}





void get_time()
{
	//get time
	DateTime now = ds1307.now();
	//save time to array
	rtc[6] = now.year();
	rtc[5] = now.month();
	rtc[4] = now.day();
	rtc[3] = now.dayOfWeek(); //returns 0-6 where 0 = Sunday
	rtc[2] = now.hour();
	rtc[1] = now.minute();
	rtc[0] = now.second();

	//flash arduino led on pin 13 every second
	//if ( (rtc[0] % 2) == 0) {
	//  digitalWrite(13, HIGH);
	//}
	//else {
	//  digitalWrite(13, LOW);
	//}
	//print the time to the serial port - useful for debuging RTC issues
}








void obnulenie()
{//start=0;the_end=1;check=0;music=1;dead=0;
	start = 0;
	the_end = 1;
	dead = 0;
	check = 0;
	music = 1;
	for (int i = 0; i < 5; i++) { life_aliens[i] = true; }
	al_x = 1;
	al_y = 0;
	x = 3;
	lose = 0;
}
//GAME


bool alines_move()
{
	if (al_y != 6)
	{//надо добавить, чтоб не просто по самому крайнему, а по самому крайнему живому

		if (((abs(al_x) + 8 == 31 || abs(al_x) + 8 == 0) && life_aliens[4] != false) || ((abs(al_x) + 6 == 33 || abs(al_x) + 6 == 0) && life_aliens[3] != false) || ((abs(al_x) + 4 == 33 || abs(al_x) + 4 == 0) && life_aliens[2] != false) || ((abs(al_x) + 2 == 33 || abs(al_x) + 2 == 0) && life_aliens[1] != false) || ((abs(al_x) == 33 || abs(al_x) == 0) && life_aliens[0] != false))
		{
			move_al = 1; al_x = al_x * -1; al_y++;
		}//if we reach right corner we chould go down and then go left 
		if (life_aliens[0] == 1)
		{
			lc.setLed(3 - ((abs(al_x) - 1) / 8), al_y, (abs(al_x) - 1) % 8, false);
			lc.setLed(3 - ((abs(al_x)) / 8), al_y, (abs(al_x)) % 8, true);


		}
		if (life_aliens[1] == 1)
		{
			lc.setLed(3 - ((abs(al_x) + 1) / 8), al_y, (abs(al_x) + 1) % 8, false);
			lc.setLed(3 - ((abs(al_x) + 2) / 8), al_y, (abs(al_x) + 2) % 8, true);

		}
		if (life_aliens[2] == 1)
		{
			lc.setLed(3 - ((abs(al_x) + 3) / 8), al_y, (abs(al_x) + 3) % 8, false);
			lc.setLed(3 - ((abs(al_x) + 4) / 8), al_y, (abs(al_x) + 4) % 8, true);
		}
		if (life_aliens[3] == 1)
		{
			lc.setLed(3 - ((abs(al_x) + 5) / 8), al_y, (abs(al_x) + 5) % 8, false);
			lc.setLed(3 - ((abs(al_x) + 6) / 8), al_y, (abs(al_x) + 6) % 8, true);

		}
		if (life_aliens[4] == 1)
		{
			lc.setLed(3 - ((abs(al_x) + 7) / 8), al_y, (abs(al_x) + 7) % 8, false);
			lc.setLed(3 - ((abs(al_x) + 8) / 8), al_y, (abs(al_x) + 8) % 8, true);

		}
		// matrix.write();
		al_x++;
	}
	else lose = 1;
	return  lose;
	delay(1000);
}


void drawLose()
{

	lc.clearDisplay(0);  lc.clearDisplay(1);  lc.clearDisplay(2);  lc.clearDisplay(3);
	for (int i = 0; i < 8; i++) {            // ѕередача массива
		lc.setRow(3, i, L[i]);
		lc.setRow(2, i, O[i]);
		lc.setRow(1, i, S[i]);
		lc.setRow(0, i, E[i]);


	}

}
void anim_shoot(int x, int y)
{
	for (int i = 0; i < 2; i++)
	{
		lc.setLed(3 - (abs(x) / 8), y, abs(x) % 8, true);//    lc.setLed(3-(abs(al_x)/8),al_y,abs(al_x)%8,true);
		delay(600);
		tone(buzzer, 262, 100);    // ¬ключаем светодиод
		lc.setLed(3 - (abs(x) / 8), y, abs(x) % 8, false);//  lc.setLed(3-(abs(al_x)/8),al_y,abs(al_x)%8,false);


		delay(200);
	}
}
void cood(int i)
{
	Serial.println("-");
	Serial.print("cood of shoot x and y ");
	Serial.print(al_x);        Serial.print(" ");
	Serial.print(al_y);       Serial.print(" index of alien ");
	Serial.print(i);
}
void shoot(int posx)
{
	for (int i = 6; i > -1; i--)
	{
		tmp_y = i + 1;
		lc.setLed(3 - (abs(posx) / 8), tmp_y, abs(posx) % 8, false);
		lc.setLed(3 - (abs(posx) / 8), i, abs(posx) % 8, true);
		delay(200);
		// когда + то на общее слева, когда - то попадание при слева и мигает через одно слева
		if (al_x < 0) tmp = 1; else tmp = -1;
		if (posx == abs(al_x) + tmp && i == al_y && life_aliens[0] != false) { cood(0); anim_shoot(abs(al_x) + tmp, al_y); al_x = al_x * -1; dead++; life_aliens[0] = false; }
		else
			if (posx == (abs(al_x) + 2 + tmp) && i == al_y && life_aliens[1] != false) { cood(1); anim_shoot(abs(al_x) + 2 + tmp, al_y); al_x = al_x * -1; dead++; life_aliens[1] = false; }
			else
				if (posx == (abs(al_x) + 4 + tmp) && i == al_y && life_aliens[2] != false) { cood(2); anim_shoot(abs(al_x) + 4 + tmp, al_y); al_x = al_x * -1; dead++; life_aliens[2] = false; }
				else
					if (posx == (abs(al_x) + 6 + tmp) && i == al_y && life_aliens[3] != false) { cood(3); anim_shoot(abs(al_x) + 6 + tmp, al_y); al_x = al_x * -1; dead++; life_aliens[3] = false; }
					else
						if (posx == (abs(al_x) + 8 + tmp) && i == al_y && life_aliens[4] != false) { cood(4); anim_shoot(abs(al_x) + 8 + tmp, al_y); al_x = al_x * -1; dead++; life_aliens[4] = false; }
	}

	lc.setLed(3 - (abs(al_x) / 8), al_y, abs(posx) % 8, false);



}
void game() {
	noTone(6);
	// ќбнуление матрицы
	lc.clearDisplay(0);  lc.clearDisplay(1);  lc.clearDisplay(2);  lc.clearDisplay(3);
	while (!start)
	{

		for (int i = 0; i < 8; i++) {            // ѕередача массива

			lc.setRow(2, i, hf[i]);
			lc.setRow(1, i, hf[i]);


		}
		delay(2000);
		tone(buzzer, 900);
		if ((digitalRead(btnL) != HIGH) || (digitalRead(btnR) != HIGH) || (digitalRead(fire) != HIGH))
		{
			lc.clearDisplay(0);  lc.clearDisplay(1);  lc.clearDisplay(2);  lc.clearDisplay(3);
			start = 1;
		}

	}
	while (the_end)//нажата кнопка
	{
		noTone(6);

		lc.clearDisplay(0);  lc.clearDisplay(1);  lc.clearDisplay(2);  lc.clearDisplay(3);
		if (dead == 5)//убиты ли все
		{

			lc.clearDisplay(0);  lc.clearDisplay(1);  lc.clearDisplay(2);  lc.clearDisplay(3);
			for (int i = 0; i < 8; i++) {            // ѕередача массива

				lc.setRow(3, i, W[i]);
				lc.setRow(2, i, I[i]);
				lc.setRow(1, i, N[i]);
			}delay(60000);
			the_end = 0;

			lc.clearDisplay(0);  lc.clearDisplay(1);  lc.clearDisplay(2);  lc.clearDisplay(3);
		}
		else
			if (check)//дошли ли инопришеленцы до конца
			{
				the_end = 0;

				lc.clearDisplay(0);  lc.clearDisplay(1);  lc.clearDisplay(2);  lc.clearDisplay(3);
				for (int i = 0; i < 8; i++) {            // ѕередача массива
					lc.setRow(3, i, L[i]);
					lc.setRow(2, i, O[i]);
					lc.setRow(1, i, S[i]);
					lc.setRow(0, i, E[i]);


				}
				if (music) {
					midi(); midi();

					lc.clearDisplay(0);  lc.clearDisplay(1);  lc.clearDisplay(2);  lc.clearDisplay(3);
				}
			}
			else
			{
				if ((digitalRead(btnL) != HIGH) && (x > 0))
				{
					tmp_x = x;
					x--;
				}
				if ((digitalRead(btnR) != HIGH) && (x < 31))
				{

					tmp_x = x;
					x++;
				}

				lc.setLed(3 - ((tmp_x - 1) / 8), y + 1, (tmp_x - 1) % 8, false);
				lc.setLed(3 - ((x - 1) / 8), y + 1, (x - 1) % 8, true);

				lc.setLed(3 - ((tmp_x + 1) / 8), y + 1, (tmp_x + 1) % 8, false);
				lc.setLed(3 - ((x + 1) / 8), y + 1, (x + 1) % 8, true);

				lc.setLed(3 - ((tmp_x) / 8), y, (tmp_x) % 8, false);
				lc.setLed(3 - (x / 8), y, x % 8, true);



				check = alines_move();

				if (digitalRead(fire) == LOW)//работающий выстрел как в морском бое
				   //надо сделать, чтобы он не внутри себ€ циклов двигалось, а внешним лупом двигалось
				{
					noTone(6); tone(buzzer, 1047, 100); tone(buzzer, 1109, 100); delay(100); noTone(6);
					shoot(x);
				}

				delay(200);
			}



	}
}