#include "LoginView.h"

LoginView::LoginView(
	const char* title,
	ImVec2 dimensions,
	int flags, 
	ImColor main_window_color,
	ImColor secondary_window_color
) : Window(title, dimensions, flags)
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	this->logo_font = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/AccidentalPresidency.ttf", 55.0f);
	this->text_font = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/pivodef.ttf", 15.0f);
	this->text_input_font = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/pivodef.ttf", 25.0f);
	this->button_font = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/pivodef.ttf", 30.0f);
}

LoginView::~LoginView()
{
	delete logo_font;
	delete text_input_font;
	delete button_font;
}

void LoginView::draw_layout()
{
	ImVec2 win_size = ImGui::GetWindowSize();
	const char* title = "Loader";
	RenderHelper::CenteredText(title, logo_font);

	ImGui::PushFont(text_input_font);
	RenderHelper::Spacing(6);

	const float input_text_width = 200.f;
	const char* username_text = "Username";
	TextInputComponent username = TextInputComponent(this, 0, 0);
	ImGui::SetCursorPosX(RenderHelper::CalcCenterPosX(input_text_width));
	RenderHelper::Text(username_text, this->text_font);

	RenderHelper::CenterItemX(input_text_width);
	ImGui::SetNextItemWidth(input_text_width);
	username.draw("##usernamein", this->username);

	const char* password_text = "Password";
	TextInputComponent password = TextInputComponent(this, 0, 0);
	ImGui::SetCursorPosX(RenderHelper::CalcCenterPosX(input_text_width));
	RenderHelper::Text(password_text, this->text_font);

	RenderHelper::CenterItemX(input_text_width);
	ImGui::SetNextItemWidth(input_text_width);
	username.draw("##passwordin", this->password, ImGuiInputTextFlags_Password);
	ImGui::PopFont();

	RenderHelper::Spacing(2);

	ImGui::PushFont(text_font);
	ImGui::SetCursorPosX(RenderHelper::CalcCenterPosX(input_text_width));
	ImGui::Checkbox("Keep me signed in", &this->stay_logged_in);
	ImGui::PopFont();

	ImGui::PushFont(button_font);
	const float button_width = 200;
	const char* button_text = "Login";
	RenderHelper::Spacing(3);
	ImGui::SetNextItemWidth(button_width);
	RenderHelper::CenterItemX(button_width);
	ButtonComponent login_button = ButtonComponent(this, 0, 0);
	if (login_button.draw(button_text, ImVec2(button_width, ImGui::CalcTextSize(button_text).y + 20)))
	{
		Loader::LoginCredentials* credentials = new struct Loader::LoginCredentials();
		HRESULT hr = Loader::GetMachineGuid(credentials->guid, sizeof(credentials->guid) / sizeof(wchar_t));
		if (FAILED(hr))
		{
			MessageBoxA(NULL, "Failed to generate machine GUID", "Error", NULL);
			return;
		}

		credentials->username = this->username;
		credentials->password = this->password;
		Loader::LoginUser(credentials);
		delete credentials;
	}
	ImGui::PopFont();
}

void LoginView::push_style_vars()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.f);
}

void LoginView::pop_style_vars()
{
	ImGui::PopStyleVar(1);
}

TextInputComponent::TextInputComponent(Window* parent, int offset_x, int offset_y) : WindowComponent(parent, offset_x, offset_y)
{
}

void TextInputComponent::draw() {}

void TextInputComponent::draw(const char* label, char* buffer, int flags)
{
	push_style_vars();
	ImVec2 win_pos = ImGui::GetWindowPos();
	//ImGui::SetCursorScreenPos(ImVec2(win_pos.x + this->offset_x, win_pos.y + this->offset_y));
	ImGui::InputText(label, buffer, sizeof(char) * MAX_PATH, flags);
	pop_style_vars();
}

void TextInputComponent::push_style_vars()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);
}

void TextInputComponent::pop_style_vars()
{
    ImGui::PopStyleVar(1);
}

ButtonComponent::ButtonComponent(Window* parent, int offset_x, int offset_y) : WindowComponent(parent, offset_x, offset_y)
{
}

void ButtonComponent::draw()
{
}

bool ButtonComponent::draw(const char* label, int flags)
{
	push_style_vars();
	bool down = ImGui::Button(label);
	pop_style_vars();
	return down;
}

bool ButtonComponent::draw(const char* label, const ImVec2& size, int flags)
{
	push_style_vars();
	bool down = ImGui::Button(label, size);
	pop_style_vars();
	return down;
}

void ButtonComponent::push_style_vars()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);
}

void ButtonComponent::pop_style_vars()
{
	ImGui::PopStyleVar(1);
}
