#include <d3dcompiler.h>
#include "Root.h"
#include "../etc/Func.h"
#include "../etc/Release.h"

// コンストラクタ
Root::Root() : 
	root(nullptr), sig(nullptr), vertex(nullptr), geometry(nullptr), pixel(nullptr), compute(nullptr)
{
}

// デストラクタ
Root::~Root()
{
	Release(compute);
	Release(pixel);
	Release(geometry);
	Release(vertex);
	Release(sig);
	Release(root);
}

// 頂点シェーダコンパイル
void Root::Vertex(const std::string& fileName, const std::string& func, const std::string& ver)
{
	if (fileName.find(".hlsl") != std::string::npos)
	{
		Compile(fileName, func, ver, &vertex);
	}
	else if (fileName.find(".cso") != std::string::npos)
	{
		Load(fileName, &vertex);
	}
	else
	{

	}
	
	RootInfo(vertex);
	CreateRoot();
}

// ジオメトリーシェーダコンパイル
void Root::Geometry(const std::string & fileName, const std::string & func, const std::string & ver)
{
	if (fileName.find(".hlsl") != std::string::npos)
	{
		Compile(fileName, func, ver, &geometry);
	}
	else if (fileName.find(".cso") != std::string::npos)
	{
		Load(fileName, &geometry);
	}
	else
	{

	}
}

// ピクセルシェーダコンパイル
void Root::Pixel(const std::string & fileName, const std::string & func, const std::string & ver)
{
	if (fileName.find(".hlsl") != std::string::npos)
	{
		Compile(fileName, func, ver, &pixel);
	}
	else if (fileName.find(".cso") != std::string::npos)
	{
		Load(fileName, &pixel);
	}
	else
	{

	}
}

// コンピュートシェーダコンパイル
void Root::Compute(const std::string & fileName, const std::string & func, const std::string & ver)
{
	if (fileName.find(".hlsl") != std::string::npos)
	{
		Compile(fileName, func, ver, &compute);
	}
	else if (fileName.find(".cso") != std::string::npos)
	{
		Load(fileName, &compute);
	}
	else
	{
		
	}
	
	RootInfo(compute);
	CreateRoot();
}

// シェーダコンパイル
long Root::Compile(const std::string& fileName, const std::string& func, const std::string& ver, ID3DBlob** blob)
{
	auto pass = func::ChangeCode(fileName);
	auto hr = D3DCompileFromFile(pass.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, func.c_str(), ver.c_str(),
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, *(&blob), nullptr);
	if (FAILED(hr))
	{
		func::DebugLog("シェーダコンパイル：失敗");
	}

	return hr;
}

// .cso読み込み
long Root::Load(const std::string& fileName, ID3DBlob** blob)
{
	auto pass = func::ChangeCode(fileName);
	auto hr = D3DReadFileToBlob(pass.c_str(), blob);
	if (FAILED(hr))
	{
		func::DebugLog(".cso読み込み：失敗");
	}

	return hr;
}

// ルート情報取得
long Root::RootInfo(ID3DBlob* blob)
{
	auto hr = D3DGetBlobPart(blob->GetBufferPointer(), blob->GetBufferSize(), D3D_BLOB_PART::D3D_BLOB_ROOT_SIGNATURE, 0, &sig);
	if (FAILED(hr))
	{
		func::DebugLog("ルートシグネチャ情報取得：失敗");
	}

	return hr;
}

// ルートシグネチャ生成
long Root::CreateRoot(void)
{
	auto hr = Dev->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&root));
	if (FAILED(hr))
	{
		func::DebugLog("ルートシグネチャ生成：失敗");
	}

	return hr;
}

// ルートシグネチャ取得
ID3D12RootSignature* Root::Get(void) const
{
	return root;
}

// シグネチャ取得
ID3DBlob* Root::GetSig(void) const
{
	return sig;
}

// 頂点シェーダ取得
ID3DBlob* Root::GetVertex(void) const
{
	return vertex;
}

// ジオメトリーシェーダ取得
ID3DBlob* Root::GetGeometry(void) const
{
	return geometry;
}

// ピクセルシェーダ取得
ID3DBlob* Root::GetPixel(void) const
{
	return pixel;
}

// コンピュートシェーダ取得
ID3DBlob* Root::GetCompute(void) const
{
	return compute;
}
