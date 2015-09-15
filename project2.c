#include <reg932.h>
#include <stdlib.h>
#include "uart.h"

//------ Mode selection switches ------//
sbit greenMode = P2^3; // SW7: Mode 1
sbit redMode = P2^0; // SW1: Mode 2

//------- Piano notes/PlayTune Mode swtiches ------//
sbit yellowKey = P2^1; // SW3: Play note A, octate 4; Tune 1
sbit amberKey = P2^2; // SW9: Play note B, octate 4; Tune 2
sbit greenKey = P0^3;	// SW6: Play note C, octate 5
 
//------ Recording mode switches/variables ------//
sbit amberKey2 = P0^2; // SW2: Play recorded tune
sbit blueKey= P1^4; // SW5: Reset button 
unsigned int frequencies[14]; // Array used to record frequencies that had been played 
float lengths[14]; // Array used to track the numbers of times a note is played in recording mode
unsigned char frequenciesSize=0; //	Index used to monitor the 'frequencies' array
unsigned char lengthsSize=0; // Index used to monitor the 'lengths' array
 
//------- LEDs used ------//
sbit redLED = P2^4; // LED1
sbit yellowLED = P2^5; // LED3
sbit greenLED = P2^7; // LED7
sbit amberLED = P2^6; // LED9

//------ Ports used correspond to LEDs on breadboard ------//
sbit C4LED = P0^0; // Port0 Bit0: Note C, ctate 4
sbit G4LED = P0^1; // Port0 Bit1: Note G
sbit Gs4LED = P0^4; // Port0 Bit4: Note G sharp
sbit A4LED = P0^5;	// Port0 Bit5: Note A
sbit B4LED = P0^6;	// Port0 Bit6: Note B
sbit C5LED = P0^7;	// Port0 Bit7: Note C, octate 5

//------ User input to generate note with input frequency ------//
unsigned int LEDFreq; 

int i;

unsigned char mode =0;

sbit speaker = P1^7; // Speaker

void breadboard(); // Turn on LEDs on breadboard according to notes played 
void clearBreadboard(); // Turn off all the LEDs on breadboard

void play_tune(unsigned char tuneNumber); // Play one of the two tunes with corresponding button
void play_note(unsigned int freq, float timeNeeded); // Outputs a note according to user 
                                                     //inputs of frequency and length of a note
void Delay(float timeNeeded); // Delay function, with user input of time delay wanted
void recording(); // Recording Mode
void serialMode(unsigned char song[], int length); // Serial Mode


void main()
{
	TMOD = 0x11; // Timer 1&0, mode 1
	P0M1 = 0x00;
	P0M2 = 0x00;
	P1M1 = 0x00;
	P1M2 = 0x00;
	P2M1 = 0x00;
	P2M2 = 0x00;

	clearBreadboard(); // Turn off all the LEDs on breadboard before proceeding

	while(1)
	{
		//------ LED indication of which mode is currently running ------//
		if(redMode==0 && greenMode==0)
		{
			redLED = ~redLED;
			greenLED = ~greenLED;
			Delay(.5);
		}
		else if(redMode==0 && greenMode==1)
		{
			redLED = ~redLED;
			Delay(.5);
		}
		else if(greenMode==0 && redMode==1)
		{
			greenLED = ~greenLED;	
			Delay(.5);
		}
		
		if(redLED && greenLED) // PlayTune Mode if none is pressed
			mode = 0;
		else if(!redLED && greenLED) // Serial Mode if redMode is pressed
			mode = 1;
		else if(redLED && !greenLED) // Piano Mode if greenMode is pressed
			mode = 2;
		else // Recording Mode is both are pressed 
			mode = 3; 	
		
		// PlayTune Mode by default
		if(mode==0)
		{
			if(yellowKey==1 && amberKey==0 && greenKey==1) // amberKey is pressed
			{
				amberLED = 0; // Turn on the adjacent LED
				play_tune(1); // Play Tune 2
				amberLED = 1; // Turn off the LED
				clearBreadboard();
		   	}
			if(yellowKey==1 && amberKey==1 && greenKey==0) // greenKey is pressed
			{
			}
			if(yellowKey==0 && amberKey==1 && greenKey==1) // yellow is pressed
			{
				yellowLED = 0; // Turn on the adjacent LED
				play_tune(0); // Play Tune 1
				yellowLED = 1; //Turn off the LED
				clearBreadboard();
			}
		}
		// Serial Mode
		else if(mode==1)
		{
			if(yellowKey==0) // yellowKey is pressed
			{
				serialMode("Shave and a Haircut\r\n", 21); // Display name of song serially
				Delay(0.2);
				while(yellowKey==0);
			}
			if(amberKey==0)
			{
				serialMode("Over the Rainbow\r\n", 18); // Display name of song serially
				Delay(0.2);
				while(amberKey==0);				
			}
		}
		// Piano Mode
		else if(mode==2)
		{	 
			if(yellowKey==0) // If yellowKey is pressed, play note A, octate 4
				play_note(440,0.1); 
			if(amberKey==0)	// If amberKey is pressed, play note B, octate 4
				play_note(494,0.1);	
			if(greenKey==0) // If greenKey is pressed, play note C, octate 5
				play_note(523,0.1);	
		}
		// Recording Mode
		else
			recording();
	}
}


