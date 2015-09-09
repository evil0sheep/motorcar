/****************************************************************************
**This file is part of the Motorcar 3D windowing framework
**
**
**Copyright (C) 2015 Forrest Reiling
**
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
**
****************************************************************************/
#include "ovr_0_5_0_1_hmd.h"
#include <GL/gl.h>
#include <X11/Xlib.h>
#include <GL/glx.h>

using namespace motorcar;

void OculusHMD::prepareForDraw()
{
	if(firstDraw){
		firstDraw=false;
	}


    RenderToTextureDisplay::prepareForDraw();


	// printf("GLX Display: %p\n", glXGetCurrentDisplay());

    	/* the drawing starts with a call to ovrHmd_BeginFrame */
	// ovrHmd_BeginFrame(hmd, 0);



	// for(int i=0; i<2; i++) {
	// 	ovrEyeType eye = hmd->EyeRenderOrder[i];

	// 	proj[i] = ovrMatrix4f_Projection(hmd->DefaultEyeFov[eye], 0.5, 500.0, 1);
		
	// 	pose[eye] = ovrHmd_GetHmdPosePerEye(hmd, eye);
	// }


}


void OculusHMD::finishDraw()
{
	RenderToTextureDisplay::finishDraw();

	// ovrHmd_EndFrame(hmd, pose, &fb_ovr_tex[0].Texture);

}


