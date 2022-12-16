#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <string>
#include "Vector2.h"
#include "Vector3.h"
#include "WinApp.h"

class ImGuiManager final
{
private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
	ImGuiManager() = default;

public:
	static ImGuiManager* GetInstance();
	ImGuiManager(const ImGuiManager& obj) = delete;

	void Initialize();
	void Begin();
	void End();
	void Draw();
	void Finalize();
	void PrintVector(std::string str, Vector2 vec);
	void PrintVector(std::string str, Vector3 vec);
	void SliderVector(std::string str, Vector2& vec, int minVal = 0, int maxVal = WinApp::kWindowWidth);
	void SliderVector(std::string str, Vector3& vec, int minVal = 0, int maxVal = WinApp::kWindowWidth);
	void DragVector(std::string str, Vector2& vec, float spd = 0.1f);
	void DragVector(std::string str, Vector3& vec, float spd = 0.1f);
};