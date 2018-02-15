#include <iostream>
#include <random>
#include <cmath>

#include "../Sources/KalmanFilter/predictor.h"
#include "../Sources/Common/types.h"



#define TEST_MODEL_ACCEL_VAR               0.1
#define TEST_OBSERVATION_POS_VAR           1
#define TEST_OBSERVATION_ACCEL_VAR         1
#define GRAVITY                            (-9.81)

using namespace std;

struct simulation_t {
	virtual void getState(state_t* state_actual , float t);
};

struct Ballistic : simulation_t {
	float v0;
	float h0;

	Ballistic(float v, float h){
		v0 = v;
		h0 = h;
	};

	void getState(state_t* state , float t){
        state->acc = GRAVITY;
        state->vel = GRAVITY * t 	 + v0;
        state->pos = 0.5*GRAVITY*t*t + v0*t + h0 ;
	};
};

struct Sinusoidal : simulation_t {
	float amplitude;
	float freq;

	Sinusoidal(float A, float period){
		amplitude = A;
		freq = 1/period;
	}

	void getState(state_t* state , float t){
        state->pos = amplitude * cos(t);
        state->vel = amplitude * sin(freq * t) * freq;
        state->acc = amplitude * cos(freq * t) * freq * freq;
	}
};


void testSinusoidalPath(float amplitude, float period, int N){
    //cout << "Testing a sinusoidal path" << endl;
    //cout << "Setting starting parameters: " << endl;

    float freq = 1/period;

    float dt = 0.1;
    float h;
    float v;
    float a;
	float t;
    float z_h;
	float z_v;
    float z_a;

    position_t pos = {
    	.x = 0,
    	.y = 0,
    	.z = 0
    };

    acceleration_t acc = {
    	.x_acc = 0,
		.y_acc = 0,
		.z_acc = 0
    };

    state_t state;



    default_random_engine generator;
    normal_distribution<double> mod_accel_distribution(0, sqrt(TEST_MODEL_ACCEL_VAR));
    normal_distribution<double> obs_accel_distribution(0, sqrt(TEST_OBSERVATION_ACCEL_VAR));
    normal_distribution<double> obs_pos_distribution(0, sqrt(TEST_OBSERVATION_POS_VAR));

    init_filters(MODEL_ACCEL_VAR, OBSERVATION_POS_VAR, OBSERVATION_VEL_VAR, OBSERVATION_ACCEL_VAR);

    printf("%3s, %10s, %10s, %10s, %10s, %10s, %10s, %10s, %10s, %10s \n\r",
    		"i", "z_h", "z_a", "h", "v", "h_est", "v_est", "a_est", "h_err", "v_err");

    for(int i = 0; i<N; i++){
        //real trajectory parameters
        t = dt * i;
        h = amplitude * cos(t);
        v = amplitude * sin(freq * t) * freq;
        a = amplitude * cos(freq * t) * freq * freq;

        z_h     = h + obs_pos_distribution(generator);
        z_a     = a + obs_accel_distribution(generator);

        pos.z	  	= z_h;
        acc.z_acc   = z_a;

        predict(dt);

        update_accel(&acc);
        update_pos(&pos);

        getzState(&state);

        printf("%3d, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f \n\r",
        		i, z_h, z_a, h, v, state.pos , state.vel, state.acc, state.pos - h, state.vel - v );
    }
}


void testBallisticPath(float h0, float v0, int N){
    cout << "Testing a ballistic path" << endl;
    cout << "Setting starting parameters: " << endl;

    float dt = 0.1;
    float h = h0;
    float v = v0;
    float a;
	float t;
    float z_h;
	float z_v;
    float z_a;
    
    position_t pos = {
    	.x = 0,
    	.y = 0,
    	.z = 0
    };

    acceleration_t acc = {
    	.x_acc = 0,
		.y_acc = 0,
		.z_acc = 0
    };

    state_t state;



    default_random_engine generator;
    normal_distribution<double> mod_accel_distribution(0, sqrt(TEST_MODEL_ACCEL_VAR));
    normal_distribution<double> obs_accel_distribution(0, sqrt(TEST_OBSERVATION_ACCEL_VAR));
    normal_distribution<double> obs_pos_distribution(0, sqrt(TEST_OBSERVATION_POS_VAR));

    init_filters(MODEL_ACCEL_VAR, OBSERVATION_POS_VAR, OBSERVATION_VEL_VAR, OBSERVATION_ACCEL_VAR);

    printf("%3s, %10s, %10s, %10s, %10s, %10s, %10s, %10s, %10s, %10s \n\r",
    		"i", "z_h", "z_a", "h", "v", "h_est", "v_est", "a_est", "h_err", "v_err");

    for(int i = 0; i<N; i++){
        a = GRAVITY; //+ mod_accel_distribution(generator);

        //real trajectory parameters
        t = dt * i;
        h = v0*t + 0.5*a*t*t; 
        v = v0 + a * t;

        z_h     = h + obs_pos_distribution(generator);
        z_a     = GRAVITY + obs_accel_distribution(generator);

        pos.z	  	= z_h;
        acc.z_acc   = z_a;

        predict(dt);



        update_accel(&acc);
        update_pos(&pos);

        getzState(&state);

        printf("%3d, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f \n\r",
        		i, z_h, z_a, h, v, state.pos , state.vel, state.acc, state.pos - h, state.vel - v );
    }
}

void simulator(simulation_t* simulation, int N){
    float dt = 0.1;
	float t  = 0;

    float h, v, a;		//Actual state
    float z_h, z_v, z_a;//Noise

    position_t pos = {
    	.x = 0,
    	.y = 0,
    	.z = 0
    };

    acceleration_t acc = {
    	.x_acc = 0,
		.y_acc = 0,
		.z_acc = 0
    };

    state_t state;

    state_t state_actual;

    default_random_engine generator;
    normal_distribution<double> mod_accel_distribution(0, sqrt(TEST_MODEL_ACCEL_VAR));
    normal_distribution<double> obs_accel_distribution(0, sqrt(TEST_OBSERVATION_ACCEL_VAR));
    normal_distribution<double> obs_pos_distribution(0, sqrt(TEST_OBSERVATION_POS_VAR));

    init_filters(MODEL_ACCEL_VAR, OBSERVATION_POS_VAR, OBSERVATION_VEL_VAR, OBSERVATION_ACCEL_VAR);

    printf("%3s, %10s, %10s, %10s, %10s, %10s, %10s, %10s, %10s, %10s \n\r",
    		"i", "z_h", "z_a", "h", "v", "h_est", "v_est", "a_est", "h_err", "v_err");

    for(int i = 0; i<N; i++){

        //real trajectory parameters
        t = dt * i;

        simulation->getState(&state_actual, t);

        h = state_actual.pos;
        v = state_actual.vel;
        a = state_actual.acc;


        z_h     = h + obs_pos_distribution(generator);
        z_a     = a + obs_accel_distribution(generator);

        pos.z	  	= z_h;
        acc.z_acc   = z_a;

        predict(dt);



        update_accel(&acc);
        update_pos(&pos);

        getzState(&state);

        printf("%3d, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f \n\r",
        		i, z_h, z_a, h, v, state.pos , state.vel, state.acc, state.pos - h, state.vel - v );
    }
}


int main(int argc, char* argv[])
{
    if(argc == 4){
    	testSinusoidalPath(atof(argv[1]), atof(argv[2]), atoi(argv[3]));
    }
    else {
    	testSinusoidalPath(1, 20, 100);
    }
}
