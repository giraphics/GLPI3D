#ifndef UNIFORM_h
#define UNIFORM_h
#include <vector>
#include "ProgramManager.h"

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


class UniformBase
{

public:

    UniformBase(PROGRAM* sh, const std::string& name, int location){}

    const std::string& GetName(){}

    void AddOwner(PROGRAM* pShader, unsigned int location){}

protected:

    void NotifyOwners(){}

protected:
    std::vector<PROGRAM*> m_owner;  // List containing references to the owners needed so they can be notified.
    std::vector<int> m_loc; // list of locations/handles for the GLSL uniform variable

    std::string m_name; // The name of the uniform variable in the GLSL program

};


//template< typename T, int count, UniformType type>
//class UniformDerived : public UniformBase
//{
//    typedef T type;
//    const enum attribs{ elementCount = count, elementSize = sizeof(T) };
//    //============================================================
//    UniformDerived(PROGRAM* sh, const std::string& name, int location)// : Uniform(sh, name, location)
//    {
//    }
//
//    //============================================================
//    UniformDerived(const std::string& name) : Uniform(name)
//    {
//    }
//
//    void SetValue(T* values)
//    {
//        memcpy(m_elements, values, attribs::elementCount * attribs::elementSize);
//    }
//
//    T m_elements[elementCount];
//};

template< typename T, int compositionCount, UniformType ti>
class Uniform : public UniformBase
{
public:
    typedef T type;
    const enum attribs{ dataStructureItemCount = compositionCount /*elementCount = count,*/, elementSize = sizeof(T) };
    //============================================================
    Uniform(PROGRAM* sh, const std::string& name, int location, int elementCount=1, bool transposeFlag = false) : UniformBase(sh, name, location)
    {
        transpose = transposeFlag; // Transpose Flag is only necessary for 2D Matrix elements
        if (dataStructureItemCount > 9 && dataStructureItemCount < 99){ // 2 Digit number, treat this as 2 dimensional case
            m_elementCount  = (GLsizei)elementCount;
            int firstDim    = dataStructureItemCount % 10;
            int secondDim   = dataStructureItemCount / 10;
            m_elements      = new T[secondDim*firstDim*elementCount];
        }
        else if (dataStructureItemCount < 9){  // 1 Digit number, treat this as 1 dimensional case
            m_elementCount  = (GLsizei)elementCount;
            m_elements      = new T[dataStructureItemCount*m_elementCount];
        }
        else{   // More than 2 dimensional case which is incorrect
            printf("Incorrect element count specified for %s", name.c_str());
            assert(0);
        }
    }

    void SetValue(T* values)
    {
        memcpy(m_elements, values, attribs::dataStructureItemCount * attribs::elementSize);
        updateNew();
    }

    void update()
    {
        switch (ti)
        {
            case TYPE_INT:
                {
                    switch (elementCount)
                    {
                    case 1:
                        glUniform1i(0, m_elements[0]);
                        break;
                    case 2:
                        glUniform2i(0, m_elements[0], m_elements[1]);
                        break;
                    case 3:
                        glUniform3i(0, m_elements[0], m_elements[1], m_elements[2]);
                        break;
                    case 4:
                        glUniform4i(0, m_elements[0], m_elements[1], m_elements[2], m_elements[3]);
                        break;
                    }
                }
                break;
        case TYPE_FLOAT:
        {
            switch (elementCount)
            {
            case 1:
                glUniform1f(0, m_elements[0]);
                break;
            case 2:
                glUniform2f(0, m_elements[0], m_elements[1]);
                break;
            case 3:
                glUniform3f(0, m_elements[0], m_elements[1], m_elements[2]);
                break;
            case 4:
                glUniform4f(0, m_elements[0], m_elements[1], m_elements[2], m_elements[3]);
                break;
            }
        }
        break;
        case TYPE_UINT:
        {
            switch (elementCount)
            {
            case 1:
                glUniform1ui(0, m_elements[0]);
                break;
            case 2:
                glUniform2ui(0, m_elements[0], m_elements[1]);
                break;
            case 3:
                glUniform3ui(0, m_elements[0], m_elements[1], m_elements[2]);
                break;
            case 4:
                glUniform4ui(0, m_elements[0], m_elements[1], m_elements[2], m_elements[3]);
                break;
            }
        }
        break;
        }
    }

