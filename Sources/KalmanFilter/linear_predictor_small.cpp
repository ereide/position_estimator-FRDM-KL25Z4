#include "linear_predictor_small.h"
#include "kalman_filter_small.h"

using namespace Eigen;

static float model_acc_var;
static float observation_pos_var;
static float observation_acc_var;


//One instance for each dimension
static KalmanFilter kalman_x;
static KalmanFilter kalman_y;
static KalmanFilter kalman_z;

//The Kalman Filter parameters 
static ObservationModelMatrix H_pos;
static ObservationModelMatrix H_acc;

static StateTransistionMatrix F;
static StateCovarianceMatrix Q;

static StateTransistionMatrix create1DStateTransition(float dt){
    StateTransistionMatrix transition;
    transition <<   1, dt,  dt*dt/2.0, 
                    0,  1, dt,
                    0,  0,          1;
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

    return Q*model_acc_var;
} 

static void getState(KalmanFilter& filter, state_t *state){
    StateVector x = filter.getStateVector();

    state->pos = x(0);
    state->vel = x(1);
    state->acc = x(2);
} 

void getxState(state_t *state){
    getState(kalman_x, state);
}

void getyState(state_t *state){
    getState(kalman_y, state);
}

void getzState(state_t *state){
    getState(kalman_z, state);
}

void init_filters(float mod_acc_var, float obs_pos_var, float obs_acc_var){
	model_acc_var 		= mod_acc_var;
	observation_pos_var = obs_pos_var;
	observation_acc_var = obs_acc_var;


    H_pos << 1, 0, 0;
    H_acc << 0, 0, 1;

    //If we want don't want to continuously update these
    #ifdef USE_MEAN_TIME
    F = create1DStateTransition(MEAN_TIME_DELAY);
    Q = create1DModelCovariance(MEAN_TIME_DELAY);
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

void update_accel(float xdd, float ydd, float zdd) {
    kalman_x.update(xdd, H_acc, observation_acc_var);
    kalman_y.update(ydd, H_acc, observation_acc_var);
    kalman_z.update(zdd, H_acc, observation_acc_var);
}

void update_pos(float x, float y, float z) {
    kalman_x.update(x, H_pos, observation_pos_var);
    kalman_y.update(y, H_pos, observation_pos_var);
    kalman_z.update(z, H_pos, observation_pos_var);
}

