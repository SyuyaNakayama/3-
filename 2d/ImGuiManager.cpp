#include "ImGuiManager.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#include "WinApp.h"
#include "DirectXCommon.h"
#include <cassert>
using namespace ImGui;

ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager instance;
	return &instance;
}

void ImGuiManager::Initialize()
{
	// インスタンスの取得
	WinApp* winApp = WinApp::GetInstance();
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// ImGuiのコンテキストを生成
	ImGui::CreateContext();
	// ImGuiのスタイルを設定
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(winApp->GetHwnd());

	// デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	// デスクリプタヒープ生成
	HRESULT result = dxCommon->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	ImGui_ImplDX12_Init(
		dxCommon->GetDevice(), static_cast<int>(dxCommon->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvHeap_.Get(),
		srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart()
	);

	ImGuiIO& io = ImGui::GetIO();
	// 標準フォントを追加する
	io.Fonts->AddFontDefault();
}

void ImGuiManager::Begin()
{
	// ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	ImGui::Render();
}

void ImGuiManager::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);
}

void ImGuiManager::Finalize()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// デスクリプタヒープを解放
	srvHeap_.Reset();
}

void ImGuiManager::PrintVector(std::string str, Vector2 vec)
{
	str += " = (%.3f, %.3f)";
	Text(str.c_str(), vec.x, vec.y);
}

void ImGuiManager::PrintVector(std::string str, Vector3 vec)
{
	str += " = (%.3f, %.3f, %.3f)";
	Text(str.c_str(), vec.x, vec.y, vec.z);
}

void ImGuiManager::SliderVector(std::string str, Vector2& vec, int minVal, int maxVal)
{
	float num[2] = { vec.x,vec.y };
	SliderFloat2(str.c_str(), num, minVal, maxVal);
	vec = { num[0],num[1] };
}

void ImGuiManager::SliderVector(std::string str, Vector3& vec, int minVal, int maxVal)
{
	float num[3] = { vec.x,vec.y,vec.z };
	SliderFloat3(str.c_str(), num, minVal, maxVal);
	vec = { num[0],num[1],num[2] };
}

void ImGuiManager::DragVector(std::string str, Vector2& vec, float spd)
{
	float num[2] = { vec.x,vec.y };
	DragFloat2(str.c_str(), num, spd);
	vec = { num[0],num[1] };
}

void ImGuiManager::DragVector(std::string str, Vector3& vec, float spd)
{
	float num[3] = { vec.x,vec.y,vec.z };
	DragFloat3(str.c_str(), num, spd);
	vec = { num[0],num[1],num[2] };
}
