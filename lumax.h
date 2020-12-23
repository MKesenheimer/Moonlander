#pragma once

// Punktinformationen
// Es handelt sich um vorzeichenlose 16-Bit-Werte
// Ch1=X, Ch2=Y:                    0=links/unten, 32768=mitte, 65535=rechts/oben
// Ch3=Red, Ch4=Green, Ch5=Blue:    0=aus, 65535=volle Helligkeit
// Ch6=Deep Blue, Ch7=Yellow:       0=aus, 65535=volle Helligkeit: 
//                                  wird nur von manchen Varianten der Minilumax Ausgabekarte unterstützt, 
//                                  sollte aber dennoch grundsätzlich von der Lasershowsoftware beschrieben werden 
//                                  (sofern diese mehr als 3 Farbkanäle unterstützt).
// Ch8=Cyan oder Intensity:         0=aus, 65535=volle Helligkeit: 
//                                  wird nur von manchen Varianten der Minilumax Ausgabekarte unterstützt, 
//                                  sollte aber dennoch grundsätzlich von der Lasershowsoftware beschrieben 
//                                  werden (sofern diese mehr als 3 Farbkanäle unterstützt). Kanal 8 kann je 
//                                  nach Variante entweder als Cyan oder Intensity Ausgang genutzt werden.
// TTL:                             manche Betriebsmodi der Minilumax-Karte erlauben ein punktsynchrones 
//                                  schalten der TTL-Ausgänge. In diesen Fällen stehen im Low-Byte die 8 TTL-Ausgänge.
//                                  Üblicherweise werden die TTL-Ausgänge aber mit Lumax_SetTTL (siehe unten) angesteuert; 
//                                  der Wert von dem hier diskutierten TTL wird dann ignoriert.
typedef struct
{
    uint16_t Ch1, Ch2, Ch3, Ch4, Ch5, Ch6, Ch7, Ch8, TTL;
} TLumax_Point;

// Liefert die Versionsnummer der DLL zurück.
int Lumax_GetApiVersion();

// Liefert Informationen zum angeschlossenen Gerät.
// Für die Abfrage der Information muss die Karte nicht unbedingt zuvor mit Lumax_OpenDevice
// geöffnet worden sein, es schadet aber auch nicht, sollte sie bereits geöffnet sein.
int Lumax_GetDeviceInfo(int physicalDevice, int infoID, uint8_t *inBuffer, uint16_t inLength, uint8_t *outBuffer, uint16_t outLength);

// Liefert als Ergebnis die Anzahl aller angeschlossenen 
// Minilumax-Ausgabekarten zurück. Sofern keine Karte 
// angeschlossen ist, wird 0 zurückgeliefert.
int Lumax_GetPhysicalDevices();

// Setzt die TTL-Ausgänge der Minilumax-Karte.
// Handle:  Handle zur eindeutigen Identifizierung des Geräts
// TTL:     Ein 32-Bit-Wert, dessen unterste 8 Bit auf den TTL-Ausgangspins ausgegeben werden.
// Hinweis: Bei älteren Firmware-Varianten der Ausgabekarten wird der Zustand der TTL-Ausgänge 
// erst dann aktualisiert, wenn ein neuer Laserframe per Lumax_SendFrame ausgegeben wird. 
// Es wird daher empfohlen, den Aufruf von Lumax_SetTTL immer unmittelbar vor den Aufruf
// von Lumax_SendFrame zu platzieren.
int Lumax_SetTTL(void *handle, uint8_t TTL);

// Wartet auf das Freiwerden des Empfangspuffers bzw. liefert dessen Status zurück.
// Handle:          Handle zur eindeutigen Identifizierung des Geräts
// Timeout:         Wenn als Timeout 0 angegeben wird, so wird lediglich der aktuelle
//                  Status abgefragt; die Funktion kehrt sofort zurück. Die Zeit bis zum 
//                  Ende des Frames wird im Parameter TimeToWait zurückgeliefert, in BufferChanged
//                  steht, ob auf den zuletzt gesendeten Puffer bereits umgeschaltet wurde.
//                  Wenn bereits umgeschaltet wurde, also die Laserausgabe dieses Frames begonnen hat, 
//                  dann ist dieser Wert logisch 1 und es kann mit der Übertragung des nächsten 
//                  Frames begonnen werden.
// TimeToWait:      Zeiger auf eine int-Variable, in der die Zeit (Millisekunden) bis zum Frameende 
//                  zurückgeliefert wird (siehe Timeout).
// BufferChanged:   Zeiger auf eine int-Variable, in der zurückgeliefert wird, ob bereits auf den
//                  nächsten Frame umgeschaltet wurde (=1) oder nicht (=0) (siehe Timeout).
int Lumax_WaitForBuffer(void* handle, int timeOut, int *timeToWait, int *bufferChanged);

// Schließt das Gerät, so dass andere Programme darauf zugreifen können.
int Lumax_CloseDevice(void* handle);

