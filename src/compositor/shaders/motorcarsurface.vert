#ifndef MOTORCAR_SHADER_MOTORCARSURFACE_VERT__
#define MOTORCAR_SHADER_MOTORCARSURFACE_VERT__

std::string shader_motorcarsurface_vert = 
"//precision highp float;\n"
"\n"
"uniform mat4 uMVPMatrix;\n"
"\n"
"attribute vec3 aPosition;\n"
"attribute vec2 aTexCoord;\n"
"\n"
"varying vec2 vTexCoord;\n"
"\n"
"void main(void)\n"
"{\n"
"    vTexCoord = aTexCoord;\n"
"\n"
"    gl_Position =   uMVPMatrix * vec4(aPosition, 1);\n"
"\n"
"\n"
"}\n";
#endif
