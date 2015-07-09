#ifndef MOTORCAR_SHADER_MOTORCARLINE_VERT__
#define MOTORCAR_SHADER_MOTORCARLINE_VERT__

std::string shader_motorcarline_vert =
"//precision highp float;\n"
"uniform mat4 uMVPMatrix;\n"
"attribute vec3 aPosition;\n"
"\n"
"void main(void)\n"
"{\n"
"    gl_Position =   uMVPMatrix * vec4(aPosition, 1);\n"
"}\n";

#endif
