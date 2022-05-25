/**
 * @file  imgui_helper.h
 * @author RubenD (rubendariopm14@gmail.com)
 * @brief Defines  imgui_helper
 * @version 0.1
 * 
 */
#ifndef imgui_helper_h
#define imgui_helper_h

#include <memory>
#include <vector>
#include "imgui.h"
#include "types.h"

/**
 * @brief Forms either a Drag and Drop "DND_REF" source or target, pulls/puts in 'refs'
 * 
 * @param refs If target will be filled with data, else cleared -- If source, it will just be read
 * @param source If true, it's a Drag and Drop (DND) source, else it's a target
 * @param source_flags Flags to be set if source
 */
void imgui_dnd_ref(std::vector<uint>& refs, bool source, ImGuiDragDropFlags source_flags = 0);

#endif	//  imgui_helper_h