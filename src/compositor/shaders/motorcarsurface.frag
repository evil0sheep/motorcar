#ifndef MOTORCAR_SHADER_MOTORCARSURFACE_FRAG__
#define MOTORCAR_SHADER_MOTORCARSURFACE_FRAG__

std::string shader_motorcarsurface_frag = 

"//precision highp float;\n"
"\n"
"uniform sampler2D uTexSampler;\n"
"varying vec2 vTexCoord;\n"
"\n"
"void main(void)\n"
"{\n"
"    gl_FragColor = texture2D(uTexSampler, vTexCoord);\n"
"}\n";
#endif
