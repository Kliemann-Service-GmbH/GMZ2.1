//////////////////////////////////////////////////////////////////////////////
// File: Menue.h
//
//
//  Written By: Micki Szorn
//
//	Description: Gasmeldezentrale 2.1; Menueführung
//
//////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
/*

Menueführung:
Wartungsjumper gesetzt
		|
		V
0 : Sensor 1 -> 10 : AP1 -> 20 : AP2 -> 30 : AP3 ->	40 : Konfiguration
		|			|			|			|			|
		|			V			V			V			V
		|		11: Wert	21: Wert	31: Wert	41 : Freigabe
		|		12: Hyst	22: Hyst	32: Hyst	42 : SensorTyp
		|		13: Halten  23: Halten  33: Halten 	43 : Einheit
		|		14: Relais  24: Relais  34: Relais	44 : Untergrenze
		|											45 : Obergrenze
		|											46 : Alarmrichtung	//MM
		|											47 : Linear-Kurve
		|											48 : Wertepaar 1
		|														:
		V											57 : Wertepaar 10

100 : Sensor 2 -> 110 : AP1 -> 120 : AP2 -> 130 : AP3 -> 140 : Konfiguraion
		|			|			|				|				|
		|			V			V				V				V
		|		  111: Wert	   121: Wert	131: Wert	 141 : Freigabe
		|		  112: Hyst	   122: Hyst	132: Hyst	 142 : SensorTyp
		|		  113: Halten  123: Halten	133: Halten  143 : Einheit
		|		  114: Relais  124: Relais	134: Relais	 144 : Untergrenze
		|												 145 : Obergrenze
		|												 146 : Alarmrichtung //MM
		|												 147 : Linear-Kurve
		|												 148 : Wertepaar 1
		|													 :
		V												 157 : Wertepaar 10

200 : System -> 211 : Kaltstart -> 212 : WartungReset -> 213 : Passwort

*/

void AnderWert (int Tmp1, int Tmp)
{
	Byte12=2;
	if(!Umschalt)
	{
		if(APSW[Tmp1][Tmp]<=(Limit-Tempo))APSW[Tmp1][Tmp]+=Tempo;
	}
	else
	{
		if(APSW[Tmp1][Tmp]>=Tempo)APSW[Tmp1][Tmp]-=Tempo;
	}
	Byte1=make8(APSW[Tmp1][Tmp],1);
	Byte2=make8(APSW[Tmp1][Tmp],0);
	if(!Tmp1)EEAdresse=EES1AP1SWH+(6*Tmp);
	else EEAdresse=EES2AP1SWH+(6*Tmp);
}

void AnderHyst (int Tmp1, int Tmp)
{
	Byte12=2;
	if(!Umschalt)
	{
		if(APHY[Tmp1][Tmp]<=(Limit-Tempo))APHY[Tmp1][Tmp]+=Tempo;
	}
	else
	{
		if(APHY[Tmp1][Tmp]>=Tempo)APHY[Tmp1][Tmp]-=Tempo;
	}
	Byte1=make8(APHY[Tmp1][Tmp],1);
	Byte2=make8(APHY[Tmp1][Tmp],0);
	if(!Tmp1)EEAdresse=EES1AP1HYH+(6*Tmp);
	else EEAdresse=EES2AP1HYH+(6*Tmp);
}

void AnderHalt (int Tmp1, int Tmp)
{
	Byte1=APHO[Tmp1][Tmp]=1-APHO[Tmp1][Tmp];
	if(!Tmp1)EEAdresse=EES1AP1HO+(6*Tmp);
	else EEAdresse=EES2AP1HO+(6*Tmp);
}

void AnderRel (int Tmp1, int Tmp)
{
	Byte12=1;
	if(!Umschalt)
	{
//		if(APRE[Tmp1][Tmp]<16)APRE[Tmp1][Tmp]++;	//MM
		if(APRE[Tmp1][Tmp]<16)APRE[Tmp1][Tmp]++;
	}
	else
	{
		if(APRE[Tmp1][Tmp]>1)APRE[Tmp1][Tmp]--;
	}
	Byte1=APRE[Tmp1][Tmp];
	if(!Tmp1)EEAdresse=EES1AP1RE+(6*Tmp);
	else EEAdresse=EES2AP1RE+(6*Tmp);
}

