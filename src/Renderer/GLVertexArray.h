//
// Created by 王晨辉 on 2023/5/28.
//

#ifndef GAMEPHYSICSINONEWEEKEND_GLVERTEXARRAY_H
#define GAMEPHYSICSINONEWEEKEND_GLVERTEXARRAY_H

#include <cstdio>
#include <glad/glad.h>
namespace renderer{
    enum BufferType : int {
        ARRAY = GL_ARRAY_BUFFER, // VAO
        ELEMENT = GL_ELEMENT_ARRAY_BUFFER // VBO
    };

    enum DrawMode : int {
        STATIC = GL_STATIC_DRAW,
        DYNAMIC = GL_DYNAMIC_DRAW,
        STREAM = GL_STREAM_DRAW
    };

    enum PrimitiveType : int {
        LINE = GL_LINES,
        TRIANGLE = GL_TRIANGLES
    };

class GLVertexArray{
public:
    GLVertexArray() = default;
    void Init();
    void AttachBuffer(BufferType type, size_t size, const void* data, DrawMode mode);
    void Bind() const;
    void EnableAttribute(GLuint index, int size, GLuint stride, const void* offset);
    void UnBind() ;
    void Delete();

private:
    GLuint m_VAO = 0;
};
}




#endif //GAMEPHYSICSINONEWEEKEND_GLVERTEXARRAY_H
