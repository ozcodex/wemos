# wemos
Code proyect for wemos device using arduino-cli and esptool

# Make it works on terminal

To work wit arduino in the terminal just install the arduino-cli.

# Working with D1 mini pro

add the wemos d1 mini pro board to the arduino cli:

- `arduino-cli config set board_manager.additional_urls http://arduino.esp8266.com/stable/package_esp8266com_index.json`
- `arduino-cli core update-index`
- `arduino-cli core install esp8266:esp8266`

Compile:

`arduino-cli compile --fqbn esp8266:esp8266:d1_mini_pro D1_rover.ino`
