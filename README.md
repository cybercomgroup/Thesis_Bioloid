# Thesis Bioloid
 Sebastian Lind and Tobias Laving

RASP IP ON CABLE:
10.90.131.122


1. CONFIG
2. USAGE BELOW

1. Config
THESE ARE CONFIGURATIONS FOR RUNNING THE PROJECT ON LINUX SYSTEMS; WORKS ON UBUNTU 16.04 AND RASPBIAN 4.4.50-v7+

.bashrc config Ubuntu:
  JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
  export JAVA_HOME
  export PATH=$PATH:$JAVA_HOME
  PROC=amd64
  export PROC

  export LD_LIBRARY_PATH=$JAVA_HOME/jre/lib/amd64:$JAVA_HOME/jre/lib/amd64/server

.bashrc on Raspberry pi
  sudo modprobe bcm2835-v4l2 <- Used to config the raspicamera to work for opencv
  JAVA_HOME=/usr/lib/jvm/jdk-8-oracle-arm32-vfp-hflt
  export JAVA_HOME
  export PATH=$PATH:$JAVA_HOME
  PROC=arm
  export PROC

  export LD_LIBRARY_PATH=$JAVA_HOME/jre/lib/arm:$JAVA_HOME/jre/lib/arm/server

Raspberrypi setup:
The audio card need to be config to use usb headset:
https://raspberrypi.stackexchange.com/questions/40831/how-do-i-configure-my-sound-for-jasper-on-raspbian-jessie


The project currently runs on JAVA SDK 8+ and OpenCV 3.2.0

2. USAGE
The project is split in one main module and two supporting modules, image and audio. The project is provided by an make file in the main folder.
This make can be used to control entire project. Inside the two module folders are separate make file used to build separate executables
that can be used for testing. These might not be updated with all functionality.

Please check the makefile for more information.
