#ifndef UNIFORM_h
#define UNIFORM_h
#include <vector>
#include "ProgramManager.h"
#include "glutils.h"

//#define CREATE_LOCAL_COPY

enum DataValueType{
    UNIFORM_1   = 0,
    UNIFORM_2   = 1,
    UNIFORM_3   = 2,
    UNIFORM_4   = 3,
    UNIFORM_2x2 = 4,
    UNIFORM_3x3 = 5,
    UNIFORM_4x4 = 6,
    UNIFORM_2x3 = 7,
    UNIFORM_2x4 = 8,
    UNIFORM_3x2 = 9,
    UNIFORM_3x4 = 10,
    UNIFORM_4x2 = 11,
    UNIFORM_4x3 = 12
};

enum UniformType{
    UNIFORM_PRIMITIVE = 0,
    UNIFORM_VECTOR = 1,
    UNIFORM_MATRIX = 2,
};


class Uniform
{

public:

    Uniform(/*PROGRAM* sh, */const std::string& uniformName/*, int location*/){ name = uniformName; isUpdated = true; }

    const std::string& GetName(){ return name;}

    //void AddOwner(PROGRAM* pShader, unsigned int location){}
	inline void SetUniformLocation(int location){ uniformLocation = location; }
	inline int&  GetUniformLocation() { return uniformLocation; }
	virtual void update() = 0;
protected:

    //void NotifyOwners(){}

protected:
    //std::vector<PROGRAM*> m_owner;  // List containing references to the owners needed so they can be notified.
    //std::vector<int> m_loc; // list of locations/handles for the GLSL uniform variable
	int uniformLocation;
    std::string name; // The name of the uniform variable in the GLSL program
	bool isUpdated;
};

template< typename T, int compositionCount, UniformType ti>
class UniformDerive : public Uniform
{
public:
    typedef T type;
    const enum attribs{ dataStructureItemCount = compositionCount, elementSize = sizeof(T) };
    //============================================================
    UniformDerive(/*PROGRAM* sh, */const std::string& name, /*int location,*/ int elementCnt=1, bool transposeFlag = false) : Uniform(/*sh,*/ name/*, location*/)
    {
        transpose = transposeFlag; // Transpose Flag is only necessary for 2D Matrix elements
        if (dataStructureItemCount > 9 && dataStructureItemCount < 99){ // 2 Digit number, treat this as 2 dimensional case
            elementCount  = (GLsizei)elementCnt;
            int firstDim    = dataStructureItemCount % 10;
            int secondDim   = dataStructureItemCount / 10;
			size			= secondDim*firstDim;
        }
        else if (dataStructureItemCount < 9){  // 1 Digit number, treat this as 1 dimensional case
            elementCount  = (GLsizei)elementCnt;
			size			= dataStructureItemCount;
        }
        else{   // More than 2 dimensional case which is incorrect
            printf("Incorrect element count specified for %s", name.c_str());
            assert(0);
        }
	#ifdef CREATE_LOCAL_COPY 
        elements      = new T[size*elementCount];
	#endif
    }
	
	~UniformDerive(){
	#ifdef CREATE_LOCAL_COPY 
		delete elements;
		elements = NULL;
	#endif
    }

    void SetValue(T* values)
    {
	#ifdef CREATE_LOCAL_COPY 
		memcpy(elements, values, size * attribs::elementSize);
	#else
		elements = values;
	#endif
		isUpdated = true;
    }

