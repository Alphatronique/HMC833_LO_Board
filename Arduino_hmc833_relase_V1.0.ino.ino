
#define strBeacondID "VE2PN VE2PN BEACON 5W FN46JU"
int MorseWPM = 5;
int Output_PWR  = 4;

// first value is number of register to send  , then 32bit register to send (fist byte is adress then 24 bit register content) , register value based on dev kit software
unsigned long PLL_Reg_Values [] = { 15, 0x01000002, 0x02000032, 0x05006C10, 0x05000000, 0x062003CA, 0x0700014D, 0x08C1BEFF, 0x09007CF9, 0x0A002205, 0x0B0F8061, 0x0C000000, 0x0F000001, 0x04000000, 0x03000AE0};//  116Mhz
//unsigned long PLL_Reg_Values [] = { 16, 0x01000002, 0x02000032, 0x05006A10,0x05002898, 0x05000000, 0x06200B4A, 0x0700014D, 0x08C1BEFF, 0x09407E7C, 0x0A002205, 0x0B0F8061, 0x0C000000, 0x0F000001, 0x045C28F6, 0x03000B46};//  144.318Mhz
//unsigned long PLL_Reg_Values [] = { 15, 0x01000002, 0x02000032, 0x05006E10, 0x05000000, 0x062003CA, 0x0700014D, 0x08C1BEFF, 0x09007CF9, 0x0A002205, 0x0B0F8061, 0x0C000000, 0x0F000001, 0x04000000, 0x03000B98};//  106Mhz
//unsigned long PLL_Reg_Values [] = {18, 0x01000002, 0x02000001, 0x05001628, 0x050060A0, 0x05002898, 0x0500E110, 0x05000000, 0x06030F4A, 0x0700014D, 0x08C1BEFF, 0x095CBFFF, 0x0A002046, 0x0B07C061, 0x0C000000, 0x0F000081, 0x0300001F, 0x04555555};//  783Mhz
//unsigned long PLL_Reg_Values [] = {18, 0x01000002, 0x02000001, 0x05001628, 0x050060A0, 0x0500E090, 0x05002898, 0x05000000, 0x06030F4A, 0x0700014D, 0x08C1BEFF, 0x095CBFFF, 0x0A002046, 0x0B07C061, 0x0C000000, 0x0F000081, 0x0300001F, 0x04555555};//  1566Mhz


///////////////////////  don't screw with code below   /////////////////////////////////////

int SEN_Pin = 8;
int SCK_Pin = 12;
int SDI_Pin = 6;
int SDO_Pin = 4;

int  DAT31_SCLK   =  15;
int  DAT31_SDATA  =  16;
int  DAT31_LE     =  11;

int LED = A0;
int LOCK = A1;

// Bung Morse into an array
char* MorseCodeCharacters[] = {
"A", ".-", 
"B","-...", 
"C","-.-.", 
"D","-..", 
"E",".", 
"F","..-.", 
"G","--.", 
"H","....", 
"I","..", 
"J",".---", 
"K","-.-", 
"L",".-..", 
"M","--", 
"N","-.", 
"O","---", 
"P",".--.", 
"Q","--.-", 
"R",".-.", 
"S","...", 
"T","-", 
"U","..-", 
"V","...-", 
"W",".--", 
"X","-..-", 
"Y","-.--",
"Z","--..",
"0","-----", 
"1",".----", 
"2","..---",
"3","...--",
"4","....-",
"5",".....",
"6","-....",
"7","--...",
"8","---..",
"9","----.",
"/","-..-.",
"*","-.-.-",
".",".-.-.",
"&","...-.-",
" "," "
};

unsigned long i=1, flag=1;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
//  while (!Serial) ;
  digitalWrite(SEN_Pin, LOW);
  digitalWrite(SCK_Pin, LOW);
  digitalWrite(SDI_Pin, LOW);
  pinMode(SEN_Pin, OUTPUT);
  pinMode(SCK_Pin, OUTPUT);
  pinMode(SDI_Pin, OUTPUT);
  pinMode(SDO_Pin, INPUT);

  pinMode(DAT31_LE, OUTPUT); 
  pinMode(DAT31_SCLK, OUTPUT);
  pinMode(DAT31_SDATA, OUTPUT);

  pinMode(LED, OUTPUT);
  pinMode(LOCK, OUTPUT);

  delay(2000);
  Serial.println("Initialized");

  DAT31_Set(Output_PWR); // full turbo overdrive
  
}
void loop() {
  // put your main code here, to run repeatedly:
  
// Test_SPI_COMM();
   
  while (i<PLL_Reg_Values[0] && flag == 1){
    writeRegister( PLL_Reg_Values[i]);
    i = i+1;
  }

//   TransmitMorse(strBeacondID, MorseWPM);   // send SW message if need (beacon mode )



  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);                       // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(200); 

  if (digitalRead(SDO_Pin) == true) digitalWrite(LOCK, HIGH); 
  else {digitalWrite(LOCK, LOW);}

}

