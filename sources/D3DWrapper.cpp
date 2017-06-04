#include "../includes/D3DWrapper.hpp"

bool	CreateBuffer(ID3D11Device* const pDevice, ID3D11Buffer** pBuffer, UINT const pBytesWidth, D3D11_USAGE const pUsage, UINT const pBindFlags, UINT const pCPUAccessFlags, UINT const pMiscFlags, UINT const pStructueBytesStride, void const* pData, UINT const pDataMemPitch, UINT const pDataMemSlicePitch)
{
	HRESULT					lResult;
	D3D11_BUFFER_DESC		lBufferDesc;
	D3D11_SUBRESOURCE_DATA	lBufferData;

	lBufferDesc.ByteWidth = pBytesWidth;
	lBufferDesc.Usage = pUsage;
	lBufferDesc.BindFlags = pBindFlags;
	lBufferDesc.CPUAccessFlags = pCPUAccessFlags;
	lBufferDesc.MiscFlags = pMiscFlags;
	lBufferDesc.StructureByteStride = pStructueBytesStride;

	if (pData != nullptr)
	{
		lBufferData.pSysMem = pData;
		lBufferData.SysMemPitch = pDataMemPitch;
		lBufferData.SysMemSlicePitch = pDataMemSlicePitch;

		lResult = pDevice->CreateBuffer(&lBufferDesc, &lBufferData, pBuffer);
		if (FAILED(lResult))
			return false;
	}
	else
	{
		lResult = pDevice->CreateBuffer(&lBufferDesc, nullptr, pBuffer);
		if (FAILED(lResult))
			return false;
	}

	return true;
}