    void updateNew()
    {
        if (std::is_same<T, GLfloat>::value)
        {
            if (ti == UniformType::UNIFORM_PRIMITIVE){
                switch (dataStructureItemCount)
                {
                case 1:
                    glUniform1f(0, m_elements[0]);
                    break;
                case 2:
                    glUniform2f(0, m_elements[0], m_elements[1]);
                    break;
                case 3:
                    glUniform3f(0, m_elements[0], m_elements[1], m_elements[2]);
                    break;
                case 4:
                    glUniform4f(0, m_elements[0], m_elements[1], m_elements[2], m_elements[3]);
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
                    glUniform1fv(0, m_elementCount, (const GLfloat*)m_elements);
                    break;
                case 2:
                    glUniform2fv(0, m_elementCount, (const GLfloat*)m_elements);
                    break;
                case 3:
                    glUniform3fv(0, m_elementCount, (const GLfloat*)m_elements);
                    break;
                case 4:
                    glUniform4fv(0, m_elementCount, (const GLfloat*)m_elements);
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
                    glUniformMatrix2fv(0, m_elementCount, transpose, (const GLfloat*)m_elements);
                    break;
                case 33:
                    glUniformMatrix3fv(0, m_elementCount, transpose, (const GLfloat*)m_elements);
                    break;
                case 44:
                    glUniformMatrix4fv(0, m_elementCount, transpose, (const GLfloat*)m_elements);
                    break;
                case 23:
                    glUniformMatrix2x3fv(0, m_elementCount, transpose, (const GLfloat*)m_elements);
                    break;
                case 32:
                    glUniformMatrix3x2fv(0, m_elementCount, transpose, (const GLfloat*)m_elements);
                    break;
                case 24:
                    glUniformMatrix2x4fv(0, m_elementCount, transpose, (const GLfloat*)m_elements);
                    break;
                case 42:
                    glUniformMatrix4x2fv(0, m_elementCount, transpose, (const GLfloat*)m_elements);
                    break;
                case 34:
                    glUniformMatrix3x4fv(0, m_elementCount, transpose, (const GLfloat*)m_elements);
                    break;
                case 43:
                    glUniformMatrix4x3fv(0, m_elementCount, transpose, (const GLfloat*)m_elements);
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
                    glUniform1i(0, m_elements[0]);
                    break;
                case 2:
                    glUniform2i(0, m_elements[0], m_elements[1]);
                    break;
                case 3:
                    glUniform3i(0, m_elements[0], m_elements[1], m_elements[2]);
                    break;
                case 4:
                    glUniform4i(0, m_elements[0], m_elements[1], m_elements[2], m_elements[3]);
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
                    glUniform1iv(0, m_elementCount, (const GLint*)m_elements);
                    break;
                case 2:
                    glUniform2iv(0, m_elementCount, (const GLint*)m_elements);
                    break;
                case 3:
                    glUniform3iv(0, m_elementCount, (const GLint*)m_elements);
                    break;
                case 4:
                    glUniform4iv(0, m_elementCount, (const GLint*)m_elements);
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
                    glUniform1ui(0, m_elements[0]);
                    break;
                case 2:
                    glUniform2ui(0, m_elements[0], m_elements[1]);
                    break;
                case 3:
                    glUniform3ui(0, m_elements[0], m_elements[1], m_elements[2]);
                    break;
                case 4:
                    glUniform4ui(0, m_elements[0], m_elements[1], m_elements[2], m_elements[3]);
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
                    glUniform1uiv(0, m_elementCount, (const GLuint*)m_elements);
                    break;
                case 2:
                    glUniform2uiv(0, m_elementCount, (const GLuint*)m_elements);
                    break;
                case 3:
                    glUniform3uiv(0, m_elementCount, (const GLuint*)m_elements);
                    break;
                case 4:
                    glUniform4uiv(0, m_elementCount, (const GLuint*)m_elements);
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
    }

    T* m_elements;
    GLsizei m_elementCount;
    GLboolean transpose;
};

typedef Uniform<GLfloat,    1, UNIFORM_PRIMITIVE> Uniform1f;
typedef Uniform<GLfloat,    2, UNIFORM_PRIMITIVE> Uniform2f;
typedef Uniform<GLfloat,    3, UNIFORM_PRIMITIVE> Uniform3f;
typedef Uniform<GLfloat,    4, UNIFORM_PRIMITIVE> Uniform4f;
typedef Uniform<GLuint,     1, UNIFORM_PRIMITIVE> Uniform1ui;
typedef Uniform<GLuint,     2, UNIFORM_PRIMITIVE> Uniform2ui;
typedef Uniform<GLuint,     3, UNIFORM_PRIMITIVE> Uniform3ui;
typedef Uniform<GLuint,     4, UNIFORM_PRIMITIVE> Uniform4ui;
typedef Uniform<GLint,      1, UNIFORM_PRIMITIVE> Uniform1i;
typedef Uniform<GLint,      2, UNIFORM_PRIMITIVE> Uniform2i;
typedef Uniform<GLint,      3, UNIFORM_PRIMITIVE> Uniform3i;
typedef Uniform<GLint,      4, UNIFORM_PRIMITIVE> Uniform4i;

//////////////////////////////////////////////////////////////
typedef Uniform<GLfloat, 1, UNIFORM_VECTOR> Uniform1fv;
typedef Uniform<GLfloat, 2, UNIFORM_VECTOR> Uniform2fv;
typedef Uniform<GLfloat, 3, UNIFORM_VECTOR> Uniform3fv;
typedef Uniform<GLfloat, 4, UNIFORM_VECTOR> Uniform4fv;
typedef Uniform<GLuint, 1, UNIFORM_VECTOR> Uniform1uiv;
typedef Uniform<GLuint, 2, UNIFORM_VECTOR> Uniform2uiv;
typedef Uniform<GLuint, 3, UNIFORM_VECTOR> Uniform3uiv;
typedef Uniform<GLuint, 4, UNIFORM_VECTOR> Uniform4uiv;
typedef Uniform<GLint, 1, UNIFORM_VECTOR> Uniform1iv;
typedef Uniform<GLint, 2, UNIFORM_VECTOR> Uniform2iv;
typedef Uniform<GLint, 3, UNIFORM_VECTOR> Uniform3iv;
typedef Uniform<GLint, 4, UNIFORM_VECTOR> Uniform4iv;

//////////////////////////////////////////////////////////////
typedef Uniform<GLfloat, 22, UNIFORM_MATRIX>     UniformMatrix2fv;     // 2x2 = elementCount 22
typedef Uniform<GLfloat, 33, UNIFORM_MATRIX>     UniformMatrix3fv;     // 3x3 = elementCount 33
typedef Uniform<GLfloat, 44, UNIFORM_MATRIX>     UniformMatrix4fv;     // 4x4 = elementCount 44
typedef Uniform<GLfloat, 23, UNIFORM_MATRIX>     UniformMatrix2x3fv;   // 2x3 = elementCount 23
typedef Uniform<GLfloat, 32, UNIFORM_MATRIX>     UniformMatrix3x2fv;   // 3x2 = elementCount 32
typedef Uniform<GLfloat, 24, UNIFORM_MATRIX>     UniformMatrix2x4fv;   // 2x4 = elementCount 24
typedef Uniform<GLfloat, 42, UNIFORM_MATRIX>     UniformMatrix4x2fv;   // 4x2 = elementCount 42
typedef Uniform<GLfloat, 34, UNIFORM_MATRIX>     UniformMatrix3x4fv;   // 3x4 = elementCount 34
typedef Uniform<GLfloat, 43, UNIFORM_MATRIX>     UniformMatrix4x3fv;   // 4x3 = elementCount 43

//void ExtractShaderUniforms()
//{
//    PROGRAM prgDummy;
//    //Uniform* uniform = new Uniform(NULL, "name", 0);
//
//    //UniformsFactory& engineUniform = EngineUniforms::GetInstance();
//    //int count;
//    //glGetProgramiv(m_programId, GL_OBJECT_ACTIVE_UNIFORMS_ARB, &count);
//
//    //for (int i = 0; i < count; ++i)
//    //{
//    //    char name[BUFF_SIZE]; // for holding the variable name
//    //    GLint size = BUFF_SIZE;
//    //    GLenum type;
//    //    GLsizei length;
//    //    GLsizei bufSize = BUFF_SIZE;
//    //    glGetActiveUniform(m_programId, i, bufSize, &length, &size, &type, name);
//    //    int location = glGetUniformLocation(m_programId, name);
//
//    //    UniformType type1 = (UniformType)type;
//    //    Uniform* uniform = engineFactory.CreateUniform(location, name, type1);
//
//    //}
//}

#endif
