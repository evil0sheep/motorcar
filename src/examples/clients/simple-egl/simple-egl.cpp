/*
 * Copyright © 2011 Benjamin Franzke
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of the copyright holders not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no representations
 * about the suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

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

struct window;
struct seat;

struct viewport{
	int32_t x;
	int32_t y;
	uint32_t width;
	uint32_t height;
};

struct viewpoint{
	motorcar_viewpoint *handle;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	struct viewport colorViewport;
	struct viewport depthViewport;

};


struct display {
	struct wl_display *display;
	struct wl_registry *registry;
	struct wl_compositor *compositor;
	struct wl_shell *shell;
	struct wl_seat *seat;
	struct wl_pointer *pointer;
	struct wl_touch *touch;
	struct wl_keyboard *keyboard;
	struct wl_shm *shm;
	struct wl_cursor_theme *cursor_theme;
	struct wl_cursor *default_cursor;
	struct wl_surface *cursor_surface;
	struct {
		EGLDisplay dpy;
		EGLContext ctx;
		EGLConfig conf;
	} egl;
	struct window *window;

	PFNEGLSWAPBUFFERSWITHDAMAGEEXTPROC swap_buffers_with_damage;

	struct motorcar_shell *motorshell;
	std::vector<struct viewpoint *> viewpoints;
};


struct geometry {
	int width, height;
};

class Box {
public:
	glm::vec3 size;
	glm::mat4 transform, grabTransform;
	bool isGrabbed;
	void draw(struct window *window, std::vector<struct viewpoint *> &viewpoints, uint32_t time);


};

class sixDofPointer{
public:
	glm::mat4 transform;
};

struct window {
	struct display *display;
	struct geometry geometry, window_size;
	struct {
		GLuint rotation_uniform;
		GLuint pos;
		GLuint col;
		GLuint vertices, colors, indices;
		GLuint frameBuffer, depthBuffer;
		GLuint colorBufferTexture, depthBufferTexture;
		GLuint drawProgram, colorBlitProgram, depthBlitProgram, clipProgram;
		GLuint textureBlitVertices, textureBlitTextureCoords;
		GLuint h_aPosition, h_aTexCoord, h_aPosition_clipping, h_uMVPMatrix_clipping;
		GLuint cuboidClippingVertices, cuboidClippingIndices;
	} gl;

	uint32_t benchmark_time, frames;
	struct wl_egl_window *native;
	struct wl_surface *surface;
	struct wl_shell_surface *shell_surface;
	EGLSurface egl_surface;
	struct wl_callback *callback;
	int fullscreen, configured, opaque, buffer_size, frame_sync;

	struct motorcar_surface *motorcar_surface;
	glm::vec3 dimensions;
	glm::mat4 transformMatrix;
	wl_array dimensions_array;
	enum motorcar_surface_clipping_mode clipping_mode;
	bool depthCompositingEnabled;

	Box m_box;
	sixDofPointer m_sixDofPointer;

};

static const char *vert_shader_text =
	"uniform mat4 rotation;\n"
	"attribute vec3 pos;\n"
	"attribute vec3 color;\n"
	"varying vec4 v_color;\n"
	"void main() {\n"
	"  gl_Position = rotation * vec4(pos, 1);\n"
	"  v_color = vec4(color, 1);\n"
	"}\n";

static const char *frag_shader_text =
	"varying vec4 v_color;\n"
	"void main() {\n"
	"  gl_FragColor = v_color;\n"
	"}\n";

static const char *blit_vert_shader_text =
	"attribute vec3 aPosition;\n"
	"attribute vec2 aTexCoord;\n"
	"varying vec2 vTexCoord;\n"
	"void main() {\n"
	"  gl_Position = vec4(aPosition, 1);\n"
	"  vTexCoord = aTexCoord;\n"
	"}\n";

static const char *blit_frag_shader_text =
	"varying vec2 vTexCoord;\n"
	"uniform sampler2D uTexSampler;\n"
	"void main() {\n"
	"  gl_FragColor = texture2D(uTexSampler, vTexCoord);\n"
	"}\n";


static const char *blit_depth_frag_shader_text =
	"varying vec2 vTexCoord;\n"
	"uniform sampler2D uTexSampler;\n"
	"vec4 pack_depth(const in float depth)"
	"{"
    	"vec4 enc = vec4(1.0, 255.0, 65025.0, 160581375.0) * depth;"
    	"enc = fract(enc);"
    	"enc -= enc.yzww * vec4(1.0/255.0,1.0/255.0,1.0/255.0,0.0);"
    	"return enc;"
	"}"
	"void main() {\n"
	"  gl_FragColor =  pack_depth(texture2D(uTexSampler, vTexCoord).r);\n"
	"}\n";

static const char *clip_vert_shader_text =
	"attribute vec3 aPosition;\n"
	"uniform mat4 uMVPMatrix;\n"
	"void main() {\n"
	"  gl_Position = uMVPMatrix * vec4(aPosition, 1);\n"
	"}\n";

static const char *clip_frag_shader_text =
	"void main() {\n"
	"  gl_FragColor = vec4(1, 0, 0, 1);\n"
	"}\n";

static int running = 1;

static void
init_egl(struct display *display, struct window *window)
{
	static const EGLint context_attribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	const char *extensions;

	EGLint config_attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RED_SIZE, 1,
		EGL_GREEN_SIZE, 1,
		EGL_BLUE_SIZE, 1,
		EGL_DEPTH_SIZE, 1,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
		EGL_NONE
	};

	EGLint major, minor, n, count, i, size;
	EGLConfig *configs;
	EGLBoolean ret;

	if (window->opaque || window->buffer_size == 16)
		config_attribs[9] = 0;

	display->egl.dpy = eglGetDisplay(display->display);
	assert(display->egl.dpy);

	ret = eglInitialize(display->egl.dpy, &major, &minor);
	assert(ret == EGL_TRUE);
	ret = eglBindAPI(EGL_OPENGL_API);
	assert(ret == EGL_TRUE);

	if (!eglGetConfigs(display->egl.dpy, NULL, 0, &count) || count < 1)
		assert(0);

	configs = (EGLConfig *) calloc(count, sizeof (*configs));
	assert(configs);

	ret = eglChooseConfig(display->egl.dpy, config_attribs,
			      configs, count, &n);
	assert(ret && n >= 1);

	for (i = 0; i < n; i++) {
		eglGetConfigAttrib(display->egl.dpy,
				   configs[i], EGL_BUFFER_SIZE, &size);
		if (window->buffer_size == size) {
			display->egl.conf = configs[i];
			break;
		}
	}
	free(configs);
	if (display->egl.conf == NULL) {
		fprintf(stderr, "did not find config with buffer size %d\n",
			window->buffer_size);
		exit(EXIT_FAILURE);
	}

	display->egl.ctx = eglCreateContext(display->egl.dpy,
					    display->egl.conf,
					    EGL_NO_CONTEXT, context_attribs);
	assert(display->egl.ctx);

	display->swap_buffers_with_damage = NULL;
	extensions = eglQueryString(display->egl.dpy, EGL_EXTENSIONS);
	if (extensions &&
	    strstr(extensions, "EGL_EXT_swap_buffers_with_damage") &&
	    strstr(extensions, "EGL_EXT_buffer_age"))
		display->swap_buffers_with_damage =
			(PFNEGLSWAPBUFFERSWITHDAMAGEEXTPROC)
			eglGetProcAddress("eglSwapBuffersWithDamageEXT");

	if (display->swap_buffers_with_damage)
		printf("has EGL_EXT_buffer_age and EGL_EXT_swap_buffers_with_damage\n");

}

static void
fini_egl(struct display *display)
{
	eglTerminate(display->egl.dpy);
	eglReleaseThread();
}

static GLuint
create_shader(struct window *window, const char *source, GLenum shader_type)
{
	GLuint shader;
	GLint status;

	shader = glCreateShader(shader_type);
	assert(shader != 0);

	glShaderSource(shader, 1, (const char **) &source, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!status) {
		char log[1000];
		GLsizei len;
		glGetShaderInfoLog(shader, 1000, &len, log);
		fprintf(stderr, "Error: compiling %s: %*s\n",
			shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment",
			len, log);
		exit(1);
	}

	return shader;
}

static void
init_gl(struct window *window)
{
	GLuint frag, vert;
	GLuint program;
	GLint status;

	    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);
	

    //draw shaders

	frag = create_shader(window, frag_shader_text, GL_FRAGMENT_SHADER);
	vert = create_shader(window, vert_shader_text, GL_VERTEX_SHADER);

	window->gl.drawProgram = glCreateProgram();
	glAttachShader(window->gl.drawProgram, frag);
	glAttachShader(window->gl.drawProgram, vert);
	glLinkProgram(window->gl.drawProgram);

	glGetProgramiv(window->gl.drawProgram, GL_LINK_STATUS, &status);
	if (!status) {
		char log[1000];
		GLsizei len;
		glGetProgramInfoLog(window->gl.drawProgram, 1000, &len, log);
		fprintf(stderr, "Error: linking:\n%*s\n", len, log);
		exit(1);
	}

	window->gl.pos = 0;
	window->gl.col = 1;

	window->gl.pos = glGetAttribLocation(window->gl.drawProgram, "pos");
	window->gl.col = glGetAttribLocation(window->gl.drawProgram,  "color");
	window->gl.rotation_uniform = glGetUniformLocation(window->gl.drawProgram, "rotation");



	//texture blit shaders

	if(window->gl.pos < 0 || window->gl.col < 0 || window->gl.rotation_uniform < 0){
       std::cout << "problem with draw shader handles: "
                 << window->gl.pos
                 << ", "<< window->gl.col
                 << ", "<< window->gl.rotation_uniform
                 << std::endl;
    }


	frag = create_shader(window, blit_frag_shader_text, GL_FRAGMENT_SHADER);
	vert = create_shader(window, blit_vert_shader_text, GL_VERTEX_SHADER);

	window->gl.colorBlitProgram = glCreateProgram();
	glAttachShader(window->gl.colorBlitProgram, frag);
	glAttachShader(window->gl.colorBlitProgram, vert);
	glLinkProgram(window->gl.colorBlitProgram);

	glGetProgramiv(window->gl.colorBlitProgram, GL_LINK_STATUS, &status);
	if (!status) {
		char log[1000];
		GLsizei len;
		glGetProgramInfoLog(window->gl.colorBlitProgram, 1000, &len, log);
		fprintf(stderr, "Error: linking:\n%*s\n", len, log);
		exit(1);
	}


	window->gl.h_aPosition =  glGetAttribLocation(window->gl.colorBlitProgram, "aPosition");
	window->gl.h_aTexCoord =  glGetAttribLocation(window->gl.colorBlitProgram, "aTexCoord");

	
	if(window->gl.h_aPosition < 0 || window->gl.h_aTexCoord < 0){
       std::cout << "problem with texture blitter shader handles: "
                 << window->gl.h_aPosition
                 << ", "<< window->gl.h_aTexCoord
                 << std::endl;
    }


	frag = create_shader(window, blit_depth_frag_shader_text, GL_FRAGMENT_SHADER);
	

    window->gl.depthBlitProgram = glCreateProgram();
	glAttachShader(window->gl.depthBlitProgram, frag);
	glAttachShader(window->gl.depthBlitProgram, vert);
	glLinkProgram(window->gl.depthBlitProgram);

	glGetProgramiv(window->gl.depthBlitProgram, GL_LINK_STATUS, &status);
	if (!status) {
		char log[1000];
		GLsizei len;
		glGetProgramInfoLog(window->gl.depthBlitProgram, 1000, &len, log);
		fprintf(stderr, "Error: linking:\n%*s\n", len, log);
		exit(1);
	}


	//clipping shader

	frag = create_shader(window, clip_frag_shader_text, GL_FRAGMENT_SHADER);
	vert = create_shader(window, clip_vert_shader_text, GL_VERTEX_SHADER);

	window->gl.clipProgram = glCreateProgram();
	glAttachShader(window->gl.clipProgram, frag);
	glAttachShader(window->gl.clipProgram, vert);
	glLinkProgram(window->gl.clipProgram);

	glGetProgramiv(window->gl.clipProgram, GL_LINK_STATUS, &status);
	if (!status) {
		char log[1000];
		GLsizei len;
		glGetProgramInfoLog(window->gl.clipProgram, 1000, &len, log);
		fprintf(stderr, "Error: linking:\n%*s\n", len, log);
		exit(1);
	}

	window->gl.h_aPosition_clipping =  glGetAttribLocation(window->gl.clipProgram, "aPosition");
	window->gl.h_uMVPMatrix_clipping = glGetUniformLocation(window->gl.clipProgram, "uMVPMatrix");

	
	if(window->gl.h_aPosition_clipping < 0 || window->gl.h_uMVPMatrix_clipping < 0){
       std::cout << "problem with texture blitter shader handles: "
                 << window->gl.h_aPosition_clipping
                 << ", "<< window->gl.h_uMVPMatrix_clipping
                 << std::endl;
    }
	


	static const GLfloat verts[8][3]= {
		{ 0.5f, 0.5f , 0.5f},
		{ 0.5f, 0.5f , -0.5f},
		{ 0.5f, -0.5f , 0.5f},
		{ 0.5f, -0.5f , -0.5f},
		{ -0.5f, 0.5f , 0.5f},
		{ -0.5f, 0.5f , -0.5f},
		{ -0.5f, -0.5f , 0.5f},
		{ -0.5f, -0.5f , -0.5f}
	};

	static const GLfloat colors[8][3] = {
	 	{ 0.0f, 0.0f, 0.0f },
	 	{ 0.0f, 0.0f, 1.0f },
	 	{ 0.0f, 1.0f, 0.0f },
	 	{ 0.0f , 1.0f, 1.0f },
	 	{ 1.0f, 0.0f, 0.0f },
	 	{ 1.0f, 0.0f, 1.0f },
	 	{ 1.0f, 1.0f, 0.0f },
	 	{ 1.0f, 1.0f, 1.0f },
	};

	// static const GLfloat colors[8][3] = {
	//  	{ 0.0, 1, 0 },
	//  	{ 0.0, 1, 0 },
	//  	{ 0.0, 1, 0 },
	//  	{ 0.0, 1, 0 },
	//  	{ 0.0, 1, 0 },
	//  	{ 0.0, 1, 0 },
	//  	{ 0.0, 1, 0 },
	//  	{ 0.0, 1, 0 },

	// };
	static const GLuint indices[12][3] = {
        { 0, 2, 1 },
        { 1, 2, 3 },
        { 4, 5, 6 },
        { 5, 7, 6 },
        { 0, 1, 4 },
        { 1, 5, 4 },
        { 2, 6, 3 },
        { 3, 6, 7 },
        { 0, 4, 2 },
        { 2, 4, 6 },
        { 1, 3, 5 },
        { 3, 7, 5 }

	};

	const GLfloat textureBlitVertices[] ={
       -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
       -1.0f,  1.0f, 0.0f
    };

	glGenBuffers(1, &window->gl.vertices);
    glBindBuffer(GL_ARRAY_BUFFER, window->gl.vertices);
    glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(float), verts, GL_STATIC_DRAW);

    glGenBuffers(1, &window->gl.colors);
    glBindBuffer(GL_ARRAY_BUFFER, window->gl.colors);
    glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(float), colors, GL_STATIC_DRAW);


 	glGenBuffers(1, &window->gl.indices);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, window->gl.indices);
 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);




 	glGenBuffers(1, &window->gl.cuboidClippingVertices);
    glBindBuffer(GL_ARRAY_BUFFER, window->gl.cuboidClippingVertices);
    glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(float), verts, GL_STATIC_DRAW);

 	glGenBuffers(1, &window->gl.cuboidClippingIndices);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, window->gl.cuboidClippingIndices);
 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);




 	glGenBuffers(1, &window->gl.textureBlitVertices);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, window->gl.textureBlitVertices);
 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * 3 * sizeof(float), textureBlitVertices, GL_STATIC_DRAW);

 	glGenBuffers(1, &window->gl.textureBlitTextureCoords);


 	//Setup framebuffers for sending depth and color to compositor

 	glGenFramebuffers(1, &window->gl.frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, window->gl.frameBuffer);

    glGenTextures(1, &window->gl.colorBufferTexture);
    glBindTexture(GL_TEXTURE_2D, window->gl.colorBufferTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window->geometry.width, window->geometry.height / 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, window->gl.colorBufferTexture, 0);

    glGenTextures(1, &window->gl.depthBufferTexture);
    glBindTexture(GL_TEXTURE_2D, window->gl.depthBufferTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, window->geometry.width, window->geometry.height / 2, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, window->gl.depthBufferTexture, 0);


  	switch(glCheckFramebufferStatus(GL_FRAMEBUFFER)){
  		case(GL_FRAMEBUFFER_COMPLETE):
  			std::cout << "Framebuffer Complete" << std::endl;
  			break;
  		case(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT):
  			std::cout << "Framebuffer Attachment Incomplete" << std::endl;
  			break;
  		case(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT):
  			std::cout << "Framebuffer Attachment Incomplete/Missing" << std::endl;
  			break;
  		case(GL_FRAMEBUFFER_UNSUPPORTED):
  			std::cout << "Framebuffer Unsupported" << std::endl;
  			break;
  	}

    // glGenRenderbuffers(1, &window->gl.depthBuffer);
    // glBindRenderbuffer(GL_RENDERBUFFER, window->gl.depthBuffer);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, window->geometry.width, window->geometry.height / 2);
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, window->gl.depthBuffer);


    glEnable(GL_TEXTURE_2D);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

static void
handle_ping(void *data, struct wl_shell_surface *shell_surface,
	    uint32_t serial)
{
	wl_shell_surface_pong(shell_surface, serial);
}

static void
handle_configure(void *data, struct wl_shell_surface *shell_surface,
		 uint32_t edges, int32_t width, int32_t height)
{
	struct window *window = (struct window *) data;

	if (window->native)
		wl_egl_window_resize(window->native, width, height, 0, 0);

	window->geometry.width = width;
	window->geometry.height = height;

	printf("handle configure : %d %d\n", width, height);

	if (!window->fullscreen)
		window->window_size = window->geometry;

	glBindTexture(GL_TEXTURE_2D, window->gl.colorBufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window->geometry.width, window->geometry.height / 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, window->gl.colorBufferTexture, 0);

    glBindTexture(GL_TEXTURE_2D, window->gl.depthBufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, window->geometry.width, window->geometry.height / 2, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, window->gl.depthBufferTexture, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
}


static void
handle_popup_done(void *data, struct wl_shell_surface *shell_surface)
{
}

static const struct wl_shell_surface_listener shell_surface_listener = {
	handle_ping,
	handle_configure,
	handle_popup_done
};

static void
configure_callback(void *data, struct wl_callback *callback, uint32_t  time)
{
	struct window *window = (struct window *) data;

	wl_callback_destroy(callback);

	window->configured = 1;
}

static struct wl_callback_listener configure_callback_listener = {
	configure_callback,
};

static void
set_fullscreen(struct window *window, int fullscreen)
{
	struct wl_callback *callback;

	window->fullscreen = fullscreen;
	window->configured = 0;

	if (fullscreen) {
		wl_shell_surface_set_fullscreen(window->shell_surface,
						WL_SHELL_SURFACE_FULLSCREEN_METHOD_DEFAULT,
						0, NULL);
		callback = wl_display_sync(window->display->display);
		wl_callback_add_listener(callback,
					 &configure_callback_listener,
					 window);

	} else {
		wl_shell_surface_set_toplevel(window->shell_surface);
		handle_configure(window, window->shell_surface, 0,
				 window->window_size.width,
				 window->window_size.height);
		window->configured = 1;
	}
}


void set_window_dimensions(struct window *window, const glm::vec3 &new_dimensions){
	   
	window->dimensions = new_dimensions;

	std::memcpy(window->dimensions_array.data, glm::value_ptr(new_dimensions), window->dimensions_array.size);
	motorcar_surface_set_size_3d(window->motorcar_surface, &window->dimensions_array);
}

void motorcar_surface_handle_transform_matrix(void *data,
				 struct motorcar_surface *motorcar_surface,
				 struct wl_array *transform)
{
	if(transform->size != 16 * sizeof(float)){
		fprintf(stderr, "array from compositor is wrong size: %lu\n", transform->size);
		exit(-1);
	}
	//printf("compositor sent new 3D window transform\n");
	struct window *window = (struct window *) data;
	window->transformMatrix = glm::make_mat4((float *)transform->data);
}
	
	 
void motorcar_surface_handle_request_size_3d(void *data,
				struct motorcar_surface *motorcar_surface,
				struct wl_array *dimensions)
{
	glm::vec3 dims = glm::make_vec3((float *)(dimensions->data));
	printf("compositor requested new 3D window dimesnsions <%f %f %f>\n", dims.x, dims.y, dims.z );
	struct window *window = (struct window *) data;

	set_window_dimensions(window, dims);
}

struct motorcar_surface_listener motorsurface_listener{
	motorcar_surface_handle_transform_matrix,
	motorcar_surface_handle_request_size_3d
};

static void
create_surface(struct window *window)
{
	struct display *display = window->display;
	EGLBoolean ret;
	
	window->surface = wl_compositor_create_surface(display->compositor);
	window->shell_surface = wl_shell_get_shell_surface(display->shell,
							   window->surface);

	wl_shell_surface_add_listener(window->shell_surface,
				      &shell_surface_listener, window);

	window->native =
		wl_egl_window_create(window->surface,
				     window->window_size.width,
				     window->window_size.height);
	window->egl_surface =
		eglCreateWindowSurface(display->egl.dpy,
				       display->egl.conf,
				       window->native, NULL);

	wl_shell_surface_set_title(window->shell_surface, "simple-egl");


	ret = eglMakeCurrent(window->display->egl.dpy, window->egl_surface,
			     window->egl_surface, window->display->egl.ctx);
	assert(ret == EGL_TRUE);



	wl_array_init(&window->dimensions_array);
    wl_array_add(&window->dimensions_array, sizeof(glm::vec3));

	window->motorcar_surface =
		motorcar_shell_get_motorcar_surface(display->motorshell, window->surface, window->clipping_mode, window->depthCompositingEnabled);



	motorcar_surface_add_listener(window->motorcar_surface, &motorsurface_listener, window);


		if (!window->frame_sync)
		eglSwapInterval(display->egl.dpy, 0);

	set_fullscreen(window, window->fullscreen);

}

static void
destroy_surface(struct window *window)
{
	/* Required, otherwise segfault in egl_dri2.c: dri2_make_current()
	 * on eglReleaseThread(). */
	eglMakeCurrent(window->display->egl.dpy, EGL_NO_SURFACE, EGL_NO_SURFACE,
		       EGL_NO_CONTEXT);

	eglDestroySurface(window->display->egl.dpy, window->egl_surface);
	wl_egl_window_destroy(window->native);

	wl_shell_surface_destroy(window->shell_surface);
	wl_surface_destroy(window->surface);

	if (window->callback)
		wl_callback_destroy(window->callback);
}


