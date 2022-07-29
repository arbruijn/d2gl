OBJS = ntmap.o tmapfade.o tmap_flt.o tmap_inc.o tmap_lin.o tmap_ll.o tmap_per.o

CFLAGS += -I ../bios -I../fix -I../includes -I../3d -I../2d -I../vecmat -I../misc
CFLAGS += -Wall
CFLAGS += -m32
LDFLAGS += -m32

#all: $(OBJS)

all:


%.o: %.nas
	nasm -f elf -o $@ $^

clean:
	rm -f $(OBJS)
