# Compiler options
CC=g++
CFLAGS= -std=c++11
LDFLAGS=
EXECUTABLE_NAME=test_predictor

# Folders
SRC=.
BIN=bin

PROJECT_SRC=../Sources
EIGEN_PATH=../External/eigen/


# Files
SOURCE_FILES=\
    $(PROJECT_SRC)/KalmanFilter/kalman_filter_small.cpp \
    $(PROJECT_SRC)/KalmanFilter/predictor.cpp \
    test_predictor.cpp
    
INC=-I$(EIGEN_PATH) -I$(PROJECT_SRC)/KalmanFilter/ -I$(PROJECT_SRC)/Common/


all:
	$(CC) $(CFLAGS) $(LDFLAGS) $(INC) $(SOURCE_FILES) -o $(BIN)/$(EXECUTABLE_NAME)