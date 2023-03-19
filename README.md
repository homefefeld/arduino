# arduino

## first stop deamon:
systemctl stop screen_rotation

## compile code:
arduino-cli compile -b adafruit:esp32:feather32 motor
arduino-cli compile -b adafruit:samd:adafruit_feather_m0 motor
sudo arduino-cli compile -b esp32:esp32:esp32 motor

##import to borad:
sudo arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 motor
or 
sudo arduino-cli upload -p /dev/ttyUSB1 --fqbn esp32:esp32:esp32 motor

## restart deamon:
sleep 2
echo "systemctl start screen_rotation"
systemctl start screen_rotation
