# Project 03 - Love-o-Meter

## Connecting to the Arduino

The following project can output some information over a serial connection between the Arduino and whatever computer it's connected to. You will, however, need software like `cu` in order to "call up" the Arduino.

Once installed, run the following command to connect to the device:

```bash
$ cu -l /dev/ttyACM0 -s 9600
```

**Note 1**: It may be necessary to run this command with root permissions.

**Note 2**: Your Arduino may not be located at `/dev/ttyACM0`. To find the correct location of your device, run `ls /dev/ | grep ACM`.

To disconnect from the Arduino, type `~.`.