void drawWindowBoundsStencil(struct window *window, struct display *display)
{
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glStencilFunc(GL_NEVER, 1, 0xFF);
    glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);



    glUseProgram(window->gl.clipProgram);

    glEnableVertexAttribArray(window->gl.h_aPosition_clipping);
    glBindBuffer(GL_ARRAY_BUFFER, window->gl.cuboidClippingVertices);
    glVertexAttribPointer(window->gl.h_aPosition_clipping, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, window->gl.cuboidClippingIndices);

    glm::mat4 modelMatrix = window->transformMatrix * glm::scale(glm::mat4(), window->dimensions);

    int numElements = 36;


    for(struct viewpoint *vp : display->viewpoints){
    	struct viewport textureViewports[] = {vp->colorViewport, vp->depthViewport};
    	for(int i = 0; i < 2; i++){

	        struct viewport textureViewport = textureViewports[i];
	        glViewport(textureViewport.x, textureViewport.y, textureViewport.width, textureViewport.height);

	       
	        glm::mat4 mvp = vp->projectionMatrix * vp->viewMatrix * modelMatrix;
	        glUniformMatrix4fv(window->gl.h_uMVPMatrix_clipping, 1, GL_FALSE, glm::value_ptr(mvp));
	        glDrawElements(GL_TRIANGLES, numElements,GL_UNSIGNED_INT, 0);
    	}
    	

    }

    glDisableVertexAttribArray(window->gl.h_aPosition_clipping);

    glUseProgram(0);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);


    glStencilMask(0x00);

    glStencilFunc(GL_EQUAL, 1, 0xFF);
}


