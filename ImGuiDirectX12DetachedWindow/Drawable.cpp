#include "Drawable.h"

Window::Window(const char* title, ImVec2 dimensions, int flags, ImColor main_window_color, ImColor secondary_window_color)
{
	this->title = title;
	this->dimensions = dimensions;
	this->flags = flags;
}

void Window::draw()
{
	ImGui::SetNextWindowSize(this->dimensions);
	ImGui::CaptureMouseFromApp(true); 
	ImGui::Begin(this->title, &visible, flags);
	{
		ImVec2 pos = ImGui::GetWindowPos();
		ImDrawList* draw = ImGui::GetWindowDrawList();
		draw->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + dimensions.x, pos.y + dimensions.y), this->main_window_color, 12.f);
		push_style_vars();
		draw_layout();
		pop_style_vars();
	}
	ImGui::End();

}

WindowComponent::WindowComponent(Window* parent, int offset_x, int offset_y)
{
	this->parent = parent;
	this->offset_x = offset_x;
	this->offset_y = offset_y;
}

void RenderHelper::DrawCenteredText(const char* text)
{
	// Get the size of the text
	ImVec2 textSize = ImGui::CalcTextSize(text);

	// Calculate the position to center the text
	ImVec2 window_pos = ImGui::GetWindowPos();
	ImVec2 window_size = ImGui::GetWindowSize();
	ImVec2 textPos = ImVec2(window_pos.x + window_size.x * 0.5f - textSize.x * 0.5f,
		window_pos.y + window_size.y * 0.5f - textSize.y * 0.5f);

	// Set the cursor position to the calculated position
	ImGui::SetCursorScreenPos(textPos);

	// Draw the centered text
	ImGui::Text("%s", text);
}

ImVec2 RenderHelper::GetItemCenterPos(const ImVec2& item_size)
{
	ImVec2 window_pos = ImGui::GetWindowPos();
	ImVec2 window_size = ImGui::GetWindowSize();
	return ImVec2(window_pos.x + window_size.x * 0.5f - item_size.x * 0.5f, window_pos.y + window_size.y * 0.5f - item_size.y * 0.5f);
}

void RenderHelper::CenterItemX(float item_width)
{
	const ImVec2 win_size = ImGui::GetWindowSize();
	ImGui::SetCursorPosX(win_size.x / 2.f - item_width / 2.f);
}

float RenderHelper::CalcCenterPosX(float item_width)
{
	const ImVec2 win_size = ImGui::GetWindowSize();
	return (win_size.x / 2.f - item_width / 2.f);
}

void RenderHelper::Spacing(int count)
{
	for (int i = 0; i < count; i++)
	{
		ImGui::Spacing();
	}
}

void RenderHelper::Text(const char* text, ImFont* font)
{
	ImGui::PushFont(font);
	ImGui::Text(text);
	ImGui::PopFont();
}

void RenderHelper::CenteredText(const char* text, ImFont* font)
{
	ImGui::PushFont(font);
	RenderHelper::CenterItemX(ImGui::CalcTextSize(text).x);
	ImGui::Text(text);
	ImGui::PopFont();
}

bool RenderHelper::ButtonConditioned(const char* label, const ImVec2& size, bool condition)
{
	if (!condition) ImGui::BeginDisabled();
	bool down = ImGui::Button(label, size);
	if (!condition) ImGui::EndDisabled();
	return condition && down;
}

ImVec2 RenderHelper::CalcTextSize(const char* text, ImFont* font)
{
	ImGui::PushFont(font);
	ImVec2 dimensions = ImGui::CalcTextSize(text);
	ImGui::PopFont();
	return dimensions;
}

HRESULT RenderHelper::LoadTexture(ID3D12Device* device, const uint8_t* data, size_t dataSize)
{
	return E_NOTIMPL;
}
