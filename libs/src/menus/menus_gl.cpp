// #pragma once

#include "_menus.h"

#include "engine_globals.h"

#include <algorithm>

#include "gl.h"
#include "cereal/archives/json.hpp"
struct Gl_Enable_Setting
{
    const int index;
    const std::string name;
    const std::string info;
    bool value = false;
    template <class Archive>
    void serialize(Archive &ar, std::uint32_t const version) {
        ar(cereal::make_nvp(name.c_str(), value));
    }
};

Gl_Enable_Setting gl_enable_settings[] = {
    {GL_BLEND, "GL_BLEND", "If enabled, blend the computed fragment color values with the values in the color buffers. See glBlendFunc."},
    {GL_COLOR_LOGIC_OP, "GL_COLOR_LOGIC_OP", "If enabled, apply the currently selected logical operation to the computed fragment color and color buffer values. See glLogicOp."},
    {GL_CULL_FACE, "GL_CULL_FACE", "If enabled, cull polygons based on their winding in window coordinates. See glCullFace."},
    {GL_DEBUG_OUTPUT, "GL_DEBUG_OUTPUT", "If enabled, debug messages are produced by a debug context. When disabled, the debug message log is silenced."},
    {GL_DEBUG_OUTPUT_SYNCHRONOUS, "GL_DEBUG_OUTPUT_SYNCHRONOUS", "If enabled, debug messages are produced synchronously by a debug context. If disabled, debug messages may be produced asynchronously."},
    {GL_DEPTH_CLAMP, "GL_DEPTH_CLAMP", "If enabled, the −wc ≤ zc ≤ wc plane equation is ignored by view volume clipping (effectively, there is no near or far plane clipping). See glDepthRange."},
    {GL_DEPTH_TEST, "GL_DEPTH_TEST", "If enabled, do depth comparisons and update the depth buffer. Note that even if the depth buffer exists and the depth mask is non-zero, the depth buffer is not updated if the depth test is disabled. See glDepthFunc and glDepthRange. "},
    {GL_DITHER, "GL_DITHER", "If enabled, dither color components or indices before they are written to the color buffer."},
    {GL_FRAMEBUFFER_SRGB, "GL_FRAMEBUFFER_SRGB", "If enabled and the value of GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING for the framebuffer attachment corresponding to the destination buffer is GL_SRGB, the R, G, and B destination color values (after conversion from fixed-point to floating-point) are considered to be encoded for the sRGB color space and hence are linearized prior to their use in blending."},
    {GL_LINE_SMOOTH, "GL_LINE_SMOOTH", "If enabled, draw lines with correct filtering. Otherwise, draw aliased lines. See glLineWidth."},
    {GL_MULTISAMPLE, "GL_MULTISAMPLE", "If enabled, use multiple fragment samples in computing the final color of a pixel. See glSampleCoverage."},
    {GL_POLYGON_OFFSET_FILL, "GL_POLYGON_OFFSET_FILL", "If enabled, and if the polygon is rendered in GL_FILL mode, an offset is added to depth values of a polygon's fragments before the depth comparison is performed. See glPolygonOffset."},
    {GL_POLYGON_OFFSET_LINE, "GL_POLYGON_OFFSET_LINE", "If enabled, and if the polygon is rendered in GL_LINE mode, an offset is added to depth values of a polygon's fragments before the depth comparison is performed. See glPolygonOffset."},
    {GL_POLYGON_OFFSET_POINT, "GL_POLYGON_OFFSET_POINT", "If enabled, an offset is added to depth values of a polygon's fragments before the depth comparison is performed, if the polygon is rendered in GL_POINT mode. See glPolygonOffset."},
    {GL_POLYGON_SMOOTH, "GL_POLYGON_SMOOTH", "If enabled, draw polygons with proper filtering. Otherwise, draw aliased polygons. For correct antialiased polygons, an alpha buffer is needed and the polygons must be sorted front to back."},
    {GL_PRIMITIVE_RESTART_FIXED_INDEX, "GL_PRIMITIVE_RESTART_FIXED_INDEX", "Enables primitive restarting with a fixed index. If enabled, any one of the draw commands which transfers a set of generic attribute array elements to the GL will restart the primitive when the index of the vertex is equal to the fixed primitive index for the specified index type. The fixed index is equal to 2n−1 where n is equal to 8 for GL_UNSIGNED_BYTE, 16 for GL_UNSIGNED_SHORT and 32 for GL_UNSIGNED_INT."},
    {GL_RASTERIZER_DISCARD, "GL_RASTERIZER_DISCARD", "If enabled, primitives are discarded after the optional transform feedback stage, but before rasterization. Furthermore, when enabled, glClear, glClearBufferData, glClearBufferSubData, glClearTexImage, and glClearTexSubImage are ignored."},
    {GL_SAMPLE_ALPHA_TO_COVERAGE, "GL_SAMPLE_ALPHA_TO_COVERAGE", "If enabled, compute a temporary coverage value where each bit is determined by the alpha value at the corresponding sample location. The temporary coverage value is then ANDed with the fragment coverage value."},
    {GL_SAMPLE_ALPHA_TO_ONE, "GL_SAMPLE_ALPHA_TO_ONE", "If enabled, each sample alpha value is replaced by the maximum representable alpha value."},
    {GL_SAMPLE_COVERAGE, "GL_SAMPLE_COVERAGE", "If enabled, the fragment's coverage is ANDed with the temporary coverage value. If GL_SAMPLE_COVERAGE_INVERT is set to GL_TRUE, invert the coverage value. See glSampleCoverage."},
    {GL_SAMPLE_MASK, "GL_SAMPLE_MASK", "If enabled, the sample coverage mask generated for a fragment during rasterization will be ANDed with the value of GL_SAMPLE_MASK_VALUE before shading occurs. See glSampleMaski."},
    {GL_SCISSOR_TEST, "GL_SCISSOR_TEST", "If enabled, discard fragments that are outside the scissor rectangle. See glScissor."},
    {GL_STENCIL_TEST, "GL_STENCIL_TEST", "If enabled, do stencil testing and update the stencil buffer. See glStencilFunc and glStencilOp."},
    {GL_TEXTURE_CUBE_MAP_SEAMLESS, "GL_TEXTURE_CUBE_MAP_SEAMLESS", "If enabled, cubemap textures are sampled such that when linearly sampling from the border between two adjacent faces, texels from both faces are used to generate the final sample value. When disabled, texels from only a single face are used to construct the final sample value."},
#if (GLAD_OPENGL_CORE_VERSION >= 45)
	{GL_SAMPLE_SHADING, "GL_SAMPLE_SHADING", "If enabled, the active fragment shader is run once for each covered sample, or at fraction of this rate as determined by the current value of GL_MIN_SAMPLE_SHADING_VALUE. See glMinSampleShading."},
	{GL_PRIMITIVE_RESTART, "GL_PRIMITIVE_RESTART", "Enables primitive restarting. If enabled, any one of the draw commands which transfers a set of generic attribute array elements to the GL will restart the primitive when the index of the vertex is equal to the primitive restart index. See glPrimitiveRestartIndex."},
    {GL_PROGRAM_POINT_SIZE, "GL_PROGRAM_POINT_SIZE", "If enabled and a vertex or geometry shader is active, then the derived point size is taken from the (potentially clipped) shader builtin gl_PointSize and clamped to the implementation-dependent point size range."},
#endif
};