void LinKu(int Paar, int Kanal)
{
	Sensor(2);
	memcpy(&Zeile1[4],"Wertepaar ",10);
	if(Paar<9)Zeile1[14]=Paar+0x31;
	else memcpy(&Zeile1[14],"10",2);

	if(Aenderung&&NeuerWert)
	{
		WWert=1;
		if(!Umschalt)
		{
			if(Pos==2)
			{
				if(WPKon[Kanal][Paar]<=(Limit-Tempo))WPKon[Kanal][Paar]+=Tempo;
			}
			else
			{
				if(!Kanal)
				{
					if(WPmA1[Paar]<=(2000-Tempo))WPmA1[Paar]+=Tempo;
				}
				else
				{
					if(WPmA2[Paar]<=(2000-Tempo))WPmA2[Paar]+=Tempo;
				}
			}
		}
		else
		{
			if(Pos==2)
			{
				if(WPKon[Kanal][Paar]>=Tempo)WPKon[Kanal][Paar]-=Tempo;
			}
			else
			{
				if(!Kanal)
				{
					if(WPmA1[Paar]>=Tempo)WPmA1[Paar]-=Tempo;
				}
				else
				{
					if(WPmA2[Paar]>=Tempo)WPmA2[Paar]-=Tempo;
				}
			}
		}
		Byte1=make8(WPKon[Kanal][Paar],1);
		Byte2=make8(WPKon[Kanal][Paar],0);

		if(!Kanal)
		{
			EEAdresse=EES1WP1KonH+(2*Paar);
			Byte3=make8(WPmA1[Paar],1);
			Byte4=make8(WPmA1[Paar],0);
		}
		else
		{
			EEAdresse=EES2WP1KonH+(2*Paar);
			Byte3=make8(WPmA2[Paar],1);
			Byte4=make8(WPmA2[Paar],0);
		}
	}
	Ausgabe(WPKon[Kanal][Paar],Dezi[Kanal],0);
	if(Einheit[Kanal]==2){Zeile2[5]='p';Zeile2[6]='p';Zeile2[7]='m';}
	else Zeile2[5]='%';

	if(Kanal==0)Ausgabe(WPmA1[Paar],1,8);
	else Ausgabe(WPmA2[Paar],1,8);
	Zeile2[14]='m';Zeile2[15]='A';
}

void Untermenue ( void )
{
int Tmp,Tmp1;

	Tmp=0;
	if(Menue<99)Tmp1=0;
	else Tmp1=1;

//	Byte12=1;

	if(Einheit[Tmp1]==2)
	{
		Limit=65000;
		Dezi[Tmp1]=0;
	}
	else
	{
		Limit=1000;
		Dezi[Tmp1]=2;
	}

	switch (Menue)
	{
		case 11:	//Wert
//			Tmp=0;
			goto Wert;
		case 21:
			Tmp=1;
			goto Wert;
		case 31:
			Tmp=2;
			goto Wert;
		case 111:
//			Tmp=0;
			Tmp1=1;
			goto Wert;
		case 121:
			Tmp=1;
			Tmp1=1;
			goto Wert;
		case 131:
			Tmp=2;
			Tmp1=1;

Wert:		Sensor(1);

			memcpy(Zeile2,"Wert:",5);
			if(Aenderung&&NeuerWert)AnderWert(Tmp1,Tmp);

			Ausgabe(APSW[Tmp1][Tmp],Dezi[Tmp1],5);
			SI(Einheit[Tmp1],11+Dezi[Tmp1]);
			break;

		case 12:	//Hysterese EES1AP1HY
//			Tmp=0;
			goto Hysterese;
		case 22:
			Tmp=1;
			goto Hysterese;
		case 32:
			Tmp=2;
			goto Hysterese;
		case 112:
//			Tmp=0;
			Tmp1=1;
			goto Hysterese;
		case 122:
			Tmp=1;
			Tmp1=1;
			goto Hysterese;
		case 132:
			Tmp=2;
			Tmp1=1;

Hysterese:	Sensor(1);

			memcpy(Zeile2,"Hyst:",5);
			if(Aenderung&&NeuerWert)AnderHyst(Tmp1,Tmp);

			Ausgabe(APHY[Tmp1][Tmp],Dezi[Tmp1],5);
			SI(Einheit[Tmp1],11+Dezi[Tmp1]);
			break;

		case 13:	//Halten
//			Tmp=0;
			goto Halt;
		case 23:
			Tmp=1;
			goto Halt;
		case 33:
			Tmp=2;
			goto Halt;
		case 113:
//			Tmp=0;
			Tmp1=1;
			goto Halt;
		case 123:
			Tmp=1;
			Tmp1=1;
			goto Halt;
		case 133:
			Tmp=2;
			Tmp1=1;

Halt:		Sensor(1);

			if(Aenderung&&NeuerWert)AnderHalt(Tmp1,Tmp);

			if(!APHO[Tmp1][Tmp])memcpy(Zeile2,"Halten: Nein",12);
			else memcpy(Zeile2,"Halten: Ja",10);

			break;

		case 14:	//Relais
//			Tmp=0;
			goto Relais;
		case 24:
			Tmp=1;
			goto Relais;
		case 34:
			Tmp=2;
			goto Relais;
		case 114:
//			Tmp=0;
			Tmp1=1;
			goto Relais;
		case 124:
			Tmp=1;
			Tmp1=1;
			goto Relais;
		case 134:
			Tmp=2;
			Tmp1=1;

Relais:		Sensor(1);

//			memcpy(Zeile2,"Rel: K0",7);
			memcpy(Zeile2,"Rel: K",6);			//MM
			if(Aenderung&&NeuerWert)AnderRel(Tmp1,Tmp);

			RelAnf(APRE[Tmp1][Tmp]);

			break;
		}

	Display();
}

