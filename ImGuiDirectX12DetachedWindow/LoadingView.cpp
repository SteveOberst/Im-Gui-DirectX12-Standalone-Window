#include "LoadingView.h"

LoadingView::LoadingView(
	const char* title, 
	ImVec2 dimensions,
	int flags, 
	ImColor main_window_color,
	ImColor secondary_window_color,
	double animation_time
) : Window(title, dimensions, flags, main_window_color, secondary_window_color)
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	this->text_font = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/AccidentalPresidency.ttf", 75.0f);
	this->animation_time = animation_time;
	this->initialization_time = ImGui::GetTime();
}

LoadingView::~LoadingView()
{
}

std::string repeat_string(const std::string& str, int count) {
	std::string result;
	for (int i = 0; i < count; ++i) {
		result += str;
	}
	return result;
}

// no explanations to this function. don't even attempt to understand what's going on here
std::string get_head_line()
{
	std::string headline = std::string("Loading");
	const double time = ImGui::GetTime();
	static const int speed = 2;
	static const double max = 2.999f / speed;
	// std::max(std::fmod(time, (double)1) / .33f), max)
	double result = (((std::fmod(time, 1 * speed) * .33f) < (max)) ? (std::fmod(time, 1 * speed) * .33f) : (max)) * 10 / speed;
	return headline.append(repeat_string(".", result));
}

void LoadingView::draw_layout()
{
	ImGui::PushFont(text_font);

	RenderHelper::DrawCenteredText(get_head_line().c_str());
	ImGui::PopFont();
	
	this->progress = min((animation_time - (((initialization_time + animation_time) - ImGui::GetTime()))) / animation_time, 1);

	const ImVec2 win_size = ImGui::GetWindowSize();
	const ImU32 col = ImColor(0.06f, 0.53f, 0.98f, 1.00f);
	const ImU32 bg = ImColor(0.26f, 0.59f, 0.98f, 0.40f);
	const ImVec2 rect_size = ImVec2(400, 6);
	ImGui::SetCursorPosX(win_size.x / 2 - rect_size.x / 3);
	ImGui::RenderHelper::LoadingBar("##buffer_bar", this->progress, rect_size, bg, col);
}

void LoadingView::push_style_vars()
{
}

void LoadingView::pop_style_vars()
{
}

bool LoadingView::is_done()
{
	return progress >= 1.0f;
}
