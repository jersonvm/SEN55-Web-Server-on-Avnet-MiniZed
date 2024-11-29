APP = sen55

# Add any other object files to this list below
APP_OBJS = xplatform_to_app.o
APP_OBJS += sensirion_i2c_hal.o
APP_OBJS += sensirion_i2c.o
APP_OBJS += sensirion_common.o
APP_OBJS += sen5x_i2c.o
APP_OBJS += main.o

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) -lm $(LDFLAGS) -o sen55 $(APP_OBJS) $(LDLIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<
clean:
	-rm -f $(APP) *.elf *.gdb *.o

