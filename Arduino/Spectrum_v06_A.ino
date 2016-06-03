#include <Average.h>
// VIVID Test Code for Spectrum v04 May 22th 2016 _ Olivier Jean
// HSV to RGB code_ 
// Circuit with N-Mofset and pull-down resistor
// Arduino Mini Version _06 - PCB vG


// just a tree val media filter. still some aberation on the color spectrum.

Average<int> ave(12); // use to be flaot - int less math ? / maybe i float all those ?
// ************* ************* ************* *************
// pot travel is 300º for 1023 value change. 1º = 3.41
// travel we want is 40 º /  -20 ---- +20
// 20 º = 68.2 |  150º = 511.5
// midle point is  511.5
// 443.3 -- 511.5 -- 597.7 is the sensor value we want
// ************* ************* ************* *************

// ************* ********** DEBUG ********* ************* //

int DEBUG = 0; // Debug enable = 1 , switch to 0 to desiable debug. ( debug slows the mini quite a lot )

// ************* ***** USER VARIABLES ***** ************* //

int potMin = 0  ;   // Value Mapped Start angle  aka (( 1023 / 300º ) * 150º ) - ((1023 / 300º)* 20º )
int potMax = 1023;   // Value Mapped End angle  aka (( 1023 / 300º ) * 150º ) + ((1023 / 300º)* 20º )
int hueMin = 0;     // Start Hue 0 for pit on the right ( looking from project box ) - invert aka 325 for other side
int hueMax = 360;   // End Hue either 325

// ************* ***** XXXX XXXXXXXXX ***** ************* //

// Pin declarations:
int LEDRed = 9;
int LEDGreen = 10;
int LEDBlue = 11;

int sensorPin = A0;
int newest = 0;
int recent = 0;
int oldest = 0;

// Variables
int valRaw; //Raw Value
int ValCons; // Constrain Value to potMin / potMax
int valMap; // Mapped Value to ( hueMin/hueMax )
int valSmooth; // Smooth'ed Val.

//  Average Library - Average Bucket Size  < int > can be float //
//Average<float> ave(10);

// Init RGB Array
int rgb[3];
// ************* ***** **** SETUP *** ****** ************* //
void setup() {

  // median filter start
  oldest = analogRead(A0);
  recent = oldest;
  newest = recent;

  // Pin Declaration
  pinMode(LEDRed, OUTPUT);
  pinMode(LEDGreen, OUTPUT);
  pinMode(LEDBlue, OUTPUT);

  if (DEBUG == 1 ) {
    Serial.begin(9600); // Set up the serial output
  }

  if (DEBUG == 0 ) { // Boot Up sequence R 4sec - G 4sec - B 4sec //
    analogWrite(LEDRed, 255);
    analogWrite(LEDBlue, 0);
    analogWrite(LEDGreen, 0);
    delay(4000);
    analogWrite(LEDRed, 0);
    analogWrite(LEDGreen, 255);
    analogWrite(LEDBlue, 0);
    delay(4000);
    analogWrite(LEDRed, 0);
    analogWrite(LEDGreen, 0);
    analogWrite(LEDBlue, 255);
    delay(4000);
    analogWrite(LEDRed, 255);
    analogWrite(LEDBlue, 0);
    analogWrite(LEDGreen, 0);
    delay(500);
    analogWrite(LEDRed, 0);
    analogWrite(LEDGreen, 255);
    analogWrite(LEDBlue, 0);
    delay(500);
    analogWrite(LEDRed, 0);
    analogWrite(LEDGreen, 0);
    analogWrite(LEDBlue, 255);
    delay(500);
  }
}

