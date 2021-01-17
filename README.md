# Snake-Game-with-Arduino-Mega-and-1602-lcd
/*Snake Game with lcd and joystick and Arduino mega By Aryan Chavan. But this code can work with other arduino boards eg: uno, nano.
  #By Aryan Chavan
  This game is like the snake game in the old phones.
# COMPONENTS NEEDED TO BUILD
  1x arduino
  jumper wires
  1x breadboard  (for compact build)  // but would advise using 2 breadboards
  1x Joystick    (from the arduino kit will work)
  1x 1602 LCD    (i used the non i2c 1602 LCD)
  2x 10k Potentiometers
  1x led         (i dint hook the led to arduino but you can if wanted)
  1x 220 ohm resistor (for the led)
  1x pushbutton   (you can add more for other things)
  2x 10k ohm resistor (for the pushbutton)
# Some inportant notes
  press the pushbutton to change the speed.
  you will only be able to change speed while holding the button down and changing the speed with the potentiometer at the same time.
  the game will only start after the speed is set.
  it takes about 15 seconds to set the speed
  as per the counter and delay the you will have 30 chances to set the speed and you will have to wait till the counter goes to 30. 
  after the counter reaches 30 the game will wait fro 3 secs and then it will start.
