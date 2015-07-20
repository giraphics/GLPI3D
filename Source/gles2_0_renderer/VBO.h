#ifndef VBO_h
#define VBO_h
#include "glutils.h"

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

#endif
