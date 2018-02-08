#include "gps.h"
#include "Adafruit_GPS.h"

Adafruit_GPS GPS;

void gps_init(void){
	GPS.begin(9600);

	//Set output to be minimum required
	GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);

	//Set GPS printout update rate to 5HZ
	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);

	//Set GPS fix rate to 5HZ
	GPS.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);
}

void gps_setzero(void);

char gps_read(void) {
	return GPS.read();
}