void loop() {

  valRaw = analogRead(sensorPin); // Read Pot Value

  ValCons = constrain (valRaw, potMin, potMax); // limits range of sensor values to between potMin(443) and potMax(597)

  valMap = map(ValCons, potMin, potMax, hueMin, hueMax); // map the ValCons'train to hue

  ave.push(valMap); // Add new value to the average bucket

  //oldest = recent;
  //recent = newest;
  //newest = valMap;

  //int median = median_of_3( oldest, recent, newest );

  valSmooth = ave.mean(); // Average Smooth

  // DO HSV to RGB

  hsi2rgb(valSmooth, 1, 1, rgb);

  //hsi2rgb(median, 1, 1, rgb);

  // ------------
  // Display to LED
  analogWrite(LEDRed, rgb[0]);
  analogWrite(LEDBlue, rgb[1]);
  analogWrite(LEDGreen, rgb[2]);
  // ------------

  // Debug Function Serial Write //
  if (DEBUG) {
    Serial.print(" raw : ");
    Serial.print(valRaw);
    //Serial.print(" Constrain : ");
    // Serial.print(ValCons);
    Serial.print(" Hue Normal : ");
    Serial.print(valMap);
    Serial.print(" Average : ");
    Serial.print(valSmooth);
    Serial.print("--   OUT R: ");
    Serial.print(rgb[0]);
    Serial.print(" G: ");
    Serial.print(rgb[1]);
    Serial.print(" B: ");
    Serial.print(rgb[2]);
    Serial.println("  ");
  }




}

//  Median Filter Function // Low Pass Filter on 3 Sample ~
/*
  median_filter.ino
  2014-03-25: started by David Cary
  http://arduino.stackexchange.com/questions/246/are-there-any-signal-smoothing-libraries-for-arduino
*/

int median_of_3( int a, int b, int c ) {
  int the_max = max( max( a, b ), c );
  int the_min = min( min( a, b ), c );
  // unnecessarily clever code
  int the_median = the_max ^ the_min ^ a ^ b ^ c;
  return ( the_median );
}


// FUNCTION STOLEN FROM
// http://blog.saikoled.com/post/43693602826/why-every-led-light-should-be-using-hsi-colorspace on 10/27/14
// Function example takes H, S, I, and a pointer to the
// returned RGB colorspace converted vector. It should
// be initialized with:
//
// int rgb[3];
//
// in the calling function. After calling hsi2rgb
// the vector rgb will contain red, green, and blue
// calculated values.
// Example to call Value:
// int rgb[3];
// hsi2rgb(90.0, 1, 1, rgb);
// H hue / Saturation / Intensity
// strip.setPixelColor(1,strip.Color(rgb[0],rgb[1],rgb[2]));

void hsi2rgb(float H, float S, float I, int* rgb) {
  int r, g, b;
  H = fmod(H, 360); // cycle H around to 0-360 degrees
  H = 3.14159 * H / (float)180; // Convert to radians.
  S = S > 0 ? (S < 1 ? S : 1) : 0; // clamp S and I to interval [0,1]
  I = I > 0 ? (I < 1 ? I : 1) : 0;

  // Math! Thanks in part to Kyle Miller. Your awesome Kyle !
  if (H < 2.09439) {
    r = 255 * I / 3 * (1 + S * cos(H) / cos(1.047196667 - H));
    g = 255 * I / 3 * (1 + S * (1 - cos(H) / cos(1.047196667 - H)));
    b = 255 * I / 3 * (1 - S);
  } else if (H < 4.188787) {
    H = H - 2.09439;
    g = 255 * I / 3 * (1 + S * cos(H) / cos(1.047196667 - H));
    b = 255 * I / 3 * (1 + S * (1 - cos(H) / cos(1.047196667 - H)));
    r = 255 * I / 3 * (1 - S);
  } else {
    H = H - 4.188787;
    b = 255 * I / 3 * (1 + S * cos(H) / cos(1.047196667 - H));
    r = 255 * I / 3 * (1 + S * (1 - cos(H) / cos(1.047196667 - H)));
    g = 255 * I / 3 * (1 - S);
  }
  // Returns / Fill the array with the newly calculated values.
  rgb[0] = r;
  rgb[1] = g;
  rgb[2] = b;
}
