/*
 * gps.cpp
 *
 *  Created on: Dec 16, 2017
 *      Author: Eivind Roson Eide (c)
 */

#include "gps.h"
#include "Adafruit_GPS.h"
#include "gps_serial.h"

#include <cmath>
#include <cstdint>

#define LAT_FIXED_PRECISSION 100000

static Adafruit_GPS GPS;

//Define offset
static bool has_zero;
static int32_t zero_latitude_fixed, zero_longitude_fixed;
static float zero_altitude;

static float m_per_deg_lat;
static float m_per_deg_lon;

static float deg_to_rad(float deg){
	return M_PI*deg/180;
}

static void gps_set_zero(int32_t longtitude_Fixed, int32_t latitude_Fixed){
	zero_latitude_fixed  = latitude_Fixed;
	zero_longitude_fixed = longtitude_Fixed;

	float latitude = (float)(latitude_Fixed)/LAT_FIXED_PRECISSION;
	float rad_lat  = deg_to_rad(latitude);

	m_per_deg_lat = 111132.954 - 559.822 * cos( 2 * rad_lat) + 1.175 * cos( 4 * rad_lat);
	m_per_deg_lon = 111132.954 * cos ( rad_lat );
	has_zero = true;
}


static float latitude_to_local(int32_t latitude_Fixed){
	return (float)(m_per_deg_lat * (latitude_Fixed - zero_latitude_fixed));
}

static float longtitude_to_local(int32_t longtitude_Fixed){
	return (float)(m_per_deg_lon * (longtitude_Fixed - zero_longitude_fixed));
}

static float altitude_to_local(float altitude){
	return altitude - zero_altitude;
}

static void fill_pos(position_t* pos){
	pos->x = longtitude_to_local(GPS.longitude_fixed);  //EAST
	pos->y = latitude_to_local(GPS.latitude_fixed); 	//North
	pos->z = altitude_to_local(GPS.altitude);			//UP
}

static void fill_vel(velocity_t* vel){
	float heading = deg_to_rad(GPS.angle);
	float speed = GPS.speed;
	vel->x_vel = speed * sin(heading);
	vel->y_vel = speed * cos(heading);
}


void init_gps(sys_status_t* status){
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

	status->gps_com = true;
	status->gps_fix = false;


	has_zero = false;
}



bool gps_read(position_t* pos, velocity_t* vel, sys_status_t* status){
	if (GPS.newNMEAreceived()) {
		GPS.parse(GPS.lastNMEA());

		if (GPS.fix) {
			if (!has_zero) {
				gps_set_zero(GPS.longitude_fixed, GPS.latitude_fixed);
			}

			//Fill the position struct
			fill_pos(pos);

			//Fill the velocity struct
			fill_vel(vel);

			status->gps_fix = true;

			return true;
		}

		else{
			return false;
		}
	}

	else {
		return false;
	}
}

