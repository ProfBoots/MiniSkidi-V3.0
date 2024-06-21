# setup

* https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json as Settings -> Additional Board Manager URL -> Install `2.0.14`  (do not install 3.x as is has [breaking changes](https://github.com/espressif/arduino-esp32/issues/9169))


if you are getting an
```
bash: /Users/{$USER}/Library/Arduino15/packages/esp32/tools/esptool_py/4.6/esptool: Bad CPU type in executable
exit status 126

Compilation error: exit status 126
```
* https://support.arduino.cc/hc/en-us/articles/7765785712156-Error-bad-CPU-type-in-executable-on-macOS if you are on a mac -> you need rosetta

# dependencies

* "ESP32Servo" by "Kevin Harrington" `Version 1.1.2` https://github.com/madhephaestus/ESP32Servo
* "ESPAsyncWebSrv" by "dvarrel" `Version 1.2.7` https://github.com/dvarrel/ESPAsyncWebSrv/releases
* "AsyncTCP.h" by "dvarrel" `Version which comes with ESPAsyncWebSrv` or  `AsyncTCP@1.1.4` according to the workshop
* "ESPAsyncTCP.h" by "dvarrel" `Version which comes with ESPAsyncWebSrv` or `ESPAsyncTCP@1.2.4`


# compilation / board

* Falko and Jan and [his elementary school children](https://35services.de/2024/diy-eine-3d-druck-und-elektronik-ag-an-der-ktg/#AG04) are using the USB-C board from https://de.aliexpress.com/item/1005006585256277.html which has a `wroom-32`ESP onboard. But you need to select `ESP32 Wrover Module`!

