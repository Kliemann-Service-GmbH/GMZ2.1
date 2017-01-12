//////////////////////////////////////////////////////////////////////////////
// File: lcd.h
//
//
//  Written By: Micki Szorn
//
//	Description: Gasmeldezentrale 2.1; Funktionen
//
//////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                 Driver for common LCD modules                     ////
////                                                                   ////
////  lcd_init()   Must be called before any other function.           ////
////                                                                   ////
////  lcd_putc(c)  Will display c on the next position of the LCD.     ////
////                     The following have special meaning:           ////
////                      \f  Clear display                            ////
////                      \n  Go to start of second line               ////
////                      \b  Move back one position                   ////
////                                                                   ////
////  lcd_gotoxy(x,y) Set write position on LCD (upper left is 1,1)    ////
////                                                                   ////
////  lcd_getc(x,y)   Returns character at position x,y on LCD         ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////

// As defined in the following structure the pin connection is as follows:
//     C0   D4
//     C1   D5
//     C2   D6
//     C3   D7
//     C4   rs
//     C5   enable
//     GND  rw

//


// Un-comment the following define to use port B
// #define use_portb_lcd TRUE


struct lcd_pin_map {                // This structure is overlayed
           int     data : 4;        // on to an I/O port to gain
           BOOLEAN rs;              // access to the LCD pins.
           BOOLEAN enable;          // The bits are allocated from
           BOOLEAN rw;           	// low order up.  ENABLE will
           BOOLEAN unused; 			// be pin D5.
} lcd;

#byte lcd = 0xF82                   // This puts the entire structure
				                 	// on to port C (at address F82)

#define set_tris_lcd(x) set_tris_c(x)

#define lcd_type 2           // 0=5x7, 1=5x10, 2=2 lines
#define lcd_line_two 0x40    // LCD RAM address for the second line


BYTE const LCD_INIT_STRING[5] = {0x20 | (lcd_type << 2), 0xf, 1, 6,2};
                             // These bytes need to be sent to the LCD
                             // to start it up.


                             // The following are used for setting
                             // the I/O port direction register.

struct lcd_pin_map const LCD_WRITE = {0,0,0,0,0}; // For write mode all pins are out


void lcd_send_nibble( BYTE n ) {
      lcd.data = n;
      delay_cycles(2);
      lcd.enable = 1;
      delay_us(3);
      lcd.enable = 0;
}


void lcd_send_byte( BYTE address, BYTE n ) {

      lcd.rs = 0;
      delay_cycles(2);
	  lcd.rs = address;
      delay_cycles(2);
      lcd.rw = 0;
      delay_cycles(2);
      lcd.enable = 0;
      lcd_send_nibble(n >> 4);
      lcd_send_nibble(n & 0xf);
}


void lcd_init() {
    BYTE i;
    set_tris_lcd(LCD_WRITE);
    lcd.rs = 0;
    lcd.rw = 0;
    lcd.enable = 0;
    delay_ms(15);
    for(i=1;i<=3;++i) {
       lcd_send_nibble(3);
       delay_ms(5);
    }
    lcd_send_nibble(2);
    for(i=0;i<=4;++i)
       lcd_send_byte(0,LCD_INIT_STRING[i]);
}


void lcd_gotoxy( BYTE x, BYTE y) {
   BYTE address;

   if(y!=1)
     address=lcd_line_two;
   else
     address=0;
   address+=x-1;
   lcd_send_byte(0,0x80|address);
}

void lcd_putc( char c) {
   switch (c) {
     case '\f'   : lcd_send_byte(0,1);
                   delay_ms(2);
                                           break;
     case '\b'   : lcd_send_byte(0,0x10);  break;
     default     : lcd_send_byte(1,c);     break;
   }
}

void Sensor(int a)
{
	if(!a)
	{
		memcpy(&Zeile1[1],"Sensor",6);
		Zeile1[7]=((Menue>99)+0x31);
	}
	else
	{
		Zeile1[0]='S';
		Zeile1[1]=(Menue>99)+0x31;
		Zeile1[2]=':';

		if(a==1)
		{
			memcpy(&Zeile1[4],"Alarmpunkt",10);
			Zeile1[14]=Pfad%4+0x31;
		}
	}
}


