#pragma once
#include "Drawable.h"
#include <string>
#include <algorithm>
#include <cmath>

class LoadingView : public Window
{
private:
	ImFont* text_font;

public:
	double initialization_time;
	double animation_time;
	float progress;

public:
	LoadingView(const char* title, ImVec2 dimensions, int flags, ImColor main_window_color, ImColor secondary_window_color, double animation_time);
	~LoadingView();

public:
	void draw_layout();
	void push_style_vars();
	void pop_style_vars();
	bool is_done();
};