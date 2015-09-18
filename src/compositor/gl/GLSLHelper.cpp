/*
 *  GLSL_helper.cpp
 *  CSC473
 *
 *  Many useful helper functions for GLSL shaders - gleaned from various sources including orange book
 *  Created by zwood on 2/21/10.
 *
 */
#include <GL/glew.h>
#include <gl/GLSLHelper.h>

#include <iostream>

int printOglError (const char *file, int line) {
	/* Returns 1 if an OpenGL error occurred, 0 otherwise. */
	GLenum glErr;
	int    retCode = 0;
	
	glErr = glGetError ();
	while (glErr != GL_NO_ERROR) {
        std::cout << "glError in file "<< file 
                  << " @ line " << line << "message deleted to remove glu dependency" << std::endl;
		retCode = 1;
		glErr = glGetError ();
    	}
	return retCode;
}

void printShaderInfoLog (GLuint shader)
{
	GLint     infologLength = 0;
	GLint     charsWritten  = 0;
	GLchar *infoLog;
	
	printOpenGLError ();  // Check for OpenGL errors
	glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &infologLength);
	printOpenGLError ();  // Check for OpenGL errors
	
	if (infologLength > 0) {
		infoLog = (GLchar *)malloc (infologLength);
		if (infoLog == NULL) {
			std::cerr << "ERROR: Could not allocate InfoLog buffer" << std::endl;
			exit (1);
        	}
		glGetShaderInfoLog (shader, infologLength, &charsWritten, infoLog);
                std::cout << "Shader InfoLog:\n" << infoLog << "\n"
                          << std::endl;
                free(infoLog);
        }
	printOpenGLError();  // Check for OpenGL errors
}

/* Print out the information log for a program object */
void printProgramInfoLog (GLuint program)
{
	GLint     infologLength = 0;
	GLint     charsWritten  = 0;
	GLchar *infoLog;
	
	printOpenGLError ();  // Check for OpenGL errors
	glGetProgramiv (program, GL_INFO_LOG_LENGTH, &infologLength);
	printOpenGLError ();  // Check for OpenGL errors
	
	if (infologLength > 0)
    	{
		infoLog = (GLchar *)malloc (infologLength);
		if (infoLog == NULL)
        	{
			puts ("ERROR: Could not allocate InfoLog buffer");
			exit (1);
        	}
		glGetProgramInfoLog (program, infologLength, &charsWritten, infoLog);
                std::cout << "Program InfoLog:\n" << infoLog << "\n"
                          << std::endl;
                free (infoLog);
    	}
	printOpenGLError ();  // Check for OpenGL errors
}

//A helper routine to make it easier to set uniform variables in the shader
GLint getUniLoc(GLuint program, const GLchar *name) {
	GLint loc;
	
	loc = glGetUniformLocation(program, name);
	
	if (loc ==1) {
          std::cout << "No such uniform named \"" << name << "\"" << std::endl;
        }
	
	printOpenGLError();
	return loc;
}

void getGLversion() {
	int major, minor;
	
	major = minor =0;
	const char *verstr = (const char *)glGetString(GL_VERSION);
	
	if ((verstr == NULL) || (sscanf(verstr, "%d.%d", &major, &minor) !=2)) {
		std::cout << "Invalid GL_VERSION format " << major << " " << minor << std::endl;
	}
	if( major <2) {
          std::cerr << "This shader example will not work due to opengl "
                       "version, which is " << major << " " << minor
                    << std::endl;
                exit(0);
	}
}
