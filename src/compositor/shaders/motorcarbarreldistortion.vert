#ifndef MOTORCAR_SHADER_MOTORCARBARRELDISTORTION_VERT__
#define MOTORCAR_SHADER_MOTORCARBARRELDISTORTION_VERT__

std::string shader_motorcarbarreldistortion_vert =
"//precision highp float;\n"
"attribute vec3 aPosition;\n"
"//attribute vec2 aTexCoord;\n"
"//varying vec2 vTexCoord;\n"
"varying vec2 vUDCPos;\n"
"void main(void)\n"
"{\n"
"    //vTexCoord = aTexCoord;\n"
"    vUDCPos = aPosition.xy;\n"
"    gl_Position = vec4(aPosition, 1);\n"
"}\n";

#endif
