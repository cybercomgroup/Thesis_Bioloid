# Thesis Bioloid
 Sebastian Lind and Tobias Laving

RASP IP ON CABLE:
10.90.131.122


1. CONFIG
2. SETUP
3. USAGE BELOW

1. Config
THESE ARE CONFIGURATIONS FOR RUNNING THE PROJECT ON LINUX SYSTEMS; WORKS ON UBUNTU 16.04 AND RASPBIAN 4.4.50-v7+

Bashrc:

Bashrc is an configuration file for the bash shell on linux. It can be used to configure visible variables for the system
and is used in the project to configure the Raspberry pi.
To access it write "nano ~/.bashrc" in the terminal. Then add these lines at the end:

.bashrc on Raspberry pi
  sudo modprobe bcm2835-v4l2 <- Used to config the raspicamera to work for opencv

Save and exit. The user need to use "source nano ~/.bashrc" to reload with the new settings, or restart the terminal.

2. SETUP
Project setup:

The project currently uses OpenCV 3.2.0 and pocketsphinx-5prealpha. These are easily installed following any tutorial. Use google or
these provided links:

Audio:
Install pocketsphinx-5prealpha or higher.
http://www.pyimagesearch.com/2016/04/18/install-guide-raspberry-pi-3-raspbian-jessie-opencv-3/
The project needs to be config for the usb microphone. Use "arecord -l" in the terminal to get an list of your listening devices.
You will get an similar output as this:
**** List of CAPTURE Hardware Devices ****
card 0: PCH [HDA Intel PCH], device 0: CX20751/2 Analog [CX20751/2 Analog]
  Subdevices: 1/1
  Subdevice #0: subdevice #0
In audio.cpp the MIC define is set to enable diffrent microphones for diffrent platform. Use "hw:<CARD>;<DEVICE>" to choose your device.

Image:
Install openCV 3.2.0 or higher.
http://www.pyimagesearch.com/2016/04/18/install-guide-raspberry-pi-3-raspbian-jessie-opencv-3/
http://pklab.net/?id=392&lang=EN


3. USAGE
The project is split in separate modules, comm, image and audio. The project has an makefile in each folder aswell as the main folder.
The makefile in the project folder can be used to control the whole project. Inside the module folders are separate makefiles used to build separate executables that can be used for testing. These executables might not be updated with all functionality.

After getting the repo on your local device use "make" in the terminal to contruct the project. Then use "./main -h" in the terminal to get instructions on how to run the program.
