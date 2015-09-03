MOTORCAR_DIR=../../../..
CC=g++

#  @@JAF - Personalized the sdk dirs
#LIBOVRPATH=/home/thegreatpissant/src/OculusSDKv2.3/LibOVR
#LIBOVRPATH=/home/thegreatpissant/src/OculusSDKv0.1.5/LibOVR
#LIBOVRPATH=/home/thegreatpissant/src/OculusSDKv0.1.4/LibOVR
#LIBOVRPATH=/home/thegreatpissant/src/OculusSDKv0.1.3/LibOVR
#LIBOVRPATH=${MOTORCAR_DIR}/../thirdPartySource/Oculus/OculusSDK/LibOVR
#  @@JAF - end
OCULUS_INCLUDES= -I${LIBOVRPATH}/Include -I${LIBOVRPATH}/Src
#OCULUS_INCLUDES= -I${LIBOVRPATH}

OCULUS_LIBS= -L${LIBOVRPATH}/Lib/Linux/x86_64/Debug -L${LIBOVRPATH}/Lib/Linux/x86_64/Release -lOVR -ldl


SIXENSEPATH=/home/thegreatpissant/src/sixenseSDK_linux_OSX
#SIXENSEPATH=${MOTORCAR_DIR}/../thirdPartySource/sixenseSDK_linux_OSX
SIXENSE_INCLUDES= -I${SIXENSEPATH}/include
SIXENSE_LIBS= -L${SIXENSEPATH}/lib/linux_x64/release -lsixense_utils_x64 -lsixense_x64 


INCLUDES= -I${MOTORCAR_DIR}/src/compositor -I${MOTORCAR_DIR}/src/protocol -I${MOTORCAR_DIR}/src/device ${OCULUS_INCLUDES} ${SIXENSE_INCLUDES}
LIBS= -L${MOTORCAR_DIR}/lib -lmotorcar-compositor -ludev -lpthread -lGL -lX11 -lXinerama
CFLAGS=-std=c++11 -g
objects=ovr_0_5_0_1_hmd.o sixensemotionsensingsystem.o sixensecontrollernode.o sixensebasenode.o 

compositor: main.cpp sixense $(objects) 
	${CC}  main.cpp $(objects) ${CFLAGS} ${INCLUDES} ${OCULUS_LIBS} ${SIXENSE_LIBS} ${LIBS} -o rift-hydra-compositor 

ovr_0_5_0_1_hmd.o: ${MOTORCAR_DIR}/src/device/ovr_0_5_0_1_hmd.cpp ${MOTORCAR_DIR}/src/device/ovr_0_5_0_1_hmd.h
	${CC} -c ${MOTORCAR_DIR}/src/device/ovr_0_5_0_1_hmd.cpp ${CFLAGS} ${INCLUDES}  ${OCULUS_LIBS} ${LIBS}

SIXENSE_HEADERS=${MOTORCAR_DIR}/src/device/sixensemotionsensingsystem.h \
				${MOTORCAR_DIR}/src/device/sixensecontrollernode.h \
				${MOTORCAR_DIR}/src/device/sixensebasenode.h 

SIXENSE_SOURCES=${MOTORCAR_DIR}/src/device/sixensemotionsensingsystem.cpp \
				${MOTORCAR_DIR}/src/device/sixensecontrollernode.cpp \
				${MOTORCAR_DIR}/src/device/sixensebasenode.cpp 

sixense: ${SIXENSE_HEADERS} ${SIXENSE_SOURCES}
	${CC} -c ${SIXENSE_SOURCES} ${CFLAGS} ${INCLUDES} ${SIXENSE_LIBS} ${LIBS}


clean:
	rm rift-hydra-compositor $(objects)
