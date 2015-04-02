MOTORCAR_PROTOCOL_XML=../protocol/motorcar.xml
LIBS= -lm -lwayland-client -lwayland-egl -lwayland-cursor -DGL_GLEXT_PROTOTYPES -lGL -lEGL 

CC=g++
OBJECTS=ClientApplication.o motorcar-wayland-extensions.o
CFLAGS=-fPIC -Wall -Werror

motorcar-client: ${OBJECTS} 
	${CC} ${CFLAGS} -shared -Wl,-soname,libmotorcar-client.so -o libmotorcar-client.so ${OBJECTS}
	
ClientApplication.o: ClientApplication.cpp ClientApplication.h motorcar-wayland-extensions
	${CC} ${CFLAGS} -c ClientApplication.cpp
	
motorcar-wayland-extensions:
	wayland-scanner client-header < $(MOTORCAR_PROTOCOL_XML) > motorcar-client-protocol.h
	wayland-scanner code < $(MOTORCAR_PROTOCOL_XML) > motorcar-wayland-extensions.c
	gcc -c -Wall -Werror -fpic motorcar-wayland-extensions.c
	
clean:
	rm -rf *.o *.so motorcar-wayland-extensions.c motorcar-client-protocol.h