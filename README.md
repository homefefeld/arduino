# arduino

## first stop deamon:
systemctl stop screen_rotation

## compile code:
sudo arduino-cli compile -b esp32:esp32:esp32 motor

##import to borad:
sudo arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 motor
or 

## restart deamon:
sleep 2
echo "systemctl start screen_rotation"
systemctl start screen_rotation
