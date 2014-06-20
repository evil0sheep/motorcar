/*
 *  GLSL_helper.cpp
 *  CSC473
 *
 *  Many useful helper functions for GLSL shaders - gleaned from various sources including orange book
 *  Created by zwood on 2/21/10.
 *
 */

#include <gl/GLSLHelper.h>

int printOglError (const char *file, int line) {
	/* Returns 1 if an OpenGL error occurred, 0 otherwise. */
	GLenum glErr;
	int    retCode = 0;
	
	glErr = glGetError ();
	while (glErr != GL_NO_ERROR) {
        printf ("glError in file %s @ line %d: %s\n", file, line, "message deleted to remove glu dependency");
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
			puts ("ERROR: Could not allocate InfoLog buffer");
			exit (1);
        	}
		glGetShaderInfoLog (shader, infologLength, &charsWritten, infoLog);
		printf ("Shader InfoLog:\n%s\n\n", infoLog);
		free (infoLog);
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
		printf ("Program InfoLog:\n%s\n\n", infoLog);
		free (infoLog);
    	}
	printOpenGLError ();  // Check for OpenGL errors
}

//A helper routine to make it easier to set uniform variables in the shader
GLint getUniLoc(GLuint program, const GLchar *name) {
	GLint loc;
	
	loc = glGetUniformLocation(program, name);
	
	if (loc ==1) {
		printf("No such uniform named %s\n", name);
	}
	
	printOpenGLError();
	return loc;
}

void getGLversion() {
	int major, minor;
	
	major = minor =0;
	const char *verstr = (const char *)glGetString(GL_VERSION);
	
	if ((verstr == NULL) || (sscanf(verstr, "%d.%d", &major, &minor) !=2)) {
		printf("Invalid GL_VERSION format %d %d\n", major, minor);
	}
	if( major <2) {
		printf("This shader example will not work due to opengl version, which is %d %d\n", major, minor);
		exit(0);
	}
}

// textfile.cpp
//
// simple reading and writing for text files
//
// www.lighthouse3d.com
//
// You may use these functions freely.
// they are provided as is, and no warranties, either implicit,
// or explicit are given
//////////////////////////////////////////////////////////////////////

char *textFileRead(char *fn) {
	
	
	FILE *fp;
	char *content = NULL;
	
	int count=0;
	
	if (fn != NULL) {
		fp = fopen(fn,"rt");
		
		if (fp != NULL) {
			
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			
			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

int textFileWrite(char *fn, char *s) {
	
	FILE *fp;
	int status = 0;
	
	if (fn != NULL) {
		fp = fopen(fn,"w");
		
		if (fp != NULL) {
			
			if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
				status = 1;
			fclose(fp);
		}
	}
	return(status);
}

