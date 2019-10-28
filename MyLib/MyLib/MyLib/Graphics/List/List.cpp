#include "List.h"
#include "../Device/Device.h"
#include "../Allocator/Allocator.h"
#include <d3d12.h>
#include <crtdbg.h>

// �R���X�g���N�^
List::List(std::weak_ptr<Allocator>allo, const D3D12_COMMAND_LIST_TYPE& type) :
	allo(allo), list(nullptr)
{
	CreateList(type);
}

// �f�X�g���N�^
List::~List()
{
}

// �R�}���h���X�g����
void List::CreateList(const D3D12_COMMAND_LIST_TYPE& type)
{
	auto hr = Device::Get().Dev()->CreateCommandList(0, type, allo.lock()->Get(), nullptr, IID_PPV_ARGS(&list));
	_ASSERT(hr == S_OK);

	hr = list->Close();
	_ASSERT(hr == S_OK);
}

// �r���[�|�[�g�Z�b�g
void List::Viewport(const Vec2& size)
{
	D3D12_VIEWPORT view{};
	view.Height   = float(size.y);
	view.MaxDepth = 1.0f;
	view.Width    = float(size.x);

	list->RSSetViewports(1, &view);
}

// �V�U�[�Z�b�g
void List::Scissor(const Vec2& size)
{
	RECT scissor{};
	scissor.bottom = long(size.y);
	scissor.right  = long(size.x);

	list->RSSetScissorRects(1, &scissor);
}

// �o���A�Z�b�g
void List::Barrier(ID3D12Resource* rsc, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& affter)
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Transition.pResource   = rsc;
	barrier.Transition.StateBefore = befor;
	barrier.Transition.StateAfter  = affter;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	list->ResourceBarrier(1, &barrier);
}

// UAV�o���A�Z�b�g
void List::Barrier(ID3D12Resource* rsc)
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type          = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.UAV.pResource = rsc;

	list->ResourceBarrier(1, &barrier);
}

// �O���t�B�b�N���[�g�e�[�u���Z�b�g
void List::SetGraphicsTable(const unsigned int& id, ID3D12DescriptorHeap* heap, const size_t& index)
{
	auto handle = heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += unsigned __int64(Device::Get().Dev()->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index);

	list->SetGraphicsRootDescriptorTable(id, handle);
}

// �R���s���[�g���[�g�e�[�u���Z�b�g
void List::SetComputeTable(const unsigned int& id, ID3D12DescriptorHeap* heap, const size_t& index)
{
	auto handle = heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += unsigned __int64(Device::Get().Dev()->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index);

	list->SetComputeRootDescriptorTable(id, handle);
}

// �R�}���h���X�g�擾
ID3D12GraphicsCommandList5* List::Get(void) const
{
	return list.Get();
}