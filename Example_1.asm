#include <reg932.inc>

MOV P2M1, #00h			;Set Port 2 to bi-directional
MOV P1M1, #00h			;Set Port 1 to bi-directional
MOV TMOD, #11h 			;Set Timer 1 and 2 operated in mode 1
MOV PSW, #00h			;Reset the Porgram Status Word

START:
JNB P2.3, INCREMENT		;Wait till Increment Switch (SW7) is activated
JNB P2.0, DECREMENT		;Wait till Decrement Switch (SW1) is activated
JNB P2.2, BLINK			;Wait till Blink Switch (SW9) is activated
JNB P2.1, MUSIC			;Wait till Music Switch (SW3) is activated
SJMP START				;Continuous Loop

MUSIC:					;Calling respective notes to create a C major chord
CALL CNOTE
CALL DNOTE
CALL ENOTE
CALL FNOTE
CALL GNOTE
CALL ANOTE
CALL BNOTE
CALL CNOTE2
CALL BNOTE
CALL ANOTE
CALL GNOTE
CALL FNOTE
CALL ENOTE
CALL DNOTE
CALL CNOTE
SJMP RED				;Jump to the LEDs

BLINK:
MOV R3, #5				;R3 = 5
LOOP:
CPL P2.4				;Complement LED1
CPL P2.5				;Complement	LED3
CPL P2.6				;Complement LED9
CPL P2.7				;Complement LED7
CALL DELAY				;Generate a delay between each blink
DJNZ R3, LOOP			;Loop it 5 times
SJMP RED				;Jump to the LEDs

INCREMENT:
CALL DELAY				;Generate a delay for the switch to response
ADD A, #1				;Increase the value in Accumulator by 1
JB PSW.6, HIGHBUZZ		;If overflow occurs, jump to HIGHBUZZ
JB PSW.2, HIGHBUZZ
SJMP RED				;Jump to the LEDs

DECREMENT:
CALL DELAY				;Generate a delay for the switch to response
ADD A, #-1				;Decrease the value in Accumulator by 1
SJMP ACCC				;Jump to ACCC to check turnover 

LOWBUZZ:				;Create a square wave with frequency of 500 Hz
MOV R4, #4				;Set up parameters for nested loop
BACK1:
MOV R3, #200			
AGAIN1:
MOV TH0, #-2765 SHR 8	;Initialize THTL to -2765
MOV TL0, #-2765
SETB TR0				;Turn on Timer 0
CLR PSW.6				;Reset the Overflow flag
CLR PSW.2				;Reset the Auxiliary Carry 
SETB P1.7				;Turn on the speaker
LOOP1:
JNB TF0, LOOP1			;Wait for overflow occurs
CLR P1.7				;Turn off the speaker 
CLR TR0					;Turn off timer 0
CLR TF0 				;Clear Overflow flag
DJNZ R3, AGAIN1			;Loop
DJNZ R4, BACK1
SJMP RED				;Jump to the LEDs

HIGHBUZZ:				;Create a square wave with frequency of 1500 Hz
MOV R4, #12				;Set up parameters for nested loop
BACK2:
MOV R3, #200
AGAIN2:
MOV TH1, #-922 SHR 8	;Initialize THTL to -922
MOV TL1, #-922
SETB TR1				;Turn on Timer 1
CLR PSW.6				;Reset the Overflow flag
CLR PSW.2				;Reset the Auxiliary Carry
SETB P1.7				;Turn on the speaker
LOOP2:
JNB TF1, LOOP2			;Wait for overflow occurs
CLR P1.7				;Turn off the speaker 
CLR TR1					;Turn off timer 0
CLR TF1 				;Clear Overflow flag
DJNZ R3, AGAIN2			;Loop
DJNZ R4, BACK2
SJMP RED				;Jump to the LEDs

ACCC:					;Accumulator Check
JNB ACC.3, RED			;Check each bit in the accumulator to determine 
JNB ACC.2, RED			;whether turnover occurs.  If not, jump to the LEDs,
JNB ACC.1, RED			;otherwise, a buzz will sound
JNB ACC.0, RED
SJMP LOWBUZZ

RED:					;LED1
MOV C, ACC.3			;Move the MSB into C
CPL C					;Complement C
MOV P2.4, C				;Use C to determine whether LED1 should be on/off
SJMP GREEN				;Jump to the next LED

GREEN:					;LED7
MOV C , ACC.2			;Move the second SB into C
CPL C					;Complement C
MOV P2.7, C				;Use C to determine whether LED7 should be on/off
SJMP AMBER				;Jump to the next LED

AMBER:					;LED9
MOV C, ACC.1			;Move the second SB into C
CPL C					;Complement C
MOV P2.6, C				;Use C to determine whether LED9 should be on/off
SJMP YELLOW				;Jump to the next LED

YELLOW:					;LED3
MOV C, ACC.0			;Move the second SB into C
CPL C					;Complement C
MOV P2.5, C				;Use C to determine whether LED3 should be on/off
LJMP START				;Jump back to Start

DELAY:					;Create a delay by using nested loop
MOV R2, #20				
AGAIN:
MOV R1, #200
OUTER:
MOV R0, #200
INNER:
DJNZ R0, INNER			 
DJNZ R1, OUTER
DJNZ R2, AGAIN
RET

