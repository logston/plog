/*
 Ping Logger
 by Paul Logston
 2014-11-08
 
 This example periodically sends an ICMP ping and logs 
 the round trip time for the packet to an SD card.
 
 Each Row of data is approximately 13 bytes.
 
 Built with code by:
 - Blake Foster (Ping)
 - Francesco Potortì (NTPclient)

 */

#include <SPI.h>         
#include <Ethernet.h>
#include <ICMPPing.h>
#include <SD.h>

/*
 *  CONSTANTS
 */
 
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; // max address for ethernet shield
IPAddress pingAddr(8, 8, 8, 8); // ip address to ping. Google's DNS server.

SOCKET pingSocket = 0;

char buffer[256];
ICMPPing ping(pingSocket, (uint16_t)random(0, 255));
EthernetUDP udp; // used for time sync
unsigned long reference_ts;
unsigned long startup_offset;
unsigned long ping_ts;

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 4;

/* 
 *   HELPER FUNCTIONS
 */

/*
 * © Francesco Potortì 2013 - GPLv3 - Revision: 1.13
 *
 * Send an NTP packet and wait for the response, return the Unix time
 *
 * To lower the memory footprint, no buffers are allocated for sending
 * and receiving the NTP packets.  Four bytes of memory are allocated
 * for transmision, the rest is random garbage collected from the data
 * memory segment, and the received packet is read one byte at a time.
 * The Unix time is returned, that is, seconds from 1970-01-01T00:00.
 */
unsigned long inline ntpUnixTime (UDP &udp)
{
  static int udpInited = udp.begin(123); // open socket on arbitrary port

  const char timeServer[] = "pool.ntp.org";  // NTP server

  // Only the first four bytes of an outgoing NTP packet need to be set
  // appropriately, the rest can be whatever.
  const long ntpFirstFourBytes = 0xEC0600E3; // NTP request header

  // Fail if WiFiUdp.begin() could not init a socket
  if (! udpInited)
    return 0;

  // Clear received data from possible stray received packets
  udp.flush();

  // Send an NTP request
  if (! (udp.beginPacket(timeServer, 123) // 123 is the NTP port
	 && udp.write((byte *)&ntpFirstFourBytes, 48) == 48
	 && udp.endPacket()))
    return 0;				// sending request failed

  // Wait for response; check every pollIntv ms up to maxPoll times
  const int pollIntv = 150;		// poll every this many ms
  const byte maxPoll = 15;		// poll up to this many times
  int pktLen;				// received packet length
  for (byte i=0; i<maxPoll; i++) {
    if ((pktLen = udp.parsePacket()) == 48)
      break;
    delay(pollIntv);
  }
  if (pktLen != 48)
    return 0;				// no correct packet received

  // Read and discard the first useless bytes
  // Set useless to 32 for speed; set to 40 for accuracy.
  const byte useless = 40;
  for (byte i = 0; i < useless; ++i)
    udp.read();

  // Read the integer part of sending time
  unsigned long time = udp.read();	// NTP time
  for (byte i = 1; i < 4; i++)
    time = time << 8 | udp.read();

  // Round to the nearest second if we want accuracy
  // The fractionary part is the next byte divided by 256: if it is
  // greater than 500ms we round to the next second; we also account
  // for an assumed network delay of 50ms, and (0.5-0.05)*256=115;
  // additionally, we account for how much we delayed reading the packet
  // since its arrival, which we assume on average to be pollIntv/2.
  time += (udp.read() > 115 - pollIntv/8);

  // Discard the rest of the packet
  udp.flush();

  return time - 2208988800ul;		// convert NTP time to Unix time
}

void send_ping() {
  // We can not condense the next two lines. 
  // For some reason, reference_ts + (millis() / 1000.0) is not equivalent 
  // to the following two lines.
  startup_offset = millis() / 1000.0;
  ping_ts = reference_ts + startup_offset;
  ICMPEchoReply echoReply = ping(pingAddr, 4);
  
  if (echoReply.status == SUCCESS) {
    sprintf(buffer, "%lu,%ld", ping_ts, millis() - echoReply.data.time);
  } else {
    sprintf(buffer, "%lu,Echo request failed; %d", ping_ts, echoReply.status);
  }
}


/*
 *   MAIN LOGIC
 */

void setup() {
  Serial.begin(9600);

  // Start Ethernet
  Serial.println("Starting Ethernet");
  Ethernet.begin(mac);
  Serial.println("Ethernet Started.");
  
  reference_ts = ntpUnixTime(udp);
  // calibrate reference_ts for how long arduino has been running 
  startup_offset = millis() / 1000.0;
  reference_ts = reference_ts - startup_offset;
  sprintf(buffer, "# Start time: %lu", reference_ts);
  Serial.println(buffer);
  
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}

void loop() {
  send_ping();

  File dataFile = SD.open("datalog.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(buffer);
    dataFile.close();
    // print to the Serial Monitor too:
    Serial.println(buffer);
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.csv");
  } 
  
  delay(2000);
}