void Test_SPI_COMM (void) {
  
 writeRegister(0x0F0000C0); //Program Reg 0x0F = 0xC0: LD/SDO pin should go logic Low;
  if (digitalRead(SDO_Pin) == false) digitalWrite(LED, LOW);
   delay(100); 
 writeRegister(0x0F0000E0);  //Program Reg 0x0F = 0xE0: LD/SDO pin should go logic High; 
  if (digitalRead(SDO_Pin) == true) digitalWrite(LED, HIGH);
   delay(100); 
}
  

void writeRegister( unsigned long data) {
  Serial.print(" DATA =");
  Serial.println(data, HEX);
  data = data<<1;
  int  B31_B24 = (data & 0xFF000000)>>24; //24
  int  B23_B16 = (data & 0xFF0000)>>16;
  int  B15_B8 = (data & 0xFF00)>>8;
  int  B7_B0 = (data & 0xFF); 

  digitalWrite(SEN_Pin, HIGH);
  shiftOut(SDI_Pin, SCK_Pin, MSBFIRST, B31_B24);
  shiftOut(SDI_Pin, SCK_Pin, MSBFIRST, B23_B16);
  shiftOut(SDI_Pin, SCK_Pin, MSBFIRST, B15_B8);
  shiftOut(SDI_Pin, SCK_Pin, MSBFIRST, B7_B0);
  digitalWrite(SEN_Pin, LOW);    
}

// ============================================================================
// Send data DAT-31.
//
void DAT31_Set( byte data )
{
  byte i;
  byte pindata;

  digitalWrite(DAT31_LE, LOW);    //  DAT31_LE_LO;
  digitalWrite(DAT31_SCLK, LOW);  //  DAT31_SCLK_LO;
  digitalWrite(DAT31_SDATA, LOW); //  DAT31_SDATA_LO;

  if (data > 31)  // make sure attenuation value fit the DAT31 max attenuation
    data = 31;
 
  // Sequential shipping 5-bit big-endian.
  //
  for( i=0; i<6; i++ )
  {
    pindata = (data & 0x10 )?( 1 ):( 0 );
     if (pindata == 0)
      { digitalWrite(DAT31_SDATA, LOW);} 
        else
      {digitalWrite(DAT31_SDATA, HIGH); }

    data <<= 1;
    digitalWrite(DAT31_SCLK, HIGH);
    delay(1);
    digitalWrite(DAT31_SCLK, LOW);
    delay(1);
  } 

    digitalWrite(DAT31_LE, HIGH);     // now all bit transfer and latch put back LE low 
    delay(1);
    digitalWrite(DAT31_LE, LOW);
}

// ============================================================================
// CW modulator
//

  void TransmitMorse(String MorseString, int MorseWPM) {
    
  int CWdot = 1200 / MorseWPM;
  int CWdash = (1200 / MorseWPM)*3;
  int istr;
  for (istr=0; istr < MorseString.length(); istr++) {
    String currentchar = MorseString.substring(istr,istr+1);
    int ichar = 0;
    while(ichar < sizeof(MorseCodeCharacters)) {
      String currentletter = MorseCodeCharacters[ichar];   // letter
      String currentcode = MorseCodeCharacters[ichar+1];   // corresponding morse code
      if (currentletter.equalsIgnoreCase(currentchar)) {
        int icp=0;
        for (icp=0;icp < currentcode.length(); icp++) {
          // Transmit Dit
          if (currentcode.substring(icp,icp+1).equalsIgnoreCase(".")) { 
            //digitalWrite(PTT, LOW);
            //tone(MorseTonePin, MorseToneFreq);
            DAT31_Set(Output_PWR);
            delay(CWdot);
            //digitalWrite(PTT, HIGH);
            //noTone(MorseTonePin);
            DAT31_Set(31);
            delay(CWdot);
          }
          // Transmit Dah
          else if (currentcode.substring(icp,icp+1).equalsIgnoreCase("-")) {
            //digitalWrite(PTT, LOW);
            //tone(MorseTonePin, MorseToneFreq);
            DAT31_Set(Output_PWR);
            delay(CWdash);
            //digitalWrite(PTT, HIGH);
            //noTone(MorseTonePin);
            DAT31_Set(31);
            delay(CWdot); 
          }
          else if (currentcode.substring(icp,icp+1).equalsIgnoreCase(" ")) {
            delay(CWdot*3);
          };
          }
        }
        ichar=ichar+2;
     }
      delay(CWdot*3);
      }    
    delay(CWdot*7);
  }




  
//  https://ez.analog.com/rf/f/q-a/71511/hmc832-spi-programming-problem
// The last write to Reg 0x05 must be zero.  Your code sets the following:
//
//Reg 0x5=0x90  set VCO divide-by 1 output
//Reg 0x5=0xF98 set Hi performance and differential output
//Reg 0x5=0x4B38 set -6dB output power
//Reg 0x5=0x4B38  I think this one should be Reg 0x5=0.
//The arrays are only initialized with 18 values.  Shouldn't  "const int N= 19" be set to 18?
//SPI registers require 5 register address bits plus 3 chip address bits. The data starts on the next byte.   I think you need "thisValue = thisValue<<8;" 
//SPI cycles are 32 bits.  You're missing the last byte: int B31_B24 = (data & 0xFF000000)>>24;

