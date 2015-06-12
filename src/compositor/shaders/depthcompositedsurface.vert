#ifndef MOTORCAR_SHADER_DEPTHCOMPOSITEDSURFACE_VERT__
#define MOTORCAR_SHADER_DEPTHCOMPOSITEDSURFACE_VERT__

std::string shader_depthcompositedsurface_vert =

"attribute vec3 aPosition;\n"
"attribute vec2 aColorTexCoord;\n"
"attribute vec2 aDepthTexCoord;\n"
"\n"
"varying vec2 vColorTexCoord;\n"
"varying vec2 vDepthTexCoord;\n"
"\n"
"void main(void)\n"
"{\n"
"    vColorTexCoord = aColorTexCoord;\n"
"    vDepthTexCoord = aDepthTexCoord;\n"
"\n"
"    gl_Position =  vec4(aPosition, 1);\n"
"\n"
"\n"
"}\n";
#endif
