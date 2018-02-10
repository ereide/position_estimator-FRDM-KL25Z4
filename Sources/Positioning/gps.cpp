#include "gps.h"
#include "Adafruit_GPS.h"
#include "gps_serial.h"

Adafruit_GPS GPS;

void init_gps(void){
	GPS.begin(9600);

	//Set output to be minimum required
	GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
	serial_delay(10);


	//Set GPS printout update rate to 5HZ
	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);
	serial_delay(10);


	//Set GPS fix rate to 5HZ
	GPS.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);
	serial_delay(10);
}

void gps_setzero(void){
	return;
}


bool gps_read(position_t* pos){
	if (GPS.newNMEAreceived()) {
		GPS.parse(GPS.lastNMEA());
		pos->x = 0;
		pos->y = 0;
		pos->z = GPS.altitude;

		print_last_NMEA();

		return GPS.fix;

	}

	else {
		return false;
	}



}

