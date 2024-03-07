# Bluepad32 support for MiniSkidi V3.0

This PR combines the two excelent projects:
1. [MiniSkidi](https://github.com/ProfBoots/MiniSkidi-V3.0) by ProfBoots and
2. [Bluepad32](https://github.com/ricardoquesada/bluepad32) by ricardoquesada

to create a remote-controlled miniature skid-steer. This is very much (and will likely remain) work-in-progress, but should work as a drop-in. The code is adapted from the Bluepad32 Arduino example included in the library. 

## Controls (for PS3)
 * Left X - Bucket Tilt
 * Left Y - Proportional Left Track Forward/Back
 * Right X - Arm Up/Down
 * Right Y - Proportional Right Track Foward Back
 * X - Toggle Lights

 Rumble activates when attempting to drive servos past their limit :) 
## Todo
- [ ] add aux control support 
- [ ] test with xbox controller 
- [ ] switch to ISO-style mapping for skid-steers
- [ ] figure out a better way to accel/decel proportional controls
- [x] add rumble

## Controller support
Any controller supported by Bluepad32 _should_ work, but the mapping might look a little different. 