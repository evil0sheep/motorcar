attribute vec3 aPosition;
attribute vec2 aTexCoord;
uniform mat4 uMVPMatrix;
varying vec2 vTexCoord;

void main(void)
{
    gl_Position = uMVPMatrix * vec4(aPosition, 1);
    vTexCoord = aTexCoord;
}
