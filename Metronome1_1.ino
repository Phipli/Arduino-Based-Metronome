
/* Phil Cranmer - Aug 2024
 *  
 *  Current Bugs
 *  - In Spd change mode, the metronome sound multitriggers.
 *  - 
 *  
 *  Things to implement
 *  - 
 *  - LED menu - Select if always show current bpm, blink bpm, centre zero, or visual side to side
 *  - Alternative rhythms, Yamaha, syncopated 
 *  - Rhythm parameter (how much to lead on some beats etc)
 *  - Quantise tap to nearest 1, 5, 10 bmp?
 *  - Hold button during power on to reset, plus reset menu option.
 *  - Save current settings to EEPROM
 *  - Battery status
 */

#include <PCM.h>
// this uses pin 11

const unsigned char sample[] PROGMEM = {
  0x82, 0x7C, 0x83, 0x7B, 0x83, 0x79, 0x88, 0x77, 0x81, 0x65, 0x48, 0xFF, 0x7F, 0x3E, 0xAC, 0x4F, 
  0x00, 0xC4, 0xEA, 0xBB, 0x84, 0x08, 0xBE, 0x67, 0x45, 0x8D, 0xBC, 0xB4, 0x61, 0x64, 0x6D, 0x92, 
  0x33, 0x64, 0xFE, 0xB1, 0x52, 0xA6, 0x67, 0x1D, 0x80, 0x87, 0xB8, 0x96, 0x46, 0x92, 0x7C, 0x43, 
  0x90, 0xC6, 0x95, 0x9C, 0x7E, 0x5A, 0x57, 0x44, 0x89, 0xBA, 0x91, 0x7C, 0x99, 0x61, 0x53, 0x8B, 
  0x8D, 0xAF, 0x8F, 0x79, 0x7A, 0x55, 0x4B, 0x7D, 0xA0, 0x8D, 0xA2, 0x89, 0x76, 0x6C, 0x69, 0x8F, 
  0x95, 0x87, 0x8C, 0x8A, 0x58, 0x5C, 0x76, 0x80, 0x9A, 0x99, 0x96, 0x87, 0x6B, 0x67, 0x7C, 0x83, 
  0x89, 0x8F, 0x86, 0x73, 0x68, 0x74, 0x73, 0x86, 0x99, 0x96, 0x91, 0x7A, 0x70, 0x70, 0x73, 0x82, 
  0x8A, 0x8B, 0x7B, 0x74, 0x79, 0x77, 0x7C, 0x89, 0x96, 0x94, 0x81, 0x7A, 0x71, 0x64, 0x78, 0x8A, 
  0x8C, 0x81, 0x7F, 0x80, 0x7A, 0x79, 0x7F, 0x92, 0x8B, 0x83, 0x80, 0x77, 0x70, 0x6D, 0x81, 0x88, 
  0x83, 0x84, 0x87, 0x81, 0x76, 0x7F, 0x84, 0x86, 0x81, 0x7B, 0x7E, 0x7D, 0x73, 0x73, 0x87, 0x85, 
  0x87, 0x89, 0x81, 0x78, 0x7F, 0x8A, 0x75, 0x85, 0x7D, 0x7B, 0x81, 0x70, 0x82, 0x82, 0x82, 0x84, 
  0x84, 0x7F, 0x82, 0x87, 0x77, 0x80, 0x7C, 0x7C, 0x85, 0x78, 0x81, 0x7D, 0x7A, 0x7F, 0x7F, 0x83, 
  0x88, 0x80, 0x7F, 0x84, 0x78, 0x82, 0x80, 0x7E, 0x80, 0x78, 0x81, 0x83, 0x7E, 0x81, 0x81, 0x81, 
  0x82, 0x84, 0x7F, 0x79, 0x7D, 0x80, 0x7F, 0x7A, 0x7F, 0x82, 0x84, 0x81, 0x81, 0x84, 0x7E, 0x79, 
  0x7E, 0x88, 0x7E, 0x7B, 0x7E, 0x7E, 0x7F, 0x7F, 0x7C, 0x83, 0x88, 0x7E, 0x82, 0x82, 0x7B, 0x7C, 
  0x80, 0x7B, 0x83, 0x80, 0x7D, 0x82, 0x7E, 0x7D, 0x83, 0x80, 0x80, 0x87, 0x79, 0x80, 0x7F, 0x7C, 
  0x7B, 0x84, 0x7D, 0x81, 0x86, 0x79, 0x86, 0x7C, 0x7E, 0x84, 0x7F, 0x7E, 0x87, 0x79, 0x7B, 0x84, 
  0x7A, 0x83, 0x82, 0x78, 0x83, 0x89, 0x79, 0x81, 0x7D, 0x7A, 0x85, 0x7D, 0x7F, 0x85, 0x81, 0x79, 
  0x7E, 0x7F, 0x80, 0x84, 0x80, 0x85, 0x7F, 0x7A, 0x7F, 0x82, 0x80, 0x7E, 0x83, 0x80, 0x7E, 0x77, 
  0x7F, 0x81, 0x7F, 0x81, 0x83, 0x84, 0x7C, 0x7D, 0x7E, 0x81, 0x82, 0x82, 0x80, 0x81, 0x77, 0x7C, 
  0x83, 0x7E
};

