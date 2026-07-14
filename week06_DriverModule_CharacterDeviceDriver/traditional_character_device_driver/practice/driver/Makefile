obj-m := rwbufdev.o

KDIR := /lib/modules/$(shell uname -r)/build

# Thêm thư mục chứa header
ccflags-y := -I$(src)/../include

all:
	$(MAKE) -C $(KDIR) M=$(CURDIR) modules

clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) clean