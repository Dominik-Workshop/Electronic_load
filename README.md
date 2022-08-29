# DC Electronic load

Has 5 modes of operation:
- constant current
- constant power
- constant resistance
- transient response
- battery capacity

<p align="center">
  <img src="/images/load_assembled.png" />
  <img src="/images/insides.png" />
  <img src="/images/insides_rear.png" />
</p>

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