void blitStencilBuffer(struct window *window, struct display *display){
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, window->gl.frameBuffer);

    for(struct viewpoint *vp : display->viewpoints){
    	struct viewport readViewport = vp->colorViewport;
    	struct viewport drawViewports[] = {vp->colorViewport, vp->depthViewport};

    	for(int i = 0; i < 2; i++){
	        struct viewport drawViewport = drawViewports[i];
	        glBlitFramebuffer(readViewport.x, readViewport.y, readViewport.x + readViewport.width, readViewport.y + readViewport.height, 
	        					drawViewport.x, drawViewport.y, drawViewport.x + drawViewport.width, drawViewport.y + drawViewport.height,  GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	       
    	}
    	

    }

    glStencilMask(0x00);

    glStencilFunc(GL_EQUAL, 1, 0xFF);
}

static void
redraw(void *data, struct wl_callback *callback, uint32_t time);

static const struct wl_callback_listener frame_listener = {
	redraw
};

void Box::draw(struct window *window, std::vector<struct viewpoint *> &viewpoints, uint32_t time){
	static const int32_t speed_div = 5, benchmark_interval = 5;
	struct wl_region *region;
	EGLint rect[4];
	EGLint buffer_age = 0;
	struct timeval tv;

	glDepthFunc(GL_LESS);
	glClearColor(.7f, .85f, 1.f, 1.0f);
	glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // drawWindowBoundsStencil(window, display);

	glUseProgram(window->gl.drawProgram);

	glEnableVertexAttribArray(window->gl.pos);
	glBindBuffer(GL_ARRAY_BUFFER, window->gl.vertices);
	glVertexAttribPointer(window->gl.pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(window->gl.col);
	glBindBuffer(GL_ARRAY_BUFFER, window->gl.colors);
	glVertexAttribPointer(window->gl.col, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, window->gl.indices);

	glm::mat4 window_offset = glm::mat4(1);

	if(window->clipping_mode == MOTORCAR_SURFACE_CLIPPING_MODE_PORTAL){
		window_offset = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.25f));
	}

	glm::mat4 model =  window->transformMatrix
						* window_offset
						* this->transform
                                                * glm::rotate(glm::mat4(), glm::radians((time / 25.0f)), glm::vec3(0.0f,1.0f,0.0f))
						* glm::scale(glm::mat4(), this->size);

	int i = 0;
	for(struct viewpoint *vp : viewpoints){

		glViewport(vp->colorViewport.x, vp->colorViewport.y, vp->colorViewport.width, vp->colorViewport.height);

		glm::mat4 projection = vp->projectionMatrix;
		glm::mat4 view = vp->viewMatrix;
		
		glUniformMatrix4fv(window->gl.rotation_uniform, 1, GL_FALSE, glm::value_ptr(projection * view * model));

		glDrawElements(GL_TRIANGLES, 36,GL_UNSIGNED_INT, 0);

		i++;
	}


	glDisableVertexAttribArray(window->gl.pos);
	glDisableVertexAttribArray(window->gl.col);

}