#define GLENABLE_ARCHIVE engine::get_absolute_from_project("gl_settings.json")

void menus::load_settings_glEnable() {
    std::ifstream fs(GLENABLE_ARCHIVE);
	// If there's no file, set current settings
    if (!fs.is_open()) {
        for (auto &s : gl_enable_settings)
            s.value = glIsEnabled(s.index);
        return;
		
    }
	// If there is a file, set the file's settings
    cereal::JSONInputArchive ar(fs);
    ar(gl_enable_settings);
	
    for (auto &s : gl_enable_settings)
        if (s.value)
            glEnable(s.index);
        else
            glDisable(s.index);
}

void menus::window_glEnable_config(bool *p_open) {
    ImGui::Begin("glEnable", p_open);
    if (ImGui::Button("Save")) {
        std::ofstream fs(GLENABLE_ARCHIVE);
        cereal::JSONOutputArchive ar(fs);
        // ar.makeArray();
        ar(CEREAL_NVP(gl_enable_settings));
    }

    for (auto &s : gl_enable_settings) {
        ImGui::Checkbox(s.name.c_str(), &s.value);
        if (ImGui::IsItemActivated())
            if (s.value)
                glEnable(s.index);
            else
                glDisable(s.index);

        ImGui::SameLine(0, 5);
        ImGui::TextDisabled("(?)");
        if (ImGui::IsItemHovered()) {
            // ImGui::SetTooltip(s.info);
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(s.info.c_str());
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }

        struct LogicOp {
            uint16_t index;
            std::string name;
        } static logic_ops[] = {
            {GL_CLEAR, "GL_CLEAR"},
            {GL_SET, "GL_SET"},
            {GL_COPY, "GL_COPY"},
            {GL_COPY_INVERTED, "GL_COPY_INVERTED"},
            {GL_NOOP, "GL_NOOP"},
            {GL_INVERT, "GL_INVERT"},
            {GL_AND, "GL_AND"},
            {GL_NAND, "GL_NAND"},
            {GL_OR, "GL_OR"},
            {GL_NOR, "GL_NOR"},
            {GL_XOR, "GL_XOR"},
            {GL_EQUIV, "GL_EQUIV"},
            {GL_AND_REVERSE, "GL_AND_REVERSE"},
            {GL_AND_INVERTED, "GL_AND_INVERTED"},
            {GL_OR_REVERSE, "GL_OR_REVERSE"},
            {GL_OR_INVERTED, "GL_OR_INVERTED"},
        };

        switch (s.index) {
            case GL_COLOR_LOGIC_OP: {
                // Find our logic op index in our array only the first time
                static int logic_ops_i = -1;
                if (logic_ops_i == -1)
                    logic_ops_i =
                        (std::find_if(std::begin(logic_ops),
                                      std::end(logic_ops),
                                      [](LogicOp &o) -> bool {
                                          int j;
                                          glGetIntegerv(GL_LOGIC_OP_MODE, &j);
                                          return j == o.index;
                                      }) -
                         std::begin(logic_ops));

                // Format the logic ops for the dropdown menu
                static std::string logic_ops_str;
                if (logic_ops_str.size() == 0)
                    for (auto &o : logic_ops) {
                        logic_ops_str += o.name + '\0';
                    }
                // Do the dropdown menu
                if (ImGui::Combo("glLogicOp", &logic_ops_i, logic_ops_str.c_str())) {
                    glLogicOp(logic_ops[logic_ops_i].index);
                }
                break;
            }
            default:
                break;
        }
    }

    ImGui::End();
}