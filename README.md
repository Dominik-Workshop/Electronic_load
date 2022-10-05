# DC Electronic load

### Project Goals
:heavy_check_mark: **300 W power dissipation**

:heavy_check_mark: **up to 8A and 60V** (obeying the 300W limit)

:heavy_check_mark: **Remote voltage sensing option**

:heavy_check_mark: **Easy to input values:** you can use an encoder as well as a keypad

:heavy_check_mark: **Constant current mode**

:heavy_check_mark: **Constant power mode**

:heavy_check_mark: **Constant resistance mode**

**Transient response mode**

:heavy_check_mark: **Battery testing mode**

:heavy_check_mark: **Nice to look at:** doesn't look like a poor DIY build

<p align="center">
  <img src="/images/front.jpg" width="49%" />
  <img src="/images/rear.jpg" width="49%" />
  <img src="/images/insides.jpg" width="49%" />
  <img src="/images/insides2.jpg" width="49%" />
  <img src="/images/main_PCB.jpg" />
</p>

<p align="center">
  <img src="/images/load_assembled_render.png" />
  <img src="/images/insides_render.png" width="49%" />
  <img src="/images/insides_rear_render.png" width="49%" />
</p>

### To-Do:

- [ ] Software
  - [X] !!!Fix memory leak!!!
  - [X] Voltage / Current measurement
  - [ ] Transient response mode
  - [X] Battery measurement mode
  - [X] Calibration mode
  - [X] Const. power regulation
  - [X] Const. resistance regulation
  - [X] Clean the code up
- [ ] Hardware
  - [ ] Move fan speed control pin to different timer than TCCR0B

### Next version:
  - [ ] Reverse polarity protection
  - [ ] Better heat dissipation from shunts
  - [ ] Higher resolution DAC

### Uploading the code using USBASP
add following code to the platformio.ini file

    upload_protocol = custom
    upload_port = usb
    upload_flags = 
        -C
        ${platformio.packages_dir}/tool-avrdude/avrdude.conf
        -p
        $BOARD_MCU
        -P
        $UPLOAD_PORT
        -c
        usbasp
    upload_command = avrdude $UPLOAD_FLAGS -U flash:w:$SOURCE:i

Uploading the code this way will reduce the boot up time.

Note: since this method removes the bootloader, in order to program the uC via UART next time,
    you have to burn the bootloader.