    void update()
    {
		if(!isUpdated || uniformLocation < 0){
			return;
    }

        if (std::is_same<T, GLfloat>::value)
        {
            if (ti == UniformType::UNIFORM_PRIMITIVE){
                switch (dataStructureItemCount)
                {
                case 1:
                    glUniform1f(uniformLocation, elements[0]);
                    break;
                case 2:
                    glUniform2f(uniformLocation, elements[0], elements[1]);
                    break;
                case 3:
                    glUniform3f(uniformLocation, elements[0], elements[1], elements[2]);
                    break;
                case 4:
                    glUniform4f(uniformLocation, elements[0], elements[1], elements[2], elements[3]);
                    break;
                default:
                    assert(0);
                    break;
                }
            }
            else if (ti == UniformType::UNIFORM_VECTOR){
                switch (dataStructureItemCount)
                {
                case 1:
                    glUniform1fv(uniformLocation, elementCount, (const GLfloat*)elements);
                    break;
                case 2:
                    glUniform2fv(uniformLocation, elementCount, (const GLfloat*)elements);
                    break;
                case 3:
                    glUniform3fv(uniformLocation, elementCount, (const GLfloat*)elements);
                    break;
                case 4:
                    glUniform4fv(uniformLocation, elementCount, (const GLfloat*)elements);
                    break;
                default:
                    assert(0);
                    break;
                }
            }
            else if (ti == UniformType::UNIFORM_MATRIX){
                switch (dataStructureItemCount)
                {
                case 22:
                    glUniformMatrix2fv(uniformLocation, elementCount, transpose, (const GLfloat*)elements);
                    break;
                case 33:
                    glUniformMatrix3fv(uniformLocation, elementCount, transpose, (const GLfloat*)elements);
                    break;
                case 44:
                    glUniformMatrix4fv(uniformLocation, elementCount, transpose, (const GLfloat*)elements);
                    break;
                case 23:
                    glUniformMatrix2x3fv(uniformLocation, elementCount, transpose, (const GLfloat*)elements);
                    break;
                case 32:
                    glUniformMatrix3x2fv(uniformLocation, elementCount, transpose, (const GLfloat*)elements);
                    break;
                case 24:
                    glUniformMatrix2x4fv(uniformLocation, elementCount, transpose, (const GLfloat*)elements);
                    break;
                case 42:
                    glUniformMatrix4x2fv(uniformLocation, elementCount, transpose, (const GLfloat*)elements);
                    break;
                case 34:
                    glUniformMatrix3x4fv(uniformLocation, elementCount, transpose, (const GLfloat*)elements);
                    break;
                case 43:
                    glUniformMatrix4x3fv(uniformLocation, elementCount, transpose, (const GLfloat*)elements);
                    break;
                default:
                    assert(0);
                    break;
                }
            }
            else{
                assert(0);
            }
        }
        else if (std::is_same<T, GLint>::value){
            if (ti == UniformType::UNIFORM_PRIMITIVE){
                switch (dataStructureItemCount)
                {
                case 1:
                    glUniform1i(uniformLocation, elements[0]);
                    break;
                case 2:
                    glUniform2i(uniformLocation, elements[0], elements[1]);
                    break;
                case 3:
                    glUniform3i(uniformLocation, elements[0], elements[1], elements[2]);
                    break;
                case 4:
                    glUniform4i(uniformLocation, elements[0], elements[1], elements[2], elements[3]);
                    break;
                default:
                    assert(0);
                    break;
                }
            }
            else if (ti == UniformType::UNIFORM_VECTOR){
                switch (dataStructureItemCount)
                {
                case 1:
                    glUniform1iv(uniformLocation, elementCount, (const GLint*)elements);
                    break;
                case 2:
                    glUniform2iv(uniformLocation, elementCount, (const GLint*)elements);
                    break;
                case 3:
                    glUniform3iv(uniformLocation, elementCount, (const GLint*)elements);
                    break;
                case 4:
                    glUniform4iv(uniformLocation, elementCount, (const GLint*)elements);
                    break;
                default:
                    assert(0);
                    break;
                }
            }
            else{
                assert(0);
            }
        }
        else if (std::is_same<T, GLuint>::value){
            if (ti == UniformType::UNIFORM_PRIMITIVE){
                switch (dataStructureItemCount)
                {
                case 1:
                    glUniform1ui(uniformLocation, elements[0]);
                    break;
                case 2:
                    glUniform2ui(uniformLocation, elements[0], elements[1]);
                    break;
                case 3:
                    glUniform3ui(uniformLocation, elements[0], elements[1], elements[2]);
                    break;
                case 4:
                    glUniform4ui(uniformLocation, elements[0], elements[1], elements[2], elements[3]);
                    break;
                default:
                    assert(0);
                    break;
                }
            }
            else if (ti == UniformType::UNIFORM_VECTOR){
                switch (dataStructureItemCount)
                {
                case 1:
                    glUniform1uiv(uniformLocation, elementCount, (const GLuint*)elements);
                    break;
                case 2:
                    glUniform2uiv(uniformLocation, elementCount, (const GLuint*)elements);
                    break;
                case 3:
                    glUniform3uiv(uniformLocation, elementCount, (const GLuint*)elements);
                    break;
                case 4:
                    glUniform4uiv(uniformLocation, elementCount, (const GLuint*)elements);
                    break;
                default:
                    assert(0);
                    break;
                }
            }
            else{
                assert(0);
            }
        }
        else{
            assert(0);
        }
		isUpdated = false;
    }

