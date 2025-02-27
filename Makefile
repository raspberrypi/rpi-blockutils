NAME = blockmap_img2simg

SOURCES = blockmap_img2simg.c

OBJECTS = $(SOURCES:.c=.o)

DEB_HOST_MULTIARCH ?= $(shell $(CC) -dumpmachine)

CFLAGS += -fPIE -fpie
LDFLAGS += -fPIE -pie

SPARSE_CPPFLAGS += \
  -I/usr/include/android \

SPARSE_LDFLAGS += \
  -L/usr/lib/$(DEB_HOST_MULTIARCH)/android \
  -Wl,-rpath=/usr/lib/$(DEB_HOST_MULTIARCH)/android \
  -lsparse \

.PHONY: all
all: $(NAME) validate_blockmap apply_simg

$(NAME): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS) $(SPARSE_LDFLAGS)

$(OBJECTS): %.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS) $(SPARSE_CPPFLAGS)

validate_blockmap: validate_blockmap.c
	$(CC) -o $@ $^ $(CPPFLAGS) $(CFLAGS) $(LDFLAGS)

apply_simg: apply_simg.c
	$(CC) -o $@ $^ $(CPPFLAGS) $(SPARSE_CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(SPARSE_LDFLAGS)

.PHONY: clean
clean:
	rm -f *.o blockmap_img2simg validate_blockmap apply_simg
