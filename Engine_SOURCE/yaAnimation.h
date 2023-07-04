#pragma once
#include "yaEntity.h"
#include "yaTexture.h"

using namespace ya::math;
using namespace ya::graphics;
namespace ya
{
	class Animation : public Entity
	{
	public:
		struct Sprite
		{
			Vector2 leftTop;	// ���� ��� ��ǥ
			Vector2 size;		// ������ܺ��� �߶� ���� ������ �ȼ� ����
			Vector2 offset;		// ������ ��ġ �����ϱ����� ��ǥ
			Vector2 atlasSize;  // �ؽ�ó �̹����� ũ��
			float duration;		// �����Ӱ��� �ð� ����

			Sprite()
				: leftTop(0.0f, 0.0f)
				, size(0.0f, 0.0f)
				, offset(0.0f, 0.0f)
				, atlasSize(0.0f)
				, duration(0.1f)
			{

			}
		};

		Animation();
		~Animation();

		UINT Update();
		void FixedUpdate();
		void Render();

		void Create(const std::wstring& name, std::shared_ptr<Texture> atlas
					,Vector2 leftTop, Vector2 size, Vector2 offset
					,UINT spriteLegth, float duration);

		void BindShader();
		void Reset();
		void Clear();

		bool IsComplete() { return mbComplete; }
		std::wstring& AnimationName() { return mAnimationName; }

	private:
		class Animator* mAnimator;
		std::wstring mAnimationName;
		std::shared_ptr<Texture> mAtlas;
		std::vector<Sprite> mSpriteSheet;
		int mIndex;
		float mTime;
		bool mbComplete;
	};
}