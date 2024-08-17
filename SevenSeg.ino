const char segmentVals[] = { 192, 249, 164, 176, 153, 146, 130, 248, 128, 144 };

const char segClearVal = 255;

const char segDPVal = 127;

char segments(char val){
  return segmentVals[val];
}

char convertChar(char val){
  char result=0;

  switch(val){
    case 's':
      result = 146;
      break;
    case 'p':
      result = 140;
      break;
    case 'd':
      result = 161;
      break;
    case 'L':
      result = 199;
      break;
    case 'E':
      result = 134;
      break;
    case '[':
      result = 198;
      break;
    case '=':
      result = 246;
      break;
    case ']':
      result = 240;
      break;
  }
  
  return result;
}

// returns the value for the decimal point - and this with a number to get both.
char segDP(){
  return segDPVal;
}

// segClear clears all bits of the character output
char segClear(){
  return segClearVal;
}

// screenOff turns off all three digits
void screenOff(){
  PORTC = PORTC | B00111000; // clear all digits
}
