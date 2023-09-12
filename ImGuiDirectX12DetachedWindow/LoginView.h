#pragma once
#include "Drawable.h"
#include "Application.h"

class LoginView : public Window
{
private:
	ImFont* logo_font {};
	ImFont* text_font{};
	ImFont* text_input_font {};
	ImFont* button_font {};
	char username[MAX_PATH];
	char password[MAX_PATH];
	bool stay_logged_in { 0 };

public:
	LoginView(const char* title, ImVec2 dimensions, int flags, ImColor main_window_color, ImColor secondary_window_color);
	~LoginView();
public:
	void draw_layout();
	void push_style_vars();
	void pop_style_vars();
};

class TextInputComponent : public WindowComponent
{
private:
	ImFont* font = nullptr;

public:
	TextInputComponent(Window* parent, int offset_x, int offset_y);

public:
	void draw();
	void draw(const char* label, char* buffer, int flags = 0);
	void push_style_vars();
	void pop_style_vars();
};

class ButtonComponent : public WindowComponent
{
private:
	ImFont* font = nullptr;

public:
	ButtonComponent(Window* parent, int offset_x, int offset_y);

public:
	void draw();
	bool draw(const char* label, int flags = 0);
	bool draw(const char* label, const ImVec2& size, int flags = 0);
	void push_style_vars();
	void pop_style_vars();
};

