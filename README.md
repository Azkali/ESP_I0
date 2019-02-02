ESPIO
======================
# What is ESPIO ?

ESPIO is, an OpenSource Wireless MusicPlayer, based on ESP32 (currently TTGO TM), using ESP-ADF library.

The goal is to use the most of the ESP-ADF library to turn any ESP32 into a MusicPlayer ( including several screen drivers thanks to [Bodmer TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)'s library).


ESP_I/0 Functions and Buttons
=============================

|Functions            | State |
| :--------           | :---- |
| AccessPoint         |   -   |
| AD2P-SINK           |   Ok  |
| AD2P-SRC            |   -   |
| APPFS               |   -   |
| GenericAudioHandler |   X   |
| GUIHandler          |   X   |
| AudioMeta           |   X   |
| FreeRTOS            |   ?   |
| HFP                 |   Ok  |
| HTTP OTA            |   -   |
| HTTP Serv.          |   -   |
| UI Drivers          |   Ok  |
| SD Card             |   -   |
| SPIFFS              |   -   |

```
- "-": Undefined state {Not tested/Not implemented/Needs Rework}
- "Ok": Functionnality has been tested and seems to work !
- "?": Unknownstate {Missing infos}
- "X": Does Not Compile
```

### Current I2S configuration


| ESP GPIO_NUM  | I2S signal   |
| :------------ | :----------- |
| GPIO_NUM_25   | LRCK         |
| GPIO_NUM_22   | DATA         |
| GPIO_NUM_26   | BCK          |

### Buttons Configuration

| ESP GPIO_NUM  | SHORT PRESS    | LONG_PRESS |
| :------------ | :------------- | :--------- |
| GPIO_NUM_37   | PAUSE TRACK    | PLAY TRACK |
| GPIO_NUM_38   | NEXT TRACK     | NEXT TRACK |
| GPIO_NUM_39   | PREVIOUS TRACK | NEXT TRACK |

```
Planning to change to either :
```

| ESP GPIO_NUM  | SHORT PRESS   | LONG_PRESS     |
| :------------ | :------------ | :---------     |
| GPIO_NUM_37   | PAUSE TRACK   | PLAY TRACK     |
| GPIO_NUM_38   | VOL+          | NEXT TRACK     |
| GPIO_NUM_39   | VOL-          | PREVIOUS TRACK |

```
or :
```
| ESP GPIO_NUM  | SHORT PRESS            | LONG_PRESS     |
| :------------ | :--------------------- | :------------- |
| GPIO_NUM_37   | PAUSE TRACK/PLAY TRACK |       -        |
| GPIO_NUM_38   | VOL+                   | NEXT TRACK     |
| GPIO_NUM_39   | VOL-                   | PREVIOUS TRACK |

## Sources

[ESP-ADF](https://github.com/espressif/esp-adf)
[ESP-IDF](https://github.com/espressif/esp-idf)
[TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)
[ARDUINO-ESP32](https://github.com/espressif/arduino-esp32)
[ADAFRUIT-GFX](https://github.com/adafruit/Adafruit-GFX-Library)