void Menuefuehrung ( void )
{

int Tmp,Tmp1;

	Tmp=0;
	if(Menue<99)Tmp1=0;
	else Tmp1=1;

//	Byte12=1;

	if(Einheit[Tmp1]==2)
	{
		Limit=65000;
		Dezi[Tmp1]=0;
	}
	else
	{
		Limit=1000;
		Dezi[Tmp1]=2;
	}

	if(Menue==0||Menue==100||Menue==200)
	{
		memcpy(&Zeile1[1],"Einstellungen",13);

		if(Menue<200)
		{
			memcpy(&Zeile2[1],"Sensor",6);
			Zeile2[7]=((Menue==100)+0x31);
		}
		else memcpy(&Zeile2[1],"System",6);

	}

	else if(Menue>=48&&Menue<=57&&LinKurv[0])		//MM nur bei Kurvenauswertung
	{
		Tmp = Menue-48;								//MM
		LinKu(Tmp,Tmp1);
	}

	else if(Menue>=148&&Menue<=157&&LinKurv[1])		//MM nur bei Kurvenauswertung
	{
		Tmp = Menue-148;								//MM
		LinKu(Tmp,Tmp1);
	}

	else if(Menue%10==0)
	{
		Sensor(0);

		if(Menue==40||Menue==140)memcpy(&Zeile2[1],"Konfiguration",13);
		else
		{
			memcpy(&Zeile2[1],"Alarmpunkt",10);
			if(Menue==10||Menue==110)Zeile2[11]='1';
			else if(Menue==20||Menue==120)Zeile2[11]='2';
			else Zeile2[11]='3';
		}
	}

	else if(Menue==41||Menue==141)
	{
		Sensor(2);
		memcpy(&Zeile1[4],"Freigabe",8);
		if(Aenderung&&NeuerWert)
		{
			Byte1=Freigabe[Tmp1]=1-Freigabe[Tmp1];
			if(!Tmp1)EEAdresse=EEFGS1;
			else EEAdresse=EEFGS2;
		}
		if(Freigabe[Tmp1])memcpy(&Zeile2[4],"Ja",2);
		else memcpy(&Zeile2[4],"Nein",4);
	}

	else if(Menue==42||Menue==142)
	{
		Sensor(2);
		memcpy(&Zeile1[4],"Typ",3);
		if(Aenderung&&NeuerWert)
		{
			if(TT[Tmp1]<21)TT[Tmp1]++;	// MM2
			else TT[Tmp1]=0;
			Byte1=TT[Tmp1];
			if(!Tmp1)EEAdresse=EESensortyp1;
			else EEAdresse=EESensortyp2;
			STyp(Tmp1,TT[Tmp1]);
		}
		memcpy(&Zeile2[4],Typ[Tmp1],5);
	}

	else if(Menue==43||Menue==143)
	{
		Sensor(2);
		memcpy(&Zeile1[4],"Einheit",7);
		if(Aenderung&&NeuerWert)
		{
			if(Einheit[Tmp1]<2)Einheit[Tmp1]++;
			else Einheit[Tmp1]=0;
			Byte1=Einheit[Tmp1];
			if(!Tmp1)EEAdresse=EEEinheitS1;
			else EEAdresse=EEEinheitS2;
			if(Einheit[Tmp]==2)Dezi[Tmp1]=0;
			else Dezi[Tmp1]=2;
		}
		SI(Einheit[Tmp1],4);
	}

	else if(Menue==44||Menue==144)
	{
		Sensor(2);
		memcpy(&Zeile1[4],"Untergrenze",11);
		if(Aenderung&&NeuerWert)
		{
			Byte12=2;
			if(!Umschalt)
			{
				if(UG[Tmp1]<=(Limit-Tempo))UG[Tmp1]+=Tempo;
			}
			else
			{
				if(UG[Tmp1]>=Tempo)UG[Tmp1]-=Tempo;
			}
			Byte1=make8(UG[Tmp1],1);
			Byte2=make8(UG[Tmp1],0);
			if(!Tmp1)EEAdresse=EEUGS1;
			else EEAdresse=EEUGS2;
		}
		Ausgabe(UG[Tmp1],Dezi[Tmp1],3);
	}

	else if(Menue==45||Menue==145)
	{
		Sensor(2);
		memcpy(&Zeile1[4],"Obergrenze",10);
		if(Aenderung&&NeuerWert)
		{
			Byte12=2;
			if(!Umschalt)
			{
				if(OG[Tmp1]<=(Limit-Tempo))OG[Tmp1]+=Tempo;
			}
			else
			{
				if(OG[Tmp1]>=Tempo)OG[Tmp1]-=Tempo;
			}
			Byte1=make8(OG[Tmp1],1);
			Byte2=make8(OG[Tmp1],0);
			if(!Tmp1)EEAdresse=EEOGS1;
			else EEAdresse=EEOGS2;
		}
		Ausgabe(OG[Tmp1],Dezi[Tmp1],3);
	}

	else if(Menue==46||Menue==146)			//MM
	{
		Sensor(2);
		memcpy(&Zeile1[3],"Alarmrichtung",13);
		if(Aenderung&&NeuerWert)
		{
			Byte1=Richtung[Tmp1]=1-Richtung[Tmp1];
			if(!Tmp1)EEAdresse=EERichtung1;
			else EEAdresse=EERichtung2;
		}
		if(!Richtung[Tmp1])memcpy(&Zeile2[4],"steigend",8);
		else memcpy(&Zeile2[4],"fallend",7);
	}

	else if(Menue==47||Menue==147)			//MM
	{
		Sensor(2);
		memcpy(&Zeile1[4],"Auswertung",10);
		if(Aenderung&&NeuerWert)
		{
			Byte1=LinKurv[Tmp1]=1-LinKurv[Tmp1];
			if(!Tmp1)EEAdresse=EELinKurvS1;
			else EEAdresse=EELinKurvS2;
		}
		if(!LinKurv[Tmp1])memcpy(&Zeile2[4],"Linear",6);
		else memcpy(&Zeile2[4],"Kurve",5);
	}

	else if(Menue==211)
	{
		// Kaltstartzeit

		memcpy(&Zeile1[1],"Kaltstartzeit",13);
		if(Aenderung&&NeuerWert)
		{
			if(!Umschalt)
			{
				if(KSZeit<30)KSZeit++;
			}
			else
			{
				if(KSZeit)KSZeit--;
			}
			Byte1=KSZeit;
			EEAdresse=EEKSZeit;
		}
		Ausgabe(KSZeit,0,-2);				// Maximal 30
		memcpy(&Zeile2[4],"Minute",6);
		if(KSZeit>1)Zeile2[10]='n';
	}

	else if(Menue==212)
	{
 		memcpy(Zeile1,"Wartungsinterval",16);
		if(Aenderung&&NeuerWert)
		{
			Byte12=2;
			if(!Umschalt)
			{
				if(Tag<=365-Tempo)Tag+=Tempo;
			}
			else
			{
				if(Tag>=Tempo)Tag-=Tempo;
			}
			JahrWartung=0;
			Byte1=make8(Tag,1);
			Byte2=make8(Tag,0);
			EEAdresse=EETagH;
		}
		memcpy(&Zeile2[1],"in",2);
		Ausgabe(Tag,0,3);
		memcpy(&Zeile2[9],"Tag",3);
		if(Tag!=1)memcpy(&Zeile2[12],"en",2);
	}

	else if(Menue==213)
	{
		if(Aenderung)Passwort = 3;

		p[0]=PW[0];
		p[1]=PW[1];
		p[2]=PW[2];
		p[3]=PW[3];

		PWDisplay();
	}
}