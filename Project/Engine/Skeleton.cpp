#include "PCH_Engine.h"
#include "Skeleton.h"

#include "define_Util.h"
#include "DefaultShader/SH_Resource.hlsli"
#include "StructBuffer.h"
#include "FBXLoader.h"
#include "PathMgr.h"
#include "Animation3D.h"

namespace mh
{
	using namespace mh::define;
	Skeleton::Skeleton()
		: m_vecBones{}
		, m_pBoneOffset{}
		, mMapAnimations{}
	{
	}

	Skeleton::~Skeleton()
	{
	}

	eResult Skeleton::Save(const std::fs::path& _filePath, const std::fs::path& _basePath)
	{
		std::fs::path fullPath = _basePath;
		if (fullPath.empty())
		{
			fullPath = PathMgr::GetContentPathRelative(eResourceType::MeshData);
		}
		fullPath /= _filePath;

		{
			std::fs::path checkDir = fullPath.parent_path();
			if (false == std::fs::exists(checkDir))
			{
				std::fs::create_directories(checkDir);
			}
		}
		fullPath.replace_extension(strKey::Ext_Skeleton);

		std::ofstream ofs(fullPath, std::ios::binary);
		if (false == ofs.is_open())
		{
			ERROR_MESSAGE_W(L"Bone 저장에 실패했습니다.");
			return eResult::Fail_OpenFile;
		}

		Binary::SaveValue(ofs, m_vecBones.size());
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			Binary::SaveStr(ofs, m_vecBones[i].strBoneName);
			Binary::SaveValue(ofs, m_vecBones[i].Values);
		}

		Binary::SaveValue(ofs, mMapAnimations.size());
		for (auto& iter : mMapAnimations)
		{
			Binary::SaveStr(ofs, iter.first);
			eResult result = iter.second->Save(iter.first, _basePath);
			if (eResultFail(result))
			{
				ERROR_MESSAGE_W(L"애니메이션 저장 실패.");
				return result;
			}
		}

		return eResult::Success;
	}
	eResult Skeleton::Load(const std::fs::path& _filePath, const std::fs::path& _basePath)
	{
		std::fs::path fullPath = _basePath;
		if (fullPath.empty())
		{
			fullPath = PathMgr::GetContentPathRelative(eResourceType::MeshData);
		}
		fullPath /= _filePath;
		fullPath.replace_extension(strKey::Ext_Skeleton);
		if (false == std::fs::exists(fullPath))
		{
			ERROR_MESSAGE_W(L"파일이 없습니다.");
			return eResult::Fail_PathNotExist;
		}

		std::ifstream ifs(fullPath, std::ios::binary);
		if (false == ifs.is_open())
		{
			ERROR_MESSAGE_W(L"Bone 저장에 실패했습니다.");
			return eResult::Fail_OpenFile;
		}

		//std::vector<define::tMTBone>			m_vecBones;
				//std::vector<define::tMTBone>			m_vecBones;
		{
			size_t size{};
			Binary::LoadValue(ifs, size);
			m_vecBones.resize(size);
			for (size_t i = 0; i < size; ++i)
			{
				Binary::LoadStr(ifs, m_vecBones[i].strBoneName);
				Binary::LoadValue(ifs, m_vecBones[i].Values);
			}
		}
		CreateBoneOffsetSBuffer();


		size_t mapSize{};
		Binary::LoadValue(ifs, mapSize);
		for (size_t i = 0; i < mapSize; ++i)
		{
			std::string animName{};
			Binary::LoadStr(ifs, animName);
			
			std::unique_ptr<Animation3D> anim3d = std::make_unique<Animation3D>();
			anim3d->SetSkeleton(this);
			eResult result = anim3d->Load(animName, _basePath);
			if (eResultFail(result))
			{
				ERROR_MESSAGE_W(L"애니메이션 로드 실패.");
				return eResult::Fail_InValid;
			}

			mMapAnimations.insert(std::make_pair(animName, anim3d.release()));
		}
		return eResult::Success;
	}

	eResult Skeleton::CreateFromFBX(FBXLoader* _fbxLoader)
	{
		const std::vector<tFBXBone>& vecBone = _fbxLoader->GetBones();
		if (vecBone.empty())
		{
			return eResult::Fail_Empty;
		}

		UINT iFrameCount = 0;
		m_vecBones.reserve(vecBone.size());
		for (UINT i = 0; i < vecBone.size(); ++i)
		{
			m_vecBones.push_back(tMTBone{});
			tMTBone& bone = m_vecBones.back();

			bone.Values.iDepth = vecBone[i].Depth;
			bone.Values.iParentIndx = vecBone[i].ParentIndx;
			bone.Values.matBone = FBXLoader::GetMatrixFromFbxMatrix(vecBone[i].matBone);
			bone.Values.matOffset = FBXLoader::GetMatrixFromFbxMatrix(vecBone[i].matOffset);
			bone.strBoneName = vecBone[i].strBoneName;
		}
		CreateBoneOffsetSBuffer();

		const auto& animClip = _fbxLoader->GetAnimations();
		for (size_t i = 0; i < animClip.size(); ++i)
		{
			std::unique_ptr<Animation3D> anim = std::make_unique<Animation3D>();

			eResult result = anim->LoadFromFBX(this, &animClip[i]);
			if (eResultFail(result))
			{
				ERROR_MESSAGE_W(L"애니메이션 생성 실패");
				return result;
			}
			
			std::string animName = anim->GetKey();
			if (animName.empty())
			{
				//애니메이션이 1000개를 넘을거같진 않으니 3자리까지만 고정
				size_t numAnim = animClip.size();
				int digits = 3;
				do
				{
					numAnim /= (size_t)10;
					--digits;
				} while (numAnim);

				if (digits < 0)
					digits = 0;

				animName = "Anim_";
				for (int i = 0; i < digits; ++i)
				{
					animName += "0";
				}
				
				animName += std::to_string(i);
			}

			mMapAnimations.insert(std::make_pair(animName, anim.release()));
		}
		return eResult::Success;
	}

	void Skeleton::CreateBoneOffsetSBuffer()
	{
		// BoneOffet 행렬
		std::vector<MATRIX> vecOffset;
		vecOffset.reserve(m_vecBones.size());
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			vecOffset.push_back(m_vecBones[i].Values.matOffset);
		}

		//Create
		m_pBoneOffset = std::make_unique<StructBuffer>();
		tSBufferDesc Desc{};
		Desc.eSBufferType = eStructBufferType::READ_ONLY;
		Desc.REGISLOT_t_SRV = Register_t_g_OffsetArray;
		m_pBoneOffset->SetDesc(Desc);
		m_pBoneOffset->Create<MATRIX>(vecOffset.size(), vecOffset.data(), vecOffset.size());
	}

	std::shared_ptr<Animation3D> Skeleton::FindAnimation(const std::string& _strAnimName)
	{
		std::shared_ptr<Animation3D> retPtr = nullptr;
		const auto& iter = mMapAnimations.find(_strAnimName);
		if (iter != mMapAnimations.end())
		{
			retPtr = iter->second;
		}
		return retPtr;
	}
}

