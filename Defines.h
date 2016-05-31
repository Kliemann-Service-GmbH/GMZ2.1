
#fuses XT,WDT256,PUT,NODEBUG,BROWNOUT,NOLVP,NOCPD,BORV27
// für Debug automatisch NOWDT,NOPROTECT,DEBUG
/*
XT       Oszillator 4MHz
WDT      Watchdog aktiv
PUT      Power up timer
NODEBUG  kein Debug-Mode
BROWNOUT Reset bei Unterspannung
NOLVP    kein Low-Voltage-Program
NOCPD    kein code protect eeprom
NOWRT    kein Code write
BORV27   Brownout-Reset bei 2,7V
*/

#use fast_io (A)
#use fast_io (B)
#use fast_io (C)
#use fast_io (D)
#use fast_io (E)

#byte ADCON0  = 0xFC2        // A/D Control
#byte ADCON1  = 0xFC1
#byte ADCON2  = 0xFC0
#byte ADRESH  = 0xFC4
#byte ADRESL  = 0xFC3

#byte PORTA   = 0xF80        // Port A
#byte PORTB   = 0xF81        // Port B
#byte PORTC   = 0xF82        // Port C
#byte PORTD   = 0xF83        // Port D
#byte PORTE   = 0xF84        // Port E

#byte LATA   = 0xF89          // Latch A
#byte LATB   = 0xF8A          // Latch B
#byte LATC   = 0xF8B          // Latch C
#byte LATD   = 0xF8C          // Latch D
#byte LATE   = 0xF8D          // Latch E

#byte TRISA   = 0xF92        // Tristate Port A
#byte TRISB   = 0xF93        // Tristate Port B
#byte TRISC   = 0xF94        // Tristate Port C
#byte TRISD   = 0xF95        // Tristate Port D
#byte TRISE   = 0xF96        // Tristate Port E

#use delay ( clock=4000000)
// Interrupts
#byte INTCON  = 0xFF2        // Interrupt Control
#byte PIR1    = 0xF9E        // Interrupt Reg
#byte PIE1    = 0xF9D        // Interrupt Enable
#byte PIE2    = 0xFA1
#bit  PEIE    = INTCON.6    // Peripheral Interrupt Enable
#bit  GIE     = INTCON.7    // Global Interrupt Enable

// Timer 1
#byte TMR1L   = 0xFCE        // Timer 1 Reg
#byte TMR1H   = 0xFCF
#byte T1CON   = 0xFCD        // Timer 1 Control
#bit  TMR1IF  = PIR1.0      // Interrupt Flag

// Timer 2
#byte TMR2    = 0xFCC        // Timer 2 Reg
#byte T2CON   = 0xFCA        // Timer 2 Control
#byte PR2     = 0xFCB        // Timer 2 Period
#bit  TMR2IE  = PIE1.1      // Timer 2 Interrupt enable
#bit  TMR2IF  = PIR1.1      // Interrupt Flag

// Timer 3
#byte T3CON   = 0xFB1        // Timer 3 Control

#byte CCP1CON = 0xFBD
#byte SSPCON1 = 0xFC6
#byte RCSTA = 0xFAB

//////////////////////////////////////////////////////////////////////////////
// Pinzuweisung
#define TReset          PIN_A2
#define TShift          PIN_A3
#define Wartung         PIN_A4
#define TUpDown         PIN_A5
#define TLeftRight      PIN_E0
#define TEnter          PIN_E1

#define LEDS1A3         PIN_B0
#define LEDS1A2         PIN_B1
#define LEDS1A1         PIN_B2
#define LEDS1SM         PIN_B3
#define LEDS2A3         PIN_B4
#define LEDS2A2         PIN_B5
#define LEDS2A1         PIN_B6
#define LEDS2SM         PIN_B7

#define HUPE            PIN_D0
#define RELAIS1         PIN_D1
#define RELAIS2         PIN_D2
#define RELAIS3         PIN_D3
#define RELAIS4         PIN_D4
#define RELAIS5         PIN_D5

