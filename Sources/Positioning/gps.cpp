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

#define LAT_FIXED_PRECISSION 	100000
#define MPS_PER_KNOT 			(0.514444)

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

static float fixed_to_float(int32_t deg_fixed){
	return (float)(deg_fixed)/LAT_FIXED_PRECISSION;
}

static float knots_to_mps(float knots){
	return MPS_PER_KNOT * knots;
}

//Uses the first fix reading as the origin, and uses this value to compute the
//conversion factor from degrees to local coordinates
static void gps_set_zero(int32_t longitude_Fixed, int32_t latitude_Fixed){
	zero_latitude_fixed  = latitude_Fixed;
	zero_longitude_fixed = longitude_Fixed;

	float latitude = fixed_to_float(latitude_Fixed);
	float rad_lat  = deg_to_rad(latitude);

	m_per_deg_lat = 111132.954 - 559.822 * cos( 2 * rad_lat) + 1.175 * cos( 4 * rad_lat);
	m_per_deg_lon = 111132.954 * cos ( rad_lat );
	has_zero = true;
}


//Converts the fixed latitude value to local coordinates accounting for origin
static float latitude_to_local(int32_t latitude_Fixed){
	return (m_per_deg_lat * fixed_to_float(latitude_Fixed - zero_latitude_fixed));
}

//Converts the fixed longitude value to local coordinates accounting for origin
static float longitude_to_local(int32_t longitude_Fixed){
	return (m_per_deg_lon * fixed_to_float(longitude_Fixed - zero_longitude_fixed));
}

static float altitude_to_local(float altitude){
	return altitude - zero_altitude;
}

//Converts geographical coordinates to a local coordinate system
static void fill_pos(position_t* pos){
	pos->x = longitude_to_local(GPS.longitude_fixed);   //East
	pos->y = latitude_to_local(GPS.latitude_fixed); 	//North
	pos->z = altitude_to_local(GPS.altitude);			//UP
}

//Converts velocity and heading to local coordinates
static void fill_vel(velocity_t* vel){
	float heading = deg_to_rad(GPS.angle);
	float speed = knots_to_mps(GPS.speed);
	vel->x_vel = speed * sin(heading); 					//East
	vel->y_vel = speed * cos(heading);					//North

	//No information about velocity in z direction
}


void init_gps(sys_status_t* status){
	GPS.begin(9600);

	//Set output to be minimum required
	GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	serial_delay(10);


	//Set GPS printout update rate to 5HZ
	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);
	serial_delay(10);


	//Set GPS fix rate to 5HZ
	GPS.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);
	serial_delay(10);

	//Communication is OK, FIX is starts as false
	status->gps_com = true;
	status->gps_fix = false;


	has_zero = false;
}


//Interface for reading position and velocity data
bool gps_read(position_t* pos, velocity_t* vel, sys_status_t* status){
	//Check if we have a new complete NMEA message
	if (GPS.newNMEAreceived()) {
		//Parse this message
		GPS.parse(GPS.lastNMEA());

		print_last_NMEA();

		//If we have a valid fix update the data
		if (GPS.fix) {

			//Set zero if not done already
			if (!has_zero) {
				gps_set_zero(GPS.longitude_fixed, GPS.latitude_fixed);
			}

			//Fill the position struct
			fill_pos(pos);

			//Fill the velocity struct
			fill_vel(vel);

			//Set the fix flag
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

