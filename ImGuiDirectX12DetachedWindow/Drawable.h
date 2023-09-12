#pragma once
#include "includes.h"
#include "DirectXTK12/WICTextureLoader.h"
#include <vector>

class Drawable
{
public:
	std::vector<Drawable*> children = std::vector<Drawable*>();

public:
	virtual void draw() = 0;
};

class Window : public Drawable
{
private:
	ImColor main_window_color;
	ImColor secondary_window_color;

public:
	const char* title;
	ImVec2 dimensions;
	int flags = 0;
	bool visible {true};

public:
	Window(const char* title, ImVec2 dimensions, int flags = 0, ImColor main_window_color = ImColor(24, 29, 59, 220), ImColor secondary_window_color = ImColor(24, 29, 59, 255));

	void draw();

	virtual void draw_layout() = 0;
	virtual void push_style_vars() = 0;
	virtual void pop_style_vars() = 0;
};

class WindowComponent : public Drawable
{
public:
	Window* parent {};
	int offset_x = 0;
	int offset_y = 0;

public:
	WindowComponent(Window* parent, int offset_x, int offset_y);
};

namespace RenderHelper
{
	void DrawCenteredText(const char* text);
	ImVec2 GetItemCenterPos(const ImVec2& item_size);
	void CenterItemX(float item_width);
	float CalcCenterPosX(float item_width);
	void Spacing(int count);
	void Text(const char* text, ImFont* font);
	void CenteredText(const char* text, ImFont* font);
	bool ButtonConditioned(const char* label, const ImVec2& size, bool condition = true);
	ImVec2 CalcTextSize(const char* text, ImFont* font);
	HRESULT LoadTexture(ID3D12Device* device, const uint8_t* data, size_t dataSize);
}