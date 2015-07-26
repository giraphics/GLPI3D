#ifndef VBO1_h
#define VBO1_h

// Miscellenous classes for VA, VBO, IBO, FBO, vertex attribute, shader program

#include "Interfaces/IModel.h"
#include "Uniforms.h"
#include "VBO.h"
#include "VAO.h"
#include "Schemes.h"
#include "Attribute.h"
#include "Indices.h"
#include "GeometryMesh.h"

#include <vector>
#include <map>

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
	inline DrawingScheme& drawScheme(){return schemeDraw;}
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

	
	BufferScheme			schemeBuf;
	DrawingScheme			schemeDraw;
	GLenum					primitiveType;
	int						isInterleaved; // Using 4 byte align datatype for boolean for performance optimization.
	InterleavedBufferInfo	interleaveBuffer;
	
	std::vector<Attribute*> attributeList;
	std::vector<Uniform*>	uniformList;
	
	Indices*				indexList;
	IModel*					parent;

	VBO*					vbo;
	VBO*					ibo;
	VAO*					vao;

public:
	GeometryMesh geometryData;

};

#endif