static void
redraw(void *data, struct wl_callback *callback, uint32_t time)
{

	//printf("drawing\n");



	struct window *window = (struct window *) data;
	struct display *display = window->display;


	 static const int32_t speed_div = 5, benchmark_interval = 5;
	struct wl_region *region;
	EGLint rect[4];
	EGLint buffer_age = 0;


	 if (callback)
		wl_callback_destroy(callback);

	 wl_callback *frame_callback = wl_surface_frame(window->surface);
	 //wl_callback *frame_callback = wl_display_sync(display->display);
	 wl_callback_add_listener(frame_callback,
		&frame_listener, window);
	

	if (!window->configured)
		return;

	
	if (window->frames == 0)
		window->benchmark_time = time;
	if (time - window->benchmark_time > (benchmark_interval * 1000)) {
		printf("%d frames in %d seconds: %f fps\n",
		       window->frames,
		       benchmark_interval,
		       (float) window->frames / benchmark_interval);
		window->benchmark_time = time;
		window->frames = 0;
	}

	// angle = (time / speed_div) % 360 * M_PI / 180.0;
	// rotation[0][0] =  cos(angle);
	// rotation[0][2] =  sin(angle);
	// rotation[2][0] = -sin(angle);
	// rotation[2][2] =  cos(angle);



	if (display->swap_buffers_with_damage)
		eglQuerySurface(display->egl.dpy, window->egl_surface,
				EGL_BUFFER_AGE_EXT, &buffer_age);


	//---- draw geometry ----
	if(window->depthCompositingEnabled){
		glBindFramebuffer(GL_FRAMEBUFFER, window->gl.frameBuffer);
	}else{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
		





	//glDisable(GL_STENCIL_TEST);

	window->m_box.draw(window, display->viewpoints, time);



	if(window->depthCompositingEnabled){
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glEnable(GL_STENCIL_TEST);
		glUseProgram(window->gl.colorBlitProgram);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearDepth(1.0f);
		glClearStencil(0);
	    glStencilMask(0xFF);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//glDisable(GL_STENCIL_TEST);
	    drawWindowBoundsStencil(window, display);
	    //blitStencilBuffer(window, display);

		glEnableVertexAttribArray(window->gl.h_aPosition);
	    glBindBuffer(GL_ARRAY_BUFFER, window->gl.textureBlitVertices);
	    glVertexAttribPointer(window->gl.h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);



	    GLfloat texCoords [8];

	    GLuint textures[] = {window->gl.colorBufferTexture, window->gl.depthBufferTexture};
	    GLuint programs[] = {window->gl.colorBlitProgram, window->gl.depthBlitProgram};
	    

	    for(struct viewpoint *vp : display->viewpoints){
	    	struct viewport textureViewports[] = {vp->colorViewport, vp->depthViewport};
	    	for(int i = 0; i < 2; i++){
	    		glUseProgram(programs[i]);
	    		glBindTexture(GL_TEXTURE_2D, textures[i]);
	    		
		    	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		    	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		        glm::vec2 textureViewportPos(vp->colorViewport.x, vp->colorViewport.y);
		        glm::vec2 textureViewportSize(vp->colorViewport.width, vp->colorViewport.height);
		        glm::vec2 textureSize(window->geometry.width, window->geometry.height / 2);

		        glm::vec2 uvPos(textureViewportPos / textureSize);
		        glm::vec2 uvSize(textureViewportSize / textureSize);

		        // const GLfloat textureCoordinates[] = {
		        //     uvPos.x, uvPos.y + uvSize.y,
		        //     uvPos.x + uvSize.x, uvPos.y + uvSize.y,
		        //     uvPos.x + uvSize.x, uvPos.y,
		        //     uvPos.x, uvPos.y,
		        // };

		        const GLfloat textureCoordinates[] = {
		        	uvPos.x, uvPos.y,
		            uvPos.x + uvSize.x, uvPos.y,
		            uvPos.x + uvSize.x, uvPos.y + uvSize.y,
		            uvPos.x, uvPos.y + uvSize.y,
		        };

		        struct viewport textureViewport = textureViewports[i];
		        glViewport(textureViewport.x, textureViewport.y, textureViewport.width, textureViewport.height);

		        glEnableVertexAttribArray(window->gl.h_aTexCoord);
		        glBindBuffer(GL_ARRAY_BUFFER, window->gl.textureBlitTextureCoords);
		        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), textureCoordinates, GL_STATIC_DRAW);
		        glVertexAttribPointer(window->gl.h_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

		        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	    	}
	    	

	    }

	    glDisable(GL_STENCIL_TEST);
	    glBindTexture(GL_TEXTURE_2D, 0);

	    glDisableVertexAttribArray(window->gl.h_aPosition);
	    glDisableVertexAttribArray(window->gl.h_aTexCoord);

	    glUseProgram(0);
	}
	


    //printf("geometry: %d, %d\n", window->geometry.width, window->geometry.height);
    //printf("window_size: %d, %d\n", window->window_size.width, window->window_size.height);



	if (window->opaque || window->fullscreen) {
		region = wl_compositor_create_region(window->display->compositor);
		wl_region_add(region, 0, 0,
			      window->geometry.width,
			      window->geometry.height);
		wl_surface_set_opaque_region(window->surface, region);
		wl_region_destroy(region);
	} else {
		wl_surface_set_opaque_region(window->surface, NULL);
	}

	if (display->swap_buffers_with_damage && buffer_age > 0) {
		rect[0] = window->geometry.width / 4 - 1;
		rect[1] = window->geometry.height / 4 - 1;
		rect[2] = window->geometry.width / 2 + 2;
		rect[3] = window->geometry.height / 2 + 2;
		display->swap_buffers_with_damage(display->egl.dpy,
						  window->egl_surface,
						  rect, 1);
	} else {
		eglSwapBuffers(display->egl.dpy, window->egl_surface);
	}
	window->frames++;

	// wl_display_dispatch_pending(display->display);
	// while (!window->configured)
	//  		wl_display_dispatch(display->display);
}