/** Sensortypen */
void STyp(Kanal,b)
{
    char STypout[5];
    memset(STypout, 0x20, sizeof(STypout));

    if(b==0){STypout[0]='C';STypout[1]='H';STypout[2]='4';}                                         // 000: CH4
    else if(b==1){STypout[0]='N';STypout[1]='H';STypout[2]='3';}                                    // 001: NH3
    else if(b==2){STypout[0]='C';STypout[1]='O';}                                                   // 002: CO (Kohlenmonoxid)
    else if(b==3){STypout[0]='C';STypout[1]='O';STypout[2]='2';}                                    // 003: CO2 (Kohlendioxid)
    else if(b==4){STypout[0]='N';STypout[1]='O';}                                                   // 004: NO (Stickstoffmonoxid, Stickoxid)
    else if(b==5){STypout[0]='N';STypout[1]='O';STypout[2]='2';}                                    // 005: NO2 (Stickstoffdioxid)
    else if(b==6){STypout[0]='C';STypout[1]='3';STypout[2]='H';STypout[3]='6';STypout[4]='O';}      // 006: C3H6O
    else if(b==7){STypout[0]='C';STypout[1]='2';STypout[2]='H';STypout[3]='2';}                     // 007: C2H2
    else if(b==8){STypout[0]='H';STypout[1]='C';}                                                   // 008: HC
    else if(b==9){STypout[0]='R';STypout[1]='2';STypout[2]='2';}                                    // 009: R22
    else if(b==10){STypout[0]='R';STypout[1]='2';STypout[2]='3';}                                   // 010: R23
    else if(b==11){STypout[0]='R';STypout[1]='1';STypout[2]='3';STypout[3]='4';STypout[4]='a';}     // 011: R134a
    else if(b==12){STypout[0]='R';STypout[1]='4';STypout[2]='0';STypout[3]='4';STypout[4]='a';}     // 012: R404a
    else if(b==13){STypout[0]='R';STypout[1]='4';STypout[2]='0';STypout[3]='7';STypout[4]='c';}     // 013: R407c
    else if(b==14){STypout[0]='C';STypout[1]='4';STypout[2]='H';STypout[3]='8';STypout[4]='O';}     // 014: C4H8O
    else if(b==15){STypout[0]='C';STypout[1]='9';STypout[2]='H';STypout[3]='2';STypout[4]='0';}     // 015: C9H20 (Nonan)
    else if(b==16){STypout[0]='C';STypout[1]='3';STypout[2]='H';STypout[3]='8';}                    // 016: Propan R290
    else if(b==17){STypout[0]='C';STypout[1]='4';STypout[2]='H';STypout[3]='1';STypout[4]='0';}     // 017: C4H10
    else if(b==18){STypout[0]='C';STypout[1]='6';STypout[2]='H';STypout[3]='2';}                    // 018: C6H2
    else if(b==19){STypout[0]='R';STypout[1]='4';STypout[2]='1';STypout[3]='0';}                    // 019: R410
    else if(b==20){STypout[0]='R';STypout[1]='a';STypout[2]='u';STypout[3]='c';STypout[4]='h';}     // 020: Rauch (Rauchgas)
    else if(b==21){STypout[0]='C';STypout[1]='2';STypout[2]='H';STypout[3]='6';}                    // 021: C2H6 (Ethan)
    else if(b==22){STypout[0]='C';STypout[1]='2';STypout[2]='H';STypout[3]='6';STypout[4]='O';}     // 022: C2H6O (Ethanol)
    else if(b==23){STypout[0]='C';STypout[1]='8';STypout[2]='H';STypout[3]='8';}                    // 023: C8H8
    else if(b==24){STypout[0]='R';STypout[1]='4';STypout[2]='0';STypout[3]='7';STypout[4]='f';}     // 024: R407f
    else if(b==25){STypout[0]='C';STypout[1]='4';STypout[2]='H';STypout[3]='1';STypout[4]='0';}     // 025: C4H10 Butan R600
    else if(b==26){STypout[0]='C';STypout[1]='L';STypout[2]='2';}                                   // 026: CL2 (Chlor)
    else if(b==27){STypout[0]='R';STypout[1]='6';STypout[2]='0';STypout[3]='1';}                    // 027: R601 (n-Pentan)
    else if(b==28){STypout[0]='H';STypout[1]='2';}                                                  // 028: H2 (Wasserstoff)
    else if(b==29){STypout[0]='C';STypout[1]='5';STypout[2]='H';STypout[3]='1';STypout[4]='2';}     // 029: C5H12 (Pentane) ist das selbe wie R601 (Pentane halt)
    else {STypout[0]='O';STypout[1]='2';}                                                           // Rest: O2 (Sauerstoff)

    memcpy(Typ[Kanal],STypout,5);
}

// SI Einheiten
void SI(int c, int c1)
{
    if(c==0)memcpy(&Zeile2[c1],"UEG",3);
    else if(c==1)memcpy(&Zeile2[c1],"VOL",3);
    else memcpy(&Zeile2[c1],"ppm",3);
}

