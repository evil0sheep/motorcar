#ifndef MOTORCAR_SHADER_MOTORCARMESHDISTORTION_FRAG__
#define MOTORCAR_SHADER_MOTORCARMESHDISTORTION_FRAG__

std::string shader_motorcarmeshdistortion_frag =

"//precision highp float;\n"
"uniform sampler2D uTexSampler;\n"
"\n"
"varying vec4 vPosition; \n"
"varying vec2 vTexCoordR; \n"
"varying vec2 vTexCoordG; \n"
"varying vec2 vTexCoordB; \n"
"\n"
"void main(void)\n"
"{\n"
"	vec3 fragColor;\n"
"   fragColor.r = texture2D(uTexSampler, vTexCoordR).r;\n"
"   fragColor.g = texture2D(uTexSampler, vTexCoordG).g;\n"
"   fragColor.b = texture2D(uTexSampler, vTexCoordB).b;\n"
"	gl_FragColor = vec4(fragColor * vPosition.w, 1);\n"
"}\n";

#endif
