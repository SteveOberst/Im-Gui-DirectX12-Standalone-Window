#pragma once
#include "includes.h"
#include "Drawable.h"
#include "LoginView.h"
#include "LoadingView.h"

#include "cpprest/http_client.h"
#include "cpprest/json.h"

#include <d3d12.h>
#include "DirectXTK12/ResourceUploadBatch.h"
#include <wrl.h>

#include <wrl/client.h>

#include <string>

#define MAX_VERSIONS 8

namespace Loader
{
	struct LoginCredentials;
	struct SessionInfo;
	struct VersionDescription;

	void Initialize(ID3D12Device* device);
	void PreRender();
	void RenderUI();
	void PostRender();
	void LoginUser(LoginCredentials* credentials);
	HRESULT GetMachineGuid(LPWSTR buffer, DWORD bufferSize);
	ID3D12Device* GetDevice();
	void WaitForGPU();

	//HRESULT D3DCreateTexture(BYTE* data, ID3D12Resource** texture);

	SessionInfo* GetCurrentSession();
	VersionDescription** GetVersions();

	struct LoginCredentials
	{
		const char* username;
		const char* password;
		wchar_t guid[39];
	};

	struct SessionInfo
	{
		const char* username;
		const char* session_key;
		int permissions;

		bool has_permission(int permission)
		{
			return permissions & permission;
		}
	};

	struct VersionDescription
	{
		const char* version_name;
		const char* feature_list;
		std::string description;
		int required_permission;
		unsigned long long expiry_date;
	};
}