uniform mat4 uMVPMatrix;

attribute vec3 aPosition;
attribute vec2 aTexCoord;

varying vec2 vTexCoord;
//varying vec4 vPosition;

void main(void)
{
    vTexCoord = aTexCoord;

    gl_Position =   uMVPMatrix * vec4(aPosition, 1);


}
