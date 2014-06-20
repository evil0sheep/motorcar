uniform sampler2D uTexSampler;

varying vec2 vColorTexCoord;
varying vec2 vDepthTexCoord;


float unpack_depth(vec4 rgba ) {
  float depth = dot(rgba, vec4(1.f, 1.f/255.0, 1.f/65025.0, 1.f/160581375.0));
  depth = (depth==0.0) ? 1.0 : depth;
  return depth;
}

void main(void)
{

    gl_FragDepth = unpack_depth(texture2D(uTexSampler, vDepthTexCoord));
    gl_FragColor = texture2D(uTexSampler, vColorTexCoord);
}


