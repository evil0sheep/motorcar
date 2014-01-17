uniform mat4 uMVPMatrix;
//uniform mat4 uModelMatrix;
//uniform mat4 uViewMatrix;
//uniform mat4 uProjectionMatrix;

attribute vec3 aPosition;
attribute vec2 aTexCoord;

varying vec2 vTexCoord;
//varying vec4 vPosition;

void main(void)
{
    vTexCoord = aTexCoord;

    gl_Position =   uMVPMatrix * vec4(aPosition, 1);
    //gl_Position =   (uProjectionMatrix  * uViewMatrix * uModelMatrix) * vec4(aPosition, 1);

}