// define the different states
#define metroMode 0
#define tapMode 1
#define menuModeBPM 2
#define menuModeLED 3

#define displayLocation 17
#define segUnits 17
#define segTens 18
#define segHuns 19

#define Button 8
#define myLED 9
#define rot1 A0
#define rot2 A1

#define startDelay 3000
#define shortTime 100

unsigned long nextEvent = 0;
int bpm = 120;
char bpmC[] = { 1, 2, 0 };
char txtC[] = { 's', 'p', 'd' };
char dig = 0;
char clickCount = 0;
int clickGaps[] = { 500, 500, 500, 500 };
unsigned long lastClick = 0;
unsigned long lastGap = 500;
unsigned long startup;
int counter = 0;
char LEDMode = 0;

char mode = 0;

void setup() {
  pinMode(Button,INPUT); // Rotary Encoder Click - Need a hardware 10k pulldown
  pinMode(rot1, INPUT);
  pinMode(rot2, INPUT);
  pinMode(segUnits,OUTPUT); // digit unit
  pinMode(segTens,OUTPUT); // digit tens
  pinMode(segHuns,OUTPUT); // digit hundreds
  pinMode(myLED,OUTPUT);
  // High is disabled with the PNP transistors I'm using
  digitalWrite(segUnits,HIGH);
  digitalWrite(segTens,HIGH);
  digitalWrite(segHuns,HIGH);
  DDRD = B11111111; // set all of port D to output - the 7 seg is connected here
  PORTD = segClear(); // set all high, which is off.
  nextEvent = millis(); // schedules next even to happen immediately
  startup = nextEvent;
}

