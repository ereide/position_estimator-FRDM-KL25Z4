#include <iostream>
#include <random>
#include <cmath>
#include <cstring>

#include "../Sources/KalmanFilter/predictor.h"
#include "../Sources/Common/types.h"



#define TEST_MODEL_ACCEL_VAR               0.01

#define TEST_OBSERVATION_POS_VAR           0.5
#define TEST_OBSERVATION_VEL_VAR           1
#define TEST_OBSERVATION_ACCEL_VAR         1

#define GRAVITY                            (-9.81)

#define TEST_RATIO						   10

using namespace std;

struct Simulation {
	public:
		virtual void getState(state_t* state_actual , float t){
			return;
		}
};

void simulator(Simulation* simulation, int N, const char* name){
	srand(1);

	FILE * pFile;
	pFile = fopen (name, "w");

    float dt = 0.1;
	float t  = 0;

    float h, v, a;		//Actual state
    float z_h, z_v, z_a;//Noise

    float z_e = 0;

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
    state_t variance;

    state_t state_actual;

    default_random_engine generator;
    normal_distribution<double> mod_accel_distribution(0, sqrt(TEST_MODEL_ACCEL_VAR));
    normal_distribution<double> obs_accel_distribution(0, sqrt(TEST_OBSERVATION_ACCEL_VAR));
    normal_distribution<double> obs_pos_distribution(0, sqrt(TEST_OBSERVATION_POS_VAR));

    z_e = obs_pos_distribution(generator) * 10;


    init_filters(TEST_MODEL_ACCEL_VAR, TEST_OBSERVATION_POS_VAR * 10, TEST_OBSERVATION_VEL_VAR, TEST_OBSERVATION_ACCEL_VAR);

    fprintf(pFile, "%3s, %10s, %10s, %10s, %10s, %10s, %10s, %10s, %10s, %10s, %10s, %10s\n\r",
    		"i", "z_h", "z_a", "h", "v", "h_est", "v_est", "a_est", "h_var", "v_var", "h_err", "v_err");

    for(int i = 0; i<N; i++){

        //real trajectory parameters
        t = dt * i;

        simulation->getState(&state_actual, t);

        h = state_actual.pos;
        v = state_actual.vel;
        a = state_actual.acc;



        predict(dt);

        z_a     = a + obs_accel_distribution(generator);
        acc.z_acc   = z_a;
        update_accel(&acc);

        //Only read GPS data at a certain frequency
        if (i % TEST_RATIO == 0) {
            z_e = (z_e + obs_pos_distribution(generator))/2.0;
            z_h     = h + z_e;
            pos.z	  	= z_h;
            update_pos(&pos);

            getzState(&state);
            getzVar(&variance);

            fprintf(pFile, "%3d, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f \n\r",
            		i, z_h, z_a, h, v, state.pos , state.vel, state.acc, log10(variance.pos), log10(variance.vel), state.pos - h, state.vel - v );
        }


        else {
            getzState(&state);
            getzVar(&variance);
            fprintf(pFile, "%3d, %10s, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %10.3f \n\r",
            		i, " ", z_a, h, v, state.pos , state.vel, state.acc, log10(variance.pos), log10(variance.vel), state.pos - h, state.vel - v );
        }
    }

    fclose(pFile);
}


//Extends Simulation to implement readings from a Ballistic path
struct Ballistic : public virtual Simulation {
	private:
		float v0;
		float h0;
	public:

		Ballistic(float v, float h) : Simulation(){
			v0 = v;
			h0 = h;
		};

		void getState(state_t* state , float t){
			state->acc = GRAVITY;
			state->vel = GRAVITY * t 	 + v0;
			state->pos = 0.5*GRAVITY*t*t + v0*t + h0 ;
		};

};

//Extends Simulation to implement readings from a Sinusoidal path
struct Sinusoidal : public virtual Simulation {
	private:
		float amplitude;
		float freq;

	public:
		Sinusoidal(float A, float period)  : Simulation(){
			amplitude = A;
			freq = 2 * M_PI/(period);
		}

		void getState(state_t* state , float t){
			state->pos = amplitude * cos(freq * t);
			state->vel = -amplitude * sin(freq * t) * freq;
			state->acc = -amplitude * cos(freq * t) * freq * freq;
		}
};


void testSinusoidalPath(float amplitude, float period, int N){
    Sinusoidal sim = Sinusoidal(amplitude, period);
    simulator(&sim, N, "out/sinusoidal.txt");
}


void testBallisticPath(float h0, float v0, int N){
    Ballistic sim = Ballistic(v0, h0);
    simulator(&sim, N, "out/ballistic.txt");
}



int main(int argc, char* argv[])
{
    if(argc == 5){
    	if (strcmp(argv[1], "sin") == 0) {
        	testSinusoidalPath(atof(argv[2]), atof(argv[3]), atoi(argv[4]));
		}

    	else if (strcmp(argv[1], "bal") == 0) {
    		testBallisticPath(atof(argv[2]), atof(argv[3]), atoi(argv[4]));
		}

    	else {
    		cout << "First parameter must be either 'sin' or 'bal'" << endl;
    	}
    }
    else if(argc == 2){
    	if (strcmp(argv[1], "sin") == 0) {
    		testSinusoidalPath(1, 20, 100);
		}

    	else if (strcmp(argv[1], "bal") == 0) {
    		testBallisticPath(20, 20, 100);
		}

    	else {
    		cout << "First parameter must be either 'sin' or 'bal'" << endl;
    	}
    }

    else {
    	cout << "First parameter must be either 'sin' or 'bal'" << endl;
    }
}
