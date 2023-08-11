#include "PCH_Client.h"
#include "guiEntity.h"

namespace gui
{
	UINT32 guiEntity::gIDNext{};

	guiEntity::guiEntity(const std::string_view _strName)
		: mID(++gIDNext)
		, mName(_strName)
		, mStrKey(_strName)
	{
	}


	guiEntity::~guiEntity()
	{
	}
}


