#ifndef EE_GAMINGMAPHELPER_HPP
#define EE_GAMINGMAPHELPER_HPP

namespace EE { namespace Gaming {

#define MAP_PROPERTY_SIZE			(64)
#define LAYER_NAME_SIZE				(64)
#define MAP_TEXTUREATLAS_PATH_SIZE	(128)

typedef struct sPropertyHdrS {
	char	Name[ MAP_PROPERTY_SIZE ];
	char	Value[ MAP_PROPERTY_SIZE ];
} sPropertyHdr;

typedef struct sMapTextureAtlasS {
	char	Path[ MAP_TEXTUREATLAS_PATH_SIZE ];
} sMapTextureAtlas;

typedef struct sVirtualObjS {
	char	Name[ MAP_PROPERTY_SIZE ];
} sVirtualObj;

typedef struct sMapHdrS {
	Uint32	Magic;
	Uint32	SizeX;
	Uint32	SizeY;
	Uint32	TileSizeX;
	Uint32	TileSizeY;
	Uint32	MaxLayers;
	Uint32	LayerCount;
	Uint32	Flags;
	Uint32	PropertyCount;
	Uint32	TextureAtlasCount;
	Uint32	VirtualObjectTypesCount;
	Uint32	BaseColor;
	Uint32	LightsCount;
} sMapHdr;

typedef struct sLayerHdrS {
	char	Name[ LAYER_NAME_SIZE ];
	Uint32	Type;
	Uint32	Flags;
	Int32	OffsetX;
	Int32	OffsetY;
	Uint32	PropertyCount;
	Uint32	ObjectCount;		//! Only used by the Object Layer
} sLayerHdr;

typedef struct sMapTileGOHdrS {
	Uint32	Type;
	Uint32	Id;
	Uint32	Flags;
} sMapTileGOHdr;

typedef struct sMapObjGOHdrS {
	Uint32	Type;
	Uint32	Id;
	Uint32	Flags;
	Int32	PosX;
	Int32	PosY;
} sMapObjGOHdr;

typedef struct sMapLightHdrS {
	Uint32	Radius;
	Int32	PosX;
	Int32	PosY;
	Uint32	Color;
	Uint32	Type;
} sMapLightHdr;

class GObjFlags {
	public:
		enum EE_GAMEOBJECT_FLAGS {
			GAMEOBJECT_STATIC				= ( 1 << 0 ),
			GAMEOBJECT_ANIMATED				= ( 1 << 1 ),
			GAMEOBJECT_MIRRORED				= ( 1 << 2 ),
			GAMEOBJECT_FLIPED				= ( 1 << 3 ),
			GAMEOBJECT_BLOCKED				= ( 1 << 4 ),
			GAMEOBJECT_ROTATE_90DEG			= ( 1 << 5 ),
			GAMEOBJECT_AUTO_FIX_TILE_POS	= ( 1 << 6 )
		};
};

enum EE_GAMEOBJECT_TYPE {
	GAMEOBJECT_TYPE_BASE			= 236430550u,	//String::Hash( "Base" ),
	GAMEOBJECT_TYPE_VIRTUAL			= 4069800883u,	//String::Hash( "Virtual" ),
	GAMEOBJECT_TYPE_SHAPE			= 3517332124u,	//String::Hash( "Shape" ) == 3517332124u for backward compatibility
	GAMEOBJECT_TYPE_SUBTEXTURE		= 1772101792u,	//String::Hash( "SubTexture" ) == 1772101792u,
	GAMEOBJECT_TYPE_SHAPEEX			= 3708695628u,	//String::Hash( "ShapeEx" ) == 3708695628u for backward compatibility
	GAMEOBJECT_TYPE_SUBTEXTUREEX	= 1378537981u,	//String::Hash( "SubTextureEx" ) == 1378537981u,
	GAMEOBJECT_TYPE_SPRITE			= 2088954976u	//String::Hash( "Sprite" )
};

enum EE_LAYER_TYPE {
	MAP_LAYER_TILED,
	MAP_LAYER_OBJECT
};

enum EE_MAP_FLAGS {
	MAP_FLAG_CLAMP_BORDERS		= ( 1 << 0 ),
	MAP_FLAG_CLIP_AREA			= ( 1 << 1 ),
	MAP_FLAG_DRAW_GRID			= ( 1 << 2 ),
	MAP_FLAG_DRAW_TILE_OVER		= ( 1 << 3 ),
	MAP_FLAG_DRAW_BACKGROUND	= ( 1 << 4 ),
	MAP_FLAG_LIGHTS_ENABLED		= ( 1 << 5 ),
	MAP_FLAG_LIGHTS_BYVERTEX	= ( 1 << 6 ),
	MAP_FLAG_SHOW_BLOCKED		= ( 1 << 7 )
};

#define MAP_EDITOR_DEFAULT_FLAGS ( MAP_FLAG_CLAMP_BORDERS | MAP_FLAG_CLIP_AREA | MAP_FLAG_DRAW_GRID | MAP_FLAG_DRAW_BACKGROUND )

enum EE_LAYER_FLAGS {
	LAYER_FLAG_VISIBLE			= ( 1 << 0 ),
	LAYER_FLAG_LIGHTS_ENABLED	= ( 1 << 1 )
};

}}

#endif