void loop() {
  if( mode == metroMode ){ ///********************* Metronome Mode
    // Idle Activity
    timeToTick(true, true);

    // Check for User Input
    if(digitalRead(rot1)==HIGH){
      mode = menuModeBPM;
      startup = millis()-startDelay; // put us outside the startDelay period
    }
    if(digitalRead(Button)){
      nextEvent = millis();
      lastClick = nextEvent;
      timeToTick(true, false); // next event is now one tick in the future!
      screenOff();
      delay(20); // debounce
      while(digitalRead(Button)){
        // wait until user releases the button
        if(millis()-lastClick>=2000 && mode == metroMode){ // beep once to show we're entering the mode
          mode = menuModeBPM;
          tone(11,500, 50);
          startup = millis()-startDelay; // put us outside the startDelay period
          txtC[0] = 's';
          txtC[1] = 'p';
          txtC[2] = 'd';
        }
      }
      delay(20);
      if(millis()-lastClick<2000){
        // short press, go to tap mode
        if (startup + startDelay < millis()){ // first click - reset timing but don't change speed
          startup = lastClick; // show bpm for 2 seconds
        }else{ // another click
          startup = lastClick; // show bpm for 2 seconds
          mode = tapMode;
        }
      }
    }
  }else if( mode == tapMode ){ //********************* Tap Mode
    // at this point, the button is up, and the last press happened at "lastClick"
    timeToTick(false, true); // just display the bpm in silence
    // Check for User Input
    if(digitalRead(Button)){
      lastGap = millis()-lastClick; // Time since the last recorded tap
      lastClick = lastClick+lastGap; // Now
      nextEvent = lastClick; // at this moment - NOW
      timeToTick(true, true); // play a click, and schedule the next one
      screenOff();
      startup = lastClick; // show bpm for 2 seconds
      delay(20); // debounce
      while(digitalRead(Button)){
        // wait until user releases the button
      }
      delay(20);
      if(millis()-lastClick>5000){ // we want to enter the menu
        mode = menuModeBPM;
      }else{ // short tap *****************
        clickGaps[clickCount++] = lastGap;
        if(clickCount>=4){
          clickCount=0;
        }
        screenOff();
        calcBPMfromTaps();
        updateDisplayArray(bpm);
      }
    }

    // If the user stops inputting, we want to time out
    if( millis()>lastClick+3000){
      mode = metroMode;
    }
  }else if( mode == menuModeBPM){ //********************* Menu Mode (BPM)
    // at this point, the button is up, and the last press happened at "lastClick"
    //  and nextEvent is in the future because I just clicked when the button was pressed
    //  and startup = basically when we entered the menu
      
    if(digitalRead(rot1)==HIGH){
      digitalWrite(myLED, LOW);
      if(digitalRead(rot2)==HIGH){
        if(lastClick>=millis()-shortTime){
          bpm= min(600,bpm+10);
        }else{
          bpm= min(600,bpm+1);
        }
      }else{
        if(lastClick>=millis()-shortTime){
          bpm= max(5,bpm-10);
        }else{
          bpm= max(5,bpm-1);
        }
      }
      startup=millis();
      lastClick=startup;
      screenOff();
      updateDisplayArray(bpm);
      delay(10);
      while(digitalRead(rot1)==HIGH){
       
      }
      delay(10);
    }else{
      digitalWrite(myLED, HIGH);
    }

    
    timeToTick(true, false);
    if(millis()-startup>startDelay){ // only show during boot
      displayDigit(convertChar(txtC[dig]),dig); // displayOutput( segments to light, digit to light. Inc digit for next time.
      dig = dig+1;
      if(dig>=3){
        dig = 0;
      }
      delay(5);
    }

    if(digitalRead(Button)){
      screenOff();
      lastClick = millis();
      delay(20);
      while(digitalRead(Button)){
        if(((millis()-lastClick)>= 2000)  && mode == menuModeBPM){
          mode = menuModeLED;
          tone(11,500, 50);
          txtC[0] = 'L';
          txtC[1] = 'E';
          txtC[2] = 'd';
        }
      }
      delay(20);
      if( (millis()-lastClick)< 2000 ){
        mode = metroMode;
      }
    }

    if((millis()-lastClick)>8000){
      mode = metroMode;
    }
      
  }else if( mode == menuModeLED){ //********************* Menu Mode (BPM)
    // at this point, the button is up, and the last press happened at "lastClick"
    //  and nextEvent is in the future because I just clicked when the button was pressed
    
    if(digitalRead(rot1)==HIGH){
      digitalWrite(myLED, LOW);
      if(digitalRead(rot2)==HIGH){
        LEDMode= min(6,LEDMode+1);
      }else{
        LEDMode= max(0,LEDMode-1);
      }
      startup=millis();
      lastClick=startup;
      screenOff();
      updateDisplayArray(LEDMode);
      delay(10);
      while(digitalRead(rot1)==HIGH){
       
      }
      delay(10);
    }else{
      digitalWrite(myLED, HIGH);
    }

    if(millis()-startup>startDelay){ // only show during boot
      displayDigit(convertChar(txtC[dig]),dig); // displayOutput( segments to light, digit to light. Inc digit for next time.
      dig = dig+1;
      if(dig>=3){
        dig = 0;
      }
      delay(5);
    }else{ // only show during boot
      displayDigit(segments(LEDMode),2); // displayOutput( segments to light, digit to light. Inc digit for next time.
      delay(5);
      screenOff();
      delay(5);
    }

    if(digitalRead(Button)){
      screenOff();
      lastClick = millis();
      delay(20);
      while(digitalRead(Button)){
        if(((millis()-lastClick)>= 2000)  && mode == menuModeLED){
          mode = menuModeBPM;
          updateDisplayArray(bpm);
          tone(11,500, 50);
          txtC[0] = 's';
          txtC[1] = 'p';
          txtC[2] = 'd';
        }
      }
      delay(20);
      if( (millis()-lastClick)< 2000 ){
        mode = metroMode;
        updateDisplayArray(bpm);
      }
    }

    if((millis()-lastClick)>8000){
      mode = metroMode;
      updateDisplayArray(bpm);
    }
    
  }

}

