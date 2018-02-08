#include "kalman_filter_small.h"

using namespace Eigen;

KalmanFilter::KalmanFilter(){
    this->x = StateVector::Zero();
    this->P = StateCovarianceMatrix::Identity()*INITIAL_STATE_VAR;
}


KalmanFilter::KalmanFilter(StateVector x0, StateCovarianceMatrix P0){
    this-> x = x0;
    this-> P = P0;
}

StateVector KalmanFilter::getStateVector(){
    return this->x;
}

StateCovarianceMatrix KalmanFilter::getStateCovarianceMatrix(){
    return this->P;
}

void KalmanFilter::predict(StateTransistionMatrix& F, StateCovarianceMatrix& Q){
    this->x = F * this->x;
    this->P = F * this->P * F.transpose() + Q;
}

void KalmanFilter::update(float z,  ObservationModelMatrix& H, float var){
    float y = z - H*this->x;
    float S = var + H * this->P * H.transpose();
    KalmanUpdatelMatrix K = (this->P * H.transpose()) / S;
    this->x = this->x + K*y;
    this->P = this->P - K * H*this->P;
}
