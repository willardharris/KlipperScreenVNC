#!/usr/bin/env bash

# Use display 10 to avoid clashing with local X server, if anyy
Xtigervnc -geometry 800x480 -rfbport 5901 -noreset -AlwaysShared -depth 16 -ZlibLevel 2 -FrameRate 25  -SecurityTypes none :10&
DISPLAY=:10 $KS_XCLIENT&
/usr/bin/xinit&
DISPLAY=:0 /home/lee/.KlipperScreen-env/bin/python /home/lee/KlipperScreen/screen.py
wait