static void
pointer_handle_enter(void *data, struct wl_pointer *pointer,
		     uint32_t serial, struct wl_surface *surface,
		     wl_fixed_t sx, wl_fixed_t sy)
{
	struct display *display = (struct display *) data;
	struct wl_buffer *buffer;
	struct wl_cursor *cursor = display->default_cursor;
	struct wl_cursor_image *image;

	if (display->window->fullscreen)
		wl_pointer_set_cursor(pointer, serial, NULL, 0, 0);
	else if (cursor) {
		image = display->default_cursor->images[0];
		buffer = wl_cursor_image_get_buffer(image);
		wl_pointer_set_cursor(pointer, serial,
				      display->cursor_surface,
				      image->hotspot_x,
				      image->hotspot_y);
		wl_surface_attach(display->cursor_surface, buffer, 0, 0);
		wl_surface_damage(display->cursor_surface, 0, 0,
				  image->width, image->height);
		wl_surface_commit(display->cursor_surface);
	}
}

static void
pointer_handle_leave(void *data, struct wl_pointer *pointer,
		     uint32_t serial, struct wl_surface *surface)
{
}

static void
pointer_handle_motion(void *data, struct wl_pointer *pointer,
		      uint32_t time, wl_fixed_t sx, wl_fixed_t sy)
{
}

