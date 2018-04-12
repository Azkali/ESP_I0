ESP-IDF A2DP-SINK demo
======================

Demo of A2DP audio sink role

This is the demo of API implementing Advanced Audio Distribution Profile to receive an audio stream.

For the I2S codec, pick whatever chip or board works for you; this code was written using a PCM5102 chip, but other I2S boards and chips will probably work as well. The default I2S connections are shown below, but these can be changed in menuconfig:

| ESP pin   | I2S signal   |
| :-------- | :----------- |
| GPIO25    | LRCK         |
| GPIO22    | DATA         |
| GPIO26    | BCK          |

If the internal DAC is selected, analog audio will be available on GPIO25 and GPIO26. The output resolution on these pins will always be limited to 8 bit because of the internal structure of the DACs.


After the program is started, other bluetooth devices such as smart phones can discover a device named "ESP_I/0". Once a connection is established, audio data can be transmitted. This will be visible in the application log including a count of audio data packets.


ESP_I/0 Functions and Buttons
=============================

| ESP pin   | Btn-function |
| :-------- | :----------- |
| GPIO33    | Deep-sleep   |
| GPIO12    | Play/Pause   |

For the Deep-sleep button you have to use a 10kΩ pulldown resistor to the ground.

## TODO: 
```
[ ] 001: Add Play/Pause Button
[ ] 002: Add Forward/Back Buttons & Function
```