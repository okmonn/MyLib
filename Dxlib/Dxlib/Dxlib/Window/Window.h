#pragma once
#include "../Vector/Vector2.h"

class Window
{
public:
	// コンストラクタ
	inline Window(const Vec2& pos, const Vec2& size, void* parent = nullptr);
	// デストラクタ
	~Window();

	// ハンドルの取得
	inline void* Get(void) const;

private:
	// ウィンドウのコールバック
#ifdef _WIN64
	static __int64 __stdcall WindowProc(void* hWnd, unsigned int message, unsigned __int64 wParam, __int64 lParam);
#else
	static long __stdcall WindowProc(void* hWnd, unsigned int message, unsigned int wParam, long lParam);
#endif

	// ウィンドウ生成
	int CreateWnd(const Vec2& pos, const Vec2& size, void* parent);


	// ウィンドウハンドル
	void* handle;

	// ウィンドウインスタンス
	void* instance;

	// ウィンドウ名
	const wchar_t* name;
};