//////////////////////////////////////////////////////////////////////////////
// Berechnungsgrundlagen
#define ADWert20mA      819 // = 4,0 V = 20,0mA  (1024 digit bei 5 Volt )
                            // -> 4 / 5 * 1024 = 819
#define Wert20mA        2000
#define Wert4mA         400 // = 0,8 V = 4,0 mA (3,98 mA)


//////////////////////////////////////////////////////////////////////////////
// Konfigurationsparameter im EEPROM

#define PasswortA       0   // Passwort Ziffer 1                    0
#define PasswortB       1   // Passwort Ziffer 2                    0
#define PasswortC       2   // Passwort Ziffer 3                    0
#define PasswortD       3   // Passwort Ziffer 4                    0
#define EEKSZeit        4   // Kaltstartverzögerung in Minuten      0
#define EETagH          5   // Wartungsinterval Tag HIGHByte    1  365
#define EETagL          6   // Wartungsinterval Tag LOWByte      6D
#define EESensortyp1    7   // Sensor1 Typ              0  CH4
#define EEEinheitS1     8   // Sensor1 Einheit            0  %UEG
#define EEOGS1          9   // Oberer Grenzwert Sensor 1 HIGHByte  3  100,0%
#define EEOGS1A         10  // Oberer Grenzwert Sensor 1 LOWByte  E8
#define EEUGS1          11  // Unterer Grenzwert Sensor 1 HIGHByte  0  0,0%
#define EEUGS1A         12  // Unterer Grenzwert Sensor 1 LOWByte  0
#define EELinKurvS1     13  // Auswertung Linear oder Kurve Sensor1  0  Linear
#define EEFGS1          14  // Freigabe Sensor 1          1
#define EES1WP1KonH     15  // Wertepaar 1 Konzentration HIGHByte  0  0
#define EES1WP1KonL     16  // Wertepaar 1 Konzentration LOWByte  0
#define EES1WP2KonH     17  // Wertepaar 2 Konzentration HIGHByte  0  1,0%  Email v. 6.4.11
#define EES1WP2KonL     18  // Wertepaar 2 Konzentration LOWByte  32
#define EES1WP3KonH     19  // Wertepaar 3 Konzentration HIGHByte  0  10,0%
#define EES1WP3KonL     20  // Wertepaar 3 Konzentration LOWByte  64
#define EES1WP4KonH     21  // Wertepaar 4 Konzentration HIGHByte  0  15,0%
#define EES1WP4KonL     22  // Wertepaar 4 Konzentration LOWByte  96
#define EES1WP5KonH     23  // Wertepaar 5 Konzentration HIGHByte  0  20,0%
#define EES1WP5KonL     24  // Wertepaar 5 Konzentration LOWByte  C8
#define EES1WP6KonH     25  // Wertepaar 6 Konzentration HIGHByte  0  25,0%
#define EES1WP6KonL     26  // Wertepaar 6 Konzentration LOWByte  FA
#define EES1WP7KonH     27  // Wertepaar 7 Konzentration HIGHByte  1  30,0%
#define EES1WP7KonL     28  // Wertepaar 7 Konzentration LOWByte  2C
#define EES1WP8KonH     29  // Wertepaar 8 Konzentration HIGHByte  1  35,0%
#define EES1WP8KonL     30  // Wertepaar 8 Konzentration LOWByte  5E
#define EES1WP9KonH     31  // Wertepaar 9 Konzentration HIGHByte  1  40,0%
#define EES1WP9KonL     32  // Wertepaar 9 Konzentration LOWByte  90
#define EES1WP10KonH    33  // Wertepaar 10 Konzentration HIGHByte  3  100,0%
#define EES1WP10KonL    34  // Wertepaar 10 Konzentration LOWByte  E8
#define EES1WP1mAH      35  // Wertepaar 1 Strom HIGHByte      1  4,00mA
#define EES1WP1mAL      36  // Wertepaar 1 Strom LOWByte      90
#define EES1WP2mAH      37  // Wertepaar 2 Strom HIGHByte      2  5,00mA   Email v. 6.4.11
#define EES1WP2mAL      38  // Wertepaar 2 Strom LOWByte      8D
#define EES1WP3mAH      39  // Wertepaar 3 Strom HIGHByte      3  9,06mA
#define EES1WP3mAL      40  // Wertepaar 3 Strom LOWByte      8A
#define EES1WP4mAH      41  // Wertepaar 4 Strom HIGHByte      3  10,11mA
#define EES1WP4mAL      42  // Wertepaar 4 Strom LOWByte      F3
#define EES1WP5mAH      43  // Wertepaar 5 Strom HIGHByte      4  11,16mA
#define EES1WP5mAL      44  // Wertepaar 5 Strom LOWByte      5C
#define EES1WP6mAH      45  // Wertepaar 6 Strom HIGHByte      4  12,00mA
#define EES1WP6mAL      46  // Wertepaar 6 Strom LOWByte      B0
#define EES1WP7mAH      47  // Wertepaar 7 Strom HIGHByte      4  12,76mA
#define EES1WP7mAL      48  // Wertepaar 7 Strom LOWByte      FC
#define EES1WP8mAH      49  // Wertepaar 8 Strom HIGHByte      5  13,44mA
#define EES1WP8mAL      50  // Wertepaar 8 Strom LOWByte      40
#define EES1WP9mAH      51  // Wertepaar 9 Strom HIGHByte      5  14,12mA
#define EES1WP9mAL      52  // Wertepaar 9 Strom LOWByte      84
#define EES1WP10mAH     53  // Wertepaar 10 Strom HIGHByte      7  20,00mA
#define EES1WP10mAL     54  // Wertepaar 10 Strom LOWByte      D0
#define EESensortyp2    55  // Sensor2 Typ              wie bei Sensor1
#define EEEinheitS2     56  // Sensor2 Einheit
#define EEOGS2          57  // Oberer Grenzwert Sensor 2 HIGHByte
#define EEOGS2A         58  // Oberer Grenzwert Sensor 2 LOWByte
#define EEUGS2          59  // Unterer Grenzwert Sensor 2 HIGHByte
#define EEUGS2A         60  // Unterer Grenzwert Sensor 2 LOWByte
#define EELinKurvS2     61  // Auswertung Linear oder Kurve Sensor 2
#define EEFGS2          62  // Freigabe Sensor 2
#define EES2WP1KonH     63  // Wertepaar 1 Konzentration HIGHByte
#define EES2WP1KonL     64  // Wertepaar 1 Konzentration LOWByte
#define EES2WP2KonH     65  // Wertepaar 2 Konzentration HIGHByte
#define EES2WP2KonL     66  // Wertepaar 2 Konzentration LOWByte
#define EES2WP3KonH     67  // Wertepaar 3 Konzentration HIGHByte
#define EES2WP3KonL     68  // Wertepaar 3 Konzentration LOWByte
#define EES2WP4KonH     69  // Wertepaar 4 Konzentration HIGHByte
#define EES2WP4KonL     70  // Wertepaar 4 Konzentration LOWByte
#define EES2WP5KonH     71  // Wertepaar 5 Konzentration HIGHByte
#define EES2WP5KonL     72  // Wertepaar 5 Konzentration LOWByte
#define EES2WP6KonH     73  // Wertepaar 6 Konzentration HIGHByte
#define EES2WP6KonL     74  // Wertepaar 6 Konzentration LOWByte
#define EES2WP7KonH     75  // Wertepaar 7 Konzentration HIGHByte
#define EES2WP7KonL     76  // Wertepaar 7 Konzentration LOWByte
#define EES2WP8KonH     77  // Wertepaar 8 Konzentration HIGHByte
#define EES2WP8KonL     78  // Wertepaar 8 Konzentration LOWByte
#define EES2WP9KonH     79  // Wertepaar 9 Konzentration HIGHByte
#define EES2WP9KonL     80  // Wertepaar 9 Konzentration LOWByte
#define EES2WP10KonH    81  // Wertepaar 10 Konzentration HIGHByte
#define EES2WP10KonL    82  // Wertepaar 10 Konzentration LOWByte
#define EES2WP1mAH      83  // Wertepaar 1 Strom HIGHByte
#define EES2WP1mAL      84  // Wertepaar 1 Strom LOWByte
#define EES2WP2mAH      85  // Wertepaar 2 Strom HIGHByte
#define EES2WP2mAL      86  // Wertepaar 2 Strom LOWByte
#define EES2WP3mAH      87  // Wertepaar 3 Strom HIGHByte
#define EES2WP3mAL      88  // Wertepaar 3 Strom LOWByte
#define EES2WP4mAH      89  // Wertepaar 4 Strom HIGHByte
#define EES2WP4mAL      90  // Wertepaar 4 Strom LOWByte
#define EES2WP5mAH      91  // Wertepaar 5 Strom HIGHByte
#define EES2WP5mAL      92  // Wertepaar 5 Strom LOWByte
#define EES2WP6mAH      93  // Wertepaar 6 Strom HIGHByte
#define EES2WP6mAL      94  // Wertepaar 6 Strom LOWByte
#define EES2WP7mAH      95  // Wertepaar 7 Strom HIGHByte
#define EES2WP7mAL      96  // Wertepaar 7 Strom LOWByte
#define EES2WP8mAH      97  // Wertepaar 8 Strom HIGHByte
#define EES2WP8mAL      98  // Wertepaar 8 Strom LOWByte
#define EES2WP9mAH      99  // Wertepaar 9 Strom HIGHByte
#define EES2WP9mAL      100  // Wertepaar 9 Strom LOWByte
#define EES2WP10mAH     101  // Wertepaar 10 Strom HIGHByte
#define EES2WP10mAL     102  // Wertepaar 10 Strom LOWByte
#define EES1AP1SWH      103  // S1 Schwelle Alarmpunkt 1 HIGHByte  1  40,0%
#define EES1AP1SWL      104  // S1 Schwelle Alarmpunkt 1 LOWByte    90
#define EES1AP1HYH      105  // S1 Hysterese Alarmpunkt 1 HIGHByte  00
#define EES1AP1HYL      106  // S1 Hysterese Alarmpunkt 1 LOWByte  0A  1,0%
#define EES1AP1HO       107  // S1 Halten Alarmpunkt 1        0  Nein
#define EES1AP1RE       108  // S1 Anforderung Relais Alarmpunkt 1  1
#define EES1AP2SWH      109  // S1 Schwelle Alarmpunkt 2 HIGHByte  2  60,0%
#define EES1AP2SWL      110  // S1 Schwelle Alarmpunkt 2 LOWByte    58
#define EES1AP2HYH      111  // S1 Hysterese Alarmpunkt 2 HIGHByte  00
#define EES1AP2HYL      112  // S1 Hysterese Alarmpunkt 2 LOWByte  0A  1,0%
#define EES1AP2HO       113  // S1 Halten Alarmpunkt 2        0  Nein
#define EES1AP2RE       114  // S1 Anforderung Relais Alarmpunkt 2  2
#define EES1AP3SWH      115  // S1 Schwelle Alarmpunkt 3 HIGHByte  3  80,0%
#define EES1AP3SWL      116  // S1 Schwelle Alarmpunkt 3 LOWByte    20
#define EES1AP3HYH      117  // S1 Hysterese Alarmpunkt 3 HIGHByte  00
#define EES1AP3HYL      118  // S1 Hysterese Alarmpunkt 3 LOWByte  0A  1,0%
#define EES1AP3HO       119  // S1 Halten Alarmpunkt 3        0  Nein
#define EES1AP3RE       120  // S1 Anforderung Relais Alarmpunkt 3  7
#define EES2AP1SWH      121  // S2 Schwelle Alarmpunkt 1 HIGHByte  wie bei Sensor1
#define EES2AP1SWL      122  // S2 Schwelle Alarmpunkt 1 LOWByte
#define EES2AP1HYH      123  // S2 Hysterese Alarmpunkt 1 HIGHByte
#define EES2AP1HYL      124  // S2 Hysterese Alarmpunkt 1 LOWByte
#define EES2AP1HO       125  // S2 Halten Alarmpunkt 1
#define EES2AP1RE       126  // S2 Anforderung Relais Alarmpunkt 1
#define EES2AP2SWH      127  // S2 Schwelle Alarmpunkt 2 HIGHByte
#define EES2AP2SWL      128  // S2 Schwelle Alarmpunkt 2 LOWByte
#define EES2AP2HYH      129  // S2 Hysterese Alarmpunkt 2 HIGHByte
#define EES2AP2HYL      130  // S2 Hysterese Alarmpunkt 2 LOWByte
#define EES2AP2HO       131  // S2 Halten Alarmpunkt 2
#define EES2AP2RE       132  // S2 Anforderung Relais Alarmpunkt 2
#define EES2AP3SWH      133  // S2 Schwelle Alarmpunkt 3 HIGHByte
#define EES2AP3SWL      134  // S2 Schwelle Alarmpunkt 3 LOWByte
#define EES2AP3HYH      135  // S2 Hysterese Alarmpunkt 3 HIGHByte
#define EES2AP3HYL      136  // S2 Hysterese Alarmpunkt 3 LOWByte
#define EES2AP3HO       137  // S2 Halten Alarmpunkt 3
#define EES2AP3RE       138  // S2 Anforderung Relais Alarmpunkt 3
#define EERichtung1     139  // S1 Alarmrichtung  //MM
#define EERichtung2     140  // S2 Alarmrichtung  //MM

