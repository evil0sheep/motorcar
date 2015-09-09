#ifndef MOTORCAR_SHADER_MOTORCARMESHDISTORTION_VERT__
#define MOTORCAR_SHADER_MOTORCARMESHDISTORTION_VERT__

std::string shader_motorcarmeshdistortion_vert =
"//precision highp float;\n"
"\n"
"uniform vec2 uEyeToSourceUVScale;\n"
"uniform vec2 uEyeToSourceUVOffset;\n"
"\n"
"attribute vec4 aPosition;    	  ///< [-1,+1],[-1,+1] over the entire framebuffer. Lerp factor in Pos.z. Vignette fade factorin Pos.w.\n"
"attribute vec2 aTanEyeAnglesR;   ///< The tangents of the horizontal and vertical eye angles for the red channel.\n"
"attribute vec2 aTanEyeAnglesG;   ///< The tangents of the horizontal and vertical eye angles for the green channel.\n"
"attribute vec2 aTanEyeAnglesB;   ///< The tangents of the horizontal and vertical eye angles for the blue channel.\n"
"\n"
"varying vec4 vPosition; \n"
"varying vec2 vTexCoordR; \n"
"varying vec2 vTexCoordG; \n"
"varying vec2 vTexCoordB; \n"
"\n"
"void main(void)\n"
"{\n"
"	vPosition = aPosition;"
"	vTexCoordR = aTanEyeAnglesR * uEyeToSourceUVScale + uEyeToSourceUVOffset;"
"	vTexCoordG = aTanEyeAnglesG * uEyeToSourceUVScale + uEyeToSourceUVOffset;"
"	vTexCoordB = aTanEyeAnglesB * uEyeToSourceUVScale + uEyeToSourceUVOffset;"
"   gl_Position = vec4(aPosition.xy, 0, 1);\n"
"}\n";

#endif