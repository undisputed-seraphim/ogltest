#include <glad/glad.h>
#include <iostream>
#include <string>
#include <string_view>

// clang-format off
constexpr std::string_view vertex =  "# version 300 es\n"
"in      highp vec3  a_xyz;\n"
"uniform highp vec2  u_pxsize;\n"
"        highp vec2  XY;\n"
"        highp float z;\n"
"void main(void) {\n"
"    z = 1.0 / a_xyz.z;\n"
"    XY = a_xyz.xy * z * (2.0 / u_pxsize);\n"

"    gl_Position = vec4(XY.x, -XY.y, 1.0, 1.0);\n"
"}";

constexpr std::string_view fragment = "# version 300 es\n"
"out     highp vec4  fragColor;\n"
"void main(void) {\n"
"    fragColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
"}";
// clang-format on

static void checkCompileErrors(GLuint object);
static void checkLinkErrors(GLuint object);

template <uint32_t ShaderType>
static GLuint Compile_(const std::string_view source) {
	if (source.empty()) {
		return 0;
	}
	GLuint shader = glCreateShader(ShaderType);
	const char* ptr = source.data();
	int32_t size = source.size();
	glShaderSource(shader, 1, &ptr, &size);
	glCompileShader(shader);
	return shader;
}

static GLuint compile() {
	GLuint sVertex = Compile_<GL_VERTEX_SHADER>(vertex);
	checkCompileErrors(sVertex);
	GLuint sFragment = Compile_<GL_FRAGMENT_SHADER>(fragment);
	checkCompileErrors(sFragment);

	// shader program
	GLuint _id = glCreateProgram();
	glAttachShader(_id, sVertex);
	glAttachShader(_id, sFragment);
	glLinkProgram(_id);
	checkLinkErrors(_id);
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	return _id;
}

GLuint GetShader() {
	static GLuint handle{0};
	if (handle == 0) {
		handle = compile();
	}
	return handle;
}

// checks if compilation or linking failed and if so, print the error logs
void checkCompileErrors(GLuint object) {
	char infoLog[512];
	GLint success;
	glGetShaderiv(object, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE) {
		glGetShaderInfoLog(object, sizeof(infoLog), NULL, infoLog);
		std::cout << "| ERROR::Shader: Compile-time error:\n" << infoLog << std::endl;
	}
}

void checkLinkErrors(GLuint object) {
	char infoLog[512];
	GLint success;
	glGetProgramiv(object, GL_LINK_STATUS, &success);
	if (success != GL_TRUE) {
		glGetProgramInfoLog(object, sizeof(infoLog), NULL, infoLog);
		std::cout << "| ERROR::Shader: Link-time error:\n" << infoLog << std::endl;
	}
}
