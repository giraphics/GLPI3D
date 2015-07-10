#include "Uniforms.h"



//template< typename T, int compositionCount, UniformType ti>
//void UniformDerive<T,compositionCount,ti>::update()
//{
//    if (std::is_same<T, GLfloat>::value)
//    {
//        if (ti == UniformType::UNIFORM_PRIMITIVE){
//            switch (dataStructureItemCount)
//            {
//            case 1:
//                glUniform1f(uniformLocation, elements[0]);
//                break;
//            case 2:
//                glUniform2f(uniformLocation, elements[0], elements[1]);
//                break;
//            case 3:
//                glUniform3f(uniformLocation, elements[0], elements[1], elements[2]);
//                break;
//            case 4:
//                glUniform4f(uniformLocation, elements[0], elements[1], elements[2], elements[3]);
//                break;
//            default:
//                assert(0);
//                break;
//            }
//        }
//        else if (ti == UniformType::UNIFORM_VECTOR){
//            switch (dataStructureItemCount)
//            {
//            case 1:
//                glUniform1fv(uniformLocation, elementCount, (const GLfloat*)elements);
//                break;
//            case 2:
//                glUniform2fv(uniformLocation, elementCount, (const GLfloat*)elements);
//                break;
//            case 3:
//                glUniform3fv(uniformLocation, elementCount, (const GLfloat*)elements);
//                break;
//            case 4:
//                glUniform4fv(uniformLocation, elementCount, (const GLfloat*)elements);
//                break;
//            default:
//                assert(0);
//                break;
//            }
//        }
//        else if (ti == UniformType::UNIFORM_MATRIX){
//            switch (dataStructureItemCount)
//            {
//            case 22:
//                glUniformMatrix2fv(uniformLocation, elementCount, transpose, (const GLfloat*)elements);
//                break;
//            case 33:
//                glUniformMatrix3fv(uniformLocation, elementCount, transpose, (const GLfloat*)elements);
//                break;
//            case 44:
//                glUniformMatrix4fv(uniformLocation, elementCount, transpose, (const GLfloat*)elements);
//                break;
//            case 23:
//                glUniformMatrix2x3fv(uniformLocation, elementCount, transpose, (const GLfloat*)elements);
//                break;
//            case 32:
//                glUniformMatrix3x2fv(uniformLocation, elementCount, transpose, (const GLfloat*)elements);
//                break;
//            case 24:
//                glUniformMatrix2x4fv(uniformLocation, elementCount, transpose, (const GLfloat*)elements);
//                break;
//            case 42:
//                glUniformMatrix4x2fv(uniformLocation, elementCount, transpose, (const GLfloat*)elements);
//                break;
//            case 34:
//                glUniformMatrix3x4fv(uniformLocation, elementCount, transpose, (const GLfloat*)elements);
//                break;
//            case 43:
//                glUniformMatrix4x3fv(uniformLocation, elementCount, transpose, (const GLfloat*)elements);
//                break;
//            default:
//                assert(0);
//                break;
//            }
//        }
//        else{
//            assert(0);
//        }
//    }
//    else if (std::is_same<T, GLint>::value){
//        if (ti == UniformType::UNIFORM_PRIMITIVE){
//            switch (dataStructureItemCount)
//            {
//            case 1:
//                glUniform1i(uniformLocation, elements[0]);
//                break;
//            case 2:
//                glUniform2i(uniformLocation, elements[0], elements[1]);
//                break;
//            case 3:
//                glUniform3i(uniformLocation, elements[0], elements[1], elements[2]);
//                break;
//            case 4:
//                glUniform4i(uniformLocation, elements[0], elements[1], elements[2], elements[3]);
//                break;
//            default:
//                assert(0);
//                break;
//            }
//        }
//        else if (ti == UniformType::UNIFORM_VECTOR){
//            switch (dataStructureItemCount)
//            {
//            case 1:
//                glUniform1iv(uniformLocation, elementCount, (const GLint*)elements);
//                break;
//            case 2:
//                glUniform2iv(uniformLocation, elementCount, (const GLint*)elements);
//                break;
//            case 3:
//                glUniform3iv(uniformLocation, elementCount, (const GLint*)elements);
//                break;
//            case 4:
//                glUniform4iv(uniformLocation, elementCount, (const GLint*)elements);
//                break;
//            default:
//                assert(0);
//                break;
//            }
//        }
//        else{
//            assert(0);
//        }
//    }
//    else if (std::is_same<T, GLuint>::value){
//        if (ti == UniformType::UNIFORM_PRIMITIVE){
//            switch (dataStructureItemCount)
//            {
//            case 1:
//                glUniform1ui(uniformLocation, elements[0]);
//                break;
//            case 2:
//                glUniform2ui(uniformLocation, elements[0], elements[1]);
//                break;
//            case 3:
//                glUniform3ui(uniformLocation, elements[0], elements[1], elements[2]);
//                break;
//            case 4:
//                glUniform4ui(uniformLocation, elements[0], elements[1], elements[2], elements[3]);
//                break;
//            default:
//                assert(0);
//                break;
//            }
//        }
//        else if (ti == UniformType::UNIFORM_VECTOR){
//            switch (dataStructureItemCount)
//            {
//            case 1:
//                glUniform1uiv(uniformLocation, elementCount, (const GLuint*)elements);
//                break;
//            case 2:
//                glUniform2uiv(uniformLocation, elementCount, (const GLuint*)elements);
//                break;
//            case 3:
//                glUniform3uiv(uniformLocation, elementCount, (const GLuint*)elements);
//                break;
//            case 4:
//                glUniform4uiv(uniformLocation, elementCount, (const GLuint*)elements);
//                break;
//            default:
//                assert(0);
//                break;
//            }
//        }
//        else{
//            assert(0);
//        }
//    }
//    else{
//        assert(0);
//    }
//	isUpdated = false;
//}
