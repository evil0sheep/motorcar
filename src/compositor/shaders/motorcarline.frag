#ifndef MOTORCAR_SHADER_MOTORCARLINE_FRAG__
#define MOTORCAR_SHADER_MOTORCARLINE_FRAG__

std::string shader_motorcarline_frag =
"//precision highp float;\n"
"uniform vec3 uColor;\n"
"\n"
"void main(void)\n"
"{\n"
"    gl_FragColor = vec4(uColor, 1);\n"
"}\n";

#endif
