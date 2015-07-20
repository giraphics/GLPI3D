#ifndef VAO_h
#define VAO_h
#include "glutils.h"

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

#endif
