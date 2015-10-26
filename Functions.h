//////////////////////////////////////////////////////////////////////////////
// File: Functions.h
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
////	Initialisierung

void InitPeripheral (void)
{
    T1CON = 0x00;       // Timer 1 aus
    T3CON = 0x00;       // Timer 3 aus
    CCP1CON = 0x00;     // Capture / Compare aus
    SSPCON1 = 0x00;     // Sync Serial Port aus
    RCSTA = 0x00;       // Serial Port aus
    	
   					// RA0:ADC0,RA1:ADC1,RA2:S1(RESET),RA3:S2(Shift),RA4:Wartung,RA5:S3(UP/DOWN)
   	TRISA = 0x3F;	// all inputs

   	LATB = 0;		// LEDs aus
   	TRISB = 0;		// all outputs

   	LATD = 0;		// RD0:Hupe,RD1-RD5:K1-K5
   	TRISD = 0;		// all outputs

   	LATE = 0x04;	// RE0:S4(LEFT/RIGHT),RE1:S5(ENTER),RE2:PowerLED an
   	TRISE = 0x03;	// RE0-RE1 input

    PIE2  = 0x00;   // unbenutzte Interrupts sperren
    PIE1 &= 0x02;   // Ints für Timer2 bleibt

/*****************************************************************************
Timer 2 interrupt alle 40 ms.
 ( DIV * PERIODE * (POSTSCALE + 1)/( FOSC/4 )) = TIMEOUT
 Hier: ( 16 * 250 * 10 ) / ( 4000000/4 )) = 0.04 s
*****************************************************************************/
	setup_timer_2(T2_DIV_BY_16, 249, 10);	// Start Grundtimer 40 ms
	enable_interrupts(INT_TIMER2);			// Timer Interupt enable
	enable_interrupts(GLOBAL);
}

void systeminit(void) 	// Einlesen der EEPROMDaten
{
int i,j;

	Freigabe[0]=read_eeprom(EEFGS1);

	TT[0]=read_eeprom(EESensortyp1);
	STyp(0,TT[0]);											// Sensortyp

	Einheit[0] = read_eeprom(EEEinheitS1);
	if(Einheit[0]==2)Dezi[0]=0;		// Für Dezimalpunkt auf LCD
	else Dezi[0]=2;

	UG[0] = make16(read_eeprom(EEUGS1),read_eeprom(EEUGS1A));	// Unterer Grenzwert
	OG[0] = make16(read_eeprom(EEOGS1),read_eeprom(EEOGS1A));	// Oberer Grenzwert

	Richtung[0]=read_eeprom(EERichtung1);						//MM

	LinKurv[0] = read_eeprom(EELinKurvS1);						// Auswertung Linear/Kurve

	for(i=0;i<3;i++)
	{
		APSW[0][i] = make16(read_eeprom(EES1AP1SWH+(6*i)),read_eeprom(EES1AP1SWL+(6*i)));	// Alarmpunkt Wert
		APHY[0][i] = make16(read_eeprom(EES1AP1HYH+(6*i)),read_eeprom(EES1AP1HYL+(6*i)));	// Alarmpunkt Hysterese
		APHO[0][i] = read_eeprom(EES1AP1HO+(6*i));	// Alarmpunkt Halten
		APRE[0][i] = read_eeprom(EES1AP1RE+(6*i));	// Alarmpunkt Relais
	}

	for(j=0;j<10;j++)
	{
		WPKon[0][j] = make16(read_eeprom(EES1WP1KonH+2*j),read_eeprom(EES1WP1KonL+2*j));
		WPmA1[j] = make16(read_eeprom(EES1WP1mAH+2*j),read_eeprom(EES1WP1mAL+2*j));
	}


	if(APHO[0][0])bit_set(Halten,0);
	if(APHO[0][1])bit_set(Halten,1);
	if(APHO[0][2])bit_set(Halten,2);


	Freigabe[1]=read_eeprom(EEFGS2);

	TT[1]=read_eeprom(EESensortyp2);
	STyp(1,TT[1]);											// Sensortyp

	Einheit[1] = read_eeprom(EEEinheitS2);
	if(Einheit[1]==2)Dezi[1]=0;		// Für Dezimalpunkt auf LCD
	else Dezi[1]=2;
	
	UG[1] = make16(read_eeprom(EEUGS2),read_eeprom(EEUGS2A));	// Unterer Grenzwert
	OG[1] = make16(read_eeprom(EEOGS2),read_eeprom(EEOGS2A));	// Oberer Grenzwert

	Richtung[1]=read_eeprom(EERichtung2);						//MM

	LinKurv[1] = read_eeprom(EELinKurvS2);						// Auswertung Linear/Kurve

	for(i=0;i<3;i++)
	{
		APSW[1][i] = make16(read_eeprom(EES2AP1SWH+(6*i)),read_eeprom(EES2AP1SWL+(6*i)));	// Alarmpunkt Wert
		APHY[1][i] = make16(read_eeprom(EES2AP1HYH+(6*i)),read_eeprom(EES2AP1HYL+(6*i)));	// Alarmpunkt Hysterese
		APHO[1][i] = read_eeprom(EES2AP1HO+(6*i));	// Alarmpunkt Halten
		APRE[1][i] = read_eeprom(EES2AP1RE+(6*i));	// Alarmpunkt Relais
	}

	for(j=0;j<10;j++)
	{
		WPKon[1][j] = make16(read_eeprom(EES2WP1KonH+2*j),read_eeprom(EES2WP1KonL+2*j));
		WPmA2[j] = make16(read_eeprom(EES2WP1mAH+2*j),read_eeprom(EES2WP1mAL+2*j));
	}
	
	KSZeit = KSZ = read_eeprom(EEKSZeit);
	Tag = make16(read_eeprom(EETagH),read_eeprom(EETagL));
	PW[0] = read_eeprom(PasswortA);
	PW[1] = read_eeprom(PasswortB);
	PW[2] = read_eeprom(PasswortC);
	PW[3] = read_eeprom(PasswortD);

	if(APHO[1][0])bit_set(Halten,3);
	if(APHO[1][1])bit_set(Halten,4);
	if(APHO[1][2])bit_set(Halten,5);

}