static void
pointer_handle_button(void *data, struct wl_pointer *wl_pointer,
		      uint32_t serial, uint32_t time, uint32_t button,
		      uint32_t state)
{
	struct display *display = (struct display *) data;

	if (button == BTN_LEFT && state == WL_POINTER_BUTTON_STATE_PRESSED)
		wl_shell_surface_move(display->window->shell_surface,
				      display->seat, serial);
}

static void
pointer_handle_axis(void *data, struct wl_pointer *wl_pointer,
		    uint32_t time, uint32_t axis, wl_fixed_t value)
{
}

static const struct wl_pointer_listener pointer_listener = {
	pointer_handle_enter,
	pointer_handle_leave,
	pointer_handle_motion,
	pointer_handle_button,
	pointer_handle_axis,
};

static void
touch_handle_down(void *data, struct wl_touch *wl_touch,
		  uint32_t serial, uint32_t time, struct wl_surface *surface,
		  int32_t id, wl_fixed_t x_w, wl_fixed_t y_w)
{
	struct display *d = (struct display *)data;

	wl_shell_surface_move(d->window->shell_surface, d->seat, serial);
}

static void
touch_handle_up(void *data, struct wl_touch *wl_touch,
		uint32_t serial, uint32_t time, int32_t id)
{
}

static void
touch_handle_motion(void *data, struct wl_touch *wl_touch,
		    uint32_t time, int32_t id, wl_fixed_t x_w, wl_fixed_t y_w)
{
}

static void
touch_handle_frame(void *data, struct wl_touch *wl_touch)
{
}

static void
touch_handle_cancel(void *data, struct wl_touch *wl_touch)
{
}

static const struct wl_touch_listener touch_listener = {
	touch_handle_down,
	touch_handle_up,
	touch_handle_motion,
	touch_handle_frame,
	touch_handle_cancel,
};

static void
keyboard_handle_keymap(void *data, struct wl_keyboard *keyboard,
		       uint32_t format, int fd, uint32_t size)
{
}

static void
keyboard_handle_enter(void *data, struct wl_keyboard *keyboard,
		      uint32_t serial, struct wl_surface *surface,
		      struct wl_array *keys)
{
}

static void
keyboard_handle_leave(void *data, struct wl_keyboard *keyboard,
		      uint32_t serial, struct wl_surface *surface)
{
}

static void
keyboard_handle_key(void *data, struct wl_keyboard *keyboard,
		    uint32_t serial, uint32_t time, uint32_t key,
		    uint32_t state)
{
	struct display *d = (struct display *) data;

	if (key == KEY_F11 && state)
		set_fullscreen(d->window, d->window->fullscreen ^ 1);
	else if (key == KEY_ESC && state)
		running = 0;
}

static void
keyboard_handle_modifiers(void *data, struct wl_keyboard *keyboard,
			  uint32_t serial, uint32_t mods_depressed,
			  uint32_t mods_latched, uint32_t mods_locked,
			  uint32_t group)
{
}

static const struct wl_keyboard_listener keyboard_listener = {
	keyboard_handle_keymap,
	keyboard_handle_enter,
	keyboard_handle_leave,
	keyboard_handle_key,
	keyboard_handle_modifiers,
};

glm::mat4 mat4_from_position_and_orientation(struct wl_array *positionArray, struct wl_array *orientationArray){
	glm::vec3 position = glm::make_vec3((float *)(positionArray->data));
	glm::mat3 orientation = glm::make_mat3((float *)(orientationArray->data));

	glm::mat4 res = glm::mat4(orientation);
	res[3] = glm::vec4(position, 1);
	return res;
}

static void
motorcar_six_dof_pointer_handle_enter(void *data,
		      struct motorcar_six_dof_pointer *motorcar_six_dof_pointer,
		      uint32_t serial,
		      struct motorcar_surface *surface,
		      struct wl_array *position,
		      struct wl_array *orientation){

	printf("6DOF pointer entered surface\n");
	struct window *window = (struct window *) data;
	window->m_sixDofPointer.transform = mat4_from_position_and_orientation(position, orientation);
}

