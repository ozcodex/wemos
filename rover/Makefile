
all: upload

compile:
	arduino-cli compile --export-binaries --verbose --fqbn esp8266:esp8266:d1_mini_pro rover.ino
	
upload:
	arduino-cli compile --upload --port /dev/ttyUSB0 --verbose --fqbn esp8266:esp8266:d1_mini_pro rover.ino

