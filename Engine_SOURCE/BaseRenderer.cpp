#include "BaseRenderer.h"

namespace mh
{
	BaseRenderer::BaseRenderer(enums::eComponentType _type)
		: Component(_type)
		, mMesh(nullptr)
		, mMaterial(nullptr)
	{

	}

	BaseRenderer::~BaseRenderer()
	{
	}

	void BaseRenderer::Initialize()
	{
	}

	void BaseRenderer::Update()
	{
	}

	void BaseRenderer::FixedUpdate()
	{
	}

	void BaseRenderer::Render()
	{
	}

}
