#ifndef MOTORCAR_SHADER_DEPTHCOMPOSITEDSURFACE_FRAG__
#define MOTORCAR_SHADER_DEPTHCOMPOSITEDSURFACE_FRAG__

std::string shader_depthcompositedsurface_frag =
"uniform sampler2D uTexSampler;\n"
"varying vec2 vColorTexCoord;\n"
"varying vec2 vDepthTexCoord;\n"
"\n"
"\n"
"float unpack_depth(vec4 rgba ) {\n"
"  float depth = dot(rgba, vec4(1.f, 1.f/255.0, 1.f/65025.0, 1.f/160581375.0));\n"
"  depth = (depth==0.0) ? 1.0 : depth;\n"
"  return depth;\n"
"}\n"
"\n"
"void main(void)\n"
"{\n"
"\n"
"    gl_FragDepth = unpack_depth(texture2D(uTexSampler, vDepthTexCoord));\n"
"    gl_FragColor = texture2D(uTexSampler, vColorTexCoord);\n"
"}\n"
"\n"
"\n";
#endif
