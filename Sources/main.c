/*
 * Copyright (c) 2018, Eivind Roson Eide
 *
 */


///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////

//Included files from std
#include <predictor.h>
#include <string.h>

//Included board configuration
#include "board.h"

// SDK Included Files
#include "fsl_debug_console.h"
#include "fsl_clock_manager.h"

//Include Common/types.h for data passing
#include "types.h"

//Include utility functions
//#include "utils.h"

//Include modules
#include "accelerometer.h"
#include "gps.h"
#include "display.h"


////////////////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////////////////
#define UPDATE_RATE_TARGET_MS	10 					  //target frequency in seconds

#define SCREEN_CYCLE_NUMBER		50						  //how often we update state on the screen
#define RTC_INSTANCE      		BOARD_RTC_FUNC_INSTANCE




////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

uint32_t last_time;


static void LED_toggle_master(void)
{
    GPIO_DRV_TogglePinOutput(kGpioLED1);
}


void init_system(void) {
	PRINTF("\n\r");

    // Initialize standard SDK demo application pins.
    hardware_init();

    // Accel device driver utilizes the OSA, so initialize it.
    OSA_Init();

    OSA_TimeDelay(5);
}


static void init_all(sys_status_t* status){
	//Initialize system drivers
    PRINTF("\n\r\n\rSystem setup: \r\n");
	init_system();
    PRINTF("System setup complete!\r\n\n\r");
	OSA_TimeDelay(100);


	//Initialize OLED screen
	PRINTF("OLED SSD1331 setup:\n\r");
	init_display(status);
	PRINTF("OLED SSD1331 setup complete! \n\r\n\r");
	OSA_TimeDelay(100);



	//Initialize Accelerometer module
	PRINTF("Accelerometer setup: \n\r");
	init_accel(status);
    PRINTF("Accelerometer setup complete!\r\n\n\r");
	OSA_TimeDelay(100);


	//Initialize GPS module
    PRINTF("GPS setup:\r\n");
	init_gps(status);
    PRINTF("GPS setup complete!\r\n\n\r");
	OSA_TimeDelay(100);


	//Init kalman filters
    PRINTF("Kalman Filter setup:\r\n\r\n");
	init_filters(MODEL_ACCEL_VAR, OBSERVATION_POS_VAR, OBSERVATION_VEL_VAR, OBSERVATION_ACCEL_VAR);
    PRINTF("Kalman Filter setup complete!\r\n\r\n");
	OSA_TimeDelay(100);

	PRINTF("All systems ready to go! \n\r");

}

//Does update last time as well as returning difference
uint32_t get_time_diff(void){
	uint32_t new_time;
	uint32_t diff;

	//Read the new time
	new_time = OSA_TimeGetMsec(); //ms

	//Calculate difference since last reading
	diff = new_time - last_time;

	//Update the last time reading to the new reading
	last_time = new_time;

	//Return the difference
	return diff;
}

//Does not update last time, bur returns difference
uint32_t get_elapsed_time(void){
	return OSA_TimeGetMsec() - last_time;
}

inline void start_timer(void){
	last_time = OSA_TimeGetMsec();
}

inline float msec_to_float(uint32_t milli_sec){
	return ((float)milli_sec)*0.001;
}

/*!
 * @brief Main demo function.
 */
int main (void)
{
	//This variable will keep track of the
	sys_status_t status = {
		.gps_com 		= false,
		.accel_com 		= false,
		.gps_fix    	= false,
		.display_com 	= false
	};

	uint16_t screen_cycle = 0;

	//Initiate state struct
	state_t state;

	//Ininitiate navigation structs
	position_t 		pos;
	velocity_t      vel;
	acceleration_t  acc;

	uint32_t dt;

	init_all(&status);

	OSA_TimeDelay(2000);

	display_write_text("Init complete \n");
	display_write_text("Starts in 3 s \n");

	OSA_TimeDelay(1000);
	display_empty_screen();
	OSA_TimeDelay(2000);

	//Start timer -> Should be the last thing before starting prediction
	start_timer();

	//Comment in to test the accelerometer conversion accuracy
	//test_accel_accuracy();

    // Main loop.  Get sensor data and update duty cycle for the TPM timer.
    while(1)
    {
        // Wait UPDATE_RATE_MS in between updates (accelerometer updates at 200Hz, GPS at 5Hz).
    	//Toggles Green LED ON and OFF
		LED_toggle_master();

		dt = get_time_diff();
        predict(msec_to_float(dt));

        //Look for new accelerometer data
        if(get_z_accel(&acc)) {
        	update_accel(&acc);
        	status.accel_com = true;
            //PRINTF("Converted Accel: x= %de-3 y = %de-3 z = %de-3\r\n", (int16_t)(acc.x_acc*1000), (int16_t)(acc.y_acc*1000), (int16_t)(acc.z_acc*1000));
        }

        else {
        	status.accel_com = false;
        }


        //Look for new GPS data
		if(gps_read(&pos, &vel, &status)) {
	        update_pos(&pos);
	        //update_vel(&vel);
	        PRINTF("Converted Pos:	 x= %de-3 y = %de-3 z = %de-3\r\n", (int32_t)(pos.x*1000), (int32_t)(pos.y*1000), (int32_t)(pos.z*1000));
		}


		//Only update the screen at set intervals
        if (screen_cycle < SCREEN_CYCLE_NUMBER) {
        	screen_cycle += 1;
		}
        else{
        	screen_cycle = 0;
    		//Read state

        	getzState(&state);
        	getPosState(&pos);

        	//Displays the local coordinates and the status
            display_write_local_coord(&status, &pos);
            // Print out the accelerometer data.
            display_write_data(&state);

            PRINTF("State z:      	 z= %de-3 v = %de-3 a = %de-3 : dt = %dms \r\n", (int32_t)(state.pos*1000), (int32_t)(state.vel*1000), (int32_t)(state.acc*1000), dt);
        }

        dt = get_elapsed_time();

        if(UPDATE_RATE_TARGET_MS > dt){
            OSA_TimeDelay(UPDATE_RATE_TARGET_MS - dt); // Wait for the ramaining time + 1 ms

        }
    }

    return 0;

}
