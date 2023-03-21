# arduino

## first stop deamon:
echo "systemctl stop screen_rotation"
systemctl stop screen_rotation

## compile code:
echo "arduino-cli compile"
sudo arduino-cli compile -b esp32:esp32:esp32 motor

##import to borad:
echo "arduino-cli upload"
sudo arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 motor

## restart deamon:
sleep 2
echo "systemctl start screen_rotation"
systemctl start screen_rotation
