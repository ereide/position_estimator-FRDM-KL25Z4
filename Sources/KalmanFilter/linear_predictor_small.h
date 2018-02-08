/*
 * linear_predictor.h
 *
 *  Created on: Dec 16, 2017
 *      Author: Eivind Roson Eide (c)
 */

#ifndef SOURCES_LINEAR_PREDICTOR_H_
#define SOURCES_LINEAR_PREDICTOR_H_

//#define USE_MEAN_TIME 1

//Porbabilistic parameters
#define MODEL_ACCEL_VAR               0.01		//(m/s^2)^2
#define OBSERVATION_POS_VAR           1			//m^2
#define OBSERVATION_ACCEL_VAR         1			//(m/s^2)^2
#define MEAN_TIME_DELAY               0.1		//s


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
	float pos;
	float vel;
	float acc;
} state_t;


void init_filters(float mod_acc_var, float obs_pos_var, float obs_acc_var);

#define USE_MEAN_TIME 1

#ifdef USE_MEAN_TIME
    void predict(void);

#else
    void predict(float dt);
#endif //USE_MEAN_TIME

void update_accel(float xdd, float ydd, float zdd);
void update_pos(float x, float y, float z);


void getxState(state_t *state);
void getyState(state_t *state);
void getzState(state_t *state);


#ifdef __cplusplus
}
#endif

#endif /* SOURCES_LINEAR_PREDICTOR_H_ */