void RelAnf ( int d )
{
	////////////////////////
	/* Relaisanforderung

		1 = K01
		2 =     K02
		3 =         K03
		4 =             K04
		5 = K01 K02
		6 =     K02 K03
		7 =         K03 K04
		8 = K01     K03
		9 =     K02     K04
		10= K01         K04
		11= K01 K02 K03							//MM
		12= K01 K02     K04						//MM
		13= K01     K03 K04						//MM
		14=     K02 K03 K04						//MM
		15= K01 K02 K03 K04						//MM
	*/
/*
	if(d==1||d==5||d==8||d==10)Zeile2[7]='1';	// MM
	else if(d==2||d==6||d==9)Zeile2[7]='2';
	else if(d==3||d==7)Zeile2[7]='3';
	else Zeile2[7]='4';

	if(d>=5)
	{
		Zeile2[8]=',';Zeile2[10]='K';Zeile2[11]='0';

		if(d==5)Zeile2[12]='2';
		else if(d==6||d==8)Zeile2[12]='3';
		else Zeile2[12]='4';
	}
*/
	if(d==1)Zeile2[6]='1';
	else if(d==2)Zeile2[6]='2';
	else if(d==3)Zeile2[6]='3';
	else if(d==4)Zeile2[6]='4';
	else if(d==5)memcpy(&Zeile2[6],"1 K2",4);
	else if(d==6)memcpy(&Zeile2[6],"2 K3",4);
	else if(d==7)memcpy(&Zeile2[6],"3 K4",4);
	else if(d==8)memcpy(&Zeile2[6],"1 K3",4);
	else if(d==9)memcpy(&Zeile2[6],"2 K4",4);
	else if(d==10)memcpy(&Zeile2[6],"1 K4",4);
	else if(d==11)memcpy(&Zeile2[6],"1 K2 K3",7);
	else if(d==12)memcpy(&Zeile2[6],"1 K2 K4",7);
	else if(d==13)memcpy(&Zeile2[6],"1 K3 K4",7);
	else if(d==14)memcpy(&Zeile2[6],"2 K3 K4",7);
	else memcpy(&Zeile2[6],"1 K2 K3 K4",10);
}

void Ausgabe(unsigned long d,char e,int i)	// d = Wert, e = Komma, i = Stelle in der Zeile
{
char Stelle[5];


	memset(Stelle,0,sizeof(Stelle));		// Stelle[0..4] = 0;

	while(d)	// Zehntausender
	{
		if(d>=10000)Stelle[0]++;
		else break;
		d-=10000;
	}

	while(d)	// Tausender
	{
		if(d>=1000)Stelle[1]++;
		else break;
		d-=1000;
	}

	while(d)	// Hunderter
	{
		if(d>=100)Stelle[2]++;
		else break;
		d-=100;
	}

	while(d)	// Zehner
	{
		if(d>=10)Stelle[3]++;
		else break;
		d-=10;
	}

	Stelle[4]=d;

	if(Stelle[0])Zeile2[i]= Stelle[0]+0x30;
	else Zeile2[i]= 0x20;
	if(e!=2)i++;

	if(Stelle[0]||Stelle[1])Zeile2[i]=Stelle[1]+=0x30;
	else if(!Stelle[0])Zeile2[i]= 0x20;
	i++;

	if(Stelle[0]||Stelle[1]||Stelle[2])Zeile2[i]=Stelle[2]+=0x30;
	else if(!Stelle[0]&&!Stelle[1])Zeile2[i]= 0x20;
	i++;

	if(e==1)
	{
		Zeile2[i]= '.';
		i++;
	}

	if(Stelle[0]||Stelle[1]||Stelle[2]||Stelle[3]||e==2)Zeile2[i]=Stelle[3]+=0x30;
	else if(!Stelle[0]&&!Stelle[1]&&!Stelle[2])Zeile2[i]= 0x20;
	i++;

	if(e==2)
	{
		Zeile2[i]= '.';
		i++;
	}
	Zeile2[i]= Stelle[4]+=0x30;

}

void Display(void)
{
	/////////////////////////////////////////////
	// Anzeige auf dem Display
int i;
	if(MS==0||MS==13)
	{
		lcd_send_byte(0,0x80);
		for(i=0;i<16;i++)lcd_putc(Zeile1[i]);
		lcd_send_byte(0,0xC0);
		for(i=0;i<16;i++)lcd_putc(Zeile2[i]);
	}
}

void PWDisplay(void)
{
	memcpy(&Zeile1[1],"Passwort",8);
	if(Passwort==2)memcpy(&Zeile1[10],"falsch",6);

	Zeile2[1]=P[0];
	Zeile2[2]=P[1];
	Zeile2[3]=P[2];
	Zeile2[4]=P[3];

	Display();

}
