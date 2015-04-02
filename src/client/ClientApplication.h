/*
 * ClientApplication.h
 *
 *  Created on: Aug 3, 2014
 *      Author: dave
 */

#ifndef CLIENTAPPLICATION_H_
#define CLIENTAPPLICATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>


#include <linux/input.h>

#include <wayland-client.h>
#include <wayland-egl.h>
#include <wayland-cursor.h>
#include "motorcar-client-protocol.h"

//#include <GLES2/gl2.h>
#include <GL/gl.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/matrix_access.hpp"

#include <vector>
#include <iostream>

#ifndef EGL_EXT_swap_buffers_with_damage
#define EGL_EXT_swap_buffers_with_damage 1
typedef EGLBoolean (EGLAPIENTRYP PFNEGLSWAPBUFFERSWITHDAMAGEEXTPROC)(EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects);
#endif

#ifndef EGL_EXT_buffer_age
#define EGL_EXT_buffer_age 1
#define EGL_BUFFER_AGE_EXT			0x313D
#endif

namespace motorcar {

class ClientApplication {
public:
	ClientApplication();
	virtual ~ClientApplication();


};

} /* namespace motorcar */

#endif /* CLIENTAPPLICATION_H_ */
