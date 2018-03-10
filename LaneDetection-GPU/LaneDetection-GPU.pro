#-------------------------------------------------
#
# Project created by QtCreator 2018-03-09T18:55:32
#
#-------------------------------------------------

QT       += core
CONFIG += c++11
QT       -= gui

TARGET = LaneDetection-GPU
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += /home/ubuntu/DriverlessCarChallenge/jetson/carControl/src/0.3/stereo_vision
INCLUDEPATH += /home/ubuntu/DriverlessCarChallenge/jetson/carControl/src/0.3/HAL
INCLUDEPATH += /home/ubuntu/Downloads/2-Linux/OpenNI-Linux-Arm-2.3/Include
INCLUDEPATH += /home/ubuntu/Downloads/2-Linux/OpenNI-Linux-Arm-2.3/Include/Driver
INCLUDEPATH += /home/ubuntu/DriverlessCarChallenge/jetson/carControl/src/0.3/peripheral_driver/i2c

INCLUDEPATH += -I/usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_cudabgsegm -lopencv_cudaobjdetect -lopencv_cudastereo -lopencv_stitching -lopencv_cudafeatures2d -lopencv_superres -lopencv_cudacodec -lopencv_videostab -lopencv_cudaoptflow -lopencv_cudalegacy -lopencv_cudawarping -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dnn -lopencv_dpm -lopencv_fuzzy -lopencv_hdf -lopencv_line_descriptor -lopencv_optflow -lopencv_plot -lopencv_reg -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_rgbd -lopencv_viz -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_face -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_photo -lopencv_imgcodecs -lopencv_cudaimgproc -lopencv_cudafilters -lopencv_imgproc -lopencv_cudaarithm -lopencv_core -lopencv_cudev

LIBS += -L/home/ubuntu/DriverlessCarChallenge/jetson/carControl/src/0.3/bin/Release/ -lkinect-cv2

INCLUDEPATH += /home/ubuntuDriverlessCarChallenge/jetson/carControl/src/0.3/bin/Release
DEPENDPATH += /home/ubuntu/DriverlessCarChallenge/jetson/carControl/src/0.3/bin/Release

PRE_TARGETDEPS += /home/ubuntu/DriverlessCarChallenge/jetson/carControl/src/0.3/bin/Release/libkinect-cv2.a

LIBS += -L/home/ubuntu/DriverlessCarChallenge/jetson/carControl/src/0.3/bin/Release/ -li2c-pwm

INCLUDEPATH += /home/ubuntu/DriverlessCarChallenge/jetson/carControl/src/0.3/bin/Release
DEPENDPATH += /home/ubuntu/DriverlessCarChallenge/jetson/carControl/src/0.3/bin/Release

PRE_TARGETDEPS += /home/ubuntu/DriverlessCarChallenge/jetson/carControl/src/0.3/bin/Release/libi2c-pwm.a

LIBS += -L/home/ubuntu/Downloads/2-Linux/OpenNI-Linux-Arm-2.3/Samples/Bin/ -lOpenNI2

INCLUDEPATH += /home/ubuntu/Downloads/2-Linux/OpenNI-Linux-Arm-2.3/Samples/Bin
DEPENDPATH += /home/ubuntu/Downloads/2-Linux/OpenNI-Linux-Arm-2.3/Samples/Bin

LIBS += -L/home/ubuntu/DriverlessCarChallenge/jetson/carControl/src/0.3/HAL/ -lHAL

INCLUDEPATH += /home/ubuntu/DriverlessCarChallenge/jetson/carControl/src/0.3/HAL
DEPENDPATH += /home/ubuntu/DriverlessCarChallenge/jetson/carControl/src/0.3/HAL

PRE_TARGETDEPS += /home/ubuntu/DriverlessCarChallenge/jetson/carControl/src/0.3/HAL/libHAL.a

SOURCES += main.cpp \
    config.cpp \
    frameprocess.cpp \
    getparam.cpp \
    imageprocess.cpp \
    lane.cpp \
    laneprocess.cpp \
    layer.cpp \
    line.cpp \
    lineprocess.cpp \
    pointprocess.cpp \
    road.cpp \
    utils.cpp \
    detector.cpp \
    debug.cpp

HEADERS += \
    config.h \
    frameprocess.h \
    getparam.h \
    imageprocess.h \
    lane.h \
    laneprocess.h \
    layer.h \
    line.h \
    lineprocess.h \
    pointsprocess.h \
    road.h \
    detector.h \
    debug.h
