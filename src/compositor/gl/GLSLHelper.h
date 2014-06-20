/*
 *  GLSL_helper.h
 *  CSC473
 *
 *  Many useful helper functions for GLSL shaders - gleaned from various sources including orange book
 *  Created by zwood on 2/21/10.
 *
 */

//#include <GLES2/gl2.h>
#include <GL/gl.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdexcept>

int printOglError (const char *file, int line);

/* Print out the information log for a program object */
void printProgramInfoLog (GLuint program);

void printShaderInfoLog (GLuint shader);

#define printOpenGLError() printOglError(__FILE__, __LINE__)

//A helper routine to make it easier to set uniform variables in the shader
GLint getUniLoc(GLuint program, const GLchar *name);

void getGLversion();

int textFileWrite(char *fn, char *s);
char *textFileRead(char *fn);

inline GLint safe_glGetAttribLocation(const GLuint program, const char varname[]) {
  GLint r = glGetAttribLocation(program, varname);
  if (r < 0)
    std::cerr << "WARN: "<< varname << " cannot be bound (it either doesn't exist or has been optimized away). safe_glAttrib calls will silently ignore it.\n" << std::endl;
  return r;
}

inline GLint safe_glGetUniformLocation(const GLuint program, const char varname[]) {
  GLint r = glGetUniformLocation(program, varname);
  if (r < 0)
    std::cerr << "WARN: "<< varname << " cannot be bound (it either doesn't exist or has been optimized away). safe_glUniform calls will silently ignore it.\n" << std::endl;
  return r;
}


inline void safe_glEnableVertexAttribArray(const GLint handle) {
  if (handle >= 0)
    glEnableVertexAttribArray(handle);
}

inline void safe_glDisableVertexAttribArray(const GLint handle) {
  if (handle >= 0)
    glDisableVertexAttribArray(handle);
}

inline void safe_glVertexAttribPointer(const GLint handle, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) {
  if (handle >= 0)
    glVertexAttribPointer(handle, size, type, normalized, stride, pointer);
}

inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]) {
  if (handle >= 0)
    glUniformMatrix4fv(handle, 1, GL_FALSE, data);
}

inline void safe_glUniform1i(const GLint handle, const GLint a) {
  if (handle >= 0)
    glUniform1i(handle, a);
}

inline void safe_glUniform2i(const GLint handle, const GLint a, const GLint b) {
  if (handle >= 0)
    glUniform2i(handle, a, b);
}

inline void safe_glUniform3i(const GLint handle, const GLint a, const GLint b, const GLint c) {
  if (handle >= 0)
    glUniform3i(handle, a, b, c);
}

inline void safe_glUniform4i(const GLint handle, const GLint a, const GLint b, const GLint c, const GLint d) {
  if (handle >= 0)
    glUniform4i(handle, a, b, c, d);
}

inline void safe_glUniform1f(const GLint handle, const GLfloat a) {
  if (handle >= 0)
    glUniform1f(handle, a);
}



// Check if there has been an error inside OpenGL and if yes, print the error and
// through a runtime_error exception.
void checkGlErrors();


// Light wrapper around a GL buffer object handle that automatically allocates
// and deallocates. Can be casted to a GLuint.
class GlBufferObject{
  GLuint handle;

public:
  GlBufferObject() {
    glGenBuffers(1, &handle);
    checkGlErrors();
  }

  ~GlBufferObject() {
    glDeleteBuffers(1, &handle);
  }

  // Casts to GLuint so can be used directly glBindBuffer and so on
  operator GLuint() const {
    return handle;
  }
};

