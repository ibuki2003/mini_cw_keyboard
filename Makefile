SRCDIR := src
OUTDIR := bin
TARGET := $(OUTDIR)/out
HDRS := $(wildcard $(SRCDIR)/*.h) $(wildcard $(SRCDIR)/*.hpp)
SRCS := $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(addprefix $(OUTDIR)/,$(patsubst %.c,%.o,$(SRCS)))
DEPS := $(OBJS:%.o=%.d)

AVR_CHIP = atmega8
AVR_WRIT = avrisp2
AVR_PORT = /dev/ttyACM0
# 20MHz
F_CPU = 20000000UL

CC = avr-gcc
CFLAGS = -Wall -Os -mmcu=$(AVR_CHIP) -DF_CPU=$(F_CPU) \
				-I src \
				-I lib/vusb/usbdrv
ASFLAGS = $(CFLAGS)

LIBS = \
			$(OUTDIR)/lib/vusb/usbdrv/usbdrv.o \
			$(OUTDIR)/lib/vusb/usbdrv/usbdrvasm.o \
			$(OUTDIR)/lib/vusb/usbdrv/oddebug.o


$(TARGET).hex: $(OBJS) $(LIBS)
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(TARGET).elf
	avr-objcopy -F ihex $(TARGET).elf $(TARGET).hex

$(OUTDIR)/%.o: %.c $(HDRS)
	mkdir -p `dirname $@`
	$(CC) $(CFLAGS) -g -c $< -o $@

$(OUTDIR)/%.o: %.S $(HDRS)
	mkdir -p `dirname $@`
	$(CC) $(CFLAGS) -g -c $< -o $@

write: $(TARGET).hex
	avrdude -c $(AVR_WRIT) -P $(AVR_PORT) -p $(AVR_CHIP) -U flash:w:$(TARGET).hex:i

write_fuse:
	# TODO: update clock settings

	# hfuse: 0b11001001
	# lfuse: 0b11101111
	avrdude -c $(AVR_WRIT) -P $(AVR_PORT) -p $(AVR_CHIP) -U lfuse:w:0xef:m -U hfuse:w:0xc9:m

# オブジェクトファイルと実行ファイル及び HEX ファイル削除
.PHONY: clean
clean:
	rm $(OUTDIR) -rf
