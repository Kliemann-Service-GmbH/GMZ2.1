//////////////////////////////////////////////////////////////////////////////
// File: GMZ2_1_Rauch.c 
//
//
//  Written By: Micki Szorn
//
//	Description: Gasmeldezentrale 2.1; Microchip PIC16F877A
//
//////////////////////////////////////////////////////////////////////////////
// History:
//  25.06.2010	Erstellung
//  28.01.2011  Relaisansteuerung auf 16 Möglichkeiten erweitert
//  04.02.2011  Sauerstoff hinzu
//  06.04.2011  MM Fehler Wertepaaare abspeichern, Freigabe 2 behoben, Alarm-
//				richtung hinzu
//  13.04.2011  MM1 Schwelle von 4 auf 5 mA erhöht, Relais "Halten" berichtigt.
//  08.12.2011  MM2 Sensoren Rauch und R410 hinzu
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Includes

#include <18f4520.h>

#include <Defines.h>
#include <LCD.h>
#include <Functions.h>
#include <Menue.h>

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Hauptprogramm

void main() 
{
	setup_wdt(WDT_ON);		// Watchdog starten
	
	InitPeripheral();

	lcd_init();
	
	systeminit();			// Werte aus dem EEPROM lesen


	lcd_putc('\f');			// Clear display 
	lcd_send_byte(0,0x0C);	// Cursor aus
	Menue = 0;
	Pfad = 0;

	GutMeldung=0;

	if(!Tag)Jahrwartung=1;	

	for(;;)
	{
        restart_wdt();                  // Watchdog rücksetzen

		/////////////////////////////////////////////
		// Analogwerte einlesen, auswerten und anzeigen

		memset(Zeile1, 0x20, sizeof(Zeile1));
		memset(Zeile2, 0x20, sizeof(Zeile2));
		
		if (input(Wartung ))
		{	
			NeuMenue=1;
			
			ADW();
			
			if(!KSZ)
			{
				Alarmpunkte();
				
				if(Alarmalt<Alarm)Hupen=1; 
				Alarmalt=Alarm; 
	 
				if(Stoerung[0]||Stoerung[1])Gutmeldung=0; 
				else Gutmeldung=32; 
				 
				PORTD=Out[0]|Out[1]|GutMeldung|Hupen;
			}
							
			TasterReset();

			if(!JahrWartung)
			{	
				WertAnzeige();
			}
			else AusWart();
		}

		///////////////////////////////////////////
		// oder Konfigurationsmenue 

		else 
		{
			if(NeuMenue)
			{
				Passwort = 1;
				p[0]='0';
				p[1]='0';
				p[2]='0';
				p[3]='0';
				Pos=2;
				Menue=0;
				Pfad=0;
				NeuMenue=0;
			}
			
			if(Passwort)
			{

				PWDisplay();
				
				lcd_gotoxy(Pos,2);
				lcd_send_byte(0,0x0F);	// Cursor ein
				Tasterauswertung();
			}
			else			
			{
				Tasterauswertung();
				Menuefuehrung();
				Untermenue();
				NeuerWert=0;
			}
		}

	}//for(;;)
}
