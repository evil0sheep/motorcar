MOTORCAR_PROTOCOL_XML=../../protocol/motorcar.xml
LIBS= -lm -lwayland-client -lwayland-egl -lwayland-cursor -lGL -lEGL 


simple-egl: motorcar-protocol
	g++ -std=c++0x -g simple-egl.c motorcar-wayland-extensions.o $(LIBS) 

motorcar-protocol:
	wayland-scanner client-header < $(MOTORCAR_PROTOCOL_XML) > motorcar-client-protocol.h
	wayland-scanner server-header < $(MOTORCAR_PROTOCOL_XML) > motorcar-server-protocol.h
	wayland-scanner code < $(MOTORCAR_PROTOCOL_XML) > motorcar-wayland-extensions.c
	gcc -c -Wall -Werror -fpic motorcar-wayland-extensions.c