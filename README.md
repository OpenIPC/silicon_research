# goke_venc

## EN
Example how to bring up GK7205V300 using only HiMPP low level API.

This example configured and tested on IP camera with IMX335 onboard. If you want to use another sensor, you should change board configuration profiles in source code. Example contains profiles for most popular sensors.

Output video format is h264 over UDP with NAL fragmentation as in RTP with cutted NAL prefixes too. Keep in mind that this is not a valid RTP stream. To receive and display video stream extra coding needed. NAL defragmentation algorithm described in **udp_depay.c**.

### How to build
Before start, make shure that you have toolchain for you camera (given from OpenIPC). Also you need apropriate camera with OpenIPC firmware installed.
```bash
cmake -Bbuild-gk7205v300-release -DTARGET_PLATFORM=gk7205v300 -DTARGET_BUILD=Release -DTARGET_PLATFORM_TOOLCHAIN_GCC_PATH={PATH_TO}/openipc/output/host/bin
cmake --build build-gk7205v300-release
```
Before start binary on your camera, please don't forget to stop standard OpneIPC streamer - majestic.

### Known bugs
Probably you will need to add symlink /dev/venc -> /dev/ven on your IP camera.

## RU
Пример того, как запустить GK7205V300, используя только низкоуровневый API HiMPP.

Этот пример настроен и протестирован на IP-камере с IMX335 на борту. Если вы хотите использовать другой сенсор, вам следует изменить профили конфигурации платы в исходном коде. Пример содержит профили для наиболее популярных сенсоров.

Формат выходного видео - h264 по UDP с фрагментацией NAL по аналогии с RTP, но не является корректным RTP и не содержит NAL префиксы. Для приема и отображения видеопотока требуется дополнительная утилита. Алгоритм дефрагментации NAL описан в **udp_depay.c**.

### Как собрать
Перед началом убедитесь, что у вас есть набор инструментов для вашей камеры (собранный при помощи OpenIPC). Также вам потребуется соответствующая камера с установленной прошивкой OpenIPC.
``` bash
cmake -Bbuild-gk7205v300-release -DTARGET_PLATFORM=gk7205v300 -DTARGET_BUILD=Release -DTARGET_PLATFORM_TOOLCHAIN_GCC_PATH={PATH_TO}/openipc/output/host/bin
cmake --build build-gk7205v300-release
```
Перед запуском примера на камере необходимо остановить стандартный для прошивок OpenIPC стример - majestic. Если через некоторе время после его остановки камера уходит в перезагрузку - отключите WDT в конфигурационном файле маджестика.

### Известные ошибки
Пример может не запускаться на камере из-за несоответветствия имени устройства в драйвере и в HiMPP.
Лечится созданием символьной ссылки /dev/venc -> /dev/ven на вашей IP-камере.