OculusHMD::OculusHMD(Skeleton *skeleton, OpenGLContext *glContext, PhysicalNode *parent)
    :RenderToTextureDisplay(glContext, glm::vec2(0.126, 0.0706), parent)
    , initialized(false)
    , firstDraw(true)
{

	printf("detected %d hmds\n", ovrHmd_Detect());
	if(!(hmd = ovrHmd_Create(0))) {
		fprintf(stderr, "failed to open Oculus HMD, falling back to virtual debug HMD\n");
		if(!(hmd = ovrHmd_CreateDebug(ovrHmd_DK2))) {
			fprintf(stderr, "failed to create virtual debug HMD\n");
			return;
		}
	}
	printf("initialized HMD: %s - %s\n", hmd->Manufacturer, hmd->ProductName);
	initialized = true;

	win_width = hmd->Resolution.w;
	win_height = hmd->Resolution.h;
	printf("Reported hmd size: %d, %d. Default Framebuffer size: %d, %d\n",win_width, win_height, glContext->defaultFramebufferSize().x, glContext->defaultFramebufferSize().y );

	//m_size = glm::vec2(win_width, win_height);

	/* enable position and rotation tracking */
	ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position, 0);
	/* retrieve the optimal render target resolution for each eye */
	eyeres[0] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left, hmd->DefaultEyeFov[0], 1.0);
	eyeres[1] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right, hmd->DefaultEyeFov[1], 1.0);

	/* and create a single render target texture to encompass both eyes */
	fb_width = eyeres[0].w + eyeres[1].w;
	fb_height = eyeres[0].h > eyeres[1].h ? eyeres[0].h : eyeres[1].h;

	setRenderTargetSize(glm::ivec2(fb_width, fb_height));

	/* fill in the ovrGLTexture structures that describe our render target texture */
	
	// for(int i=0; i<2; i++) {
	// 	fb_ovr_tex[i].OGL.Header.API = ovrRenderAPI_OpenGL;
	// 	fb_ovr_tex[i].OGL.Header.TextureSize.w = fb_tex_width;
	// 	fb_ovr_tex[i].OGL.Header.TextureSize.h = fb_tex_height;
	// 	/* this next field is the only one that differs between the two eyes */
	// 	fb_ovr_tex[i].OGL.Header.RenderViewport.Pos.x = i == 0 ? 0 : fb_width / 2.0;
	// 	fb_ovr_tex[i].OGL.Header.RenderViewport.Pos.y = 0;
	// 	fb_ovr_tex[i].OGL.Header.RenderViewport.Size.w = fb_width / 2.0;
	// 	fb_ovr_tex[i].OGL.Header.RenderViewport.Size.h = fb_height;
	// 	fb_ovr_tex[i].OGL.TexId = m_colorBufferTexture;	/* both eyes will use the same texture id */
	// }

	// /* fill in the ovrGLConfig structure needed by the SDK to draw our stereo pair
	//  * to the actual HMD display (SDK-distortion mode)
	//  */
	// memset(&glcfg, 0, sizeof glcfg);
	// glcfg.OGL.Header.API = ovrRenderAPI_OpenGL;
	// glcfg.OGL.Header.BackBufferSize.w = win_width;
	// glcfg.OGL.Header.BackBufferSize.h = win_height;
	// glcfg.OGL.Header.Multisample = 1;

	// // glcfg.OGL.Disp = glXGetCurrentDisplay();
	// // printf("GLX Display: %p\n", glcfg.OGL.Disp);

	// if(hmd->HmdCaps & ovrHmdCap_ExtendDesktop) {
	// 	printf("running in \"extended desktop\" mode\n");
	// } else {
	// 	printf("running in \"direct-hmd\" mode\n");
	// 	printf(" \"direct-hmd\" mode not supported!\n");
	// 	initialized=false;
	// 	return;
	// }

	/* enable low-persistence display and dynamic prediction for lattency compensation */
	hmd_caps = ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction;
	ovrHmd_SetEnabledCaps(hmd, hmd_caps);


	// /* configure SDK-rendering and enable OLED overdrive and timewrap, which
	//  * shifts the image before drawing to counter any lattency between the call
	//  * to ovrHmd_GetEyePose and ovrHmd_EndFrame.
	//  */
	// distort_caps =  ovrDistortionCap_Overdrive;
	// if(!ovrHmd_ConfigureRendering(hmd, &glcfg.Config, distort_caps, hmd->DefaultEyeFov, eye_rdesc)) {
	// 	fprintf(stderr, "failed to configure distortion renderer\n");
	// 	// initialized =false;
	// 	// return;
	// }

	distortionCaps =  ovrDistortionCap_Overdrive;

	float near = .01f, far = 10.0f;
	RenderToTextureDisplay::DistortionMesh distortionMeshes[2];
	for(int i=0; i<2; i++) {
		ovrEyeType eye = hmd->EyeRenderOrder[i];
		ovrDistortionMesh mesh;
		ovrHmd_CreateDistortionMesh(hmd, eye, hmd->DefaultEyeFov[eye], distortionCaps, &mesh);
		assert(sizeof(ovrDistortionVertex) == sizeof(RenderToTextureDisplay::DistortionVertex));
		

		distortionMeshes[i].VertexCount = mesh.VertexCount;
		size_t vertexDataSize = mesh.VertexCount * sizeof(RenderToTextureDisplay::DistortionVertex);
		distortionMeshes[i].VertexData = (RenderToTextureDisplay::DistortionVertex *) malloc(vertexDataSize);
		memcpy(distortionMeshes[i].VertexData, mesh.pVertexData, vertexDataSize);

		distortionMeshes[i].IndexCount = mesh.IndexCount;
		size_t indexDataSize = mesh.IndexCount * sizeof(unsigned short);
		distortionMeshes[i].IndexData = (unsigned short*) malloc(indexDataSize);
		memcpy(distortionMeshes[i].IndexData, mesh.pIndexData, indexDataSize);

		ovrHmd_DestroyDistortionMesh(&mesh);

		ovrEyeRenderDesc desc = ovrHmd_GetRenderDesc(hmd, eye, hmd->DefaultEyeFov[eye]);

		ovrVector2f uvScaleOffsetOut[2];
		ovrHmd_GetRenderScaleAndOffset(desc.Fov, hmd->Resolution, desc.DistortedViewport, uvScaleOffsetOut);
		distortionMeshes[i].EyeToSourceUVScale = glm::vec2(uvScaleOffsetOut[0].x, uvScaleOffsetOut[0].y);
        distortionMeshes[i].EyeToSourceUVOffset = glm::vec2(uvScaleOffsetOut[1].x, uvScaleOffsetOut[1].y);

		printf("size: %d, %d; viewport offest: %d, %d; viewport size %d, %d\n", hmd->Resolution.w, hmd->Resolution.h, 
																				desc.DistortedViewport.Pos.x, desc.DistortedViewport.Pos.y, 
																				desc.DistortedViewport.Size.w, desc.DistortedViewport.Size.h);

		proj[i] = ovrMatrix4f_Projection(desc.Fov, near, far, 1);

		glm::vec3 HmdToEyeViewOffset = -1.0f * glm::vec3(	desc.HmdToEyeViewOffset.x,
													desc.HmdToEyeViewOffset.y,
													desc.HmdToEyeViewOffset.z );

		ViewPoint *cam = new ViewPoint(near, far, this, this,
	                                 glm::translate(glm::mat4(), HmdToEyeViewOffset),
	                                 glm::vec4(0.5f - (i * 0.5f),0.0f,.5f,1.0f), glm::vec3(0));
		addViewpoint(cam);



		
	}

	setDistortionMesh(distortionMeshes);

	





	// initialized =false;

}



OculusHMD::~OculusHMD()
{
	ovrHmd_Destroy(hmd);
	ovr_Shutdown();
}

void myOvrLogCallback(int level, const char* message){
    printf("OVR_LOG: %s\n", message);
}
void OculusHMD::initializeLibOVR() {
    ovrInitParams initparams = {ovrInit_Debug, 0, myOvrLogCallback, 0 };
    ovr_Initialize(&initparams);
}