//
// Created by 王晨辉 on 2023/5/28.
//

#include "GLVertexArray.h"

namespace renderer{
    void GLVertexArray::Init() {
        glGenVertexArrays(1, &m_VAO);
    }

    void GLVertexArray::AttachBuffer(BufferType type, size_t size, const void *data,
                                     DrawMode mode) {
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(type, buffer);
        glBufferData(type, size, data, mode);
    }

    void GLVertexArray::Bind() const {
        glBindVertexArray(m_VAO);
    }

    void GLVertexArray::EnableAttribute(GLuint index, int size, GLuint stride, const void *offset) {
        glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, offset);
        glEnableVertexAttribArray(index);
    }

    void GLVertexArray::Unbind() {
        glBindVertexArray(0);
    }

    void GLVertexArray::Delete() {
        glDeleteVertexArrays(1, &m_VAO);
    }
}