    T*			elements;
    GLsizei		elementCount;
    GLsizei		size;
    GLboolean transpose;
};

typedef UniformDerive<GLfloat,    1, UNIFORM_PRIMITIVE> Uniform1f;
typedef UniformDerive<GLfloat,    2, UNIFORM_PRIMITIVE> Uniform2f;
typedef UniformDerive<GLfloat,    3, UNIFORM_PRIMITIVE> Uniform3f;
typedef UniformDerive<GLfloat,    4, UNIFORM_PRIMITIVE> Uniform4f;
typedef UniformDerive<GLuint,     1, UNIFORM_PRIMITIVE> Uniform1ui;
typedef UniformDerive<GLuint,     2, UNIFORM_PRIMITIVE> Uniform2ui;
typedef UniformDerive<GLuint,     3, UNIFORM_PRIMITIVE> Uniform3ui;
typedef UniformDerive<GLuint,     4, UNIFORM_PRIMITIVE> Uniform4ui;
typedef UniformDerive<GLint,      1, UNIFORM_PRIMITIVE> Uniform1i;
typedef UniformDerive<GLint,      2, UNIFORM_PRIMITIVE> Uniform2i;
typedef UniformDerive<GLint,      3, UNIFORM_PRIMITIVE> Uniform3i;
typedef UniformDerive<GLint,      4, UNIFORM_PRIMITIVE> Uniform4i;

typedef UniformDerive<GLfloat, 1, UNIFORM_VECTOR> Uniform1fv;
typedef UniformDerive<GLfloat, 2, UNIFORM_VECTOR> Uniform2fv;
typedef UniformDerive<GLfloat, 3, UNIFORM_VECTOR> Uniform3fv;
typedef UniformDerive<GLfloat, 4, UNIFORM_VECTOR> Uniform4fv;
typedef UniformDerive<GLuint, 1, UNIFORM_VECTOR> Uniform1uiv;
typedef UniformDerive<GLuint, 2, UNIFORM_VECTOR> Uniform2uiv;
typedef UniformDerive<GLuint, 3, UNIFORM_VECTOR> Uniform3uiv;
typedef UniformDerive<GLuint, 4, UNIFORM_VECTOR> Uniform4uiv;
typedef UniformDerive<GLint, 1, UNIFORM_VECTOR> Uniform1iv;
typedef UniformDerive<GLint, 2, UNIFORM_VECTOR> Uniform2iv;
typedef UniformDerive<GLint, 3, UNIFORM_VECTOR> Uniform3iv;
typedef UniformDerive<GLint, 4, UNIFORM_VECTOR> Uniform4iv;

typedef UniformDerive<GLfloat, 22, UNIFORM_MATRIX>     UniformMatrix2fv;     // 2x2 = elementCount 22
typedef UniformDerive<GLfloat, 33, UNIFORM_MATRIX>     UniformMatrix3fv;     // 3x3 = elementCount 33
typedef UniformDerive<GLfloat, 44, UNIFORM_MATRIX>     UniformMatrix4fv;     // 4x4 = elementCount 44
typedef UniformDerive<GLfloat, 23, UNIFORM_MATRIX>     UniformMatrix2x3fv;   // 2x3 = elementCount 23
typedef UniformDerive<GLfloat, 32, UNIFORM_MATRIX>     UniformMatrix3x2fv;   // 3x2 = elementCount 32
typedef UniformDerive<GLfloat, 24, UNIFORM_MATRIX>     UniformMatrix2x4fv;   // 2x4 = elementCount 24
typedef UniformDerive<GLfloat, 42, UNIFORM_MATRIX>     UniformMatrix4x2fv;   // 4x2 = elementCount 42
typedef UniformDerive<GLfloat, 34, UNIFORM_MATRIX>     UniformMatrix3x4fv;   // 3x4 = elementCount 34
typedef UniformDerive<GLfloat, 43, UNIFORM_MATRIX>     UniformMatrix4x3fv;   // 4x3 = elementCount 43

#endif
