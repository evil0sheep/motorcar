#!/usr/bin/env bash
function check_error {
	if [ "$1" -ne 0 ]; then exit 1; fi

}
	
function build-qtwayland {
    echo "building qtwayland dependency"
    QT_SRC_DIR="$DEPEND_DIR/qt5"
    QT_BUILD_DIR="$BUILD_DIR/qt5"
    if [ ! -d  "$QT_SRC_DIR" ]
	then
	  	echo "qt5 path: $QT_SRC_DIR"
	  	git clone git://code.qt.io/qt/qt5.git $QT_SRC_DIR
		check_error $?
		cd $QT_SRC_DIR
		git checkout 5.5
		$QT_SRC_DIR/init-repository --no-webkit  --module-subset=qtbase,qtdeclarative,qtwayland
		check_error $?

	fi

	#Reset Repositories
	cd $QT_SRC_DIR
	git clean -fdx
	check_error $?
	git checkout 5.5
	check_error $?


	cd $QT_SRC_DIR/qtbase
	git clean -fdx
	check_error $?
	git checkout 5.5
	check_error $?
	
	cd $QT_SRC_DIR/qtwayland
	git clean -fdx
	check_error $?
	git checkout 5.5
	check_error $?
	
	#Configure

	cd $QT_SRC_DIR
	./configure -prefix $QT_BUILD_DIR  -debug -confirm-license -opensource -egl -opengl -no-xcb-xlib
	check_error $?
	echo "Please verify that EGL on Desktop OpenGL is configured correctly"
    read -p "Good to Continue? [y/n] " -n 1 -r
	if [[ ! $REPLY =~ ^[Yy]$ ]]
	then
	    exit 1
	fi


	cd $QT_SRC_DIR/qtwayland
	$QT_SRC_DIR/qtbase/bin/qmake CONFIG+=wayland-compositor
	check_error $?
	echo "Please verify that Wayland EGL is present"
	read -p "Good to Continue? [y/n] " -n 1 -r
	if [[ ! $REPLY =~ ^[Yy]$ ]]
	then
	    exit 1
	fi

	cd $QT_SRC_DIR
	make -j4
	check_error $?

    make -j4 install
    check_error $?
}

function build-libmotorcar-compositor {
    echo "building libmotorcar-compositor"
    cd $MOTORCAR_DIR
    $BUILD_DIR/qt5/bin/qmake
	check_error $?
    make clean
	check_error $?
    make
	check_error $?
}

function build-rift-hydra-compositor {
    echo "building rift-hydra-compositor"

    OVR_SDK_NAME="ovr_sdk_linux_0.5.0.1"
    OVR_SDK_ARCHIVE_NAME="$OVR_SDK_NAME.tar.xz"
    OVR_SDK_DIR="$DEPEND_DIR/$OVR_SDK_NAME"
    if [ ! -d "$OVR_SDK_DIR" ]
	then
	    cd $DEPEND_DIR
	    echo "Enter path to $OVR_SDK_ARCHIVE_NAME"
	    echo "(Download from https://developer.oculus.com/downloads/pc/0.5.0.1-beta/Oculus_SDK_for_Linux_(Experimental)/ )"
	    echo "(e.g. ~/Downloads/$OVR_SDK_ARCHIVE_NAME )"
		read -p "> " OVR_SDK_TAR
		OVR_SDK_TAR="${OVR_SDK_TAR/#\~/$HOME}"
		cp $OVR_SDK_TAR "$DEPEND_DIR/$OVR_SDK_ARCHIVE_NAME"
		check_error $?
		cd $DEPEND_DIR
		tar -xvf "$DEPEND_DIR/$OVR_SDK_ARCHIVE_NAME"
		check_error $?
		rm "$DEPEND_DIR/$OVR_SDK_ARCHIVE_NAME"
		check_error $?
	fi 

	SIXENSE_SDK_NAME="sixenseSDK_linux_OSX"
	SIXENSE_SDK_ARCHIVE_NAME="$SIXENSE_SDK_NAME.zip"
	SIXENSE_SDK_DIR="$DEPEND_DIR/$SIXENSE_SDK_NAME"
	if [ ! -d "$SIXENSE_SDK_DIR" ]
	then
		cd $DEPEND_DIR
	    echo "Enter path to $SIXENSE_SDK_ARCHIVE_NAME"
	    echo "(download from http://sixense.com/linuxsdkdownload )"
	    echo "(e.g. ~/Downloads/$SIXENSE_SDK_ARCHIVE_NAME )"
		read -p "> " SIXENSE_SDK_ZIP
		SIXENSE_SDK_ZIP="${SIXENSE_SDK_ZIP/#\~/$HOME}"
		cp $SIXENSE_SDK_ZIP "$DEPEND_DIR/$SIXENSE_SDK_ARCHIVE_NAME"
		check_error $?
	    cd $DEPEND_DIR
		unzip $DEPEND_DIR/$SIXENSE_SDK_ARCHIVE_NAME
		check_error $?
		rm $DEPEND_DIR/$SIXENSE_SDK_ARCHIVE_NAME
		check_error $?

	fi 

	RIFT_HYDRA_COMPOSITOR_DIR="$MOTORCAR_DIR/src/examples/compositors/rift-hydra-compositor"
	cd $RIFT_HYDRA_COMPOSITOR_DIR
	check_error $?
	make clean
	check_error $?
	make  LIBOVRPATH=$OVR_SDK_DIR/LibOVR SIXENSEPATH=$SIXENSE_SDK_DIR
	check_error $?

	cd $MOTORCAR_DIR
	RIFT_HYDRA_COMPOSITOR_SCRIPT="$MOTORCAR_DIR/run-rift-hydra-compositor.sh"
	echo "#!/usr/bin/env bash" > $RIFT_HYDRA_COMPOSITOR_SCRIPT
	echo "LD_LIBRARY_PATH=$MOTORCAR_DIR/lib:$OVR_SDK_DIR/LibOVR/Lib/Linux/x86_64/Debug:$SIXENSE_SDK_DIR/lib/linux_x64/release:$LD_LIBRARY_PATH $RIFT_HYDRA_COMPOSITOR_DIR/rift-hydra-compositor" > $RIFT_HYDRA_COMPOSITOR_SCRIPT
	chmod ug+x $RIFT_HYDRA_COMPOSITOR_SCRIPT


}

