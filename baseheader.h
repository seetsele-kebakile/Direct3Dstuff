#pragma once
#define GLFW_EXPOSE_NATIVE_WIN32
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

#include<DirectXMath.h>
#include<d3d11.h>
#include<d3d.h>
#include<d3dcommon.h>
#include<d3dcompiler.h>
#include"glfw3.h"
#include"glfw3native.h"

#include"vshaderheader.h"
#include"pshaderheader.h"


using namespace DirectX;
ID3D11Device* dev;
ID3D11DeviceContext* dconxt;
ID3D11RenderTargetView* rtv;
IDXGISwapChain* sc;

ID3D11VertexShader* vshader;
ID3D11PixelShader* pshader;
ID3D11RasterizerState* rs;
ID3D11InputLayout* il;
ID3D11Buffer* vbuff;
ID3D11Buffer* ibuff;
ID3D11Buffer* cbuff;
ID3D11DepthStencilView* dsv;
ID3D11Texture2D* dsp;
struct Cbcontents
{
	XMMATRIX wvp;
};
Cbcontents cbcontents;
XMMATRIX world;
XMMATRIX view;
XMMATRIX projection;