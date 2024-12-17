#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

using namespace std;

// clang-format off
static auto original_camera = glm::mat4{
	2, 0, 0, 0,
	0, 2, 0, 322,
	0, 0, 1, 0,
	0, 0, 0, 1
};
// clang-format on
//m[0][0] and m[1][1] must be same value to maintain aspect ratio

static auto camera = glm::mat4{1.0};

// Expand 4 xy coordinates to 4 xyz coordinates
static glm::mat4x3 _2dto3d(const glm::mat4x2 v, const float z = 1.0) {
	return glm::mat4x3(
		glm::vec3{v[0], z},
		glm::vec3{v[1], z},
		glm::vec3{v[2], z},
		glm::vec3{v[3], z}
	);
}

// IN:  Quad vertices
// OUT: Triangle vertices, transformed
glm::mat4x3 correct_transform(const glm::mat4x2 vertices) {
	const auto v2 = glm::mat4{
		glm::vec4(vertices[0], 1.0, 1.0),
		glm::vec4(vertices[1], 1.0, 1.0),
		glm::vec4(vertices[2], 1.0, 1.0),
		glm::vec4(vertices[3], 1.0, 1.0),
	};

	const glm::mat4x3 v = glm::transpose(camera) * v2;
	const auto h = glm::transpose(glm::mat3{
		glm::cross(glm::cross(v[0], v[2]), glm::cross(v[1], v[3])),
		glm::cross(glm::cross(v[0], v[1]), glm::cross(v[3], v[2])),
		glm::cross(glm::cross(v[0], v[3]), glm::cross(v[1], v[2]))});

	// perspective_quad
	const auto h_inv = glm::mat3{0, 0, 0.005, -0.001, 0, 0.015, 0, 0.001, -0.015};
	const glm::mat3 m3 = h_inv * h;
	const auto t = glm::mat4x3{
		glm::vec3{10, 10, 1} * m3,
		glm::vec3{20, 10, 1} * m3,
		glm::vec3{20, 20, 1} * m3,
		glm::vec3{10, 20, 1} * m3,
	};
	return t;
}

glm::mat4x3 test_transform(const glm::mat4x2 vertices) {
        cout << "TEST TEST TEST\n";
		cout << glm::to_string(_2dto3d(vertices)) << endl;
        const auto projection = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f);
        cout << glm::to_string(projection) << endl;
        return glm::mat4x3();
}