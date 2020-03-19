#include "menus/menus.h"

namespace menus
{
    // bool _inspector_open=true;
    // bool inspector_open(){return _inspector_open;}
    void inspector(GameObject* o, bool* p_open)
    {
        ImGui::Begin("Inspector",p_open);
        // ImGui::BeginGroup();
        ImGui::DragFloat3("Position",&(o->pos.x));
        ImGui::DragFloat3("Rotation",&(o->rot.x));
        ImGui::DragFloat3("Scale",&(o->sca.x));
        // ImGui::EndGroup();
        ImGui::End();
    }
} // namespace menus


