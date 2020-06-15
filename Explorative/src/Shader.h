#pragma once

#include <string>
#include <unordered_map>
#include "glm\glm.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_vertex_filePath;
	std::string m_fragment_filePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& vertex_filepath, const std::string& fragment_filepath);
	~Shader();


	void Bind() const;
	void Unbind() const;

	//set uniform
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name,const glm::mat4 matrix);

private:
//	bool CompileShader();
	ShaderProgramSource ParseShader(const std::string& vertex_filepath, const std::string& fragment_filepath);

	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);

	int GetUniformLocation(const std::string name);

};