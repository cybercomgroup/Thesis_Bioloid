# Thesis Bioloid
 Sebastian Lind and Tobias Laving

NOTE:
The make in image is now independent from the env, if you can't make edit the JAVA varialbe.


Raspberrypi setup:

The audio card need to be config to use usb headset:
https://raspberrypi.stackexchange.com/questions/40831/how-do-i-configure-my-sound-for-jasper-on-raspbian-jessie

Also, add this to .bashrc end in pi dir /home/pi: 
JAVA_HOME=/usr/lib/jvm/jdk-8-oracle-arm32-vfp-hflt <= This path may vary. 
export JAVA_HOME
export PATH=$PATH:$JAVA_HOME


export LD_LIBRARY_PATH=$JAVA_HOME/jre/lib/arm:$JAVA_HOME/jre/lib/arm/server
