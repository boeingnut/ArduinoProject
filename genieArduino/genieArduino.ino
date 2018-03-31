#include <genieArduino.h>


Genie genie;
#define RESETLINE 2  // Change this if you are not using an Arduino Adaptor Shield Version 2 (see code below)
static int gaugeVal = 25;

void setup()
{
  // NOTE, the genieBegin function (e.g. genieBegin(GENIE_SERIAL_0, 115200)) no longer exists.  Use a Serial Begin and serial port of your choice in
  // your code and use the genie.Begin function to send it to the Genie library (see this example below)
  // 200K Baud is good for most Arduinos. Galileo should use 115200.  
  Serial.begin(9600);  // Serial0 @ 200000 (200K) Baud
  genie.Begin(Serial);   // Use Serial0 for talking to the Genie Library, and to the 4D Systems display

  // genie.AttachEventHandler(myGenieEventHandler); // Attach the user function Event Handler for processing events

  // Reset the Display (change D4 to D2 if you have original 4D Arduino Adaptor)
  // THIS IS IMPORTANT AND CAN PREVENT OUT OF SYNC ISSUES, SLOW SPEED RESPONSE ETC
  // If NOT using a 4D Arduino Adaptor, digitalWrites must be reversed as Display Reset is Active Low, and
  // the 4D Arduino Adaptors invert this signal so must be Active High.  
  pinMode(RESETLINE, OUTPUT);  // Set D4 on Arduino to Output (4D Arduino Adaptor V2 - Display Reset)
  digitalWrite(RESETLINE, 1);  // Reset the Display via D4
  delay(100);
  digitalWrite(RESETLINE, 0);  // unReset the Display via D4

  delay (3500); //let the display start up after the reset (This is important)

  // Set the brightness/Contrast of the Display - (Not needed but illustrates how)
  // Most Displays, 1 = Display ON, 0 = Display OFF. See below for exceptions and for DIABLO16 displays.
  // For uLCD-43, uLCD-220RD, uLCD-70DT, and uLCD-35DT, use 0-15 for Brightness Control, where 0 = Display OFF, though to 15 = Max Brightness ON.
  genie.WriteContrast(10); 

  //Write a string to the Display to show the version of the library used
  //genie.WriteStr(0, GENIE_VERSION);
}

void loop()
{
  //static long waitPeriod = millis();
  //static int gaugeAddVal = 25; // Simulation code variable. Value to change the gauge by each loop
  //static int gaugeVal = 50; // Simulation code variable. Value to start the gauge at when powered on

  genie.DoEvents(); // This calls the library each loop to process the queued responses from the display
  genie.WriteObject(GENIE_OBJ_LED_DIGITS, 0, gaugeVal*101);
  genie.WriteObject(GENIE_OBJ_LED_DIGITS, 1, (gaugeVal+20)*101);
  genie.WriteObject(GENIE_OBJ_LED_DIGITS, 2, (gaugeVal+1)*101);
  genie.WriteObject(GENIE_OBJ_LED_DIGITS, 3, (gaugeVal+21)*101);
  genie.DoEvents();
  genie.WriteObject(GENIE_OBJ_LED_DIGITS, 4, (gaugeVal+2)*101);
  genie.WriteObject(GENIE_OBJ_LED_DIGITS, 5, (gaugeVal+22)*101);
  genie.WriteObject(GENIE_OBJ_LED_DIGITS, 6, (gaugeVal+3)*101);
  genie.WriteObject(GENIE_OBJ_LED_DIGITS, 7, (gaugeVal+23)*101);
  delay (200);
  genie.DoEvents();
  genie.WriteObject(GENIE_OBJ_COOL_GAUGE, 0, gaugeVal);
  genie.WriteObject(GENIE_OBJ_COOL_GAUGE, 1, gaugeVal+20);
  genie.WriteObject(GENIE_OBJ_COOL_GAUGE, 2, gaugeVal+1);
  genie.WriteObject(GENIE_OBJ_COOL_GAUGE, 3, gaugeVal+21);
  genie.DoEvents();
  genie.WriteObject(GENIE_OBJ_COOL_GAUGE, 4, gaugeVal+2);
  genie.WriteObject(GENIE_OBJ_COOL_GAUGE, 5, gaugeVal+22);
  genie.WriteObject(GENIE_OBJ_COOL_GAUGE, 6, gaugeVal+3);
  genie.WriteObject(GENIE_OBJ_COOL_GAUGE, 7, gaugeVal+23);
  delay (200);
  genie.DoEvents(); // This calls the library each loop to process the queued responses from the display
  genie.WriteObject(GENIE_OBJ_LED_DIGITS, 8, gaugeVal*101);
  genie.WriteObject(GENIE_OBJ_LED_DIGITS, 12, (gaugeVal+20)*101);
  genie.WriteObject(GENIE_OBJ_LED_DIGITS, 9, (gaugeVal+1)*101);
  genie.WriteObject(GENIE_OBJ_LED_DIGITS, 13, (gaugeVal+21)*101);
  genie.DoEvents();
  genie.WriteObject(GENIE_OBJ_LED_DIGITS, 10, (gaugeVal+2)*101);
  genie.WriteObject(GENIE_OBJ_LED_DIGITS, 14, (gaugeVal+22)*101);
  genie.WriteObject(GENIE_OBJ_LED_DIGITS, 11, (gaugeVal+3)*101);
  genie.WriteObject(GENIE_OBJ_LED_DIGITS, 15, (gaugeVal+23)*101);
  if (gaugeVal > 25)
  {
    genie.WriteObject(GENIE_OBJ_LED, 0, 1);
    
  }
  else
  {
    genie.WriteObject(GENIE_OBJ_LED, 0, 0);
    
  }
  // Simulation code, just to increment and decrement gauge value each loop, for animation
  // The results of this call will be available to myGenieEventHandler() after the display has responded
  // genie.ReadObject(GENIE_OBJ_USER_LED, 0); // Do a manual read from the UserLEd0 object

  
  gaugeVal = gaugeVal+1;
  if (gaugeVal > 50)
  {
    gaugeVal = 20;  
  }
  delay(500);
}