static void
motorcar_six_dof_pointer_handle_leave(void *data,
		      struct motorcar_six_dof_pointer *motorcar_six_dof_pointer,
		      uint32_t serial,
		      struct motorcar_surface *surface){
	printf("6DOF pointer left surface\n");
	struct window *window = (struct window *) data;
	window->m_box.isGrabbed = false;
	window->m_box.grabTransform = glm::mat4();
}

static void
motorcar_six_dof_pointer_handle_motion(void *data,
		       struct motorcar_six_dof_pointer *motorcar_six_dof_pointer,
		       uint32_t time,
		       struct wl_array *position,
		       struct wl_array *orientation){

	//printf("6DOF pointer motion event\n");
	struct window *window = (struct window *) data;
	window->m_sixDofPointer.transform = mat4_from_position_and_orientation(position, orientation);
	if(window->m_box.isGrabbed){
		window->m_box.transform = glm::inverse(window->transformMatrix) * window->m_sixDofPointer.transform  * window->m_box.grabTransform;
	}

}
static void
motorcar_six_dof_pointer_handle_button(void *data,
		       struct motorcar_six_dof_pointer *motorcar_six_dof_pointer,
		       uint32_t serial,
		       uint32_t time,
		       uint32_t button,
		       uint32_t state){
	printf("6DOF pointer button event, button state = %d\n", state);
	struct window *window = (struct window *) data;
	switch(state){
	case WL_POINTER_BUTTON_STATE_PRESSED:
		window->m_box.isGrabbed = true;
		window->m_box.grabTransform = glm::inverse(window->m_sixDofPointer.transform) * window->transformMatrix * window->m_box.transform;
		break;
	case WL_POINTER_BUTTON_STATE_RELEASED:
		window->m_box.isGrabbed = false;
		window->m_box.grabTransform = glm::mat4();
		break;
	}
}

struct motorcar_six_dof_pointer_listener motorcar_pointer_listener = {
	motorcar_six_dof_pointer_handle_enter,
	motorcar_six_dof_pointer_handle_leave,
	motorcar_six_dof_pointer_handle_motion,
	motorcar_six_dof_pointer_handle_button,
};



static void
motorcar_viewpoint_handle_view_matrix(void *data,
			    struct motorcar_viewpoint *motorcar_viewpoint,
			    struct wl_array *view){

	struct viewpoint *vp = static_cast<struct viewpoint *>(data);

	vp->viewMatrix = glm::make_mat4((float *)(view->data));

	// printf("viewpoint updated view matrix :\n");
	// printMatrix(vp->viewMatrix);
}

static void
motorcar_viewpoint_handle_projection_matrix(void *data,
			    struct motorcar_viewpoint *motorcar_viewpoint,
			    struct wl_array *projection){

	struct viewpoint *vp = static_cast<struct viewpoint *>(data);

	vp->projectionMatrix = glm::make_mat4((float *)(projection->data));

	printf("viewpoint updated projection matrix :\n");
	//printMatrix(vp->projectionMatrix);
}

static void
motorcar_viewpoint_handle_view_port(void *data,
			  struct motorcar_viewpoint *motorcar_viewpoint,
			  int32_t color_x,
			  int32_t color_y,
			  uint32_t color_width,
			  uint32_t color_height,
			  int32_t depth_x,
			  int32_t depth_y,
			  uint32_t depth_width,
			  uint32_t depth_height){
	printf("viewpoint updated color viewport : %u, %u, %u, %u\n", color_x, color_y, color_width, color_height);
	printf("viewpoint updated depth viewport : %u, %u, %u, %u\n", depth_x, depth_y, depth_width, depth_height);

	struct viewpoint *vp = static_cast<struct viewpoint *>(data);

	vp->colorViewport.x = color_x;
	vp->colorViewport.y = color_y;
	vp->colorViewport.width = color_width;
	vp->colorViewport.height = color_height;

	vp->depthViewport.x = depth_x;
	vp->depthViewport.y = depth_y;
	vp->depthViewport.width = depth_width;
	vp->depthViewport.height = depth_height;

}



struct motorcar_viewpoint_listener viewpoint_listener= {
	motorcar_viewpoint_handle_view_matrix,
	motorcar_viewpoint_handle_projection_matrix,
	motorcar_viewpoint_handle_view_port

};






static void
seat_handle_capabilities(void *data, struct wl_seat *seat,
			 uint32_t caps)
{
	struct display *d = (struct display *) data;

	if ((caps & WL_SEAT_CAPABILITY_POINTER) && !d->pointer) {
		d->pointer = wl_seat_get_pointer(seat);
		wl_pointer_add_listener(d->pointer, &pointer_listener, d);
	} else if (!(caps & WL_SEAT_CAPABILITY_POINTER) && d->pointer) {
		wl_pointer_destroy(d->pointer);
		d->pointer = NULL;
	}

	if ((caps & WL_SEAT_CAPABILITY_KEYBOARD) && !d->keyboard) {
		d->keyboard = wl_seat_get_keyboard(seat);
		wl_keyboard_add_listener(d->keyboard, &keyboard_listener, d);
	} else if (!(caps & WL_SEAT_CAPABILITY_KEYBOARD) && d->keyboard) {
		wl_keyboard_destroy(d->keyboard);
		d->keyboard = NULL;
	}

	if ((caps & WL_SEAT_CAPABILITY_TOUCH) && !d->touch) {
		d->touch = wl_seat_get_touch(seat);
		wl_touch_set_user_data(d->touch, d);
		wl_touch_add_listener(d->touch, &touch_listener, d);
	} else if (!(caps & WL_SEAT_CAPABILITY_TOUCH) && d->touch) {
		wl_touch_destroy(d->touch);
		d->touch = NULL;
	}
}

static const struct wl_seat_listener seat_listener = {
	seat_handle_capabilities,
};