CNOTE:					;Create a square wave with frequency of 262 Hz
MOV R5, #21				;Set up parameters for nested loop
BACKC:
MOV R6, #5
AGAINC:
MOV TH0, #-7035 SHR 8	;Initialize THTL to -7035
MOV TL0, #-7035
SETB TR0				;Turn on Timer 0
SETB P1.7				;Turn on the speaker
LOOPC:
JNB TF0, LOOPC			;Wait for overflow occurs
CLR P1.7				;Turn off timer 0
CLR TR0					;Turn off the speaker 
CLR TF0 				;Clear Overflow flag
DJNZ R6, AGAINC			;Loop
DJNZ R5, BACKC
RET						;Return to MUSIC

DNOTE:					;Create a square wave with frequency of 294 Hz
MOV R5, #24				;Set up parameters for nested loop
BACKD:
MOV R6, #5
AGAIND:
MOV TH0, #-6270 SHR 8	;Initialize THTL to -6270
MOV TL0, #-6270
SETB TR0				;Turn on Timer 0
SETB P1.7				;Turn on the speaker
LOOPD:
JNB TF0, LOOPD			;Wait for overflow occurs
CLR P1.7				;Turn off timer 0
CLR TR0					;Turn off the speaker 
CLR TF0 				;Clear Overflow flag
DJNZ R6, AGAIND			;Loop
DJNZ R5, BACKD
RET						;Return to MUSIC

ENOTE:					;Create a square wave with frequency of 330 Hz
MOV R5, #27				;Set up parameters for nested loop
BACKE:
MOV R6, #5
AGAINE:
MOV TH0, #-5586 SHR 8	;Initialize THTL to -5586
MOV TL0, #-5586
SETB TR0				;Turn on Timer 0
SETB P1.7				;Turn on the speaker
LOOPE:
JNB TF0, LOOPE			;Wait for overflow occurs
CLR P1.7				;Turn off timer 0
CLR TR0					;Turn off the speaker
CLR TF0 				;Clear Overflow flag
DJNZ R6, AGAINE			;Loop
DJNZ R5, BACKE
RET						;Return to MUSIC

FNOTE:					;Create a square wave with frequency of 349 Hz
MOV R5, #28				;Set up parameters for nested loop
BACKF:
MOV R6, #5
AGAINF:
MOV TH0, #-5282 SHR 8	;Initialize THTL to -5282
MOV TL0, #-5282
SETB TR0				;Turn on Timer 0
SETB P1.7				;Turn on the speaker
LOOPF:
JNB TF0, LOOPF			;Wait for overflow occurs
CLR P1.7				;Turn off timer 0
CLR TR0					;Turn off the speaker
CLR TF0 				;Clear Overflow flag
DJNZ R6, AGAINF			;Loop
DJNZ R5, BACKF
RET						;Return to MUSIC

GNOTE:					;Create a square wave with frequency of 392 Hz
MOV R5, #32				;Set up parameters for nested loop
BACKG:
MOV R6, #5
AGAING:
MOV TH0, #-4702 SHR 8	;Initialize THTL to -4702
MOV TL0, #-4702
SETB TR0				;Turn on Timer 0
SETB P1.7				;Turn on the speaker
LOOPG:
JNB TF0, LOOPG			;Wait for overflow occurs
CLR P1.7				;Turn off timer 0
CLR TR0					;Turn off the speaker
CLR TF0 				;Clear Overflow flag
DJNZ R6, AGAING			;Loop
DJNZ R5, BACKG
RET						;Return to MUSIC

ANOTE:					;Create a square wave with frequency of 440 Hz
MOV R5, #36				;Set up parameters for nested loop
BACKA:
MOV R6, #5
AGAINA:
MOV TH0, #-4189 SHR 8	;Initialize THTL to -4189
MOV TL0, #-4189
SETB TR0				;Turn on Timer 0
SETB P1.7				;Turn on the speaker
LOOPA:
JNB TF0, LOOPA			;Wait for overflow occurs
CLR P1.7				;Turn off timer 0
CLR TR0					;Turn off the speaker
CLR TF0 				;Clear Overflow flag
DJNZ R6, AGAINA			;Loop
DJNZ R5, BACKA
RET						;Return to MUSIC

BNOTE:					;Create a square wave with frequency of 494 Hz
MOV R5, #40				;Set up parameters for nested loop
BACKB:
MOV R6, #5
AGAINB:
MOV TH0, #-3731 SHR 8	;Initialize THTL to -3731
MOV TL0, #-3731
SETB TR0				;Turn on Timer 0
SETB P1.7				;Turn on the speaker
LOOPB:
JNB TF0, LOOPB			;Wait for overflow occurs
CLR P1.7				;Turn off timer 0
CLR TR0					;Turn off the speaker
CLR TF0 				;Clear Overflow flag
DJNZ R6, AGAINB			;Loop
DJNZ R5, BACKB
RET						;Return to MUSIC

CNOTE2:					;Create a square wave with frequency of 523 Hz
MOV R5, #43				;Set up parameters for nested loop
BACKC2:
MOV R6, #5
AGAINC2:
MOV TH0, #-3524 SHR 8	;Initialize THTL to -3524
MOV TL0, #-3524
SETB TR0				;Turn on Timer 0
SETB P1.7				;Turn on the speaker
LOOPC2:
JNB TF0, LOOPC2			;Wait for overflow occurs
CLR P1.7				;Turn off timer 0
CLR TR0					;Turn off the speaker
CLR TF0 				;Clear Overflow flag
DJNZ R6, AGAINC2		;Loop
DJNZ R5, BACKC2
RET						;Return to MUSIC

END						


