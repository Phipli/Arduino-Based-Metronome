# Arduino-Based-Metronome
A metronome based on the Atmel ATMega328, written in the Arduino IDE. This code should be compatible with an Arduino Uno, but was designed for a custom circuit.

The external circuit uses... 
- A three digit, common cathode, 7 segment display. A common annode part would have been better, but this is what I had and the code assumes.
- 8x current limiting resistors for the display LEDs - I've used 1k ohm resistors for this.
- 3x BC 557 transistors for driving each digit.
- 3x current limiting reisistors for the transistors - I've used 1k ohm resistors for this.
- 1x 2N7000 MOSFET, for driving the speaker.
- 1x Rotary encoder (not mounted on a breakout, just the bare 5 pin component).
- 3x 10k ohm pull-down resistors for the rotary encoder push button, and the two encoder signals.
- 1x 10k ohm pull-down resistor for the MOSFET gate.
- 1x 10k ohm pull-up resistor for the reset pin. (Not needed if you're using an Arduino Uno or other pre-built board).
- 1x 16MHz crystal.
- 2x 22pF capacitors for the crystal.
- 1x 10uF electrolytic capacitor for the power rails.
- 1x 100uF tantalum capacitor for the power rails.
- Optional 1x LED and appropriate current limiting resistor (1k ohm?) for debug.

#Known bugs
Currently, while in the "tap" speed setting mode, the metronome sound sometimes double triggers. I haven't investigated why yet.

Phipli - August 2024
