#ifndef SH_RESOURCE
#define SH_RESOURCE

#include "SH_CommonStruct.hlsli"

//t 버퍼

//Material Default Texture
TEXTURE2D(Tex_0, t, 0);
TEXTURE2D(Tex_1, t, 1);
TEXTURE2D(Tex_2, t, 2);
TEXTURE2D(Tex_3, t, 3);
TEXTURE2D(Tex_4, t, 4);
TEXTURE2D(Tex_5, t, 5);
TEXTURE2D(Tex_6, t, 6);
TEXTURE2D(Tex_7, t, 7);


//3D Material
TEXTURE2D(AlbedoTexture, t, 0);
TEXTURE2D(NormalTexture, t, 1);
TEXTURE2D(SpecularTexture, t, 2);
TEXTURE2D(EmissiveTexture, t, 3);
#define IsAlbedoTex bTex_0
#define IsNormalTex bTex_1
#define IsSpecularTex bTex_2
#define IsEmissiveTex bTex_3


//Deffered + Light MRT
TEXTURE2D(AlbedoTarget, t, 0);
TEXTURE2D(NormalTarget, t, 1);
TEXTURE2D(SpecularTarget, t, 2);
TEXTURE2D(EmissiveTarget, t, 3);
TEXTURE2D(PositionTarget, t, 4);
TEXTURE2D(DiffuseLightTarget, t, 5);
TEXTURE2D(SpecularLightTarget, t, 6);




//Bone Matrices(Animation 3D)
SBUFFER(g_FrameTransArray, tAnimKeyframeTranslation, t, 16);
SBUFFER(g_OffsetArray, Matrix, t, 17);
SBUFFER(g_ChangeFrameTransArray, tAnimKeyframeTranslation, t, 18);

//최종 행렬이 저장되는 구조화버퍼
SBUFFER(g_BoneMatrixArray, Matrix, t, 19);
//SBUFFER(g_InstancingBoneMatrixArray, Matrix, t, 18);
SBUFFER_RW(g_FinalBoneMatrixArray, Matrix, u, 0);

SBUFFER_RW(g_BoneSocketMatrixArray, tOutputBoneInfo, u, 1);
SBUFFER_RW(g_InstancingBoneMatrixArray, Matrix, u, 2);




// Light
SBUFFER(lightAttributes, tLightAttribute, t, 14);


//StructuredBuffer<LightAttribute> lightAttributes : register(t13);
//StructuredBuffer<LightAttribute> lightAttributes3D : register(t14);




// noise
TEXTURE2D(NoiseTexture, t, 16);



// postProcess
TEXTURE2D(postProcessTexture, t, 60);
TEXTURE2D(guiGameTexture, t, 61);


#endif
