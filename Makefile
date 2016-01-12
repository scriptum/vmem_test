CC     = gcc
LIBS   = -lGL -lGLU -lX11 -lm
CFLAGS = -O2
TARGET = vmem_test
#LDFLAGS += -s -lGL -lGLU -lX11 -lm -Wl,-Bstatic -static-libgcc -pie -lc

SOURCES=test.c render.c SOIL/image_DXT.c SOIL/image_helper.c SOIL/SOIL.c SOIL/stb_image_aug.c
OBJECTS=$(SOURCES:.c=.o)

all: $(SOURCES) $(TARGET)

.c.o:
	$(CC) -c $(CFLAGS) $(LIBS) $(LDFLAGS) -o $@ $<

$(TARGET): $(OBJECTS) 
	$(CC) $(OBJECTS) $(CFLAGS) $(LIBS) $(LDFLAGS) -o $@
	
clean:
	rm -f *.o */*.o $(TARGET)
