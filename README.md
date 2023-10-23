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

Stop majestic (pre-packaged streamer) and the start venc:
```sh
killall majestic
/tmp/venc
```

### Links
* https://github.com/svpcom/wfb-ng-osd
* https://github.com/grz0zrg/fbg
