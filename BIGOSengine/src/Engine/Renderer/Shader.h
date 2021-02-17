#pragma once
#include "Engine/math/mat4.h"

namespace BIGOS {

	class Shader
	{
	public:
		static const Shader* s_CurrentlyBound;
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;
	public:
		static std::unique_ptr<Shader> Create(const std::string& filepath);
	};

}