void Alarmpunkte (void)
{ 
  
	///////////////////////////////////////////// 
	// Alarmpunktauswertung 
	if(!Richtung[0])
	{
		// Alarmpunkt1 
		if(Messwert[0]>APSW[0][0]) 
		{ 
			 bit_set(Alarm,0); 
			 if(!bit_test(Alarm,2))Out[0]=AnfRelais[APRE[0][0]]; 
		}   
		 
		// Alarmpunkt2 
		if(Messwert[0]>APSW[0][1]) 
		{ 
			 bit_set(Alarm,2); 
			 if(!bit_test(Alarm,4))Out[0]=AnfRelais[APRE[0][1]]; 
		} 
		 
		// Alarmpunkt3 
		if(Messwert[0]>APSW[0][2]) 
		{ 
			 bit_set(Alarm,4); 
			 Out[0]=AnfRelais[APRE[0][2]]; 
		} 
		 
		if(Messwert[0]<APSW[0][2]-APHY[0][2]) 
		{ 
			
	        if(bit_test(Alarm,4))
	        {
				if((Halten&0x4)==0)
		        {
			        Out[0]=AnfRelais[APRE[0][1]];
		    		bit_clear(Alarm,4);
		    	}
		    }
		} 
		 
		if(Messwert[0]<APSW[0][1]-APHY[0][1]) 
		{ 
	        if(bit_test(Alarm,2))
	        {
				if((Halten&0x2)==0&&!bit_test(Alarm,4))	//MM1
		        {
		        	Out[0]=AnfRelais[APRE[0][0]]; 
	       			bit_clear(Alarm,2);
	       		}
	       	}
		} 
		 
		if(Messwert[0]<APSW[0][0]-APHY[0][0]) 
		{ 
	
			 if(bit_test(Alarm,0))
			 {
				if((Halten&0x1)==0&&!bit_test(Alarm,4)&&!bit_test(Alarm,2))	//MM1
		        {
				 	Out[0]=0;
			 		bit_clear(Alarm,0); 
			 	}
			 }
		} 
	}
	else
	{
		// Alarmpunkt1 
		if(Messwert[0]<APSW[0][0]) 
		{ 
			 bit_set(Alarm,0); 
			 if(!bit_test(Alarm,2))Out[0]=AnfRelais[APRE[0][0]]; 
		}   
		 
		// Alarmpunkt2 
		if(Messwert[0]<APSW[0][1]) 
		{ 
			 bit_set(Alarm,2); 
			 if(!bit_test(Alarm,4))Out[0]=AnfRelais[APRE[0][1]]; 
		} 
		 
		// Alarmpunkt3 
		if(Messwert[0]<APSW[0][2]) 
		{ 
			 bit_set(Alarm,4); 
			 Out[0]=AnfRelais[APRE[0][2]]; 
		} 
		 
		if(Messwert[0]>APSW[0][2]+APHY[0][2]) 
		{ 
			
	        if(bit_test(Alarm,4))
	        {
				if((Halten&0x4)==0)
		        {
			        Out[0]=AnfRelais[APRE[0][1]];
		    		bit_clear(Alarm,4);
		    	}
		    }
		} 
		 
		if(Messwert[0]>APSW[0][1]+APHY[0][1]) 
		{ 
	        if(bit_test(Alarm,2))
	        {
				if((Halten&0x2)==0&&!bit_test(Alarm,4))	//MM1
		        {
		        	Out[0]=AnfRelais[APRE[0][0]]; 
	       			bit_clear(Alarm,2);
	       		}
	       	}
		} 
		 
		if(Messwert[0]>APSW[0][0]+APHY[0][0]) 
		{ 
	
			 if(bit_test(Alarm,0))
			 {
				if((Halten&0x1)==0&&!bit_test(Alarm,4)&&!bit_test(Alarm,2))	//MM1
		        {
				 	Out[0]=0;
			 		bit_clear(Alarm,0); 
			 	}
			 }
		}
	} 
	// LEDs entsprechend einschalten 
	output_bit(LEDS1A1,bit_test(Alarm,0)); 
	output_bit(LEDS1A2,bit_test(Alarm,2)); 
	output_bit(LEDS1A3,bit_test(Alarm,4)); 
	 
	///////////////////////////////////////////// 
	// Alarmpunktauswertung 
 	if(!Richtung[1])
	{ 
		// Alarmpunkt1 
		if(Messwert[1]>APSW[1][0]) 
		{ 
			 bit_set(Alarm,1); 
			 if(!bit_test(Alarm,3))Out[1]=AnfRelais[APRE[1][0]]; 
		}   
		 
		// Alarmpunkt2 
		if(Messwert[1]>APSW[1][1]) 
		{ 
			 bit_set(Alarm,3); 
			 if(!bit_test(Alarm,5))Out[1]=AnfRelais[APRE[1][1]]; 
		} 
		 
		// Alarmpunkt3 
		if(Messwert[1]>APSW[1][2]) 
		{ 
			 bit_set(Alarm,5); 
			 Out[1]=AnfRelais[APRE[1][2]]; 
		} 
		 
		if(Messwert[1]<APSW[1][2]-APHY[1][2]) 
		{ 
			
	        if(bit_test(Alarm,5))
	        {
		        if((Halten&0x20)==0)
		        {
			        Out[1]=AnfRelais[APRE[1][1]]; 
	        		bit_clear(Alarm,5);
	        	}
	        } 
		} 
		 
		if(Messwert[1]<APSW[1][1]-APHY[1][1]) 
		{ 
	        if(bit_test(Alarm,3))
	        {
		        if((Halten&0x10)==0&&!bit_test(Alarm,5))	//MM1
		        {
			        Out[1]=AnfRelais[APRE[1][0]]; 
	        		bit_clear(Alarm,3);
	        	}
	        }
		} 
		 
		if(Messwert[1]<APSW[1][0]-APHY[1][0]) 
		{ 
	
			 if(bit_test(Alarm,1))
			 {
				if((Halten&0x8)==0&&!bit_test(Alarm,5)&&!bit_test(Alarm,3))	//MM1
		        {
			        Out[1]=0;
					bit_clear(Alarm,1);
				}
			}
		} 
	}
	else
	{
		// Alarmpunkt1 
		if(Messwert[1]<APSW[1][0]) 
		{ 
			 bit_set(Alarm,1); 
			 if(!bit_test(Alarm,3))Out[1]=AnfRelais[APRE[1][0]]; 
		}   
		 
		// Alarmpunkt2 
		if(Messwert[1]<APSW[1][1]) 
		{ 
			 bit_set(Alarm,3); 
			 if(!bit_test(Alarm,5))Out[1]=AnfRelais[APRE[1][1]]; 
		} 
		 
		// Alarmpunkt3 
		if(Messwert[1]<APSW[1][2]) 
		{ 
			 bit_set(Alarm,5); 
			 Out[1]=AnfRelais[APRE[1][2]]; 
		} 
		 
		if(Messwert[1]>APSW[1][2]+APHY[1][2]) 
		{ 
			
	        if(bit_test(Alarm,5))
	        {
		        if((Halten&0x20)==0)
		        {
			        Out[1]=AnfRelais[APRE[1][1]]; 
	        		bit_clear(Alarm,5);
	        	}
	        } 
		} 
		 
		if(Messwert[1]>APSW[1][1]+APHY[1][1]) 
		{ 
	        if(bit_test(Alarm,3))
	        {
		        if((Halten&0x10)==0&&!bit_test(Alarm,5))	//MM1
		        {
			        Out[1]=AnfRelais[APRE[1][0]]; 
	        		bit_clear(Alarm,3);
	        	}
	        }
		} 
		 
		if(Messwert[1]>APSW[1][0]+APHY[1][0]) 
		{ 
	
			 if(bit_test(Alarm,1))
			 {
				if((Halten&0x8)==0&&!bit_test(Alarm,5)&&!bit_test(Alarm,3))	//MM1
		        {
			        Out[1]=0;
					bit_clear(Alarm,1);
				}
			}
		}
	}

	// LEDs entsprechend einschalten 

	output_bit(LEDS2A1,bit_test(Alarm,1)); 
	output_bit(LEDS2A2,bit_test(Alarm,3)); 
	output_bit(LEDS2A3,bit_test(Alarm,5)); 

	if(Hupen)
	{
	if(APHO[0][0])bit_set(Halten,0);
	if(APHO[0][1])bit_set(Halten,1);
	if(APHO[0][2])bit_set(Halten,2);
	if(APHO[1][0])bit_set(Halten,3);
	if(APHO[1][1])bit_set(Halten,4);
	if(APHO[1][2])bit_set(Halten,5);

	}
} 