//A  typical HMC Mode  WRITE cycle is  shown in  Figure  45.
//a.   The  Master  (host)  both  asserts SEN (Serial Port  Enable) and clears  SDI to  indicate  a WRITE cycle,  
//followed  by  a rising  edge  of  SCK.
//b.   The  slave (synthesizer) reads SDI on  the 1st rising  edge  of  SCK after SEN.  SDI low indi  cates a Write 
//cycle (/WR).
//c.   Host places  the six address bits  on  the next  six falling edges of  SCK,  MSB first.
//d.   Slave  shifts  the address bits  in  the next  six rising  edges of  SCK (2-7).
//e.   Host places  the 24  data  bits  on  the next  24  falling edges of  SCK,  MSB first.
//f.   Slave  shifts  the data  bits  on  the next  24  rising  edges of  SCK (8-31).
//g.   The  data  is  registered  into  the chip  on  the 32nd  rising  edge  of  SCK.
//h.   SEN  is  cleared after a minimum delay of  t5. This  completes the write cycle.


//Then the question is how to determine which SPI Mode is active. The procedure
//to determine the active SPI Mode is simple:
//Program Reg0Fh = C0h: LD/SDO pin should go logic Low;
//Program Reg0FH = E0h: LD/SDO pin should go logic High;
//If the output levels do not correspond to the expected level then either the device is in the other SPI mode
//or there is another issue with SPI communication. Attempt the same exercise with the other SPI mode to
//confirm communication.

 //Fractional Mode (Integer-N mode does not require the use of CP Offset current.
 
// All Hittite PLL+VCO products use this technique with 5 switched capacitors in the tank circuit (offering 32
//VCO „sub-bands‟). Some products have multiple VCO cores. Each VCO core has 32 sub-bands to cover
//the specified frequency range.

//VCO AutoCal on Frequency Change Assuming Reg 0Ah[11]=0, the VCO calibration starts automatically whenever a frequency change is requested.

//const int N= 17;
//unsigned long PLL_Reg_Values [N] = { 0x01000002,
//                                     0x02000001,
//                                     0x05001628, 
//                                     0x050060A0, 
//                                     0x05002898,
//                                     0x0500E110, 
//                                     0x05000000, 
//                                     0x06030F4A, 
//                                     0x0700014D, 
//                                     0x08C1BEFF, 
//                                     0x095CBFFF, 
//                                     0x0A002046, 
//                                     0x0B07C061, 
//                                     0x0C000000, 
//                                     0x0F000081, 
//                                     0x0300001F, 
//                                     0x04555555};//  783Mhz


//  04025C151KAT2A AVX   cap for loop filter
//lock detector issue check reg 7
//Your register configuration most likely is correct if you do not see any  problem when using the eval board.  The order of register writes does not matter except send the frequency control registers last (Reg 0x03 followed by Reg 0x04) and program Reg 0x05=0 after all other writes to Reg 0x05.
//You need to change VCO_reg 0x04 and VCO_reg 0x05 but can use the default value for VCO Reg 0x06. the other writes to Register 0x05 setup the gain and output divider.  The complete set of Register 0x05 writes are as follows:
//
//Reg 0x05=0x60A0 set VCO bias in VCO_reg 0x04
//
//Reg 0x05=0x1628 set VCO calibration in VCO_reg 0x05
//
//Reg 0x05=0x2898 set fundamental mode
//
//Reg 0x05=0xE090 set output divide-by one and max gain
//
//Reg 0x05=0x0

//Check the HMC833 reference input is a least 0.6Vpp.

//Muting of the VCO Output Buffer is accomplished by writing:
//Reg05h = E010h (VCO_Reg02h[5:0]=0)

//VCO Output DividebyMax O/P  ,power11  ,power10 ,power01  ,power00
//1 E090 C090 A090 8090
//2 E110 C110 A110 8110
//4 6210 4210 2210 210
//6 6310 4310 2310 310
//8 6410 4410 2410 410
//10 6510 4510 2510 510
//12 6610 4610 2610 610
//14 6710 4710 2710 710
//16 6810 4810 2810 810
//18 6910 4910 2910 910
//20 6A10 4A10 2A10 A10
//22 6B10 4B10 2B10 B10
//24 6C10 4C10 2C10 C10
//26 6D10 4D10 2D10 D10
//28 6E10 4E10 2E10 E10
//30 6F10 4F10 2F10 F10
//32 7010 5010 3010 1010
//34 7110 5110 3110 1110
//36 7210 5210 3210 1210
//38 7310 5310 3310 1310
//40 7410 5410 3410 1410
//42 7510 5510 3510 1510
//44 7610 5610 3610 1610
//46 7710 5710 3710 1710
//48 7810 5810 3810 1810
//50 7910 5910 3910 1910
//52 7A10 5A10 3A10 1A10
//54 7B10 5B10 3B10 1B10
//56 7C19 5C19 3C19 1C19
//58 7D10 5D10 3D10 1D10
//60 7E10 5E10 3E10 1E10
//62 7F10 5F10 3F10 1F10