//Pre: Switches 2,5,3,6,9 must be programmed for user input
//Post: Upon user input it records values pressed or if switch 2 is pressed
//      the recorded notes are played back and then cleared
//Desc: Allows the user to record notes and play them back
void recording()
{	
	if(frequenciesSize<14)
	{
		if(yellowKey==0)
		{	
			play_note(523,0.1);		
			if(frequenciesSize==0)
			{//if array is empty enter first value
				frequencies[frequenciesSize++]=523;
				lengths[lengthsSize++]=0.1;
			}
			else if(frequencies[frequenciesSize-1]==523)
			{//if the same note is played extend the length of the note
				lengths[lengthsSize-1]+= 0.1;
			}
			else if(frequencies[frequenciesSize-1]!=523)
			{//if a new note is played, add the note and increase the size of the array
				frequencies[frequenciesSize++]=523;
				lengths[lengthsSize++]=0.1;
			}
		}
		else if(amberKey==0)
		{
			play_note(659,0.1);	// Note E
			if(frequenciesSize==0)
			{//if array is empty enter first value
				frequencies[frequenciesSize++]=659;
				lengths[lengthsSize++]=0.1;
			}
			else if(frequencies[frequenciesSize-1]==659)
			{//if the same note is played extend the length of the note
				lengths[lengthsSize-1]+= 0.1;
			}
			else if(frequencies[frequenciesSize-1]!=659)
			{//if a new note is played, add the note and increase the size of the array
				frequencies[frequenciesSize++]=659;
				lengths[lengthsSize++]=0.1;
			}
		}
		else if(greenKey==0)
		{
			play_note(784,0.1);	// G
			if(frequenciesSize==0)
			{//if array is empty enter first value
				frequencies[frequenciesSize++]=784;
				lengths[lengthsSize++]=0.1;
			}
			else if(frequencies[frequenciesSize-1]==784)
			{//if the same note is played extend the length of the note
				lengths[lengthsSize-1]+= 0.1;
			}
			else if(frequencies[frequenciesSize-1]!=784)
			{//if a new note is played, add the note and increase the size of the array
				frequencies[frequenciesSize++]=784;
				lengths[lengthsSize++]=0.1;
			}
		}
		else if(blueKey==0)
		{
			if(frequenciesSize==0)
			{//if array is empty enter first value
				frequencies[frequenciesSize++]=0;
				lengths[lengthsSize++]=0.0001;
			}
			else if(frequencies[frequenciesSize-1]==0)
			{//if the same note is played extend the length of the note
				lengths[lengthsSize-1]+= 0.0001;
			}
			else if(frequencies[frequenciesSize-1]!=0)
			{//if a new note is played, add the note and increase the size of the array
				frequencies[frequenciesSize++]=0;
				lengths[lengthsSize++]=0.0001;
			}
		}

	}
	if(amberKey2==0)
	{//play back all values
		for(i=0; i<frequenciesSize; i++)
		{
			if(frequencies[i]==0)
				Delay(lengths[i]);
			else
				play_note(frequencies[i],lengths[i]);
		}
		//reset size indexes back to 0
		frequenciesSize = 0;
		lengthsSize = 0;
	}	
}

