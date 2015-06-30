#ifndef VBO1_h
#define VBO1_h

// Miscellenous classes for VA, VBO, IBO, FBO, vertex attribute, shader program

#include "glutils.h"
#include "IModel.h"
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
    std::vector<unsigned int>* geometryIndices;

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

class AttributeInfo{
public:
	AttributeInfo(std::string name, GLint itemPerElement, size_t size, GLenum typeInfo, void* arr);
	~AttributeInfo();
	std::string name;
	void* dataArray;
	int index;
	int attributeLocation;
	GLenum type;
	GLint itemNum;
	size_t size;
};

class GeometryBuffer{
public:
	GeometryBuffer(IModel* parent, BufferScheme scheme = BUFFER_VAO, bool isInterleaved = false);
	~GeometryBuffer();
	void addAttribute(AttributeInfo attr);
	void init();
	void bind();
    void unbind();
    void sendAttributeData();
	GeometryMesh* geometry();
private:
	BufferScheme schemeBuf;
	bool interleaved;
	std::vector<AttributeInfo> attributeList;
	IModel* parent;
	VBO* vbo;
	VAO* vao;

public:
	GeometryMesh geometryData;
};

#endif
