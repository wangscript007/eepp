#ifndef EE_GRAPHICSCGLOBALTEXTUREATLAS_HPP
#define EE_GRAPHICSCGLOBALTEXTUREATLAS_HPP

#include <eepp/graphics/base.hpp>
#include <eepp/graphics/ctextureatlas.hpp>

namespace EE { namespace Graphics {

/** @brief Any SubTexture that doesn't belong to an specific TextureAtlas ( a real texture atlas texture ), goes here. */
class EE_API cGlobalTextureAtlas : public cTextureAtlas {
	SINGLETON_DECLARE_HEADERS(cGlobalTextureAtlas)

	public:
		cGlobalTextureAtlas();

		~cGlobalTextureAtlas();
};

}}

#endif