//------ One of the two tunes will be played depending on the input ------//
void play_tune(unsigned char tuneNumber)
{
	if(!tuneNumber) // Play Shave and a Haircut
	{
		play_note(523,0.6); // Note 5C
		Delay(0.1);
		play_note(392,0.4); // Note 4G
		Delay(0.06);
		play_note(392,0.4); // Note 4G
		Delay(0.05);
		play_note(415,0.8); // Note 4G#
		Delay(0.05);
		play_note(392,0.8); // Note 4G
		Delay(0.3);
		play_note(494,0.8); // Note 4B
		Delay(0.03);
		play_note(523,0.8); // Note 5C
	}		
	else // Play Over the Rainbow
	{								 
	    play_note(262,0.8);	//4C half note
		Delay(0.1);	
		play_note(523,0.8);//5C half note
		Delay(0.1);
		play_note(494,0.6);//4B quarter note
		Delay(0.05);
		play_note(392,0.4);//4G eigth note
		Delay(0.05);
		play_note(440,0.4);//4A eigth note
		Delay(0.1);
		play_note(494,0.6);//4B quarter note
		Delay(0.1);
		play_note(523,0.6);//5C quarter note	
		Delay(0.1);
		play_note(262,0.8);//4C half note	
		Delay(0.1);
		play_note(440,0.8);//4A half note	
		Delay(0.1);
		play_note(392,1);//4G whole note
	}
}


void play_note(unsigned int freq, float timeNeeded)
{
	long fosc = 7373000; // Oscillator frequency
	unsigned char MC = 2; // Machine cycle
	float timeDelay = 0.5/freq;
	unsigned int numberTicks = timeDelay*(fosc/MC);	
	unsigned int loopNum = timeNeeded/timeDelay;
	LEDFreq = freq;
	if(mode!=3)
		breadboard();
	while(loopNum)
	{
		TH0 = -1*numberTicks >> 8;
		TL0 = -1*numberTicks;
		TR0 = 1;
		speaker = 1; // Turn on speaker
		while(!TF0);
		speaker = 0; // Turn off speaker
		TR0 = 0;
		TF0 = 0;
		loopNum--;
	}
}

void Delay(float timeNeeded)
{
	long fosc = 7373000;
	unsigned char MC = 2;
	unsigned int freq = 1000;
	float timeDelay = 0.5/freq;
	unsigned int numberTicks = timeDelay*(fosc/MC);	
	unsigned int loopNum = timeNeeded/timeDelay;
		
	while(loopNum)
	{
		TH0 = -1*numberTicks >> 8;
		TL0 = -1*numberTicks;
		TR0 = 1;
		while(!TF0);
		TR0 = 0;
		TF0 = 0;
		loopNum--;
	}
}

void serialMode(unsigned char song[], int length)
{
	EA = 1;
	uart_init();

	for(i = 0; i < length; i++)
    {
		EA = 1;
		uart_transmit(song[i]);
		EA = 0;
	}
}

void breadboard()
{
	if(LEDFreq == 262) // Turns on LED corresponds to Note 4C
	{
		C4LED = 1;
		G4LED = 0;
		Gs4LED = 0;
		A4LED = 0;
		B4LED = 0;
		C5LED = 0;
	}
	if(LEDFreq == 392) // Turns on LED corresponds to Note 4G
	{
		C4LED = 0;
		G4LED = 1;
		Gs4LED = 0;
		A4LED = 0;
		B4LED = 0;
		C5LED = 0;
	}
	if(LEDFreq == 415) // Turns on LED corresponds to Note 4G#
	{
		C4LED = 0;
		G4LED = 0;
		Gs4LED = 1;
		A4LED = 0;
		B4LED = 0;
		C5LED = 0;
	}
	if(LEDFreq == 440) // Turns on LED corresponds to Note 4A
	{
		C4LED = 0;
		G4LED = 0;
		Gs4LED = 0;
		A4LED = 1;
		B4LED = 0;
		C5LED = 0;
	}
	if(LEDFreq == 494) // Turns on LED corresponds to Note 4B
	{
		C4LED = 0;
		G4LED = 0;
		Gs4LED = 0;
		A4LED = 0;
		B4LED = 1;
		C5LED = 0;
	}
	if(LEDFreq == 523) // Turns on LED corresponds to Note 5C
	{
		C4LED = 0;
		G4LED = 0;
		Gs4LED = 0;
		A4LED = 0;
		B4LED = 0;
		C5LED = 1;
	}
}

void clearBreadboard() // Turn off all the LEDs on breadboard
{
	C4LED = 0;
	G4LED = 0;
	Gs4LED = 0;
	A4LED = 0;
	B4LED = 0;
	C5LED = 0;
}
