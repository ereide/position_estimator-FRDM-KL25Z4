/*
 * predictor.h
 *
 *	API for the underlying kalman filter
 *
 *  Created on: Dec 16, 2017
 *      Author: Eivind Roson Eide (c)
 */

#ifndef SOURCES_LINEAR_PREDICTOR_H_
#define SOURCES_LINEAR_PREDICTOR_H_

#include "types.h"


//#define USE_MEAN_TIME 0

//Porbabilistic parameters
#define MODEL_ACCEL_VAR               1			//(m/s^2)^2
#define MODEL_NUMERICAL_ACCEL_VAR	  0.0001


#define OBSERVATION_POS_VAR           0.1		//m^2
#define OBSERVATION_VEL_VAR           0.1		//(m/s)^2
#define OBSERVATION_ACCEL_VAR         0.3		//(m/s^2)^2
#define PREDICTOR_MEAN_TIME_DELAY     0.01		//s

//Set to 1 if we expect acceleration to be approximately constant over time
//Set to 0 if we expect little on the acceleration data between measurments
#define ACCELL_DECAY_RATE		      1	    //Assumes the acceleration does not depend heavily on the previous value


#ifdef __cplusplus
extern "C" {
#endif



void init_filters(float mod_acc_var, float obs_pos_var, float obs_vel_var,  float obs_acc_var);

#ifdef USE_MEAN_TIME
    void predict(void);

#else
    void predict(float dt);
#endif //USE_MEAN_TIME

void update_accel(acceleration_t *acc);
void update_vel(velocity_t *vel);
void update_pos(position_t *pos);


//void getxState(state_t *state);
//void getxVar(state_t *state);

//void getyState(state_t *state);
//void getyVar(state_t *state);

void getzState(state_t *state);
void getzVar(state_t *state);


void getPosState(position_t* pos);


#ifdef __cplusplus
}
#endif

#endif /* SOURCES_LINEAR_PREDICTOR_H_ */
