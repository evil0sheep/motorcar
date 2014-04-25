uniform sampler2D uTexSampler;

varying vec2 vColorTexCoord;
varying vec2 vDepthTexCoord;

float unpack_depth(const in vec4 rgba_depth)
{
    const vec4 bit_shift = vec4(1.0/(256.0*256.0*256.0), 1.0/(256.0*256.0), 1.0/256.0, 1.0);
    float depth = dot(rgba_depth, bit_shift);
    return depth;
}

void main(void)
{
    gl_FragDepth = unpack_depth(texture2D(uTexSampler, vDepthTexCoord));
    gl_FragColor = texture2D(uTexSampler, vColorTexCoord);
}
