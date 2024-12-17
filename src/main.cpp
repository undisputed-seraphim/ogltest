#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <string>
#include <vector>

constexpr int W = 1920, H = 1080;

const glm::mat4x2 QUAD = {-49.78125, -23.5625, -54.3125, 8.65625, -83.28125, 8.78125, -78.84375, -23.53125};

GLuint GetShader();

// working answer
glm::mat4x3 correct_transform(glm::mat4x2 vertices);

glm::mat4x3 test_transform(glm::mat4x2 vertices);

int main(int argc, char* argv[]) {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_Window* window = SDL_CreateWindow("Rosalinde", W, H, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
	if (window == nullptr) {
		printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
		return -1;
	}
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		throw std::runtime_error("Failed to initialize GLAD");
	}

	printf(" Version: %s\n", glGetString(GL_VERSION));
	printf("  Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf(" Shading: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1); // Enable vsync
	SDL_ShowWindow(window);

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	std::vector<glm::mat4x3> vertices;
	std::vector<unsigned> indices = {0, 1, 2, 0, 2, 3};

	const auto v = correct_transform(QUAD);
	std::cout << glm::to_string(v) << std::endl;

    auto v2 = v;
    for (int i = 0; i < 4; ++i) {
        const float z = 1.0 / v2[i][2];
        v2[i][0] *= z * (2.0 / W);
        v2[i][1] *= z * (2.0 / H);
    }
    std::cout << glm::to_string(v2) << std::endl;

    const auto v3 = test_transform(QUAD);

	// Our state
	const auto clear_color = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);

	bool done = false;
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT)
				done = true;
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
				done = true;
			if (event.type == SDL_EVENT_MOUSE_WHEEL) {
				// camera
			}
		}
		if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
			SDL_Delay(10);
			continue;
		}

		// Rendering
		glViewport(0, 0, W, H);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		const GLuint shader = GetShader();
		glUseProgram(shader);
		const GLint loc = glGetUniformLocation(shader, "u_pxsize");
		glUniform2f(loc, float(W), float(H));

		vertices.clear();
		vertices.emplace_back(v);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4x3) * vertices.size(), &vertices[0][0][0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), indices.data(), GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, vertices.size() * 4 * 3, GL_UNSIGNED_INT, 0);

		SDL_GL_SwapWindow(window);
	}

	// Cleanup
	SDL_GL_DestroyContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
