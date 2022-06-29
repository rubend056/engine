#include "vao.h"

#include "engine_globals.h"

VAO::VAO() { glGenVertexArrays(1, &vao_id); }
VAO::~VAO() { glDeleteVertexArrays(1, &vao_id); }

void VAO::vao_bind() { glBindVertexArray(vao_id); }

void VAO::vao_attrib_enable(uint32_t attribs) {
	uint32_t enabled = (positions ? _BV32(0) : 0) |
					   (normals ? _BV32(1) : 0) |
					   (tex_cords ? _BV32(2) : 0);
	for (int i = 0; i < 3; ++i)
		if (attribs & enabled & _BV32(i))
			glEnableVertexArrayAttrib(vao_id, i);
};
void VAO::vao_attrib_disable() {
	for (int i = 0; i < 3; ++i)
		glDisableVertexArrayAttrib(vao_id, i);
};
void VAO::gl_draw() {
	glDrawArrays(draw_function, 0, n_vertices);
}

void VAO::imgui_draw() {
	ImGui::Text("ID: %d", vao_id);
	ImGui::Text("Vertices: %d", n_vertices);
	ImGui::Separator();
	ImGui::Text("Positions: %s", positions ? "true" : "false");
	ImGui::Text("Normals: %s", normals ? "true" : "false");
	ImGui::Text("Tex_cords: %s", tex_cords ? "true" : "false");
}