void Auswertung0(void)
{
int32 Tmp;
long ADCTmp;
int KurvenBereich;

	/////////////////////////////////////////////
	// Stör - und Wartungsmeldungen
	// 

	if(mAWert[0]<Wert4mA||mAWert[0]>Wert20mA)
	{
		
		if(mAWert[0]>2200)								// größer 22 mA
		{
			if(Stoerung[0]!=1)
			{
				FuenfSekWarten=0;
				Stoerung[0]=1;
			}
			if(FuenfSekWarten>=5)
			{
				SMLEDTmp[0]=(MS<12);					// Störung LED
				bit_set(Alarm,6);						// Hupe an
			}
		}

		else
		{
			if(mAWert[0]<250)							// kleiner 2,5 mA 
			{											// Sensor defekt
				if(JahrWartung!=2)
				{
					memcpy(&Zeile1[1],"Sensor",6);
					memcpy(&Zeile2[1],"defekt",6);
				}					

				if(Stoerung[0]!=2)
				{
					FuenfSekWarten=0;
					Stoerung[0]=2;
				}
				if(FuenfSekWarten>=5)
				{
					SMLEDTmp[0]=1;
					bit_set(Alarm,6);					// Hupe an
				}
			}	
			else if(mAWert[0]<300)						// kleiner 3,0 mA
			{	
				if(JahrWartung!=2)
				{
						memcpy(&Zeile1[1],"Drift",5);
						memcpy(&Zeile2[1],"zu hoch",7);
				}

				if(Stoerung[0]!=3)
				{
					FuenfSekWarten=0;
					Stoerung[0]=3;
				}
				if(FuenfSekWarten>=5)
				{										// Sensordrift zu hoch
					SMLEDTmp[0]=(MS<12);
				}
			}
		}
		
		if(mAWert[0]<Wert4mA)mAWert[0]=Wert4mA;			// Wert für Anzeige begrenzen
		else mAWert[0]=Wert20mA;

	}
	
	else
	{
		if(mAWert[0]<500)mAWert[0]=Wert4mA;			// MM1

		Stoerung[0]=0;
		SMLEDTmp[0]=0;
		bit_clear(Alarm,6);					// Hupe aus bei Gutmeldung
		bit_clear(Alarmalt,6);				

	}
	if(!JahrWartung)
	{
		output_bit(LEDS1SM,SMLEDTmp[0]);				// Störung LED 1
	}	

	if(LinKurv[0])
	{
	/////////////////////////////////////////////
	// Umrechnung AD-Wert in Abhängigkeit der Kurve
	// (Messwert - Nullpunkt (4mA)) * Messbereich
	// geteilt durch ( Maxwert (20mA) - Nullpunkt (4mA))

		for(KurvenBereich=0;KurvenBereich<10;KurvenBereich++)
		{
			if(mAWert[0]>=WPmA1[KurvenBereich]&&
			mAWert[0]<=WPmA1[KurvenBereich+1])
			{
				BereichmA[0]=WPmA1[KurvenBereich+1]-WPmA1[KurvenBereich];
				UGmA[0]=WPmA1[KurvenBereich];
				break;
			}
		}

		BereichKon[0]=WPKon[0][KurvenBereich+1]-WPKon[0][KurvenBereich];
		UG[0]=WPKon[0][KurvenBereich];

	} 
	else
	{
		BereichKon[0]=OG[0]-UG[0];				// Messbereich
		BereichmA[0]=1600;
		UGmA[0]=400;
	}
	/////////////////////////////////////////////
	// Lineare Umrechnung AD-Wert in Prozent oder ppm
	// (Messwert - Untergrenze mA) * Messbereich
	// geteilt durch ( Maxwert (20mA) - Nullpunkt (4mA))

	ADCTmp=mAWert[0]-UGmA[0];
	Tmp=(int32)ADCTmp*(int32)BereichKon[0];
	Tmp/=(int32)BereichmA[0];
	Messwert[0]=Tmp;
	Messwert[0]+=UG[0];
}

