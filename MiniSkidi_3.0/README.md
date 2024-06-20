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

```
Downloading ESPAsyncTCP@1.2.4
ESPAsyncTCP@1.2.4
Installing ESPAsyncTCP@1.2.4
Installed ESPAsyncTCP@1.2.4
Downloading ESPAsyncWebSrv@1.2.7
ESPAsyncWebSrv@1.2.7
Installing ESPAsyncWebSrv@1.2.7
Installed ESPAsyncWebSrv@1.2.7
Downloading AsyncTCP@1.1.4
AsyncTCP@1.1.4
Installing AsyncTCP@1.1.4
Installed AsyncTCP@1.1.4
```

