Motorcar
========

Motorcar is a framework for 3D windowing built on top of Wayland which I originally developed for my Master's thesis at Cal Poly ([pdf](https://github.com/evil0sheep/MastersThesis/blob/master/thesis.pdf?raw=true), [defense slides](https://docs.google.com/presentation/d/1svgGMxxbfmcHy_KuS5Q9hah8PQOsXqvjBKOoMIzW24Y/edit?usp=sharing)). It is designed to provide basic 3D windowing infrastructure that gives 3D applications desktop flexibility in the how their 3D content is drawn while supporting unmodified Wayland applications in the same 3D compositor space, and to do this with the simplest mechanism possible.

Motorcar is free and open source (under the BSD license), and I am very open to contributions (both conceptual and functional) from the community. If you have any questions, comments, or critical feedback about the software, or if you are interested in using or contributing Motorcar, or if you are working on something related, please feel free to contact me, I would love to hear from you.

Doxygen generated class documentation is available in the repository [here](http://htmlpreview.github.io/?https://github.com/evil0sheep/motorcar/blob/stable/doc/html/annotated.html), though it is somewhat sparse.

Building Motorcar
=================

In general if you are looking to build Motorcar you should pull the stable branch, as the master branch may periodically go into unbuildable states. Please note that the software in the stable branch is not guaranteed to *actually be* stable, but I will not knowingly commit broken code to stable.

If you have trouble building Motorcar or feedback regarding the build process please contact me. At the time of me writing these build instructions this software has not been built on another system to my knowledge, so there could be significant problems with the software or build system that I am simply unaware of.


Dependencies
------------

Motorcar has significant external dependencies, some of which may need to be built from source depending on your Linux distribution. These dependencies can be summarized at a high level as a dependency on the [QtWayland QtCompositor](http://qt-project.org/wiki/QtWayland) module with support for EGL with desktop OpenGL (not OpenGL ES). This in turn requires [Wayland](http://wayland.freedesktop.org/), which may be available as a package depending on your Linux distribution, or may need to be [built from source](http://wayland.freedesktop.org/building.html), as well as support for EGL with desktop OpenGL, which may require building Mesa from source. Instructions for building Mesa for Wayland EGL can be found in the [Wayland build instructions](http://wayland.freedesktop.org/building.html). I use the following configurations:

* Mesa

		$ autogen.sh --prefix=$WLD --enable-gles2 --disable-gallium-egl --with-egl-platforms=x11,wayland,drm --enable-gbm --enable-shared-glapi --with-gallium-drivers=r300,r600,swrast,nouveau --enable-glx-tls
	

* Cairo

		$ autogen.sh --prefix=$WLD --enable-xcb=yes --enable-gl=yes -enable-egl=yes
	


Build instructions for QtWayland can be found on the [QtWayland page](http://qt-project.org/wiki/QtWayland), and these cover most everything needed to get build QtWayland. Getting it to work properly with EGL and desktop OpenGL is a little bit tricky, and I have copied my build command below for reference, which I imagine will probably work on most systems, though this is of course not guaranteed. 

Below are the hashes of the commits which I am currently using for the Qt dependencies, which I include mainly for reference since they are known to work together. Other combinations may work as well. Please note that qtbase and qtwayland are git submodules of the qt5 repository.

* Qt5: 
	* e198c124d3259dea657fcfa4c9b9b43bcd2d9fd0
	* The most recent commit might work here but if the version exceeds 5.3.0 you may run into problems with the QtWayland private includes
* qtbase 
	* 625002f7067271b8f03f7bfa13baff6128c72e68
	* The most recent commit will probably work here
* qtwayland
	* 5c605d363e2fc42f5ec80413d093b69614027da4
	* This is the most likely to cause problems as the Compositor API changes quite frequently.

Here is the build sequence I use for Qt5 and QtWayland to get them to support EGL with desktop OpenGL. Again, this is certainly not guaranteed to work on every system, it is included here mainly as a reference.  Please refer to the [Qt5 build instructions](http://qt-project.org/wiki/Building_Qt_5_from_Git) and the [QtWayland build instructions](http://qt-project.org/wiki/QtWayland) for more information.

	$ git clone git://gitorious.org/qt/qt5.git qt5
	$ cd qt5
	$ git checkout e198c124d3259dea657fcfa4c9b9b43bcd2d9fd0
	$ ./init-repository --no-webkit  --module-subset=qtbase,qtjsbackend,qtdeclarative,qtwayland

Other modules may be required to run some of the Qt clients, but not for Motorcar itself

	$ cd qtbase
	$ git checkout 625002f7067271b8f03f7bfa13baff6128c72e68
	$ cd ../qtwayland
	$ git checkout e7a1e121d9cb92e89e53baba81aceedca24f2b94
	$ cd ../
	$ ./configure -prefix /opt/qt5  -debug -confirm-license -opensource -egl -opengl  -no-xcb-xlib

I install into opt/qt5, but this is not a hard requirement, just make sure that when you run qmake it is running the executable installed here. The -no-xcb-xlib argument is required to build against EGL and desktop OpenGL in the commit listed above, but this may have been fixed in newer versions of QtWayland

	$ cd qtwayland
	$ git clean -fdx
	$ ../qtbase/bin/qmake CONFIG+=wayland-compositor
	$ cd ../
	$ make
	$ make install

If this gives you trouble you might want to try making and installing QtWayland from the qtwayland subdirectory.

Building Motorcar Itself
------------------------
Motorcar is separated into several components, which are designed to be able to be used together or independently from one another. The Wayland protocol extensions used for 3D windowing and 3D input are specified in [motorcar.xml](https://github.com/evil0sheep/motorcar/blob/stable/src/protocol/motorcar.xml) and the language bingings used by the compositor and clients are generated when those components are compiled. 

The compositor is built in two steps. The first step builds the Motorcar compositor library which contains the Wayland backend and Qt dependency, and most of the scenegraph and compositing logic. The second step builds the compositor itself, which is a lightweight program that essentially just uses the compositor library to set up the scene and insert devices into the scenegraph. 

This allows many compositors to be built with the core windowing infrastructure, and allows developers implementing compositors to add support for their devices or to replace components of the windowing logic (like the window manager) with their own classes without those classes needing to be in the core Motorcar code base (though I am very open to pull requests). It also keeps device specific dependencies out of the core compositor code.

### Building the Motorcar Compositor Library

Currently the entire compositor library is built with qmake, but eventually I would like to transition to CMake and only use qmake to build the Qt dependent components (since this is a relatively small portion of the code base). To build the motorcar compositor library:
	
	$ cd path/to/motorcar/repo
	$ qmake 
	$ make

This will build shared objects in the lib directory (under the repository root directory) which compositors will link against when using the Motorcar compositor library.

**Be sure that you are running the qmake binary that you just built.** If you configure Qt with the prefix '/opt/qt5' be sure you are running /opt/qt5/bin/qmake. Specify the full path if neccessary

### Building the Example Compositor ###

This repository contains an example compositor which uses an Oculus Rift as a 3D display and a Sixense/Razer Hydra as a 6DoF input device. Building this example compositor requires that both the OculusVR SDK and the Sixense SDK be present in the system. To build the example compositor:

	$ cd path/to/motorcar/repo
	$ cd src/examples/compositors/rift-hydra-compositor
	$ make LIBOVRPATH=/path/to/LibOVR SIXENSEPATH=/path/to/sixenseSDK_linux_OSX

This will build an executable called rift-hydra-compositor in the current directory

In the future there will likely be more compositors, and possibly some mechanism for automatically detecting which device SDKs are present and auto generating a compositor which reads in some kind of scene configuration file to control scene layout.

### Building the Motorcar Demo Client ###

This repository also contains an example client which supports the Motorcar protocol extensions for view-dependent depth-composited 3D rendering based on weston-simple-egl. Eventually this client will be broken apart into a client-side windowing library and example clients built with this library, similar to the compositor code, and when this happens these build instructions will be updated accordingly. To build the demo client

	$ cd path/to/motorcar/repo
	$ cd src/examples/clients/simple-egl/
	$ make 

This will build an executable called motorcar-demo-client in the current directory. This client takes several flags, most of which were inherited from weston-simple-egl. The -p flag enables portal clipping mode, rather than the default cuboid clipping mode, and the -d flag disables depth compositing on the 3D window. See [my thesis](https://github.com/evil0sheep/MastersThesis/blob/master/thesis.pdf?raw=true) and my [thesis defense slides](https://docs.google.com/presentation/d/1svgGMxxbfmcHy_KuS5Q9hah8PQOsXqvjBKOoMIzW24Y/edit?usp=sharing) for a conceptual explanation of what this means.






