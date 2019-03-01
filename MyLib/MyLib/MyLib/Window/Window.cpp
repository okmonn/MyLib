#include "Window.h"
#include "../etc/Func.h"
#include "../../resource.h"
#include <Windows.h>
#include <tchar.h>

std::optional<std::string> Window::dropFile = std::nullopt;

// コンストラクタ
Window::Window(const Vec2& pos, const Vec2& size, void* parent) : 
	handle(nullptr), instance(nullptr), name(nullptr)
{
	CreateWnd(pos, size, parent);
}

// デストラクタ
Window::~Window()
{
	DestroyWindow(HWND(handle));
	UnregisterClass(name, HINSTANCE(instance));
}

// ウィンドウコールバック
#ifdef _WIN64

__int64 __stdcall Window::WindowProc(void * hWnd, uint message, uint64 wParam, int64 lParam)
#else 
long __stdcall Window::WindowProc(void * hWnd, uint message, uint wParam, long lParam)
#endif
{
	//ドロップされたファイル数
	uint fileNum = 0;
	//ファイル名文字数
	uint size = 0;
	//ドロップ管理
	HDROP drop = nullptr;

	switch (message)
	{
	case WM_DESTROY:
		if (GetParent(HWND(hWnd)) != nullptr)
		{
			break;
		}
		PostQuitMessage(0);
		return 0;
	case WM_DROPFILES:
		//ファイル数取得
		fileNum = DragQueryFileA(HDROP(wParam), -1, nullptr, 0);
		if (fileNum > 1)
		{
			break;
		}

		size = DragQueryFileA(HDROP(wParam), 0, nullptr, 0);
		drop = HDROP(wParam);
		dropFile->resize(size);

		//ファイルパス取得
		DragQueryFileA(drop, 0, &dropFile->at(0), sizeof(dropFile->at(0)) * size);

		//ウィンドウをアクティブにする
		SetForegroundWindow(HWND(hWnd));
		break;
	default:
		break;
	}

	return DefWindowProc(HWND(hWnd), message, wParam, lParam);
}

// ウィンドウ生成
int Window::CreateWnd(const Vec2& pos, const Vec2& size, void* parent)
{
	WNDCLASSEX wnd{};
	wnd.cbSize        = sizeof(WNDCLASSEX);
	wnd.hbrBackground = CreateSolidBrush(COLOR_APPWORKSPACE);
	wnd.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wnd.hIcon         = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(AVICII));
	wnd.hIconSm       = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(AVICII));
	wnd.hInstance     = GetModuleHandle(0);
	wnd.lpfnWndProc   = WNDPROC(WindowProc);
	wnd.lpszClassName = _T("おかもん");
	wnd.lpszMenuName  = _T("おかもん");
	wnd.style         = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&wnd);

	auto flag = (parent == nullptr) ? (WS_OVERLAPPEDWINDOW) : (WS_OVERLAPPEDWINDOW | WS_POPUP);

	RECT rect{};
	rect.bottom = static_cast<long>(size.y);
	rect.right  = static_cast<long>(size.x);
	AdjustWindowRect(&rect, flag, false);

	handle = CreateWindowEx(WS_EX_ACCEPTFILES, wnd.lpszClassName, _T("おかもん"), flag, pos.x, pos.y,
		(rect.right - rect.left), (rect.bottom - rect.top), HWND(parent), nullptr, wnd.hInstance, nullptr);
	if (handle == nullptr)
	{
		func::DebugLog("ウィンドウ生成：失敗");
		return -1;
	}

	instance = wnd.hInstance;
	name     = wnd.lpszClassName;

	ShowWindow(reinterpret_cast<HWND>(handle), SW_SHOW);

	return 0;
}

// ハンドルの取得
void * Window::Get(void) const
{
	return handle;;
}

// ドロップファイル取得
std::optional<std::string> Window::GetDrop(void)
{
	auto tmp = dropFile;
	tmp = std::nullopt;
	return tmp;
}
