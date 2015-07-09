#ifndef MOTORCAR_SHADER_DEPTHCOMPOSITEDSURFACEBLITTER_VERT__
#define MOTORCAR_SHADER_DEPTHCOMPOSITEDSURFACEBLITTER_VERT__

std::string shader_depthcompositedsurfaceblitter_vert =

"attribute vec3 aPosition;\n"
"attribute vec2 aTexCoord;\n"
"\n"
"varying vec2 vTexCoord;\n"
"\n"
"\n"
"void main(void)\n"
"{\n"
"    vTexCoord = aTexCoord;\n"
"\n"
"    gl_Position =  vec4(aPosition, 1);\n"
"\n"
"}\n";

#endif
