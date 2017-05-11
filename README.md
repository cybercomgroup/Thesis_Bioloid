# Thesis Bioloid
 Sebastian Lind and Tobias Laving

RASP IP ON CABLE:
10.90.131.122


1. CONFIG
2. USAGE BELOW

1. Config
THESE ARE CONFIGURATIONS FOR RUNNING THE PROJECT ON LINUX SYSTEMS; WORKS ON UBUNTU 16.04 AND RASPBIAN 4.4.50-v7+

.bashrc config Ubuntu:
  JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64 <-This are for Voce, not needed
  export JAVA_HOME <-This are for Voce, not needed
  export PATH=$PATH:$JAVA_HOME <-This are for Voce, not needed
  PROC=amd64 <-This are for Voce, not needed
  export PROC <-This are for Voce, not needed

  export LD_LIBRARY_PATH=$JAVA_HOME/jre/lib/amd64:$JAVA_HOME/jre/lib/amd64/server <-This are for Voce, not needed

.bashrc on Raspberry pi
  sudo modprobe bcm2835-v4l2 <- Used to config the raspicamera to work for opencv
  JAVA_HOME=/usr/lib/jvm/jdk-8-oracle-arm32-vfp-hflt
  export JAVA_HOME <-This are for Voce, not needed
  export PATH=$PATH:$JAVA_HOME <-This are for Voce, not needed
  PROC=arm <-This are for Voce, not needed
  export PROC <-This are for Voce, not needed

  export LD_LIBRARY_PATH=$JAVA_HOME/jre/lib/arm:$JAVA_HOME/jre/lib/arm/server <-This are for Voce, not needed

Raspberrypi setup:

The project currently runs on and OpenCV 3.2.0 and pocketsphinx-5prealpha.

Audio:
Install pocketsphinx-5prealpha or higher.
http://www.pyimagesearch.com/2016/04/18/install-guide-raspberry-pi-3-raspbian-jessie-opencv-3/

Depending on the config inside mic the audio card need to be config to use usb headset as default:
https://raspberrypi.stackexchange.com/questions/40831/how-do-i-configure-my-sound-for-jasper-on-raspbian-jessie
This can otherwise be changed inside audio.cpp
Set MIC declaration to whatever arecord -l says your mic is on.

Image:
Install openCV 3.2.0 or higher.
http://www.pyimagesearch.com/2016/04/18/install-guide-raspberry-pi-3-raspbian-jessie-opencv-3/ should work.


2. USAGE
The project is split in separate modules, comm, image and audio. The project is provided by an make file in the main folder.
This make can be used to control entire project. Inside the module folders are separate make file used to build separate executables
that can be used for testing. These might not be updated with all functionality.

Use ./main -h instructions on running the program
