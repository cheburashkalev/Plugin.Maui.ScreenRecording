#pragma once
#include "CommonTypes.h"
#include "ScreenCaptureBase.h"
#include <memory>
#include "MouseManager.h"
#include "TextureManager.h"

class DesktopDuplicationCapture : public CaptureBase
{
public:
	DesktopDuplicationCapture();
	virtual ~DesktopDuplicationCapture();
	virtual HRESULT Initialize(_In_ ID3D11DeviceContext *pDeviceContext, _In_ ID3D11Device *pDevice) override;
	virtual HRESULT AcquireNextFrame(_In_ DWORD timeoutMillis, _Outptr_opt_ ID3D11Texture2D **ppFrame) override;
	virtual HRESULT WriteNextFrameToSharedSurface(_In_ DWORD timeoutMillis, _Inout_ ID3D11Texture2D *pSharedSurf, INT offsetX, INT offsetY, _In_ RECT destinationRect, _In_opt_ ID3D11Texture2D *pTexture = nullptr) override;
	virtual HRESULT StartCapture(_In_ RECORDING_SOURCE_BASE &recordingSource) override;
	virtual HRESULT GetNativeSize(_In_ RECORDING_SOURCE_BASE &recordingSource, _Out_ SIZE *nativeMediaSize) override;
	virtual HRESULT GetMouse(_Inout_ PTR_INFO *pPtrInfo, _In_ RECT frameCoordinates, _In_ int offsetX, _In_ int offsetY) override;
	virtual inline std::wstring Name() override { return L"DesktopDuplicationCapture"; };
private:
	static const int NUMVERTICES = 6;
	// methods
	HRESULT InitializeDesktopDuplication(std::wstring deviceName);
	HRESULT GetNextFrame(_In_ DWORD timeoutMillis, _Inout_ DUPL_FRAME_DATA *pData);
	HRESULT CopyDirty(_In_ ID3D11Texture2D *pSrcSurface, _Inout_ ID3D11Texture2D *pSharedSurf, _In_reads_(dirtyCount) RECT *pDirtyBuffer, UINT dirtyCount, INT offsetX, INT offsetY, _In_ RECT desktopCoordinates, _In_ DXGI_MODE_ROTATION rotation);
	HRESULT CopyMove(_Inout_ ID3D11Texture2D *pSharedSurf, _In_reads_(moveCount) DXGI_OUTDUPL_MOVE_RECT *pMoveBuffer, UINT moveCount, INT offsetX, INT offsetY, _In_ RECT desktopCoordinates, _In_ DXGI_MODE_ROTATION rotation);
	void SetDirtyVert(_Out_writes_(NUMVERTICES) VERTEX *pVertices, _In_ RECT *pDirty, INT offsetX, INT offsetY, _In_ RECT desktopCoordinates, _In_ DXGI_MODE_ROTATION rotation, _In_ D3D11_TEXTURE2D_DESC *pFullDesc, _In_ D3D11_TEXTURE2D_DESC *pThisDesc);
	void SetMoveRect(_Out_ RECT *SrcRect, _Out_ RECT *pDestRect, _In_ DXGI_MODE_ROTATION rotation, _In_ DXGI_OUTDUPL_MOVE_RECT *pMoveRect, INT texWidth, INT texHeight);
	HRESULT SendBitmapCallback(_In_ ID3D11Texture2D *pSharedSurf, _In_ SIZE frameOffset, _In_ SIZE contentOffset, _In_ RECT destinationRect);

	std::unique_ptr<MouseManager> m_MouseManager;
	DUPL_FRAME_DATA m_CurrentData;

	ID3D11Texture2D *m_BitmapDataCallbackTexture;
	D3D11_TEXTURE2D_DESC m_BitmapDataCallbackTextureDesc;
	PTR_INFO m_BitmapDataCallbackPtrInfo;

	int m_CursorOffsetX;
	int m_CursorOffsetY;
	float m_CursorScaleX;
	float m_CursorScaleY;

	bool m_IsInitialized;
	LARGE_INTEGER m_LastSampleUpdatedTimeStamp;

	bool m_OutputIsOnSeparateGraphicsAdapter;
	IDXGIOutputDuplication *m_DeskDupl;
	ID3D11Texture2D *m_MoveSurf;
	_Field_size_bytes_(m_MetaDataSize) BYTE *m_MetaDataBuffer;
	UINT m_MetaDataSize;
	DXGI_OUTPUT_DESC m_OutputDesc;
	ID3D11VertexShader *m_VertexShader;
	ID3D11PixelShader *m_PixelShader;
	ID3D11InputLayout *m_InputLayout;
	ID3D11RenderTargetView *m_RTV;
	ID3D11SamplerState *m_SamplerLinear;
	BYTE *m_DirtyVertexBufferAlloc;
	UINT m_DirtyVertexBufferAllocSize;
};
