DEATH VALLEY
------------

[![Build Status](https://travis-ci.org/nardinan/death_valley.svg?branch=master)](https://travis-ci.org/nardinan/death_valley)

A very simple terminal controller for thermal chambers (VT4010 from Votsch, ) that gives to you the ability to execute some batches. This software needs Miranda (http://github.com/nardinan/miranda) and has been tested on Scientific Linux (>= 6.5) and MacOS.

![Alt text](/screenshot.jpg?raw=true)

# Compile it
The software has been sucessfully compiled on Linux (Scientific Linux, Ubuntu and Fedora) and MacOS.

### Dependencies
The software depends on the following libraries:
* libmiranda (https://github.com/nardinan/miranda)

### Compilation on Ubuntu (and possibly any other _Debian-based_ distribution), Fedora, Scientific Linux and MacOS
Open a termina and install Miranda:
```bash
$ git clone https://github.com/nardinan/miranda
$ cd miranda
$ make
$ sudo make install
$ cd ..
```
Great, Miranda is now installed in your /usr/local/lib directory. **Be sure to add the corrisponding entry "/usr/local/lib" to LD_LIBRARY_PATH (DYLD_LIBRARY_PATH on MacOS).** After that, from your terminal, perform the following steps to build Death Valley:
```bash
$ git clone https://github.com/nardinan/death_valley
$ cd death_valley
$ make
```

# The software

Deat Valley requires the VT4010 chamber to be connected to your computer via a **serial to USB** cable. Once the cable has been connected, a new device will be visible in the /dev folder (e.g. /dev/ttyUSB0). Launch death valley, passing as parameter the location of the device:
```bash
$ ./death_valley /dev/ttyUSB0
```
The prompt of the software should show you that the chamber is online and ready to be used. Optionally, the software accepts a third parameter: the /dev path that points to a Rotronic's HC2-WIN probe connected via USB to the computer. In that case, you may have:
```bash
$ ./death_valley /dev/ttyUSB0 /dev/ttyUSB1
```