// Dieses Macro gibt das High und Low Bit von x zurück
#define hilo(x)  make8(x,1),make8(x,0)

// Initial EEPROM Daten für das Hexfile
#rom int8 0xF00000={
    49,         // Passwort Ziffer 1
    50,         // Passwort Ziffer 2
    51,         // Passwort Ziffer 3
    52,         // Passwort Ziffer 4
    2,          // Kaltstartverzögerung in Minuten
    hilo(364),  // Wartungsinterval Tag LOWByte

   // Sensor1
    0,          // Typ Sensor1 (siehe Datei LCD.h)
    0,          // Einheit Sensor1 (0=UEG, 1=VOL, 2=ppm)
    hilo(1000), // Oberer Grenzwert (UEG x10)
    hilo(0),    // Unterer Grenzwert
    1,          // Auswertung Linear oder Kurve Sensor1 (0=linear; 1=Kurve)
    1,          // Freigabe Sensor1
    hilo(0),    // Wertepaar 1 Konzentration LOWByte Sensor1
    hilo(1),    // Wertepaar 2 Konzentration LOWByte Sensor1
    hilo(100),  // Wertepaar 3 Konzentration LOWByte Sensor1
    hilo(150),  // Wertepaar 4 Konzentration LOWByte Sensor1
    hilo(200),  // Wertepaar 5 Konzentration LOWByte Sensor1
    hilo(250),  // Wertepaar 6 Konzentration LOWByte Sensor1
    hilo(300),  // Wertepaar 7 Konzentration LOWByte Sensor1
    hilo(350),  // Wertepaar 8 Konzentration LOWByte Sensor1
    hilo(400),  // Wertepaar 9 Konzentration LOWByte Sensor1
    hilo(1000), // Wertepaar 10 Konzentration LOWByte Sensor1
    hilo(400),  // Wertepaar 1 Strom LOWByte Sensor1
    hilo(500),  // Wertepaar 2 Strom LOWByte Sensor1
    hilo(906),  // Wertepaar 3 Strom LOWByte Sensor1
    hilo(1011), // Wertepaar 4 Strom LOWByte Sensor1
    hilo(1116), // Wertepaar 5 Strom LOWByte Sensor1
    hilo(1200), // Wertepaar 6 Strom LOWByte Sensor1
    hilo(1276), // Wertepaar 7 Strom LOWByte Sensor1
    hilo(1344), // Wertepaar 8 Strom LOWByte Sensor1
    hilo(1412), // Wertepaar 9 Strom LOWByte Sensor1
    hilo(2000), // Wertepaar 10 Strom LOWByte Sensor1
            
    // Sensor2
    0,          // Typ Sensor2 (siehe Datei LCD.h)
    0,          // Einheit Sensor2 (0=UEG, 1=VOL, 2=ppm)
    hilo(1000), // Oberer Grenzwert (UEG x10)
    hilo(0),    // Unterer Grenzwert
    1,          // Auswertung Linear oder Kurve Sensor2 (0=linear; 1=Kurve)
    1,          // Freigabe Sensor2
    hilo(0),    // Wertepaar 1 Konzentration LOWByte Sensor2
    hilo(1),    // Wertepaar 2 Konzentration LOWByte Sensor2
    hilo(100),  // Wertepaar 3 Konzentration LOWByte Sensor2
    hilo(150),  // Wertepaar 4 Konzentration LOWByte Sensor2
    hilo(200),  // Wertepaar 5 Konzentration LOWByte Sensor2
    hilo(250),  // Wertepaar 6 Konzentration LOWByte Sensor2
    hilo(300),  // Wertepaar 7 Konzentration LOWByte Sensor2
    hilo(350),  // Wertepaar 8 Konzentration LOWByte Sensor2
    hilo(400),  // Wertepaar 9 Konzentration LOWByte Sensor2
    hilo(1000), // Wertepaar 10 Konzentration LOWByte Sensor2
    hilo(400),  // Wertepaar 1 Strom LOWByte Sensor2
    hilo(500),  // Wertepaar 2 Strom LOWByte Sensor2
    hilo(906),  // Wertepaar 3 Strom LOWByte Sensor2
    hilo(1011), // Wertepaar 4 Strom LOWByte Sensor2
    hilo(1116), // Wertepaar 5 Strom LOWByte Sensor2
    hilo(1200), // Wertepaar 6 Strom LOWByte Sensor2
    hilo(1276), // Wertepaar 7 Strom LOWByte Sensor2
    hilo(1344), // Wertepaar 8 Strom LOWByte Sensor2
    hilo(1412), // Wertepaar 9 Strom LOWByte Sensor2
    hilo(2000), // Wertepaar 10 Strom LOWByte Sensor2

    // Alarm Einstellungen
    // Sensor1
    hilo(200),  // Sensor1 Schwelle Alarmpunkt 1 LOWByte
    hilo(50),   // Sensor1 Hysterese Alarmpunkt 1 LOWByte
    0,          // Sensor1 Halten Alarmpunkt 1
    1,          // Sensor1 Anforderung Relais Alarmpunkt 1 (1=K01; 2=K02; 3=K03; 4=K04; 5=K01K02; 6=K02K03; 7=K03K04; 8=K01K03; 9=K02K04; 10=K01K04; 11=K01K02K03; 12=K01K02K04; 13=K01K03K04; 14=K02K03K04; 15=K01K02K03K04;)
    hilo(300),  // Sensor1 Schwelle Alarmpunkt 2 LOWByte
    hilo(50),   // Sensor1 Hysterese Alarmpunkt 2 LOWByte
    0,          // Sensor1 Halten Alarmpunkt 2
    5,          // Sensor1 Anforderung Relais Alarmpunkt 2 (1=K01; 2=K02; 3=K03; 4=K04; 5=K01K02; 6=K02K03; 7=K03K04; 8=K01K03; 9=K02K04; 10=K01K04; 11=K01K02K03; 12=K01K02K04; 13=K01K03K04; 14=K02K03K04; 15=K01K02K03K04;)
    hilo(400),  // Sensor1 Schwelle Alarmpunkt 3 LOWByte
    hilo(50),   // Sensor1 Hysterese Alarmpunkt 3 LOWByte
    0,          // Sensor1 Halten Alarmpunkt 3
    11,         // Sensor1 Anforderung Relais Alarmpunkt 3 (1=K01; 2=K02; 3=K03; 4=K04; 5=K01K02; 6=K02K03; 7=K03K04; 8=K01K03; 9=K02K04; 10=K01K04; 11=K01K02K03; 12=K01K02K04; 13=K01K03K04; 14=K02K03K04; 15=K01K02K03K04;)

    // Alarm Einstellungen
    // Sensor2
    hilo(200),  // Sensor2 Schwelle Alarmpunkt 1 LOWByte
    hilo(50),   // Sensor2 Hysterese Alarmpunkt 1 LOWByte
    0,          // Sensor2 Halten Alarmpunkt 1
    1,          // Sensor2 Anforderung Relais Alarmpunkt 1 (1=K01; 2=K02; 3=K03; 4=K04; 5=K01K02; 6=K02K03; 7=K03K04; 8=K01K03; 9=K02K04; 10=K01K04; 11=K01K02K03; 12=K01K02K04; 13=K01K03K04; 14=K02K03K04; 15=K01K02K03K04;)
    hilo(300),  // Sensor2 Schwelle Alarmpunkt 2 LOWByte
    hilo(50),   // Sensor2 Hysterese Alarmpunkt 2 LOWByte
    0,          // Sensor2 Halten Alarmpunkt 2
    5,          // Sensor2 Anforderung Relais Alarmpunkt 2 (1=K01; 2=K02; 3=K03; 4=K04; 5=K01K02; 6=K02K03; 7=K03K04; 8=K01K03; 9=K02K04; 10=K01K04; 11=K01K02K03; 12=K01K02K04; 13=K01K03K04; 14=K02K03K04; 15=K01K02K03K04;)
    hilo(400),  // Sensor2 Schwelle Alarmpunkt 3 LOWByte
    hilo(50),   // Sensor2 Hysterese Alarmpunkt 3 LOWByte
    0,          // Sensor2 Halten Alarmpunkt 3
    11,         // Sensor2 Anforderung Relais Alarmpunkt 3 (1=K01; 2=K02; 3=K03; 4=K04; 5=K01K02; 6=K02K03; 7=K03K04; 8=K01K03; 9=K02K04; 10=K01K04; 11=K01K02K03; 12=K01K02K04; 13=K01K03K04; 14=K02K03K04; 15=K01K02K03K04;)

    // Alarmrichtung
    0,          // Sensor1 Alarmrichtung (0 steigend; 1 fallend)    //MM
    0           // Sensor2 Alarmrichtung (0 steigend; 1 fallend)    //MM
}


