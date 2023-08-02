#include "EnginePCH.h"

#include "Material.h"
#include "ConstBuffer.h"

#include "json-cpp/json.h"
#include "ResMgr.h"

namespace mh
{
    using namespace mh::math;

    Material::Material()
        : IRes(eResourceType::Material)
        , mCB{}
        , mMode(eRenderingMode::Opaque)
    {

    }

    Material::~Material()
    {

    }

    eResult Material::Load(const std::filesystem::path& _path)
    {
        return eResult::Fail_NotImplemented;
    }

    eResult Material::SaveJson(Json::Value* _pJVal)
    {
        //nullptr 확인
        if (nullptr == _pJVal)
        {
            return eResult::Fail_Nullptr;
        }

        //부모 클래스 저장
        eResult result = IRes::SaveJson(_pJVal);
        if (eResultFail(result))
        {
            return result;
        }

        //레퍼런스로 바꾸고
        Json::Value& jVal = *_pJVal;

        //포인터의 경우에는 포인터 자체를 저장하는게 아니라 Key를 저장
        if (mShader)
        {
            jVal[JSONKEY(mShader)] = mShader->GetKey();
        }
            
        //mTexture은 텍스처 배열이므로 Key를 가져와서 저장
        for (int i = 0; i < (int)eTextureSlot::End; ++i)
        {
            if (mTexture[i])
            {
                jVal[JSONKEY(mTexture)].append(mTexture[i]->GetKey());
            }
            else
            {
                jVal[JSONKEY(mTexture)].append(Json::Value(Json::nullValue));
            }
        }


        //단순 Value의 경우에는 매크로로 바로 저장 가능
        MH_SAVE_VALUE(_pJVal, mMode);

        return eResult::Success;
    }

    eResult Material::LoadJson(const Json::Value* _pJVal)
    {
        //nullptr 확인
        if (nullptr == _pJVal)
        {
            return eResult::Fail_Nullptr;
        }

        //부모 클래스 저장
        eResult result = IRes::LoadJson(_pJVal);
        if (eResultFail(result))
        {
            return result;
        }
        const Json::Value& jVal = *_pJVal;

        //쉐이더 데이터가 있는지 확인하고
        if (jVal.isMember(JSONKEY(mShader)))
        {
            //가져온 키값으로 쉐이더를 로드
            ResMgr::Load<GraphicsShader>(jVal[JSONKEY(mShader)].asString());
        }

        
        //포인터 배열은 MHGetJsonArrayPtr 함수를 통해서 Key값을 싹 받아올 수 있음.
        const auto& vecLoad = Json::MHGetJsonArrayPtr(_pJVal, JSONKEY(mTexture));
        for (size_t i = 0; i < vecLoad.size(); ++i)
        {
            mTexture[i] = ResMgr::Load<Texture>(vecLoad[i]);
        }


        //단순 Value의 경우에는 매크로로 바로 저장 가능
        MH_LOAD_VALUE(_pJVal, mMode);

        return eResult();
    }

    void Material::SetData(eGPUParam _param, void* _data)
    {
        /*switch (_param)
        {
        case mh::eGPUParam::Int:
            mCB.iData = *static_cast<int*>(_data);
            break;
        case mh::eGPUParam::Float:
            mCB.fData = *static_cast<float*>(_data);
            break;
        case mh::eGPUParam::Vector2:
            mCB.XY = *static_cast<Vector2*>(_data);
            break;
        case mh::eGPUParam::Vector3:
            mCB.XYZ = *static_cast<Vector3*>(_data);
            break;
        case mh::eGPUParam::Vector4:
            mCB.XYZW = *static_cast<Vector4*>(_data);
            break;
        case mh::eGPUParam::Matrix:
            mCB.Matrix = *static_cast<Matrix*>(_data);
            break;
        default:
            break;
        }*/

    }

    void Material::Bind()
    {
        for (size_t slotIndex = 0; slotIndex < (UINT)eTextureSlot::End; slotIndex++)
        {
            if (mTexture[slotIndex] == nullptr)
            {
				continue;
            }

            mTexture[slotIndex]->BindDataSRV((UINT)slotIndex, eShaderStageFlag::ALL);
        }

        if (mTexture[(UINT)eTextureSlot::Albedo])
        {
            mCB.UsedAlbedo = 1;
        }

        if (mTexture[(UINT)eTextureSlot::Normal])
        {
            mCB.UsedNormal = 1;
        }

        ConstBuffer* CB = RenderMgr::GetConstBuffer(eCBType::Material);
        CB->SetData(&mCB);

        eShaderStageFlag_ flag = eShaderStageFlag::VS | eShaderStageFlag::GS | eShaderStageFlag::PS;
        CB->BindData(flag);

        mShader->Binds();
    }

    void Material::Clear()
    {
        for (size_t slotIndex = 0; slotIndex < (UINT)eTextureSlot::End; slotIndex++)
        {
            if (mTexture[slotIndex] == nullptr)
			{
				continue;
			}

            mTexture[slotIndex]->UnBind();
        }
    }
}
