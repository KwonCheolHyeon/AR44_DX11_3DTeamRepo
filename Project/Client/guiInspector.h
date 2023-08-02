#pragma once
#include "guiWidget.h"
#include <Engine/GameObject.h>
#include "guiComponent.h"
#include "guiResource.h"

namespace gui
{
	class Inspector : public Widget
	{
	public:
		Inspector();
		virtual ~Inspector();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		mh::GameObject* GetTargetGameObject() const { return mTargetGameObject; }
		void SetTargetGameObject(mh::GameObject* target) { mTargetGameObject = target; }
		mh::IRes* GetTargetResource() const { return mTargetResource; }
		void SetTargetResource(mh::IRes* target) { mTargetResource = target; }

		void ClearTarget();
		void InitializeTargetGameObject();
		void InitializeTargetResource();

	private:
		mh::GameObject* mTargetGameObject;
		mh::IRes* mTargetResource;

		guiComponent* mTransform;
		std::vector<gui::guiComponent*> mComponents;
		std::vector<gui::IRes*> mResources;
	};
}