void timeToTick(bool wthSnd, bool wthDisp){
  if(nextEvent<=millis()){ // event due - Tick
      if(wthSnd && ((millis()-nextEvent)<10)){
        startPlayback(sample, sizeof(sample));
      }
      // Then blink the centre Character, but only after the first 2 seconds.
      if((millis()-startup>startDelay) && (wthDisp)){
        if(LEDMode==0){
          displayDigit(segments(0),1); // send a zero to the tens column
        }else if(LEDMode==1){
          int box[] = { '[', '=', ']' };
          for(int x=0; x<30; x++){
            displayDigit(convertChar(box[dig]),dig);
            dig = dig+1;
            if(dig>=3){
              dig = 0;
            }
            delay(2);
          }
        }else if(LEDMode==2){
          displayDigit(segments(0),dig);
          if(dig==0){
            dig=2;
          }else{
            dig=0;
          }
        }else if(LEDMode==3){
          displayDigit(segments(0),dig);
          dig = dig+1;
          if(dig>=3){
            dig=0;
          }
        }else if(LEDMode==4){
          int box[] = { convertChar('['), 207, segments(1), convertChar(']') };
          char custDig[] = { 0, 1, 1, 2 };
          displayDigit(box[dig],custDig[dig]);
          dig = dig+1;
          if(dig>=4){
            dig = 0;
          }
        }else if(LEDMode==5){
          displayDigit(segments(0),3);
        }else if(LEDMode==6){
          displayDigit(segments(8),3);
        }
        delay(10);
        screenOff();
      }
      // schedule next time - based on previous time for accuracy
      // rather than now plus delay, which adds a little creep
      while(nextEvent<millis()){ // while loop in case we're way behind. stops echo noise.
        nextEvent = nextEvent+(60000/bpm);
      }
    }else if(nextEvent>millis()+5){ // 5ms spare so that this code doesn't run just before the next tick
      // lower priority stuff, but still, be quick
      if(millis()-startup<=startDelay){ // only show during boot
        displayDigit(segments(bpmC[dig]),dig); // displayOutput( segments to light, digit to light. Inc digit for next time.
        dig = dig+1;
        if(dig>=3){
          dig = 0;
        }
      }else{
        screenOff();
      }
    }
}

void calcBPMfromTaps(){// calc the average time, removing the value furthest from average
  int avgTime = 0;
  char deviant = 0;
  long deviation = 0;
  long temp;
  avgTime = 0;
  for(int i=0; i<4; i++){
    avgTime += clickGaps[i];
  }
  avgTime = avgTime/4;
  for(int i=0; i<4; i++){ // which has the biggest deviation
    temp = (avgTime-clickGaps[i]);
    if( abs(temp)>abs(deviation) ){ // larger deviation
      deviation = temp; // store the larger deviation including sign
      deviant = i; // and store which one is the most distant from average
    }
  }
  // then, use maths to remove it from the average
  avgTime = avgTime+(deviation/3); // the 3 here is 1 less than the number of fields
  // we now have an average of the three most consistent times. I hope. 
  bpm = 60000/avgTime;
  //return b;
}

void updateDisplayArray(int toDisp){ // converts an int into an array of single digits for the display
  int x;
  int last_x = 0;
  for(int i=0; i<3; i++){
    x = toDisp/(100/(int)(pow(10, i))); // This maths works out the characters for the LED display
    bpmC[i] = x-(10*last_x);
    //bpmC[i] = 9;
    last_x = x;
  }
}

void displayDigit(int seg, char whichDig){
  PORTC = PORTC | B00111000; // clear all digits
  PORTD = seg; // while all digits dark, set the segments
  if(whichDig<3){
    digitalWrite(displayLocation+(whichDig),LOW); // turn on one digit
  }else{
    PORTC = PORTC ^ B00111000;
  }
}
