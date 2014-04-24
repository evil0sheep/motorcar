uniform sampler2D uTexSampler;

varying vec2 vColorTexCoord;
varying vec2 vDepthTexCoord;

void main(void)
{
    gl_FragDepth = texture2D(uTexSampler, vDepthTexCoord).r;
    gl_FragColor = texture2D(uTexSampler, vColorTexCoord);
}
