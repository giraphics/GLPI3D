#ifndef VBO1_h
#define VBO1_h

// Miscellenous classes for VA, VBO, IBO, FBO, vertex attribute, shader program

#include "glutils.h"
#include "IModel.h"
#include "Uniforms.h"
#include <vector>
#include <map>

struct positionInfo{
	void* positionData;
	int size;
};

struct normalInfo{
	void* normalData;
	int size;
};

// 9988315536
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
    //std::vector<glm::vec3>* positions;
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
class VAO {
public:
	VAO();
	~VAO();

    void		bind();
    void		unbind();
protected:
    struct MemData{
		MemData(){}
		~MemData(){mId = 0;}

        GLuint	mId;
    };

private:
    MemData*	memObj;

};

class VBO {
public:
	VBO(GLenum aTarget);
	~VBO();

    void		bind();
    void		unbind();

    void		bufferData(size_t size, const void *data, GLenum usage);
    void		bufferSubData(ptrdiff_t offset, size_t size, const void *data);

    void*		map(GLenum access);
	void*		mapRange(size_t offset, size_t length, GLenum access);
    void		unmap();

    GLenum		getTarget() const { return memObj->mTarget; }
    GLuint		getId() const { return memObj->mId; }

protected:
    struct MemData{
		MemData(GLenum aTarget){mTarget = aTarget;}
		~MemData(){mTarget = 0; mId = 0;}

        GLenum			mTarget;
        GLuint			mId;
    };
private:
    MemData*	memObj;
};

enum BufferScheme{
	BUFFER_VA = 0,
	BUFFER_VBO = 1,
	BUFFER_VAO = 2,
};

enum DrawingScheme{
	DRAW_ARRAY = 0,
	DRAW_ELEMENT = 1,
};

enum PrimitiveScheme{
	PRIMITIVE_POINTS = 0,
	PRIMITIVE_LINES = 1,
	PRIMITIVE_LINE_LOOP = 2,
	PRIMITIVE_LINE_STRIP = 3,
	PRIMITIVE_TRIANGLES = 4,
	PRIMITIVE_TRIANGLE_STRIP = 5,
	PRIMITIVE_TRIANGLE_FAN = 6,
};

class Attribute{
public:
	Attribute(std::string name, GLint itemPerElement, size_t size, GLenum typeInfo, void* arr, int stride = 0);
	~Attribute();
	std::string name;
	void* dataArray;
	int index;
	int attributeLocation;
	GLenum type;
	GLint itemNum;
	size_t size;
};

class Indices{
public:
	Indices(size_t indexCount, GLenum typeInfo, void* arr){ size = indexCount; type = typeInfo; dataArray = arr; indexOrDataPtr = NULL;}
	
	~Indices(){}
	
	void* dataArray;

	// The indexOrDataPtr is common interface for drawing API for either (VA) or (VBO and VAO) 
	void* indexOrDataPtr;
	
	GLenum type;
	
	size_t size;
};

class GeometryBuffer{
public:
	GeometryBuffer(IModel* parent, BufferScheme scheme = BUFFER_VAO, DrawingScheme draw = DRAW_ARRAY, bool isInterleavedForm = false);
	~GeometryBuffer();

	void addAttribute(Attribute* attributeItem);
	void addUniform(Uniform* uniformItem);
	void setIndices(Indices* indexItem);
	void setInterleavedBuffer(void* dataArray, size_t size);
	void setDrawingPrimitive(PrimitiveScheme primitiveMode){ primitiveType = GetPrimitiveMode(primitiveMode); }
	void init();
	void initUniforms();
	void bind();
    void unbind();
    void update();
    void draw();
	inline GeometryMesh* geometry(){ return &geometryData; }
	inline DrawingScheme& DrawScheme(){return schemeDraw;}
	inline void setDrawingPrimitive(GLenum drawPrimitive){ primitiveType = drawPrimitive; }
private:
	void (GeometryBuffer::*drawMethod)();
    void drawArray();
    void drawElement();
	void populateVBO();
	void populateIBO();
	void enableAttributes();
	void vertexAttribPointerVA();
	void vertexAttribPointerVBO();
	GLenum GetPrimitiveMode(PrimitiveScheme primitiveMode);
	BufferScheme schemeBuf;
	DrawingScheme schemeDraw;
	GLenum primitiveType;
	int isInterleaved; // Using 4 bite align datatype for boolean for performance optimization.
	std::vector<Attribute*> attributeList;
	std::vector<Uniform*> uniformList;
	Indices* indexList;
	IModel* parent;
	VBO* vbo;
	VBO* ibo;
	VAO* vao;

	GLsizei startIndex;
	GLsizei itemCount;
public:
	GeometryMesh geometryData;
};

#endif
