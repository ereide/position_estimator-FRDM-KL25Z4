#include <Eigen/Dense>

#define INITIAL_STATE_VAR    1000000; //Some very large number

#define STATE_NUMB 3 //x, dx/dt, d^2x/dt^2

typedef Eigen::Matrix<float, STATE_NUMB, STATE_NUMB>   StateCovarianceMatrix;
typedef Eigen::Matrix<float, STATE_NUMB, STATE_NUMB>   StateTransistionMatrix;

typedef Eigen::Matrix<float, 1, STATE_NUMB>     ObservationModelMatrix;
typedef Eigen::Matrix<float, STATE_NUMB, 1>     KalmanUpdatelMatrix;

typedef Eigen::Matrix<float, STATE_NUMB, 1>            StateVector;

class KalmanFilter {
    private:
        StateCovarianceMatrix   P;
        StateVector             x;

    public:
        KalmanFilter();
        KalmanFilter(StateVector x0, StateCovarianceMatrix P0);

        StateVector getStateVector();

        StateCovarianceMatrix getStateCovarianceMatrix();
        

        void predict(StateTransistionMatrix& F, StateCovarianceMatrix& Q);

        void update(float z,  ObservationModelMatrix& H, float var);
}; 