// Handle:      Handle zur eindeutigen Identifizierung des Geräts
// Points:      Zeiger auf einen Speicherbereich, in dem nacheinander 
//              alle Punkte des Frames hinterlegt sind. Die einzelnen Punkte 
//              haben den Datentyp TLumax_Point (siehe unten).
// NumOfPoints: Gibt die Anzahl der Punkte des Frames an.
// ScanSpeed:   Gibt die Ausgabegeschwindigkeit (PPS, points per second) an. 
//              Gültig sind Werte zwischen 250 und 70000 PPS. Diese Ausgabegeschwindigkeit 
//              kommt erst zur Anwendung, wenn die Ausgabe dieses Frames startet. 
//              Die Geschwindigkeit eines möglicherweise aktuell noch laufenden Frames 
//              wird nicht geändert.
// UpdateMode:  Immer auf 0 setzen.
// TimeToWait:  Immer auf 0 setzen.
int Lumax_SendFrame(void *handle, TLumax_Point *points, int numOfPoints, int scanSpeed, int updateMode, int *timeToWait);

// Hält die Laserausgabe an. Alle Farbkanäle werden auf den Wert 0 gesetzt. X- und Y-Achse werden auf die Bildmitte positioniert. Alle TTL-Ausgänge werden abgeschaltet.
// Handle: Handle zur eindeutigen Identifizierung des Geräts
int Lumax_StopFrame(void* handle);

// Notiz: Wird nur für die "Bare"-Lumax aufgerufen
// Und auch dann hat der Aufruf keine Auswirkung auf den weiteren 
// Programmablauf
int Lumax_DongleCom();

// Aktiviert bzw. deaktiviert den DMX512 Sender bzw. Empfänger auf der Minilumax-Karte. 
// Es ist zu beachten, dass ein aktiver Sender/Empfänger Rechenleistung auf der Ausgabekarte 
// benötigt, und daher nicht mehr die volle Ausgabegeschwindigkeit gewährleistet werden 
// kann. Wenn DMX512 nicht benötigt wird, sollte es daher auch nicht aktiviert werden.
// Handle:          Handle zur eindeutigen Identifizierung des Geräts
// NumOfTxChannels: Anzahl der zu übertragenden DMX512-Nutzkanäle. Für einen kompletten 
//                  DMX-Frame mit 1 Startbyte und 512 Nutzbytes muss der Wert also 512 
//                  sein. Der Wert 0 schaltet den DMX-Sender aus.
// NumOfRxChannels: Anzahl der maximal zu empfangenen DMX512-Nutzkanäle. Da laut 
//                  DMX512-Spezifikation eine Länge von 512 Byte plus 1 Startbyte erlaubt 
//                  ist, sollte der Wert auf 512 gesetzt werden, um den Empfänger zu aktivieren.
//                  Der Wert 0 schaltet den DMX-Empfänger aus.
int Lumax_SetDmxMode(void *handle, uint8_t a2, uint8_t a3);

// Öffnet das angegebene Gerät für die weitere Nutzung. Eine möglicherweise 
// von vorheriger Nutzung noch laufende Laserausgabe wird angehalten, 
// DMX512-Sender und -Empfänger werden abgeschaltet.
// numDev:  Nummer der zu öffnenden Ausgabekarte. 
//          Wenn die Funktion Lumax_GetPhysicalDevices als Ergebnis z.B. 3 
//          zurückgeliefert hat, so können als mögliche Parameter für PhysicalDevice 
//          die Werte 1, 2 oder 3 übergeben werden. Alle angeschlossenen Karten 
//          werden anhand ihrer alphanumerischen Seriennummer aufsteigend sortiert. 
//          Karte Nummer 1 ist also immer die alphabetisch erste Karte.
// channel: Immer auf 0 setzen.
// return:  Der Rückgabewert der Funktion ist ein Handle auf das Gerät, das beim Zugriff
//          auf das Gerät mit Hilfe der folgenden Routinen angegeben werden muss. Wenn 
//          das Gerät nicht geöffnet werden konnte, so wird 0 zurückgegeben.
void* Lumax_OpenDevice(int numDev, int channel);

// DEBUG Flags
#define DEBUG_POSSIBLE
const uint32_t DBG_INFO = 1;
const uint32_t DBG_GENERAL = 2;
const uint32_t DBG_WRITETODEV = 4;
const uint32_t DBG_READFROMDEV = 8;
const uint32_t DBG_WRITEFRAMEBUFFER = 16;
const uint32_t DBG_READPASSWORD = 32;
const uint32_t DBG_READMEMORY = 64;
const uint32_t DBG_WAITFORBUFFER = 128;
const uint32_t DBG_SENDFRAME = 256;
const uint32_t DBG_SETDMXMODE = 512;
const uint32_t DBG_OPENDEVICE = 1024;
const uint32_t DBG_CHECKIFBUSY = 2048;
const uint32_t DBG_ISOPEN = 4096;
extern uint32_t lumax_verbosity;