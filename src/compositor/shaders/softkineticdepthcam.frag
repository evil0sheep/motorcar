#ifndef MOTORCAR_SHADER_SOFTKINETICDEPTHCAM_FRAG__
#define MOTORCAR_SHADER_SOFTKINETICDEPTHCAM_FRAG__

std::string shader_softkineticdepthcam_frag = 

"uniform sampler2D uTexSampler;\n"
"varying vec2 vTexCoord;\n"
"varying float vIsValid;\n"
"void main(void)\n"
"{\n"
"    if(vIsValid < 1.0){\n"
"        discard;\n"
"    }\n"
"    //gl_FragColor = vec4(vTexCoord.x, vTexCoord.y, 0, 1);\n"
"    gl_FragColor = texture2D(uTexSampler, vTexCoord).bgra;\n"
"}\n";

#endif
