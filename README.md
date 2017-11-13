# espmotion

Simple program for esp motion detection using MQTT

Start with an ESP07 or ESP12 on one of those minimalist (white) carrier boards with the 3.3 volt regulator on the back.

Use a cheapo ebay motion detector module (the kind for arduino, etc). Motion detector + and - are connected directly to 4xAA battery back. ESP carrier v-reg in and ground are also connected to the battery pack.

Now, grab a cheapo NPN transistor (2n3904 in my case.)
Motion detector output connected to base with a 4700 omh resistor (value not important.)
Collector connected to esp8266 RST pin.
Emitter connected to ground.

Program will boot up using static IP, connect to wifi and send a motion detected message via mqtt, it will run for 30 seconds to wait for over-the-air firmware update, and then go to sleep.

Motion detector will see motion and reset the ESP, waking it up from sleep and the process starts over again.

Fiddle with the motion detector so it's not resetting the ESP too often or it will never get a chance to actually send its message via mqtt.

