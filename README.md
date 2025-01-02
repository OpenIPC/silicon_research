![OpenIPC logo][logo]

## Simple FPV
[![Telegram](https://openipc.org/images/telegram_button.svg)][telegram_fpv]

[logo]: https://openipc.org/assets/openipc-logo-black.svg
[telegram_fpv]: https://t.me/+BMyMoolVOpkzNWUy

## Encoder

Here is an example of getting video from a GK7205V300 board using only HiMPP low level API.

This example is configured and tested on an IP camera using IMX335 image sensor. 
If your module has another sensor, you need to adjust board configuration profile in the source code. 
This example includes profiles for most popular sensors.

Video output format is H.264 over UDP with NAL fragmentation like in RTP, although NAL prefixes are omitted. 
Keep in mind that this is not a valid RTP stream! 
To receive and display video stream extra coding is needed. 
The NAL defragmentation algorithm is described in **vdec-sample.c**.

## How to build
Build script usage:
```bash
./build.sh venc-goke
./build.sh venc-hisi
./build.sh vdec
```

Upload the compiled binary onto your camera:
```bash
scp -O venc/venc root@192.168.1.10:/tmp
```

Stop majestic (pre-packaged streamer) and start venc:
```sh
killall majestic
/tmp/venc
```

## Extra OSD

| Element   | Description       |
|-----------|-------------------|
| osd_ele1  | # ALT            |
| osd_ele2  | # SPD            |
| osd_ele3  | $ VSPD           |
| osd_ele4  | # BATT           |
| osd_ele5  | # CONS           |
| osd_ele6  | # CURR           |
| osd_ele7  | # THR            |
| osd_ele8  | # SATS           |
| osd_ele9  | # HDG            |
| osd_ele10 | # LAT            |
| osd_ele11 | # LON            |
| osd_ele12 | # DIST           |
| osd_ele13 | # RSSI           |
| osd_ele14 | # OpenIPC Image  |
| osd_ele15 | # RX Packets     |
| osd_ele16 | # RATE           |
| osd_ele17 | # TIME           |
| osd_ele18 | # HOR            |
| osd_ele19 | # TEMP           |



### Links
* https://github.com/svpcom/wfb-ng-osd
* https://github.com/grz0zrg/fbg
