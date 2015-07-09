#ifndef MOTORCAR_SHADER_DEPTHCOMPOSITEDSURFACEBLITTER_FRAG__
#define MOTORCAR_SHADER_DEPTHCOMPOSITEDSURFACEBLITTER_FRAG__

std::string shader_depthcompositedsurfaceblitter_frag =
"uniform sampler2D uColorSampler;\n"
"uniform sampler2D uDepthSampler;\n"
"varying vec2 vTexCoord;\n"
"\n"
"\n"
"void main(void)\n"
"{\n"
"\n"
"    vec4 color = texture2D(uColorSampler, vTexCoord);\n"
"    float depth = texture2D(uDepthSampler, vTexCoord).r;\n"
"\n"
"    gl_FragDepth = depth;\n"
"\n"
"    //(depth > 0.999) depth = 0;\n"
"\n"
"    gl_FragColor = color; //vec4(0, depth == 1.0f ? 0.0f : depth, min(color.g, 1),  1);\n"
"\n"
"}\n";

#endif
