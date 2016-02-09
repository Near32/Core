cmake_minimum_required (VERSION 2.6)
project (HAROEXP)
set(CMAKE_CXX_COMPILER /usr/bin/g++-4.7)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -lbcm2835 -lm -lwiringPi -pthread -lSDL -lSDLmain -std=c++0x")

#########################################################
# FIND SDL
#########################################################
find_package(SDL REQUIRED)
include_directories(${SDL_INCLUDE_DIRS})
link_directories(${SDL_LIBRARY_DIRS})
add_definitions(${SDL_DEFINITIONS})
if(NOT SDL_FOUND)
    message(ERROR " SDL not found!")
endif(NOT SDL_FOUND)
#########################################################
# FIND OPENGL
#########################################################
#find_package(OpenGL REQUIRED)
#include_directories(${OpenGL_INCLUDE_DIRS})
#link_directories(${OpenGL_LIBRARY_DIRS})
#add_definitions(${OpenGL_DEFINITIONS})
#if(NOT OPENGL_FOUND)
#    message(ERROR " OPENGL not found!")
#endif(NOT OPENGL_FOUND)
#########################################################
# Include Files
#########################################################
#add_executable(Gunvarrel src/mainENGINE.cpp src/EtatEngine.cpp src/IEngine.cpp src/VueEngine.cpp src/Game.cpp src/Environnement.cpp src/IElement.cpp src/TrackBallCamera.cpp  src/utils/math.cpp src/utils/Quaternion/Quaternion.cpp src/common/objloader.cpp)
#add_executable(Gunvarrel src/mainENGINE.cpp src/EtatEngine.cpp src/IEngine.cpp src/VueEngine.cpp src/Game.cpp src/Environnement.cpp src/IElement.cpp src/IElementFixe.cpp src/Obstacle.cpp src/OrbeBonus.cpp src/IFabricant.cpp src/Fabriques.cpp src/TrackBallCamera.cpp  src/utils/math.cpp src/utils/Quaternion/Quaternion.cpp src/common/objloader.cpp )
add_executable(HAROEXP mainHAROEXP.cpp ../EXP/EXP.cpp ../FUNC/FUNC.cpp ../VAR/VAR.cpp ../Operations/Operations.cpp ./HARO/HAROLegsEXP.cpp ./HARO/HAROEXP.cpp ./PE/PE.cpp ./Servo/Servo.cpp ./ServoEXP/ServoEXP.cpp ./Frame/Frame.cpp ./FrameEXP/FrameEXP.cpp ./MVG/MVG.cpp ./Quaternion/Quaternion.cpp ./PE/MPU6050/MPU6050.cpp ./PE/RPI/I2Cdev/I2Cdev.cpp ./PIDController/PIDController/PIDControllerM.cpp )

########################################################
# Linking & stuff
#########################################################

# create the program "HAROEXP"
target_link_libraries(HAROEXP ${SDL_LIBRARY} -lm -lbcm2835)

