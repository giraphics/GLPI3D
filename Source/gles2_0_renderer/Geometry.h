#ifndef VBO1_h
#define VBO1_h

// Miscellenous classes for VA, VBO, IBO, FBO, vertex attribute, shader program

#include "glutils.h"
#include "IModel.h"
#include "Uniforms.h"
#include <vector>
#include <map>
class GeometryMesh{
public:
    // Store the geometry vertices
    std::vector<glm::vec3>* positions;
    
    // Store the Normals
    std::vector<glm::vec3>* normals;
    
    // Store the Tangents information
    std::vector<glm::vec3>* tangents;
    
    // Store the Primary Texture Coordinates information
    std::vector<glm::vec2>* texCoords;
    
    // Store the Secondary Texture Coordinates information
    std::vector<glm::vec2>* texCoordsSec;
    
    // Store the Bolor information
    std::vector<glm::vec3>* colors;

    // Store the Binormals for bump mapping
    std::vector<glm::vec3>* binormals;

    // Store the Index information
    std::vector<unsigned short>* geometryIndices;

public:
    GeometryMesh()
    {
        positions       = NULL;
        normals         = NULL;
        tangents        = NULL;
        texCoords       = NULL;
        texCoordsSec    = NULL;
        colors          = NULL;
        binormals       = NULL;
        geometryIndices = NULL;
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
	Attribute(std::string name, GLint itemPerElement, size_t size, GLenum typeInfo, void* arr);
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
	Indices(size_t indexCount, GLenum typeInfo, void* arr){ size = indexCount; type = typeInfo; dataArray = arr; }
	~Indices(){}
	void* dataArray;
	int index;
	GLenum type;
	size_t size;
};

class GeometryBuffer{
public:
	GeometryBuffer(IModel* parent, BufferScheme scheme = BUFFER_VAO, bool isInterleaved = false, DrawingScheme draw = DRAW_ARRAY);
	~GeometryBuffer();
	//void addAttribute(std::string name, GLint itemPerElement, size_t size, GLenum typeInfo, void* arr);
	void addAttribute(Attribute* attributeItem);
	void addUniform(Uniform* uniformItem);
	void setIndices(Indices* indexItem);
	void setDrawingPrimitive(PrimitiveScheme primitiveMode){ primitiveType = GetPrimitiveMode(primitiveMode); }
	void init();
	void initUniforms();
	void bind();
    void unbind();
    void update();
    void draw();
    //void sendAttributeData();
	inline GeometryMesh* geometry(){ return &geometryData; }
private:
	void (GeometryBuffer::*drawMethod)();
    void drawArray();
    void drawElement();
	GLenum GetPrimitiveMode(PrimitiveScheme primitiveMode);
	BufferScheme schemeBuf;
	DrawingScheme schemeDraw;
	GLenum primitiveType;
	bool interleaved;
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
