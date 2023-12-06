#include <classes/Shader.hpp>

#define VRTX 1 << 0
#define FGMT 1 << 1
#define GMTR 1 << 2

Shader::Shader(const char* folderPath) {
	u_char activeShaders = 0;
	ID = glCreateProgram();

	for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
		if (entry.is_regular_file()) {
			const std::string& filePath = entry.path().string();
			const std::string& fileName = entry.path().filename().string();

			if (fileName.find("vertex_shader") != std::string::npos) {
				GLuint vertexShader = CompileSingleShader(filePath.c_str(), GL_VERTEX_SHADER, "VERTEX");
				glAttachShader(ID, vertexShader);
				glDeleteShader(vertexShader);
				activeShaders |= VRTX;
			} else if (fileName.find("fragment_shader") != std::string::npos) {
				GLuint fragmentShader = CompileSingleShader(filePath.c_str(), GL_FRAGMENT_SHADER, "FRAGMENT");
				glAttachShader(ID, fragmentShader);
				glDeleteShader(fragmentShader);
				activeShaders |= FGMT;
			} else if (fileName.find("geometry_shader") != std::string::npos) {
				GLuint geometryShader = CompileSingleShader(filePath.c_str(), GL_GEOMETRY_SHADER, "GEOMETRY");
				glAttachShader(ID, geometryShader);
				glDeleteShader(geometryShader);
				activeShaders |= GMTR;
			}
		}
	}
	if (!(activeShaders & VRTX) || !(activeShaders & FGMT)) {
		assert(!"Shader::Shader Minimum shader requirement not met");
	}
	glLinkProgram(ID);
	CheckCompileErrors(ID, "PROGRAM");
}

u_int Shader::CompileSingleShader(const char *path, GLenum type, std::string sType) {
	std::string shaderCode;
	std::ifstream shaderFile;

	shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

	try 
    {
		shaderFile.open(path);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
	}

	const char * cShaderCode = shaderCode.c_str();
	u_int shader = glCreateShader(type);
	glShaderSource(shader, 1, &cShaderCode, NULL);
	glCompileShader(shader);
	CheckCompileErrors(shader, sType);

	return shader;
}

void Shader::Use() { 
    glUseProgram(ID); 
}
void Shader::SetBool(const std::string &name, bool value) const {         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}
void Shader::SetInt(const std::string &name, int value) const { 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}
void Shader::SetFloat(const std::string &name, float value) const { 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}
void Shader::SetFloat4(const std::string &name, float value1, float value2, float value3, float value4) const { 
    glUniform4f(glGetUniformLocation(ID, name.c_str()), value1 , value2, value3, value4); 
}
void Shader::Setmat4(const std::string &name, glm::mat4 value) const { 
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::CheckCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }