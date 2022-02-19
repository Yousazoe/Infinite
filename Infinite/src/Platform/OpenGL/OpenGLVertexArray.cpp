#include "ifnpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Infinite {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Infinite::ShaderDataType::Float:    return GL_FLOAT;
		case Infinite::ShaderDataType::Float2:   return GL_FLOAT;
		case Infinite::ShaderDataType::Float3:   return GL_FLOAT;
		case Infinite::ShaderDataType::Float4:   return GL_FLOAT;
		case Infinite::ShaderDataType::Mat3:     return GL_FLOAT;
		case Infinite::ShaderDataType::Mat4:     return GL_FLOAT;
		case Infinite::ShaderDataType::Int:      return GL_INT;
		case Infinite::ShaderDataType::Int2:     return GL_INT;
		case Infinite::ShaderDataType::Int3:     return GL_INT;
		case Infinite::ShaderDataType::Int4:     return GL_INT;
		case Infinite::ShaderDataType::Bool:     return GL_BOOL;
		}

		IFN_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RedererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RedererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		IFN_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

		glBindVertexArray(m_RedererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index + m_VertexBufferIndexOffset);
			glVertexAttribPointer(index + m_VertexBufferIndexOffset,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)(intptr_t)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
		m_VertexBufferIndexOffset += layout.GetElements().size();
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RedererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}


}