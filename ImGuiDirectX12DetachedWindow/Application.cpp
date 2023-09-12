#include "Application.h"

#define DEFAULT_VIEW_FLAGS ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | \
					       ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar

#define LOGIN_VIEW_FLAGS DEFAULT_VIEW_FLAGS
#define LOADING_VIEW_FLAGS DEFAULT_VIEW_FLAGS

/*utility::string_t toUtilityString(const char* str) {
ifdef _UTF16_STRINGS
	return utility::conversions::to_utf16string(str);
#else
	return utility::conversions::to_string_t(str);
#endif
}*/

//static http_client* hClient = nullptr;

static Drawable* current_view;
static Drawable* loading_view;
static Drawable* login_view;

static Loader::VersionDescription* descriptions[MAX_VERSIONS] = {
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
};

static Loader::SessionInfo current_session;

ID3D12Device* pDevice;
ID3D12CommandQueue* pCommandQueue;


void Loader::Initialize(ID3D12Device* device)
{
	pDevice = device;

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; // Specify the command list type you are using

	ID3D12CommandQueue* commandQueue;
	HRESULT hr = device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Error", "ID3D12CommandQueue creation failed.", NULL);
	}

	loading_view = new LoadingView("Loading", ImVec2(900, 530), LOADING_VIEW_FLAGS, ImColor(24, 29, 59, 220), ImColor(24, 29, 59, 255), 5);
	login_view = new LoginView("Login", ImVec2(350, 360), LOGIN_VIEW_FLAGS, ImColor(24, 29, 59, 220), ImColor(24, 29, 59, 255));
	current_view = loading_view;
}

/*HRESULT Loader::D3DCreateTexture(BYTE* data, ID3D12Resource** texture)
{
	// Create a ResourceUploadBatch instance
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	Loader::GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	Loader::GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList));

	DirectX::ResourceUploadBatch resourceUpload(Loader::GetDevice());

	// Schedule the texture upload
	resourceUpload.Begin(commandList.Get()->GetType());
	HRESULT hr = DirectX::CreateWICTextureFromMemory(
		Loader::GetDevice(),
		resourceUpload,
		reinterpret_cast<const uint8_t*>(data),
		strlen(reinterpret_cast<const char*>(data)),
		texture
	);
	resourceUpload.End(pCommandQueue);

	// Execute the commands
	commandList->Close();
	ID3D12CommandList* commandLists[] = { commandList.Get() };
	pCommandQueue->ExecuteCommandLists(1, commandLists);

	// Wait for the GPU to finish executing the commands
	Loader::WaitForGPU();

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Error", "Failed to create texture.", NULL);
	}
	return hr;
}*/

void Loader::WaitForGPU()
{
	// Create a fence object
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	HRESULT hr = pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	if (FAILED(hr))
	{
		// Handle the error
		return;
	}

	// Signal the fence with the current value
	UINT64 fenceValue = 1; // Initial fence value
	hr = pCommandQueue->Signal(fence.Get(), fenceValue);
	if (FAILED(hr))
	{
		// Handle the error
		return;
	}

	// Wait until the fence is signaled
	if (fence->GetCompletedValue() < fenceValue)
	{
		HANDLE eventHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (eventHandle != nullptr)
		{
			hr = fence->SetEventOnCompletion(fenceValue, eventHandle);
			if (SUCCEEDED(hr))
			{
				WaitForSingleObject(eventHandle, INFINITE);
			}
			CloseHandle(eventHandle);
		}
	}
}

HRESULT Loader::GetMachineGuid(LPWSTR buffer, DWORD bufferSize) {
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Cryptography", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
		DWORD dataSize = bufferSize;
		DWORD regType;

		if (RegQueryValueEx(hKey, L"MachineGuid", nullptr, &regType, reinterpret_cast<BYTE*>(buffer), &dataSize) == ERROR_SUCCESS) {
			RegCloseKey(hKey);
			return S_OK;
		}

		RegCloseKey(hKey);
	}

	return HRESULT_FROM_WIN32(GetLastError());
}

void Loader::PreRender()
{

}

void Loader::RenderUI()
{
	current_view->draw();
}

void Loader::PostRender()
{
	LoadingView* loading_view;
	if ((loading_view = dynamic_cast<LoadingView*>(current_view)) != nullptr)
	{
		if (loading_view->is_done())
		{
			// TODO: check if login session is valid and skip loginview if valid
			current_view = login_view;
		}
	}
}

void Loader::LoginUser(LoginCredentials* credentials)
{
	MessageBox(NULL, L"demo ended", L"Demo", NULL);
	exit(0);

	/*utility::string_t api_endpoint = toUtilityString("/api/auth");
	uri_builder builder(api_endpoint);
	hClient->request(methods::GET, builder.to_string())
		.then([](http_response response) {
			// Check if the response was successful
			if (response.status_code() == status_codes::OK) { 
				// Read the response body as JSON
				return response.extract_json();
			}
			else {
				throw std::runtime_error("HTTP request failed");
			}
		})
		.then([](json::value jsonData) {
			auto username = utility::conversions::to_utf8string(jsonData.at(0).as_string());
			auto session_key = utility::conversions::to_utf8string(jsonData.at(1).as_string());
			auto permissions = jsonData.at(2).as_integer();
			current_session = SessionInfo{ username.c_str(), session_key.c_str(), permissions};
		})
		.wait();*/
}

Loader::VersionDescription** Loader::GetVersions()
{
	return descriptions;
}

Loader::SessionInfo* Loader::GetCurrentSession()
{
	return &current_session;
}

ID3D12Device* Loader::GetDevice()
{
	return pDevice;
}