void Auswertung1(void)
{
int32 Tmp;
long ADCTmp;
int KurvenBereich;

	/////////////////////////////////////////////
	// Stör - und Wartungsmeldungen
	// 

	if(mAWert[1]<Wert4mA||mAWert[1]>Wert20mA)
	{
		
		if(mAWert[1]>2200)								// größer 22 mA
		{
			if(Stoerung[1]!=1)
			{
				FuenfSekWarten=0;
				Stoerung[1]=1;
			}
			if(FuenfSekWarten>=5)
			{
				SMLEDTmp[1]=(MS<12);					// Störung LED
				bit_set(Alarm,7);						// Hupe an
			}
		}

		else
		{
			if(mAWert[1]<250)							// kleiner 2,5 mA 
			{											// Sensor defekt
				if(JahrWartung!=2)
				{
					memcpy(&Zeile1[10],"Sensor",6);
					memcpy(&Zeile2[10],"defekt",6);
				}

				if(Stoerung[1]!=2)
				{
					FuenfSekWarten=0;
					Stoerung[1]=2;
				}
				if(FuenfSekWarten>=5)
				{
					SMLEDTmp[1]=1;
					bit_set(Alarm,7);			// Hupe an
				}
			}	
			else if(mAWert[1]<300)						// kleiner 3,0 mA
			{	
				if(JahrWartung!=2)
				{
					memcpy(&Zeile1[9],"Drift",5);
					memcpy(&Zeile2[9],"zu hoch",7);
				}

				if(Stoerung[1]!=3)
				{
					FuenfSekWarten=0;
					Stoerung[1]=3;
				}
				if(FuenfSekWarten>=5)
				{										// Sensordrift zu hoch
					SMLEDTmp[1]=(MS<12);
				}
			}
		}
		
		if(mAWert[1]<Wert4mA)mAWert[1]=Wert4mA;			// Wert für Anzeige begrenzen
		else mAWert[1]=Wert20mA;

	}
	
	else
	{
		if(mAWert[1]<500)mAWert[1]=Wert4mA;			// MM1

		Stoerung[1]=0;
		SMLEDTmp[1]=0;
		bit_clear(Alarm,7);					// Hupe aus bei Gutmeldung
		bit_clear(Alarmalt,7);				

	}
	if(!JahrWartung)
	{
		output_bit(LEDS2SM,SMLEDTmp[1]);				// Störung LED 2
	}	

	if(LinKurv[1])
	{
	/////////////////////////////////////////////
	// Umrechnung AD-Wert in Abhängigkeit der Kurve
	// (Messwert - Nullpunkt (4mA)) * Messbereich
	// geteilt durch ( Maxwert (20mA) - Nullpunkt (4mA))

		for(KurvenBereich=0;KurvenBereich<10;KurvenBereich++)
		{
			if(mAWert[1]>=WPmA2[KurvenBereich]&&
			mAWert[1]<=WPmA2[KurvenBereich+1])
			{
				BereichmA[1]=WPmA2[KurvenBereich+1]-WPmA2[KurvenBereich];
				UGmA[1]=WPmA2[KurvenBereich];
				break;
			}
		}

		BereichKon[1]=WPKon[1][KurvenBereich+1]-WPKon[1][KurvenBereich];
		UG[1]=WPKon[1][KurvenBereich];

	} 
	else
	{
		BereichKon[1]=OG[1]-UG[1];				// Messbereich
		BereichmA[1]=1600;
		UGmA[1]=400;
	}
	/////////////////////////////////////////////
	// Lineare Umrechnung AD-Wert in Prozent oder ppm
	// (Messwert - Untergrenze mA) * Messbereich
	// geteilt durch ( Maxwert (20mA) - Nullpunkt (4mA))

	ADCTmp=mAWert[1]-UGmA[1];
	Tmp=(int32)ADCTmp*(int32)BereichKon[1];
	Tmp/=(int32)BereichmA[1];
	Messwert[1]=Tmp;
	Messwert[1]+=UG[1];

}
void ADW(void)
{
long ADCM[2];


	/////////////////////////////////////////////
	// Analog - Digital Wandlung
	// 20 mA * 200 Ohm / 5 Volt * 1024 = 819
	//
	// Mittelwertbildung über 8 Messungen

	if(Freigabe[0])
	{
		ADCM[0]=0;
		/////////////////////////////////////////////
		// Sensor 1

		ADCON1 = 0x0D;		// AD Control Register1:Ref auf Betriebsspg, RA0 und RA1 auf analog
							// alle anderen auf digital
		ADCON0 = 0x00;		// Select Channel RA0
		ADCON2 = 0xA0;		// Select Acquisition Time ( 8 TAD )
		ADCON2 = 0xA1;		// Select AD Conversion Clock ( FOsc/8 )
		
		ADCON0 = 0x01;		// Turnon AD - Module
	
	    delay_us(40);
	
	   	// Start conversion
	   	ADCON0 = 0x03;
	   	// wait for conversion complete indicated by GO_DONE bit=0
	
	   	while ( TRUE )
	    {
		   	if (ADCON0 ==0x01 ) { break; }
	   	}
	   	
	   	MW8S1[MW8] = make16(ADRESH,ADRESL);	

		ADCM[0]=MW8S1[0]+MW8S1[1]+MW8S1[2]+MW8S1[3]+MW8S1[4]+MW8S1[5]+MW8S1[6]+MW8S1[7];
		ADC_ip[0] = ADCM[0] >>3;
		
		/////////////////////////////////////////////
		// Umrechnen in MilliAmpere
	
		mAWert[0]=(int32)ADC_ip[0];
		mAWert[0]*=2000;
		mAWert[0]/=ADWert20mA;

		Auswertung0();
	}
	if(Freigabe[1])
	{
		ADCM[1]=0;
		/////////////////////////////////////////////
		// Sensor 2

		ADCON1 = 0x0D;		// AD Control Register1:Ref auf Betriebsspg, RA0 und RA1 auf analog
							// alle anderen auf digital
		ADCON0 = 0x04;		// Select Channel RA1
		ADCON2 = 0xA0;		// Select Acquisition Time ( 8 TAD )
		ADCON2 = 0xA1;		// Select AD Conversion Clock ( FOsc/8 )
		
		ADCON0 = 0x05;		// Turnon AD - Module
	
	    delay_us(40);
	
	   	// Start conversion
	   	ADCON0 = 0x07;
	   	// wait for conversion complete indicated by GO_DONE bit=0

	   	while ( TRUE )
	    {
		   	if (ADCON0 ==0x05 ) { break; }
	   	}
	   	
	   	MW8S2[MW8] = make16(ADRESH,ADRESL);
		
		ADCM[1]=MW8S2[0]+MW8S2[1]+MW8S2[2]+MW8S2[3]+MW8S2[4]+MW8S2[5]+MW8S2[6]+MW8S2[7];
		ADC_ip[1] = ADCM[1] >>3;

		/////////////////////////////////////////////
		// Umrechnen in MilliAmpere
	
		mAWert[1]=(int32)ADC_ip[1];
		mAWert[1]*=2000;
		mAWert[1]/=ADWert20mA;

		Auswertung1();
	}

	ADCON1 = 0x0F;
	
	MW8++;
	if(MW8>7)MW8=0;
}

