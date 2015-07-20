#ifndef GEOMETRY_MESH_h
#define GEOMETRY_MESH_h

struct positionInfo{
	void* positionData;
	int size;
};

struct normalInfo{
	void* normalData;
	int size;
};

struct tangentInfo{
	void* tagnetData;
	int size;
};

struct texCoordInfo{
	void* textureData;
	int size;
};

struct colorInfo{
	void* colorData;
	int size;
};

struct binormalInfo{
	void* binormalData;
	int size;
};

struct indexInfo{
	void* indexData;
	int size;
};

class GeometryMesh{
public:
    // Store the geometry vertices
    positionInfo positions;

	// Store the Normals
    normalInfo normals;
    
    // Store the Tangents information
    tangentInfo tangents;
    
    // Store the Primary Texture Coordinates information
    texCoordInfo texCoords;
    
    // Store the Secondary Texture Coordinates information
    texCoordInfo texCoordsSec;
    
    // Store the Color information
    colorInfo colors;

    // Store the Binormals for bump mapping
    binormalInfo binormals;

    // Store the Index information
    indexInfo geometryIndices;

public:
    GeometryMesh()
    {
		positions.positionData		= NULL;
		positions.size				= NULL;

		normals.normalData			= NULL;
		normals.size				= NULL;

		tangents.tagnetData			= NULL;
		tangents.size				= NULL;

		texCoords.textureData		= NULL;
		texCoords.size				= NULL;

		texCoordsSec.textureData	= NULL;
		texCoordsSec.size			= NULL;

		colors.colorData			= NULL;
		colors.size					= NULL;

		binormals.binormalData      = NULL;
		binormals.size				= NULL;

		geometryIndices.indexData	= NULL;
		geometryIndices.size		= NULL;
    }
};

#endif
