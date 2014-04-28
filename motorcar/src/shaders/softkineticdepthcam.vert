uniform mat4 uMVPMatrix;
attribute vec3 aPosition;
attribute float aConfidence;
attribute vec2 aTexCoord;

varying vec2 vTexCoord;
varying float vIsValid;

void main(void)
{
    if(aConfidence < 500.f || aPosition == vec3(-2.0f)){

        vIsValid = 0.f;
    }else{
        vIsValid = 1.f;
    }
    gl_Position =   uMVPMatrix * vec4(aPosition, 1);
    vTexCoord = aTexCoord;

}
