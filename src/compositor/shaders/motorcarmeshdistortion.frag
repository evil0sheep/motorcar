#ifndef MOTORCAR_SHADER_MOTORCARMESHDISTORTION_FRAG__
#define MOTORCAR_SHADER_MOTORCARMESHDISTORTION_FRAG__

std::string shader_motorcarmeshdistortion_frag =

"//precision highp float;\n"
"uniform sampler2D uTexSampler;\n"
"\n"
"varying vec4 vPos; \n"
"varying vec2 vTanEyeAnglesR; \n"
"varying vec2 vTanEyeAnglesG; \n"
"varying vec2 vTanEyeAnglesB; \n"
"//varying vec2 vTexCoord;\n"
"\n"
"void main(void)\n"
"{\n"
"	gl_FragColor = vec4(1,1,1,0) * vPos.w;\n"
"}\n";

#endif
