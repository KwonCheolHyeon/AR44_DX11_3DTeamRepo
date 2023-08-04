#include "EnginePCH.h"

#include "IRenderer.h"
#include "json-cpp\json.h"
#include "ResMgr.h"

namespace mh
{
	IRenderer::IRenderer()
		: IComponent(define::eComponentType::Renderer)
		, mMesh(nullptr)
		, mMaterial(nullptr)
	{
	}

	IRenderer::~IRenderer()
	{
	}

	eResult IRenderer::SaveJson(Json::Value* _pJson)
	{
		if (nullptr == _pJson)
		{
			return eResult::Fail_Nullptr;
		}
		eResult result = IComponent::SaveJson(_pJson);
		if (eResultFail(result))
		{
			return result;
		}
		Json::Value& jVal = *_pJson;

		//포인터의 경우 키값을 저장
		if (mMesh)
		{
			jVal[JSON_KEY(mMesh)] = mMesh->GetKey();
		}
		if (mMaterial)
		{
			jVal[JSON_KEY(mMaterial)] = mMaterial->GetKey();
		}
		
		return eResult::Success;
	}

	eResult IRenderer::LoadJson(const Json::Value* _pJson)
	{
		if (nullptr == _pJson)
		{
			return eResult::Fail_Nullptr;
		}
		eResult result = IComponent::LoadJson(_pJson);
		if (eResultFail(result))
		{
			return result;
		}
		const Json::Value& jVal = *_pJson;

		if (jVal.isMember(JSON_KEY(mMesh)))
		{
			mMesh = ResMgr::Load<Mesh>(jVal[JSON_KEY(mMesh)].asString());
		}

		if (jVal.isMember(JSON_KEY(mMaterial)))
		{
			mMaterial = ResMgr::Load<Material>(jVal[JSON_KEY(mMaterial)].asString());
		}

		return eResult::Success;
	}

}



