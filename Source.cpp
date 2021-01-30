#include"baseheader.h"
#include<String>

int width = 800;
int height = 600;
struct vertex
{
	float x, y,z;
	float r, g, b;
};
void initbuffers(void* vbdata,void* ibdata, int numvdata, int numidata)
{
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));

    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.ByteWidth = sizeof(DWORD) * numidata;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;

	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(D3D11_BUFFER_DESC));

	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(vertex) * numvdata;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA ibsd, vbsd;
	ibsd.pSysMem = ibdata;
	vbsd.pSysMem = vbdata;
	unsigned int stride, offset;
	stride = sizeof(vertex);
	offset = 0;
	
		D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(D3D11_BUFFER_DESC));
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.MiscFlags = 0;
	cbd.CPUAccessFlags = 0;
	cbd.ByteWidth = sizeof(cbcontents);
	
	
	dev->CreateBuffer(&cbd,NULL, &cbuff);
	dev->CreateBuffer(&vbd, &vbsd, &vbuff);
	dev->CreateBuffer(&ibd, &ibsd, &ibuff);
	dconxt->IASetVertexBuffers(0, 1, &vbuff, &stride, &offset);
	dconxt->IASetIndexBuffer(ibuff, DXGI_FORMAT_R32_UINT, 0);
	dconxt->VSSetConstantBuffers(0, 1, &cbuff);
	



}
void initd3d11(GLFWwindow* win)
{
	HWND hwnd;
	
	glfwGetWindowSize(win, &width, &height);
	hwnd = glfwGetWin32Window(win);


	DXGI_MODE_DESC md;
	md.Width = width;
	md.Height = height;
	md.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	md.RefreshRate.Numerator = 60;
	md.RefreshRate.Denominator = 1;
	md.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	md.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;
	scd.OutputWindow = hwnd;
	scd.Windowed = true;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.SampleDesc.Count = 2;
	scd.SampleDesc.Quality = 0;
	scd.BufferDesc = md;
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &scd, &sc, &dev, NULL, &dconxt);
	ID3D11Texture2D* BackBuffer;
	sc->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	
	dev->CreateRenderTargetView(BackBuffer, NULL, &rtv);
	BackBuffer->Release();//note: cleanup in freed3d11 func in case of later use
	 
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;

	
	dconxt->RSSetViewports(1, &viewport);
	
	dconxt->OMSetRenderTargets(1, &rtv, NULL);
	D3D11_RASTERIZER_DESC rsd;
	ZeroMemory(&rsd, sizeof(D3D11_RASTERIZER_DESC));
	rsd.CullMode = D3D11_CULL_NONE;
	rsd.FillMode = D3D11_FILL_WIREFRAME;
	rsd.MultisampleEnable = true;

	dev->CreateRasterizerState(&rsd, &rs);
	dconxt->RSSetState(rs);
	D3D11_TEXTURE2D_DESC depthStencilDesc;


	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	
	dev->CreateTexture2D(&depthStencilDesc, NULL, &dsp);
	dev->CreateDepthStencilView(dsp, NULL, &dsv);
	dsp->Release();//note: cleanup in freed3d11 func in case of later use
	dconxt->OMSetRenderTargets(1, &rtv, dsv);


	dev->CreateVertexShader(vshaderobj, sizeof(vshaderobj), nullptr, &vshader);
	dev->CreatePixelShader(pshaderobj, sizeof(pshaderobj), nullptr, &pshader);

	dconxt->VSSetShader(vshader, nullptr, 0);
	dconxt->PSSetShader(pshader, nullptr, 0);

	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR"  ,0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 } };
	unsigned int arraysize = ARRAYSIZE(ied);
	dev->CreateInputLayout(ied, arraysize, vshaderobj, sizeof(vshaderobj), &il);
	dconxt->IASetInputLayout(il);
	dconxt->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    }
void cleard3d11(float r, float g, float b, float a)
{
	float col[] = { r,g,b,a };
	dconxt->ClearRenderTargetView(rtv, col);
	dconxt->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

}
void swapd3d11(UINT j, UINT k)
{
	sc->Present(j, k);
}
void freed3d11()
{ 
	rs->Release();
	dsv->Release();
	cbuff->Release();
	il->Release();
	vbuff->Release();
	ibuff->Release();
	pshader->Release();
	vshader->Release();
	sc->Release();
	rtv->Release();
	dev->Release();
	dconxt->Release();

}
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mod)
{
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(win, true);
	}
}
int WINAPI WinMain(HINSTANCE h, HINSTANCE hprev, LPSTR cmdln, int cmd)
{
	
	glfwInit();
	
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* win = glfwCreateWindow(width,600 , "Direct X App", NULL, NULL);
	glfwSetWindowPos(win, 300, 30);
	glfwSetKeyCallback(win, key_callback);

	initd3d11(win);   
	
	
	
	
	                       
	vertex verticies[] = {
		{-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f },
		{-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f },
	    {1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f },
	    {1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f},
	    {-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
	    {-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
	    {1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
	    {1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f }
	}; 

	DWORD triindicies[] = { 
		//forward face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	
	};
	
	XMVECTOR eyepos, focuspos, updirection;
	eyepos = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
	focuspos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	updirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	
	
	
	world = XMMatrixIdentity();
	view = XMMatrixLookAtLH(eyepos, focuspos, updirection);
	projection = XMMatrixPerspectiveFovLH(0.4f*3.14f, float(width/height), 1.0F, 1000.0F);
   
	cbcontents.wvp = XMMatrixTranspose( world * view * projection);
	float rotvalue = 0.0f;
	initbuffers(verticies, triindicies,8 , 36);
	while (!glfwWindowShouldClose(win))
	{
		glfwPollEvents();
		cleard3d11(0.0f, 0.0f, 0.0f, 1.0f);
		
		rotvalue += 0.001f;

		world = XMMatrixRotationNormal(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),rotvalue);
		cbcontents.wvp = XMMatrixTranspose(world * view * projection);
		
		
	   dconxt->UpdateSubresource(cbuff, 0, 0, &cbcontents, 0, 0);
		dconxt->DrawIndexed(36, 0, 0);

		swapd3d11(0, 0);
	}
	glfwDestroyWindow(win);
	
	freed3d11();
	glfwTerminate();
	return 0;
}