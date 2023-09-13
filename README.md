# Im-Gui-DirectX12-Standalone-Window
### Illustrates how one could create a standalone window in ImGui DirectX12 with no borders and such.

![ALT TEXT](/images/window_showcase.png)

This project includes a loading view and a small login form.
It is made solely for the purpose of learning and providing "intellectual content" and is not made for production.
Most of the graphics are hand-drawn into the window using ImGui as DirectX12 "interface".

## Changes made to ImGui
All changes made to ImGui are located in the imgui.h/imgui.cpp and are contained within the RenderHelper namespace.
```c++
namespace RenderHelper
{
    void LoadingIndicatorCircle(const char* label, const float indicator_radius, const ImVec4& main_color, const ImVec4& backdrop_color, const int circle_count, const float speed);
    bool BufferingBar(const char* label, float value, const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col);
    bool LoadingBar(const char* label, float value, const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col);
    bool Spinner(const char* label, float radius, int thickness, const ImU32& color);
}
```

<details>
  <summary>LoadingIndicatorCircle Implementation</summary>
  
  ```c++
  void ImGui::RenderHelper::LoadingIndicatorCircle(const char* label, const float indicator_radius, const ImVec4& main_color, const ImVec4& backdrop_color, const int circle_count, const float speed)
     {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) {
        return;
    }

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label);

    const ImVec2 pos = window->DC.CursorPos;
    const float circle_radius = indicator_radius / 10.0f;
    const ImRect bb(pos, ImVec2(pos.x + indicator_radius * 2.0f,
        pos.y + indicator_radius * 2.0f));

    
    ItemSize(bb, GetStyle().FramePadding.y);
    if (!ItemAdd(bb, id)) {
        return;
    }
    const float t = g.Time;
    const auto degree_offset = 2.0f * IM_PI / circle_count;
    for (int i = 0; i < circle_count; ++i) {
        const auto x = indicator_radius * sin(degree_offset * i);
        const auto y = indicator_radius * cos(degree_offset * i);
        const auto growth = max(0.0f, sin(t * speed - i * degree_offset));
        ImVec4 color;
        color.x = main_color.x * growth + backdrop_color.x * (1.0f - growth);
        color.y = main_color.y * growth + backdrop_color.y * (1.0f - growth);
        color.z = main_color.z * growth + backdrop_color.z * (1.0f - growth);
        color.w = 1.0f;
        window->DrawList->AddCircleFilled(ImVec2(pos.x + indicator_radius + x,
            pos.y + indicator_radius - y),
            circle_radius + growth * circle_radius,
            GetColorU32(color));
    }
}
  ```
</details>

<details>
  <summary>BufferingBar Implementation</summary>
  
  ```c++
  bool ImGui::RenderHelper::BufferingBar(const char* label, float value, const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = size_arg;
    size.x -= style.FramePadding.x * 2;

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    // Render
    const float circleStart = size.x * 0.7f;
    const float circleEnd = size.x;
    const float circleWidth = circleEnd - circleStart;

    PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);
    window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart, bb.Max.y), bg_col, 20.f);
    window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart * value, bb.Max.y), fg_col, 20.f);
    PopStyleVar();

    const float t = g.Time;
    const float r = size.y / 2;
    const float speed = 1.5f;

    const float a = speed * 0;
    const float b = speed * 0.333f;
    const float c = speed * 0.666f;

    const float o1 = (circleWidth + r) * (t + a - speed * (int)((t + a) / speed)) / speed;
    const float o2 = (circleWidth + r) * (t + b - speed * (int)((t + b) / speed)) / speed;
    const float o3 = (circleWidth + r) * (t + c - speed * (int)((t + c) / speed)) / speed;

    window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o1, bb.Min.y + r), r, bg_col);
    window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o2, bb.Min.y + r), r, bg_col);
    window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o3, bb.Min.y + r), r, bg_col);
}
  ```
</details>

<details>
  <summary>LoadingBar Implementation</summary>
  
  ```c++
  bool ImGui::RenderHelper::LoadingBar(const char* label, float value, const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = size_arg;
    size.x -= style.FramePadding.x * 2;

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    // Render
    const float circleStart = size.x * 0.7f;
    const float circleEnd = size.x;
    const float circleWidth = circleEnd - circleStart;

    PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);
    window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart, bb.Max.y), bg_col, 20.f);
    window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart * value, bb.Max.y), fg_col, 20.f);
    PopStyleVar();
}
  ```
</details>

<details>
  <summary>Spinner Implementation</summary>
  
  ```c++
  bool ImGui::RenderHelper::Spinner(const char* label, float radius, int thickness, const ImU32& color) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    // Render
    window->DrawList->PathClear();

    int num_segments = 30;
    int start = abs(ImSin(g.Time * 1.8f) * (num_segments - 5));

    const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
    const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

    const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

    for (int i = 0; i < num_segments; i++) {
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + g.Time * 8) * radius,
            centre.y + ImSin(a + g.Time * 8) * radius));
    }

    window->DrawList->PathStroke(color, false, thickness);
}
  ```
</details>

# Demo
![](https://i.gyazo.com/0d7261a83201d2f6ddff9c62de9d9d0d.gif))

# This Project is running under the MIT license
```
MIT License

Copyright (c) 2023 Steve Oberst

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
