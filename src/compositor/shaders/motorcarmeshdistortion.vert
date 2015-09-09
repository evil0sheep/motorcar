#ifndef MOTORCAR_SHADER_MOTORCARMESHDISTORTION_VERT__
#define MOTORCAR_SHADER_MOTORCARMESHDISTORTION_VERT__

std::string shader_motorcarmeshdistortion_vert =
"//precision highp float;\n"
"\n"
"attribute vec4 aPos;    			///< [-1,+1],[-1,+1] over the entire framebuffer. Lerp factor in Pos.z. Vignette fade factorin Pos.w.\n"
"attribute vec2 aTanEyeAnglesR;   ///< The tangents of the horizontal and vertical eye angles for the red channel.\n"
"attribute vec2 aTanEyeAnglesG;   ///< The tangents of the horizontal and vertical eye angles for the green channel.\n"
"attribute vec2 aTanEyeAnglesB;   ///< The tangents of the horizontal and vertical eye angles for the blue channel.\n"
"//attribute vec2 aTexCoord;\n"
"\n"
"varying vec4 vPos; \n"
"varying vec2 vTanEyeAnglesR; \n"
"varying vec2 vTanEyeAnglesG; \n"
"varying vec2 vTanEyeAnglesB; \n"
"//varying vec2 vTexCoord;\n"
"\n"
"void main(void)\n"
"{\n"
"	vPos = aPos;"
"	vTanEyeAnglesR = aTanEyeAnglesR;"
"	vTanEyeAnglesG = aTanEyeAnglesG;"
"	vTanEyeAnglesB = aTanEyeAnglesB;"
"    //vTexCoord = aTexCoord;\n"
"   gl_Position = vec4(aPos.xy, 0, 1);\n"
"}\n";

#endif