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

using namespace motorcar;

void OculusHMD::prepareForDraw()
{

    RenderToTextureDisplay::prepareForDraw();

    ovrPosef outEyePoses[2];
    ovrTrackingState outHmdTrackingState;

	ovrHmd_GetEyePoses(m_hmd, m_frameIndex, m_hmdToEyeViewOffset, outEyePoses, &outHmdTrackingState);

	ovrPosef pose = outHmdTrackingState.HeadPose.ThePose;
	glm::vec3 position = glm::vec3(pose.Position.x, pose.Position.y, pose.Position.z);
    glm::quat orientation = glm::make_quat(&pose.Orientation.x);
    glm::mat4 transform = glm::translate(glm::mat4(), position) * glm::mat4_cast(orientation);
    this->setTransform(transform);

    /* the drawing starts with a call to ovrHmd_BeginFrame */
	//ovrHmd_BeginFrame(hmd, 0);



}


void OculusHMD::finishDraw()
{
	RenderToTextureDisplay::finishDraw();

	//ovrHmd_EndFrame(hmd, pose, &fb_ovr_tex[0].Texture);
	m_frameIndex++;

}


OculusHMD::OculusHMD(Skeleton *skeleton, OpenGLContext *glContext, PhysicalNode *parent)
    :RenderToTextureDisplay(glContext, glm::vec2(0.126, 0.0706), parent, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.10f)))
    , m_initialized(false)
    , m_frameIndex(0)
{
    
    int win_width, win_height;
    int fb_width, fb_height;

	printf("detected %d hmds\n", ovrHmd_Detect());
	if(!(m_hmd = ovrHmd_Create(0))) {
		fprintf(stderr, "failed to open Oculus HMD, falling back to virtual debug HMD\n");
		if(!(m_hmd = ovrHmd_CreateDebug(ovrHmd_DK2))) {
			fprintf(stderr, "failed to create virtual debug HMD\n");
			return;
		}
	}
	printf("initialized HMD: %s - %s\n", m_hmd->Manufacturer, m_hmd->ProductName);
	m_initialized = true;

	win_width = m_hmd->Resolution.w;
	win_height = m_hmd->Resolution.h;
	printf("Reported hmd size: %d, %d. Default Framebuffer size: %d, %d\n",win_width, win_height, glContext->defaultFramebufferSize().x, glContext->defaultFramebufferSize().y );

	//m_size = glm::vec2(win_width, win_height);

	/* enable position and rotation tracking */
	ovrHmd_ConfigureTracking(m_hmd, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position, 0);
	/* retrieve the optimal render target resolution for each eye */
	ovrSizei eyeres[2];
	eyeres[0] = ovrHmd_GetFovTextureSize(m_hmd, ovrEye_Left, m_hmd->DefaultEyeFov[0], 1.0);
	eyeres[1] = ovrHmd_GetFovTextureSize(m_hmd, ovrEye_Right, m_hmd->DefaultEyeFov[1], 1.0);

	/* and create a single render target texture to encompass both eyes */
	fb_width = eyeres[0].w + eyeres[1].w;
	fb_height = eyeres[0].h > eyeres[1].h ? eyeres[0].h : eyeres[1].h;

	setRenderTargetSize(glm::ivec2(fb_width, fb_height));

	/* enable low-persistence display and dynamic prediction for lattency compensation */
	unsigned int hmdCaps = ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction;
	ovrHmd_SetEnabledCaps(m_hmd, hmdCaps);


	unsigned int distortionCaps =  ovrDistortionCap_Overdrive;

	float near = .01f, far = 10.0f;
	RenderToTextureDisplay::DistortionMesh distortionMeshes[2];
	for(int i=0; i<2; i++) {
		ovrEyeType eye = m_hmd->EyeRenderOrder[i];
		ovrDistortionMesh mesh;
		ovrHmd_CreateDistortionMesh(m_hmd, eye, m_hmd->DefaultEyeFov[eye], distortionCaps, &mesh);
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
		ovrEyeRenderDesc renderDescriptors[2];
		renderDescriptors[i] = ovrHmd_GetRenderDesc(m_hmd, eye, m_hmd->DefaultEyeFov[eye]);
		m_hmdToEyeViewOffset[i] = renderDescriptors[i].HmdToEyeViewOffset;

		ovrVector2f uvScaleOffsetOut[2];
		ovrHmd_GetRenderScaleAndOffset(m_hmd->DefaultEyeFov[eye], m_hmd->Resolution, renderDescriptors[i].DistortedViewport, uvScaleOffsetOut);
		distortionMeshes[i].EyeToSourceUVScale = glm::vec2(uvScaleOffsetOut[0].x, uvScaleOffsetOut[0].y);
        distortionMeshes[i].EyeToSourceUVOffset = glm::vec2(uvScaleOffsetOut[1].x, uvScaleOffsetOut[1].y);

		printf("size: %d, %d; viewport offest: %d, %d; viewport size %d, %d\n", m_hmd->Resolution.w, m_hmd->Resolution.h, 
																				renderDescriptors[i].DistortedViewport.Pos.x, renderDescriptors[i].DistortedViewport.Pos.y, 
																				renderDescriptors[i].DistortedViewport.Size.w, renderDescriptors[i].DistortedViewport.Size.h);

		glm::vec4 normalizedViewportParams = glm::vec4( renderDescriptors[i].DistortedViewport.Pos.x / ((float)m_hmd->Resolution.w),
														renderDescriptors[i].DistortedViewport.Pos.y / ((float)m_hmd->Resolution.h),
														renderDescriptors[i].DistortedViewport.Size.w / ((float)m_hmd->Resolution.w),
														renderDescriptors[i].DistortedViewport.Size.h / ((float)m_hmd->Resolution.h));

		ovrMatrix4f ovrProjection = ovrMatrix4f_Projection(m_hmd->DefaultEyeFov[eye], near, far, ovrProjection_ClipRangeOpenGL | ovrProjection_RightHanded);
		float projectionOverrideValues[16];
		for(int j = 0; j < 4; j ++){
			for(int k = 0; k < 4; k++){
				projectionOverrideValues[j * 4 + k] = ovrProjection.M[j][k];
			}

		}
		glm::mat4 projectionOverride = glm::transpose(glm::make_mat4(projectionOverrideValues));


		glm::vec3 HmdToEyeViewOffset = -1.0f * glm::vec3(m_hmdToEyeViewOffset[i].x,
													m_hmdToEyeViewOffset[i].y,
													m_hmdToEyeViewOffset[i].z );

		ViewPoint *vp = new ViewPoint(near, far, this, this,
	                                 glm::translate(glm::mat4(), HmdToEyeViewOffset),
	                                 normalizedViewportParams, glm::vec3(0));
		vp->overrideProjectionMatrix(projectionOverride);
		addViewpoint(vp);



		
	}

	setDistortionMesh(distortionMeshes);

	





	// m_initialized =false;

}



OculusHMD::~OculusHMD()
{
	ovrHmd_Destroy(m_hmd);
	ovr_Shutdown();
}

void myOvrLogCallback(int level, const char* message){
    printf("OVR_LOG: %s\n", message);
}
void OculusHMD::initializeLibOVR() {
    ovrInitParams initparams = {ovrInit_Debug, 0, myOvrLogCallback, 0 };
    ovr_Initialize(&initparams);
}