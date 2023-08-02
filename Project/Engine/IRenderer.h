#pragma once
#include "IComponent.h"
#include "Mesh.h"
#include "Material.h"

namespace mh
{
	class IRenderer : public IComponent
	{
	public:
		IRenderer();

		IRenderer(const IRenderer& _other) = default;

		virtual ~IRenderer();

		virtual eResult SaveJson(Json::Value* _pJson) override;
		virtual eResult LoadJson(const Json::Value* _pJson) override;


		//virtual void Init() override {};
		//virtual void Update() {};
		//virtual void FixedUpdate() override;
		virtual void Render() = 0;

		void SetMesh(const std::shared_ptr<Mesh> _mesh) { mMesh = _mesh; }
		void SetMaterial(const std::shared_ptr <Material> _shader) { mMaterial = _shader; }
		std::shared_ptr<Mesh> GetMesh() const { return mMesh; }
		std::shared_ptr<Material> GetMaterial() const { return mMaterial; }
		
	private:
		std::shared_ptr <Mesh> mMesh;
		std::shared_ptr <Material> mMaterial;
	};
}