// Globale Variablen
static long ADC_ip[2], MW8S1[8], MW8S2[8]; // 10 - bit Messwerte und Mittelwert
int MW8;
int32 mAWert[2];
static int GutMeldung, Stoerung[2], SMLEDTmp[2], FuenfSekWarten, DreiSekWarten;
static int Menue, Pfad;
static int TShiftgedr, TResetgedr, TUpDowngedr, TLeftRightgedr, TEntergedr;
int TEntp;
long Tempo;
int Passwort=1,P[4],Pos,PW[4];
static int Umschalt, Eingabemoeglich, Eingabe, Aenderung, NeuMenue=1;
static char Zeile1[16], Zeile2[16];
static int MS, Sek, Min, Std, KSZeit, KSZ, JahrWartung;
static long Tag;
static long APSW[2][3],APHY[2][3],UG[2],UGmA[2],OG[2];
static long WPKon[2][10],WPmA1[10],WPmA2[10];
static int APHO[2][3],APRE[2][3],Halten;
static int Typ[2][5],TT[2],Einheit[2],Dezi[2],LinKurv[2];
static int EEAdresse,Byte1, Byte2, Byte3,Byte4, Byte12=1;
static int NeuerWert;
static unsigned long Limit;
static unsigned long Messwert[2],BereichKon[2],BereichmA[2],Freigabe[2],Richtung[2]; //MM
int AnfRelais[16]={0,2,4,8,16,6,12,24,10,20,18,14,22,26,28,30};
static int Alarm, Alarmalt, Out[2],Hupen;
static int WWert;