void WertAnzeige(void)
{

	/////////////////////////////////////////////
	// Messwerte an das Display ausgeben
	
	if(Freigabe[0]&&!Stoerung[0])
	{
		memcpy(&Zeile1[2],Typ[0],sizeof Typ[0]);

		Ausgabe(Messwert[0],Dezi[0],0);
		SI(Einheit[0],5);
	}
	if(Freigabe[1]&&!Stoerung[1])
	{
		memcpy(&Zeile1[10],Typ[1],sizeof Typ[1]);

		Ausgabe(Messwert[1],Dezi[1],8);
		SI(Einheit[1],13);
	}
	Display();
}

void TasterReset(void)
{
	if(input(TReset))
	{
		// Relais Halten bis 3 Sek gedrückt
		if(DreiSekWarten==3)
		{
			Halten=0;
//			Alarm=0;
		}

		if(TResetgedr==0)
		{
			TResetgedr=1;
			Menue=0;
			Pfad=0;
			Aenderung=0;
			Eingabemoeglich=0;
			Eingabe=0;
			Byte12=1;
			Passwort=0;
			lcd_send_byte(0,0x0C);	// Cursor aus
			Hupen=0;
			output_bit(HUPE,0);
		}
	}
	else
	{
		if(TResetgedr)TResetgedr=0;
		DreiSekWarten=0;
	}
}
	