static void
registry_handle_global(void *data, struct wl_registry *registry,
		       uint32_t name, const char *interface, uint32_t version)
{
	struct display *d = (struct display *) data;

	if (strcmp(interface, "wl_compositor") == 0) {
		d->compositor = (wl_compositor*) wl_registry_bind(registry, name,
					 &wl_compositor_interface, 1);
	} else if (strcmp(interface, "wl_shell") == 0) {
		d->shell = (wl_shell *) wl_registry_bind(registry, name,
					    &wl_shell_interface, 1);
	} else if (strcmp(interface, "wl_seat") == 0) {
		d->seat = (wl_seat *) wl_registry_bind(registry, name,
					   &wl_seat_interface, 1);
		wl_seat_add_listener(d->seat, &seat_listener, d);
	} else if (strcmp(interface, "wl_shm") == 0) {
		d->shm = (wl_shm *) wl_registry_bind(registry, name,
					  &wl_shm_interface, 1);
		d->cursor_theme = wl_cursor_theme_load(NULL, 32, d->shm);
		d->default_cursor =
			wl_cursor_theme_get_cursor(d->cursor_theme, "left_ptr");
	} else if (strcmp(interface, "motorcar_shell") == 0) {
		d->motorshell = (motorcar_shell *) wl_registry_bind(registry, name,
					   &motorcar_shell_interface, 1);
		printf("got motorcar shell %p\n", d->motorshell);
	} else if (strcmp(interface, "motorcar_viewpoint") == 0) {
		struct motorcar_viewpoint *motorcar_viewpoint_handle =
		 (motorcar_viewpoint *) wl_registry_bind(registry, name, &motorcar_viewpoint_interface, 1);

		printf("got motorcar viewpoint, numviewpoints=%lu\n", d->viewpoints.size() + 1);

		struct viewpoint *vp = (struct viewpoint *) malloc(sizeof(struct viewpoint));
		vp->handle = motorcar_viewpoint_handle;
		motorcar_viewpoint_add_listener(motorcar_viewpoint_handle, &viewpoint_listener, vp);
		d->viewpoints.push_back(vp);

	} else if (strcmp(interface, "motorcar_six_dof_pointer") == 0) {
		struct motorcar_six_dof_pointer *motorcar_pointer_handle =
		 (motorcar_six_dof_pointer *) wl_registry_bind(registry, name, &motorcar_six_dof_pointer_interface, 1);

		printf("got motorcar six_dof pointer\n");

		motorcar_six_dof_pointer_add_listener(motorcar_pointer_handle, &motorcar_pointer_listener, d->window);

	}
}

static void
registry_handle_global_remove(void *data, struct wl_registry *registry,
			      uint32_t name)
{
}

static const struct wl_registry_listener registry_listener = {
	registry_handle_global,
	registry_handle_global_remove
};

static void
signal_int(int signum)
{
	running = 0;
}

static void
usage(int error_code)
{
	fprintf(stderr, "Usage: simple-egl [OPTIONS]\n\n"
		"  -f\tRun in fullscreen mode\n"
		"  -o\tCreate an opaque surface\n"
		"  -s\tUse a 16 bpp EGL config\n"
		"  -b\tDon't sync to compositor redraw (eglSwapInterval 0)\n"
		"  -p\tuse portal clipping mode\n"
		"  -d\tDisable depth compositing\n"
		"  -h\tThis help text\n\n");

	exit(error_code);
}

int
main(int argc, char **argv)
{
	struct sigaction sigint;
	struct display display = { 0 };
	struct window  window  = { 0 };
	int i, ret = 0;


	window.m_box.size = glm::vec3(0.3);
	window.m_box.transform = glm::mat4(1); 
	window.m_box.grabTransform = glm::mat4(1); 
	window.m_box.isGrabbed = false;

	window.display = &display;
	display.window = &window;
	window.window_size.width  = 400;//2194;
	window.window_size.height = 400;//2742;//2650;
	window.buffer_size = 32;
	window.frame_sync = 1;
	window.clipping_mode = MOTORCAR_SURFACE_CLIPPING_MODE_CUBOID;
	window.depthCompositingEnabled = true;


	for (i = 1; i < argc; i++) {
		if (strcmp("-f", argv[i]) == 0)
			window.fullscreen = 1;
		else if (strcmp("-o", argv[i]) == 0)
			window.opaque = 1;
		else if (strcmp("-s", argv[i]) == 0)
			window.buffer_size = 16;
		else if (strcmp("-b", argv[i]) == 0)
			window.frame_sync = 0;
		else if (strcmp("-p", argv[i]) == 0)
			window.clipping_mode = MOTORCAR_SURFACE_CLIPPING_MODE_PORTAL;
		else if (strcmp("-d", argv[i]) == 0)
			window.depthCompositingEnabled = false;
		else if (strcmp("-h", argv[i]) == 0)
			usage(EXIT_SUCCESS);
		else
			usage(EXIT_FAILURE);
	}

	display.display = wl_display_connect(NULL);
	assert(display.display);

	display.registry = wl_display_get_registry(display.display);
	wl_registry_add_listener(display.registry,
				 &registry_listener, &display);

	wl_display_dispatch(display.display);

	init_egl(&display, &window);
	create_surface(&window);
	init_gl(&window);

	display.cursor_surface =
		wl_compositor_create_surface(display.compositor);

	sigint.sa_handler = signal_int;
	sigemptyset(&sigint.sa_mask);
	sigint.sa_flags = SA_RESETHAND;
	sigaction(SIGINT, &sigint, NULL);

	/* The mainloop here is a little subtle.  Redrawing will cause
	 * EGL to read events so we can just call
	 * wl_display_dispatch_pending() to handle any events that got
	 * queued up as a side effect. */
	// while (running && ret != -1) {
	// 	wl_display_dispatch_pending(display.display);
	// 	while (!window.configured)
	// 		wl_display_dispatch(display.display);
	// 	redraw(&window, NULL, 0);
	// }

	
	 // wl_callback *frame_callback = wl_surface_frame(window.surface);
	 // wl_callback_add_listener(frame_callback, &frame_listener, &window);

	 wl_callback *sync_callback = wl_display_sync(display.display);
	 wl_callback_add_listener(sync_callback, &frame_listener, &window);


	 while (running && ret != -1) {
	 	wl_display_flush(display.display);
	 	//wl_display_read_events(display.display);
	 	//wl_display_dispatch_pending(display.display);
	 	//while (!window.configured)
	 		wl_display_dispatch(display.display);
	 	//redraw(&window, NULL, 0);

		
	 	

	}

	fprintf(stderr, "simple-egl exiting\n");

	destroy_surface(&window);
	fini_egl(&display);

	wl_surface_destroy(display.cursor_surface);
	if (display.cursor_theme)
		wl_cursor_theme_destroy(display.cursor_theme);

	if (display.shell)
		wl_shell_destroy(display.shell);

	if (display.compositor)
		wl_compositor_destroy(display.compositor);

	wl_registry_destroy(display.registry);
	wl_display_flush(display.display);
	wl_display_disconnect(display.display);

	return 0;
}
