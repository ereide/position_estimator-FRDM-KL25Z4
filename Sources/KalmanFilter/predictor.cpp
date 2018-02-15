#include <predictor.h>
#include "kalman_filter_small.h"

using namespace Eigen;

static float model_acc_var;
static float observation_pos_var;
static float observation_vel_var;
static float observation_acc_var;


//One instance for each dimension
static KalmanFilter kalman_x;
static KalmanFilter kalman_y;
static KalmanFilter kalman_z;

//The Kalman Filter parameters 
static ObservationModelMatrix H_pos;
static ObservationModelMatrix H_vel;
static ObservationModelMatrix H_acc;

static StateTransistionMatrix F;
static StateCovarianceMatrix Q;

static StateTransistionMatrix create1DStateTransition(float dt){
    StateTransistionMatrix transition;
    transition <<   1, dt,  		  dt*dt/2.0,
                    0,  1,  				 dt,
                    0,  0,  ACCELL_DECAY_RATE*1;
    return transition;
}


static StateCovarianceMatrix create1DModelCovariance(float dt){
    StateCovarianceMatrix Q;
    float dt_2 = dt*dt; 
    float dt_3 = dt*dt_2;
    float dt_4 = dt_2 * dt_2;
    Q <<    dt_4/4, dt_3/2, dt_2/2, 
            dt_3/2,   dt_2,     dt,
            dt_2/2,     dt,      1;

    return Q*model_acc_var + StateCovarianceMatrix::Identity()*MODEL_NUMERICAL_ACCEL_VAR;
} 

static void getState(KalmanFilter& filter, state_t *state){
    StateVector x = filter.getStateVector();

    state->pos = x(0);
    state->vel = x(1);
    state->acc = x(2);
} 

/*
void getxState(state_t *state){
    getState(kalman_x, state);
}

void getyState(state_t *state){
    getState(kalman_y, state);
}

*/

void getzState(state_t *state){
    getState(kalman_z, state);
}

void getPosState(position_t* pos){
    StateVector state;

	state = kalman_x.getStateVector();
	pos->x = state(0);

    state = kalman_y.getStateVector();
	pos->y = state(0);

    state = kalman_z.getStateVector();
	pos->z = state(0);
}

void init_filters(float mod_acc_var, float obs_pos_var, float obs_vel_var,  float obs_acc_var){
	model_acc_var 		= mod_acc_var;
	observation_pos_var = obs_pos_var;
	observation_vel_var = obs_vel_var;
	observation_acc_var = obs_acc_var;


    H_pos << 1, 0, 0;
    H_vel << 0, 1, 0;
    H_acc << 0, 0, 1;

    //If we want don't want to continuously update these
    #ifdef USE_MEAN_TIME
    F = create1DStateTransition(PREDICTOR_MEAN_TIME_DELAY);
    Q = create1DModelCovariance(PREDICTOR_MEAN_TIME_DELAY);
    #endif //USE_MEAN_TIME

    kalman_x = KalmanFilter();
    kalman_y = KalmanFilter();
    kalman_z = KalmanFilter();
}

#ifdef USE_MEAN_TIME
void predict(void){
    kalman_x.predict(F, Q);
    kalman_y.predict(F, Q);
    kalman_z.predict(F, Q);
}

#else
void predict(float dt){
    F = create1DStateTransition(dt);
    Q = create1DModelCovariance(dt);

    kalman_x.predict(F, Q);
    kalman_y.predict(F, Q);
    kalman_z.predict(F, Q);
}
#endif //USE_MEAN_TIME

void update_accel(acceleration_t *acc) {
	//Assuming x, y accel is ~0 -> no valid data
	//kalman_x.update(acc->x_acc, H_acc, observation_acc_var);
    //kalman_y.update(acc->y_acc, H_acc, observation_acc_var);

	//update z velocity
    kalman_z.update(acc->z_acc, H_acc, observation_acc_var);
}

void update_vel(velocity_t *vel){
    kalman_x.update(vel->x_vel, H_vel, observation_vel_var);
    kalman_y.update(vel->y_vel, H_vel, observation_vel_var);

    //Currently no data updating the z velocity correctly
    //kalman_y.update(pos->y	  , H_pos, observation_pos_var);
}


void update_pos(position_t *pos) {
    kalman_x.update(pos->x	  , H_pos, observation_pos_var);
    kalman_y.update(pos->y	  , H_pos, observation_pos_var);
    kalman_z.update(pos->z	  , H_pos, observation_pos_var);
}