void Tasterauswertung(void)
{

	if(input(TUpDown))		// Taster gedrückt
	{
		if(!TUpDowngedr&&!TEntp)	// Entprellzeit abgelaufen
		{
			TEntp=10;
			TUpDowngedr=1;
			if(Passwort)
			{
				if(Passwort==2)Passwort=1;
				if(!Umschalt)
				{
					P[Pos-2]++;
					if(P[Pos-2]>0x39)P[Pos-2]=0x30;
				}
				else
				{
					P[Pos-2]--;
					if(P[Pos-2]<0x30)P[Pos-2]=0x39;
				}
				Display();
			}

			else if(Aenderung)
			{
				NeuerWert=1;
				if(FuenfSekWarten==3)Tempo=10;
				if(FuenfSekWarten==5)Tempo=100;
				if(FuenfSekWarten==8)Tempo=1000;
				
				TUpDowngedr=0;
			}

			else
			{
				switch(Pfad)
				{
					case 0:		// Sensor1 - Sensor2 - System
						if(Menue==200)Menue=0;
						else Menue+=100;
						break;
					case 1:		// Sensor1 - AP1 - AP2 - AP3 - Konfiguration
						if(Menue==40)Menue=10;
						else Menue+=10;
						break;
					case 2:		// Sensor2 - AP1 - AP2 - AP3 - Konfiguration
						if(Menue==140)Menue=110;
						else Menue+=10;
						break;
					case 3:	// System-Kaltstart-Systemzeit-Wartung-Passwort
						if(Menue==213)Menue=211;
						else Menue++;
						break;
					case 4:
						if(Menue==14)Menue=11;
						else Menue++;
						break;
					case 5:
						if(Menue==24)Menue=21;
						else Menue++;
						break;
					case 6:
						if(Menue==34)Menue=31;
						else Menue++;
						break;
					case 7:
						if(Menue==LinKurv[0]*10+47)Menue=41;	//MM
						else Menue++;
						break;
					case 8:
						if(Menue==114)Menue=111;
						else Menue++;
						break;
					case 9:
						if(Menue==124)Menue=121;
						else Menue++;
						break;
					case 10:
						if(Menue==134)Menue=131;
						else Menue++;
						break;
					case 11:
						if(Menue==LinKurv[1]*10+147)Menue=141;	//MM
						else Menue++;
						break;
				}
			}
		}
	}	

	else
	{
		if(TUpDowngedr)TUpDowngedr=0;
		Tempo=1;
		DreiSekWarten=0;
		FuenfSekWarten=0;
	}
	
	if(input(TEnter))
	{
		if(TEntergedr==0&&!TEntp)
		{
			TEntp=10;
			TEntergedr=1;
			

			if(Passwort==1)
			{
				if((P[0]==PW[0]&&P[1]==PW[1]&&P[2]==PW[2]&&P[3]==PW[3])||
					(P[0]=='2'&&P[1]=='3'&&P[2]=='1'&&P[3]=='1'))			// Masterpasswort 2311 (seit 10/2015 stm)
				{
					Passwort=0;
					lcd_send_byte(0,0x0C);	// Cursor aus
					Eingabemoeglich=0;
					Pos=2;
				}
				else Passwort=2;
			}
		
			else if(Eingabemoeglich==1)
			{
				if(Eingabe==0)
				{
					Aenderung=1;
					Pos=2;
				}

				else
				{
					if(Passwort==3)
					{
						PW[0]=p[0];
						write_eeprom(PASSWORTA,PW[0]);
						PW[1]=p[1];
						write_eeprom(PASSWORTB,PW[1]);
						PW[2]=p[2];
						write_eeprom(PASSWORTC,PW[2]);
						PW[3]=p[3];
						write_eeprom(PASSWORTD,PW[3]);
						Passwort=0;
					}
					else if(WWert)
					{
						write_eeprom(EEAdresse,Byte1);		// Wert schreiben Konzentration
						write_eeprom(EEAdresse+1,Byte2);	// 16 bit Wert schreiben
						write_eeprom(EEAdresse+20,Byte3);	// Wert schreiben mA
						write_eeprom(EEAdresse+21,Byte4);	// 16 bit Wert schreiben
					}
					else
					{
						write_eeprom(EEAdresse,Byte1);					// 8 bit Wert schreiben
						if(Byte12==2)write_eeprom(EEAdresse+1,Byte2);	// 16 bit Wert schreiben
						
						Byte12=1;
					}
					Aenderung=0;
				}
				Eingabe=1-Eingabe;

			}
			
			else
			{
				if(Menue==0)
				{
					Pfad=1;
					Menue=10;
				}	
				else if(Menue==100)
				{
					Pfad=2;
					Menue=110;
				}
				else if(Menue==200)
				{
					Pfad=3;
					Menue=211;
				}
				else if(Menue==10)
				{
					Pfad=4;
					Menue=11;
				}
				else if(Menue==20)
				{
					Pfad=5;
					Menue=21;
				}
				else if(Menue==30)
				{
					Pfad=6;
					Menue=31;
				}
				else if(Menue==40)
				{
					Pfad=7;
					Menue=41;
				}
				else if(Menue==110)
				{
					Pfad=8;
					Menue=111;
				}
				else if(Menue==120)
				{
					Pfad=9;
					Menue=121;
				}
				else if(Menue==130)
				{
					Pfad=10;
					Menue=131;
				}
				else if(Menue==140)
				{
					Pfad=11;
					Menue=141;
				}
			}
		}
	}
	else
	{
		if(TEntergedr)TEntergedr=0;
	}
	
	if(input(TLeftRight))	// Taster gedrückt
	{
		if(TLeftRightgedr==0&&!TEntp)	// Entprellzeit abgelaufen
		{
			TEntp=10;
			TLeftRightgedr=1;
			if(Passwort)
			{
				if(Passwort==2)Passwort=1;
				if(!Umschalt)
				{
					Pos++;
					if(Pos==6)Pos=2;
				}
				else
				{
					Pos--;
					if(Pos==1)Pos=5;
				}
			}

			else if(Aenderung)
			{
				if(Pos==2)Pos=15;
				else Pos=2;
			}
		}
	}
	else
	{
		if(TLeftRightgedr)TLeftRightgedr=0;
	}
	
	if(Passwort!=1&&!Aenderung) TasterReset();

	if(input(TShift))
	{
		if(TShiftgedr==0)
		{
			TShiftgedr=1;
			Umschalt=1-Umschalt;
		}
	}
	else
	{
		if(TShiftgedr)TShiftgedr=0;
	}
	
	if(Menue%10!=0||Menue==50||Menue==150)Eingabemoeglich=1;
	
	if(Passwort!=1)
	{
		if(Aenderung)
		{
			lcd_gotoxy(Pos,2);
			lcd_send_byte(0,0x0F);	// Cursor an
		}
		else 
		{
			lcd_send_byte(0,0x0C);	// Cursor aus
		}
	}
}

