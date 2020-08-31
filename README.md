# Arduino Suntracker

This is a toy project used to find and track the brightest lightsource in a room.  
This particular project uses servos with a restricted range of motion, but it can  
be modified easily to move 1 degree at a time instead of 1 position.

This particular build uses 4 photoresistors that were tested to have closely matching  
resistances under the same lighting conditions.

The program opens and writes to an attached SD card (standard 5 pin breakout module), 
and the frame I built used lego and micro:bit Robotbit lego compatible servos.