function build-simple-compositor {
    echo "building simple-compositor"
    SIMPLE_COMPOSITOR_DIR="$MOTORCAR_DIR/src/examples/compositors/simple-compositor"
	cd $SIMPLE_COMPOSITOR_DIR
	check_error $?
	make clean
	check_error $?
	make 
	check_error $?

	cd $MOTORCAR_DIR
	SIMPLE_COMPOSITOR_SCRIPT="$MOTORCAR_DIR/run-simple-compositor.sh"
	echo "#!/usr/bin/env bash" > $SIMPLE_COMPOSITOR_SCRIPT
	echo "LD_LIBRARY_PATH=$MOTORCAR_DIR/lib:$LD_LIBRARY_PATH $SIMPLE_COMPOSITOR_DIR/simple-compositor" > $SIMPLE_COMPOSITOR_SCRIPT
	chmod ug+x $SIMPLE_COMPOSITOR_SCRIPT

}

function build-motorcar-demo-client {
    echo "building motorcar-demo-client"
   	DEMO_CLIENT_DIR="$MOTORCAR_DIR/src/examples/clients/simple-egl"
   	cd $DEMO_CLIENT_DIR
	check_error $?
	make 
	check_error $?

	cd $MOTORCAR_DIR
	DEMO_CLIENT_SCRIPT="$MOTORCAR_DIR/run-motorcar-demo-client.sh"
	echo "#!/usr/bin/env bash" > $DEMO_CLIENT_SCRIPT
	echo "$DEMO_CLIENT_DIR/motorcar-demo-client" > $DEMO_CLIENT_SCRIPT
	chmod ug+x $DEMO_CLIENT_SCRIPT

}

MOTORCAR_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

echo "Script directory rooted at $MOTORCAR_DIR"


DEPEND_DIR="$MOTORCAR_DIR/dependencies"
BUILD_DIR="$DEPEND_DIR/build"

if [ ! -d "$DEPEND_DIR" ]
then
  mkdir -p "$DEPEND_DIR"
  mkdir -p "$BUILD_DIR"
fi 


for arg in "$@"
do

	case "$arg" in
        qtwayland)
			build-qtwayland
            ;;
        libmotorcar-compositor)
			build-libmotorcar-compositor 
            ;;
        rift-hydra-compositor)
			build-rift-hydra-compositor
            ;;
        simple-compositor)
			build-simple-compositor
            ;;
        motorcar-demo-client)
			build-motorcar-demo-client
			;;
        *)
			echo "Invalid target: $arg"
			echo "This script accepts the following targets: qtwayland libmotorcar-compositor rift-hydra-compositor simple-compositor motorcar-demo-client"
            echo "Usage: $0 [target1 [target2 [...]]]"
            exit 1
            ;;
 
	esac
done