attribute vec3 aPosition;
attribute vec2 aTexCoord;

varying vec2 vTexCoord;

void main(void)
{
    vTexCoord = aTexCoord;
    gl_Position = aPosition;
}
