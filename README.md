# Thesis Bioloid
 Sebastian Lind and Tobias Laving

NOTE:
The make in image is now independent from the env, if you can't make edit the JAVA varialbe.


Raspberrypi setup:

The audio card need to be config to use usb headset:
https://raspberrypi.stackexchange.com/questions/40831/how-do-i-configure-my-sound-for-jasper-on-raspbian-jessie

Also, add this to .bashrc in pi dir /home/pi: 
export LD_LIBRARY_PATH=/usr/lib/jvm/jdk-8-oracle-arm32-vfp-hflt/jre/lib/arm:/usr/lib/jvm/jdk-8-oracle-arm32-vfp-hflt/jre/lib/arm/server
(Note the java path)