/////////////////////////////////////////////////////////////////////
//
// This is the user's event handler. It is called by genieDoEvents()
// when the following conditions are true
//
//		The link is in an IDLE state, and
//		There is an event to handle
//
// The event can be either a REPORT_EVENT frame sent asynchronously
// from the display or a REPORT_OBJ frame sent by the display in
// response to a READ_OBJ (genie.ReadObject) request.
//

/* COMPACT VERSION HERE, LONGHAND VERSION BELOW WHICH MAY MAKE MORE SENSE
void myGenieEventHandler(void)
{
  genieFrame Event;
  genieDequeueEvent(&Event);

  int slider_val = 0;

  //Filter Events from Slider0 (Index = 0) for a Reported Message from Display
  if (genieEventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_SLIDER, 0))
  {
    slider_val = genieGetEventData(&Event);  // Receive the event data from the Slider0
    genieWriteObject(GENIE_OBJ_LED_DIGITS, 0, slider_val);     // Write Slider0 value to to LED Digits 0
  }

  //Filter Events from UserLed0 (Index = 0) for a Reported Object from Display (triggered from genie.ReadObject in User Code)
  if (genieEventIs(&Event, GENIE_REPORT_OBJ,   GENIE_OBJ_USER_LED, 0))
  {
    bool UserLed0_val = genie.GetEventData(&Event);               // Receive the event data from the UserLed0
    UserLed0_val = !UserLed0_val;                                 // Toggle the state of the User LED Variable
    genie.WriteObject(GENIE_OBJ_USER_LED, 0, UserLed0_val);       // Write UserLed0_val value back to to UserLed0
  }
} */

// LONG HAND VERSION, MAYBE MORE VISIBLE AND MORE LIKE VERSION 1 OF THE LIBRARY
void myGenieEventHandler(void)
{
  genieFrame Event;
  genie.DequeueEvent(&Event); // Remove the next queued event from the buffer, and process it below

  int slider_val = 0;

  //If the cmd received is from a Reported Event (Events triggered from the Events tab of Workshop4 objects)
  if (Event.reportObject.cmd == GENIE_REPORT_EVENT)
  {
    if (Event.reportObject.object == GENIE_OBJ_SLIDER)                // If the Reported Message was from a Slider
    {
      if (Event.reportObject.index == 0)                              // If Slider0 (Index = 0)
      {
        slider_val = genie.GetEventData(&Event);                      // Receive the event data from the Slider0
        genie.WriteObject(GENIE_OBJ_LED_DIGITS, 0, slider_val);       // Write Slider0 value to to LED Digits 0
      }
    }
  }

  //If the cmd received is from a Reported Object, which occurs if a Read Object (genie.ReadOject) is requested in the main code, reply processed here.
  if (Event.reportObject.cmd == GENIE_REPORT_OBJ)
  {
    if (Event.reportObject.object == GENIE_OBJ_USER_LED)              // If the Reported Message was from a User LED
    {
      if (Event.reportObject.index == 0)                              // If UserLed0 (Index = 0)
      {
        bool UserLed0_val = genie.GetEventData(&Event);               // Receive the event data from the UserLed0
        UserLed0_val = !UserLed0_val;                                 // Toggle the state of the User LED Variable
        genie.WriteObject(GENIE_OBJ_USER_LED, 0, UserLed0_val);       // Write UserLed0_val value back to to UserLed0
      }
    }
  }

  /********** This can be expanded as more objects are added that need to be captured *************
  *************************************************************************************************
  Event.reportObject.cmd is used to determine the command of that event, such as an reported event
  Event.reportObject.object is used to determine the object type, such as a Slider
  Event.reportObject.index is used to determine the index of the object, such as Slider0
  genie.GetEventData(&Event) us used to save the data from the Event, into a variable.
  *************************************************************************************************/
}

