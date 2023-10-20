VDEC := main.c udp_stream.c vo.c \
	fbg_fbdev.c fbgraphics.c font_16x16.c lodepng/lodepng.c nanojpeg/nanojpeg.c
LIB := -lmpi -lhdmi -ljpeg -ldnvqe -lupvqe -lVoiceEngine -lm

FLAG := -Wno-address-of-packed-member -Os -s
SDK := ../sdk/hi3536dv100

vdec:
	$(CC) $(VDEC) -I $(SDK)/include -L $(DRV) $(LIB) $(FLAG) -o $@