void AusWart ( void )
{
	if(!Sek)
	{
		JahrWartung=2;

		memcpy(Zeile1,"BGV/UVV: Wartung",16);
		memcpy(Zeile2,"vorgeschrieben",14);
		Display();
	}
	else if(Sek==20)
	{
		JahrWartung=2;

		memcpy(Zeile1,"RA-GAS GmbH",11);
		memcpy(Zeile2,"07151/930440",12);
		Display();
	}
	else if(Sek >=40)
	{
		JahrWartung=1;
		WertAnzeige();
	}
	
	output_bit(LEDS1SM,MS<12);				// Störung LED 1
	output_bit(LEDS2SM,MS<12);				// Störung LED 2

}

#int_timer2
void Timer2Int (void)	// 40 Millisekunden 
{

	if(TEntp!=0)TEntp--;	// Entprellung 400 ms

	MS++;

	if(MS==25)
	{
		Sek++;
		MS=0;
		if(FuenfSekWarten<8)FuenfSekWarten++;
		if(DreiSekWarten<3)DreiSekWarten++;
	}

	if(Sek==60)
	{
		Min++;
		Sek=0;
		if(KSZ)KSZ--;
	}
	
	if(Min==60)
	{
		Std++;
		Sek=0;
		Min=0;
	}
	if(Std==24)
	{
		if(Tag)
		{
			Tag--;
			write_eeprom(EETagH,make8(Tag,1));	// Tage bis zur Jahreswartung speichern
			write_eeprom(EETagL,make8(Tag,0));	
		}
		if(!Tag) JahrWartung=1;
		Std=0;
	}
}
