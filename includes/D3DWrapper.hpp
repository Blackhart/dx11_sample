#ifndef __D3D_WRAPPER_HPP__
#define __D3D_WRAPPER_HPP__

#include <d3d11.h>

extern bool	CreateBuffer(ID3D11Device* const pDevice, ID3D11Buffer** pBuffer, UINT const pBytesWidth, D3D11_USAGE const pUsage, UINT const pBindFlags, UINT const pCPUAccessFlags, UINT const pMiscFlags, UINT const pStructueBytesStride, void const* pData, UINT const pDataMemPitch, UINT const pDataMemSlicePitch);

#endif