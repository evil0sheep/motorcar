uniform sampler2D uTexSampler;
varying vec2 vTexCoord;
//varying vec4 vPosition;

void main(void)
{
   // gl_FragColor = vPosition;
    gl_FragColor = texture2D(uTexSampler, vTexCoord);
}
