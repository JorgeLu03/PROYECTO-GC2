#ifndef _dxrr
#define _dxrr
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dx10math.h>
#include "TerrenoRR.h"
#include "Camara.h"
#include "SkyDome.h"
#include "Billboard.h"
#include "ModeloRR.h"
#include "XACT3Util.h"
#include "Colisiones.h"
#include "GUI.h"
#include "Objeto.h"
#include <chrono>


class DXRR{	

private:
	int ancho;
	int alto;
public:	
	HINSTANCE hInstance;
	HWND hWnd;

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* backBufferTarget;

	ID3D11Texture2D* depthTexture;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilState* depthStencilDisabledState;

	ID3D11BlendState *alphaBlendState, *commonBlendState;

	int frameBillboard;
	float rotation = 0.0f;

	TerrenoRR *terreno;
	SkyDome *skydome;
	BillboardRR *billboard;
	Camara *camara;
	ModeloRR* model;
	//BILLBOARDS
	BillboardRR* ARBOL;
	BillboardRR* aguaBillboard;

	//NEW
	ModeloRR* fuente;
	ModeloRR* casa1;
	ModeloRR* casa2;
	ModeloRR* ruins;
	ModeloRR* tent;
    ModeloRR* marijuana;
    ModeloRR* horse;
    ModeloRR* bronzeSword;
    ModeloRR* woodenWatchTower;
    ModeloRR* tree;
    ModeloRR* tree2;
	ModeloRR* tree3;
	//GUI
	GUI* HP100;
	GUI* HP75;
	GUI* HP50;
	GUI* HP25;
	GUI* HP0;
	GUI* Espada;
	GUI* Planta;
	int ultimoHP = 100;
	int ultimoObjetivo = 0;
	bool dibujaEspada = false;
	bool dibujaPlanta = false;
	bool objP1 = true;
	bool objP2 = false;

	//Timer
	chrono::steady_clock::time_point tiempoInicio = std::chrono::steady_clock::now();
	GUI* pantallaInicio;
	bool mostrarPantallaInicio = true;
	bool inicTimer = false;
	//Minimap

	void DrawObjectOnTerrain(ModeloRR* objeto, float scale, float rotationY, char flag, float multiplier)	
	{
		if (objeto == nullptr || terreno == nullptr)
		{
			return;
		}


		float objPosX = objeto->getPosX();
		float objPosZ = objeto->getPosZ(); 

		D3DXVECTOR3 posObject = D3DXVECTOR3(objPosX, terreno->Superficie(objPosX, objPosZ), objPosZ);

		objeto->DrawTPS(camara->vista, camara->proyeccion, camara->posCam, posObject, 2.0f, scale, posObject, rotationY);

	}


	//Objetivos
	GUI* pantallaGameOver;
	GUI* pantallaFIN;
	GUI* OBJ1;
	GUI* OBJ2;
	GUI* OBJ3;
	GUI* OBJ4;
	bool obj1 = false;
	bool obj2 = false;
	bool obj3 = false;
	bool obj4 = false;
	bool objFinal = false;
	bool inicioJuego= false;
	bool escondeEspada = false;
	bool escondePlanta = false;
	//COL
	Objeto* caballo;
	Objeto* colision;
    Objeto* colisionTerreno;
	Objeto* fuenteCol;
	Objeto* casa1Col;
	Objeto* casa2Col;
	Objeto* ruinsCol;
	Objeto* tentCol;
	Objeto* marijuanaCol;
	Objeto* bronzeSwordCol;
	Objeto* woodenWatchTowerCol;
	Objeto* treeCol;
	Objeto* tree2Col;
	Objeto* tree3Col;


	float izqder;
	float arriaba;
	float vel,dirLeft;
	bool breakpoint;
	vector2 uv1[32];
	vector2 uv2[32];
	vector2 uv3[32];
	vector2 uv4[32];

	XACTINDEX cueIndex;
	CXACT3Util m_XACT3;
	CXACT3Util Música;
	
    DXRR(HWND hWnd, int Ancho, int Alto)
	{
		breakpoint = false;
		frameBillboard = 0;
		ancho = Ancho;
		alto = Alto;
		driverType = D3D_DRIVER_TYPE_NULL;
		featureLevel = D3D_FEATURE_LEVEL_11_0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
		IniciaD3D(hWnd);
		izqder = 0;
		arriaba = 0;
		billCargaFuego();
		camara = new Camara(D3DXVECTOR3(0,80,6), D3DXVECTOR3(0,80,0), D3DXVECTOR3(0,1,0), Ancho, Alto);
		terreno = new TerrenoRR(300, 300, d3dDevice, d3dContext);
		skydome = new SkyDome(32, 32, 100.0f, &d3dDevice, &d3dContext, L"Assets/SkyDome.jpg");
		//Billboards
		billboard = new BillboardRR(L"Assets/Billboards/fuego-anim.png",L"Assets/Billboards/fuego-anim-normal.png", d3dDevice, d3dContext, 5);
		ARBOL = new BillboardRR(L"Assets/Billboards/ARBOL.png", L"Assets/Billboards/ARBOL.png",d3dDevice,d3dContext, 20.0f);
		aguaBillboard = new BillboardRR(L"Assets/Billboards/AGUA.png",L"Assets/Billboards/AGUA_N.png",d3dDevice, d3dContext, 30.0f);

				//
		fuente = new ModeloRR(d3dDevice, d3dContext, "Assets/MODELOS/FUENTE2.obj", L"Assets/MODELOS/FUENTE.png", L"Assets/MODELOS/FUENTE_SPEC2.png", -3, -30);
		//der
		casa1 = new ModeloRR(d3dDevice, d3dContext, "Assets/MODELOS/CASA4.obj", L"Assets/MODELOS/CASA1.png", L"Assets/MODELOS/CASA1_SPEC.png", -115, 15);
		//izq
		casa2 = new ModeloRR(d3dDevice, d3dContext, "Assets/MODELOS/CASA3.obj", L"Assets/MODELOS/CASA2.png", L"Assets/MODELOS/CASA2_SPEC.png", 100, -130);
		ruins = new ModeloRR(d3dDevice, d3dContext, "Assets/MODELOS/RUINS.obj", L"Assets/MODELOS/RUINS.png", L"Assets/MODELOS/RUINS_SPEC.png", -90, 110);
		tent = new ModeloRR(d3dDevice, d3dContext, "Assets/MODELOS/TENT2.obj", L"Assets/MODELOS/TENT2.png", L"Assets/MODELOS/RUINS_SPEC.png", 40, -130);

        marijuana = new ModeloRR(d3dDevice, d3dContext, "Assets/MODELOS/MARIHUANA.obj", L"Assets/MODELOS/branchdiffuse.jpg", L"Assets/MODELOS/bump leaf.jpg", -70, -90);
        horse = new ModeloRR(d3dDevice, d3dContext, "Assets/MODELOS/CABALLO.obj", L"Assets/MODELOS/textures/HORSE_COLOR.png", L"Assets/MODELOS/textures/HORSE_SPEC.png", 0, 0);

        bronzeSword = new ModeloRR(d3dDevice, d3dContext, "Assets/MODELOS/Bronze_sword.obj", L"Assets/MODELOS/Bronze_sword_Specular.bmp", L"Assets/MODELOS/Bronze_sword_SPEC.jpg", 38.5, -127);
        woodenWatchTower = new ModeloRR(d3dDevice, d3dContext, "Assets/MODELOS/wooden watch tower2.obj", L"Assets/MODELOS/textures/Wood_Tower_Col.jpg", L"Assets/MODELOS/textures/Wood_Tower_Col_SPEC.jpg", -90, -90);
        tree = new ModeloRR(d3dDevice, d3dContext, "Assets/MODELOS/UGLYTREE1.obj", L"Assets/MODELOS/TREE_DIFFUSE.png", L"Assets/MODELOS/TREE_SPECULAR.png", -15, -100);
		tree2 = new ModeloRR(d3dDevice, d3dContext, "Assets/MODELOS/UGLYTREE2.obj", L"Assets/MODELOS/TREE_DIFFUSE.png", L"Assets/MODELOS/TREE_SPECULAR.png", -65, 27);
		tree3 = new ModeloRR(d3dDevice, d3dContext, "Assets/MODELOS/UGLYTREE3.obj", L"Assets/MODELOS/TREE_DIFFUSE.png", L"Assets/MODELOS/TREE_SPECULAR.png", 5, 80);

		caballo = new Objeto(D3DXVECTOR3(-115, terreno->Superficie(-115, -5), -5), D3DXVECTOR3(-115, terreno->Superficie(115, -5), -5), 8);
		HP100 = new GUI(d3dDevice, d3dContext, 0.25f, 0.5f, L"Assets/GUI/100.png");
		HP75 = new GUI(d3dDevice, d3dContext, 0.25f, 0.5f, L"Assets/GUI/75.png");
		HP50 = new GUI(d3dDevice, d3dContext, 0.25f, 0.5f, L"Assets/GUI/50.png");
		HP25 = new GUI(d3dDevice, d3dContext, 0.25f, 0.5f, L"Assets/GUI/25.png");
		HP0 = new GUI(d3dDevice, d3dContext, 0.25f, 0.5f, L"Assets/GUI/0.png");
		Espada = new GUI(d3dDevice, d3dContext, 0.18f, 0.12f, L"Assets/GUI/ESPADA.png");
		Planta = new GUI(d3dDevice, d3dContext, 0.2f, 0.18f, L"Assets/GUI/CANN.png");

		OBJ1 = new GUI(d3dDevice, d3dContext, 0.63f, 0.63f, L"Assets/GUI/OBJ1.png");
		OBJ2 = new GUI(d3dDevice, d3dContext, 0.63f, 0.63f, L"Assets/GUI/OBJ2.png");
		OBJ3 = new GUI(d3dDevice, d3dContext, 0.63f, 0.63f, L"Assets/GUI/OBJ33.png");
		OBJ4 = new GUI(d3dDevice, d3dContext, 0.63f, 0.63f, L"Assets/GUI/OBJ4.png");

		pantallaGameOver = new GUI(d3dDevice, d3dContext, 1.8f, 1.8f, L"Assets/GUI/GAMEOVER.jpg");
		pantallaFIN = new GUI(d3dDevice, d3dContext, 1.8f, 1.8f, L"Assets/GUI/FIN.jpg");
		pantallaInicio = new GUI(d3dDevice, d3dContext, 1.8f, 1.8f, L"Assets/GUI/PANTALLA.jpg"); // Ajusta la ruta de la imagen
		//COLISIONES
		colisionTerreno = new Objeto(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 290, 1000, 290);
        // Cambia la creación de los objetos de colisión para que usen el constructor de caja (con 3 dimensiones)
        // Sustituye el bloque seleccionado por este:

        fuenteCol = new Objeto(
            D3DXVECTOR3(fuente->getPosX(), terreno->Superficie(fuente->getPosX(), fuente->getPosZ()), fuente->getPosZ()),
            D3DXVECTOR3(fuente->getPosX(), terreno->Superficie(fuente->getPosX(), fuente->getPosZ()), fuente->getPosZ()),
            15, 8, 15);

        casa1Col = new Objeto(
            D3DXVECTOR3(casa1->getPosX(), terreno->Superficie(casa1->getPosX(), casa1->getPosZ()), casa1->getPosZ()),
            D3DXVECTOR3(casa1->getPosX(), terreno->Superficie(casa1->getPosX(), casa1->getPosZ()), casa1->getPosZ()),
            12, 12, 12);

        casa2Col = new Objeto(
            D3DXVECTOR3(casa2->getPosX(), terreno->Superficie(casa2->getPosX(), casa2->getPosZ()), casa2->getPosZ()),
            D3DXVECTOR3(casa2->getPosX(), terreno->Superficie(casa2->getPosX(), casa2->getPosZ()), casa2->getPosZ()),
            12, 12, 12);

        ruinsCol = new Objeto(
            D3DXVECTOR3(ruins->getPosX(), terreno->Superficie(ruins->getPosX(), ruins->getPosZ()), ruins->getPosZ()),
            D3DXVECTOR3(ruins->getPosX(), terreno->Superficie(ruins->getPosX(), ruins->getPosZ()), ruins->getPosZ()),
            68, 10, 20);

        tentCol = new Objeto(
            D3DXVECTOR3(tent->getPosX(), terreno->Superficie(tent->getPosX(), tent->getPosZ()), tent->getPosZ()),
            D3DXVECTOR3(tent->getPosX(), terreno->Superficie(tent->getPosX(), tent->getPosZ()), tent->getPosZ()),
            8, 8, 8);

        marijuanaCol = new Objeto(
            D3DXVECTOR3(marijuana->getPosX(), terreno->Superficie(marijuana->getPosX(), marijuana->getPosZ()), marijuana->getPosZ()),
            D3DXVECTOR3(marijuana->getPosX(), terreno->Superficie(marijuana->getPosX(), marijuana->getPosZ()), marijuana->getPosZ()),
            6, 10, 6);

        bronzeSwordCol = new Objeto(
            D3DXVECTOR3(bronzeSword->getPosX(), terreno->Superficie(bronzeSword->getPosX(), bronzeSword->getPosZ()), bronzeSword->getPosZ()),
            D3DXVECTOR3(bronzeSword->getPosX(), terreno->Superficie(bronzeSword->getPosX(), bronzeSword->getPosZ()), bronzeSword->getPosZ()),
            4, 12, 4);

        woodenWatchTowerCol = new Objeto(
            D3DXVECTOR3(woodenWatchTower->getPosX(), terreno->Superficie(woodenWatchTower->getPosX(), woodenWatchTower->getPosZ()), woodenWatchTower->getPosZ()),
            D3DXVECTOR3(woodenWatchTower->getPosX(), terreno->Superficie(woodenWatchTower->getPosX(), woodenWatchTower->getPosZ()), woodenWatchTower->getPosZ()),
            17, 20, 17);

        treeCol = new Objeto(
            D3DXVECTOR3(tree->getPosX(), terreno->Superficie(tree->getPosX(), tree->getPosZ()), tree->getPosZ()),
            D3DXVECTOR3(tree->getPosX(), terreno->Superficie(tree->getPosX(), tree->getPosZ()), tree->getPosZ()),
            8, 16, 8);

        tree2Col = new Objeto(
            D3DXVECTOR3(tree2->getPosX(), terreno->Superficie(tree2->getPosX(), tree2->getPosZ()), tree2->getPosZ()),
            D3DXVECTOR3(tree2->getPosX(), terreno->Superficie(tree2->getPosX(), tree2->getPosZ()), tree2->getPosZ()),
            8, 16, 8);

        tree3Col = new Objeto(
            D3DXVECTOR3(tree3->getPosX(), terreno->Superficie(tree3->getPosX(), tree3->getPosZ()), tree3->getPosZ()),
            D3DXVECTOR3(tree3->getPosX(), terreno->Superficie(tree3->getPosX(), tree3->getPosZ()), tree3->getPosZ()),
            8, 16, 8);


		Música.Initialize();
		Música.LoadWaveBank(L"Assets/Sonido/WAVES.xwb");
		Música.LoadSoundBank(L"Assets/Sonido/SOUNDS.xsb");

		// Replace the problematic line with the following code to fix the errors:
		if (Música.m_pSoundBank) {
			// Retrieve the cue index for the sound "musica" from the sound bank
			XACTINDEX cueIndex = Música.m_pSoundBank->GetCueIndex("music");
			if (cueIndex != XACTINDEX_INVALID) {
				// Play the sound using the cue index
				Música.m_pSoundBank->Play(cueIndex, 0, 0, NULL);
			}
		}

	}

	~DXRR()
	{

		LiberaD3D();
		m_XACT3.Terminate();
	}
	
	bool IniciaD3D(HWND hWnd)
	{
		this->hInstance = hInstance;
		this->hWnd = hWnd;

		//obtiene el ancho y alto de la ventana donde se dibuja
		RECT dimensions;
		GetClientRect(hWnd, &dimensions);
		unsigned int width = dimensions.right - dimensions.left;
		unsigned int heigth = dimensions.bottom - dimensions.top;

		//Las formas en como la pc puede ejecutar el DX11, la mas rapida es D3D_DRIVER_TYPE_HARDWARE pero solo se puede usar cuando lo soporte el hardware
		//otra opcion es D3D_DRIVER_TYPE_WARP que emula el DX11 en los equipos que no lo soportan
		//la opcion menos recomendada es D3D_DRIVER_TYPE_SOFTWARE, es la mas lenta y solo es util cuando se libera una version de DX que no sea soportada por hardware
		D3D_DRIVER_TYPE driverTypes[]=
		{
			D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE
		};
		unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

		//La version de DX que utilizara, en este caso el DX11
		D3D_FEATURE_LEVEL featureLevels[]=
		{
			D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
		};
		unsigned int totalFeaturesLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = heigth;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		HRESULT result;
		unsigned int driver = 0, creationFlags = 0;
		for(driver = 0; driver<totalDriverTypes; driver++)
		{
			result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0,
				creationFlags, featureLevels, totalFeaturesLevels, 
				D3D11_SDK_VERSION, &swapChainDesc, &swapChain,
				&d3dDevice, &featureLevel, &d3dContext);

			if(SUCCEEDED(result))
			{
				driverType = driverTypes[driver];
				break;
			}
		}

		if(FAILED(result))
		{

			//Error al crear el Direct3D device
			return false;
		}
		
		ID3D11Texture2D* backBufferTexture;
		result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
		if(FAILED(result))
		{
			//"Error al crear el swapChainBuffer
			return false;
		}

		result = d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget);
		if(backBufferTexture)
			backBufferTexture->Release();

		if(FAILED(result))
		{
			//Error al crear el renderTargetView
			return false;
		}


		D3D11_VIEWPORT viewport;
		viewport.Width = (FLOAT)width;
		viewport.Height = (FLOAT)heigth;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		d3dContext->RSSetViewports(1, &viewport);

		D3D11_TEXTURE2D_DESC depthTexDesc;
		ZeroMemory(&depthTexDesc, sizeof(depthTexDesc));
		depthTexDesc.Width = width;
		depthTexDesc.Height = heigth;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.SampleDesc.Count = 1;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;
		
		result = d3dDevice->CreateTexture2D(&depthTexDesc, NULL, &depthTexture);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear la DepthTexture", MB_OK);
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = depthTexDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		result = d3dDevice->CreateDepthStencilView(depthTexture, &descDSV, &depthStencilView);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el depth stencil target view", MB_OK);
			return false;
		}

		d3dContext->OMSetRenderTargets(1, &backBufferTarget, depthStencilView);

		return true;			
		
	}

	void LiberaD3D(void)
	{
		if(depthTexture)
			depthTexture->Release();
		if(depthStencilView)
			depthStencilView->Release();
		if(backBufferTarget)
			backBufferTarget->Release();
		if(swapChain)
			swapChain->Release();
		if(d3dContext)
			d3dContext->Release();
		if(d3dDevice)
			d3dDevice->Release();

		depthTexture = 0;
		depthStencilView = 0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
	}
	
	void Render(void)
	{
		//Música.DoWork();
		float sphere[3] = { 0,0,0 };
		float prevPos[3] = { camara->posCam.x, camara->posCam.z, camara->posCam.z };
		static float angle = 0.0f;
		angle += 0.005;
		if (angle >= 360) angle = 0.0f;
		bool collide = false;
		if( d3dContext == 0 )
			return;

		float clearColor[4] = { 0, 0, 0, 1.0f };
		d3dContext->ClearRenderTargetView( backBufferTarget, clearColor );
		d3dContext->ClearDepthStencilView( depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
		camara->posCam.y = terreno->Superficie(camara->posCam.x, camara->posCam.z) + 5 ;
		camara->UpdateCam(vel,dirLeft, arriaba, izqder);
		skydome->Update(camara->vista, camara->proyeccion);

		float camPosXZ[2] = { camara->posCam.x, camara->posCam.z };

		TurnOffDepth();
		skydome->Render(camara->posCam);
		TurnOnDepth();
		terreno->Draw(camara->vista, camara->proyeccion);
		//TurnOnAlphaBlending();
		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-11, -78, 10, 5, uv1, uv2, uv3, uv4, frameBillboard);


        // Por el siguiente bloque para dibujar 15 billboards (árboles) en posiciones distintas a los modelos:
        float billboardPositions[15][2] = {
            { 0, 20 },   { -60, 80 },  { 80, -20 }, { -130, 100 }, { 70, -110 },
            { -40, -120 },{ -35, 10 },   { -100, -40 },{ 35, 0 },   { -100, 40 },
            { 130, -120 },  { -120, -100 },{0, -120 },{130, -78 },   { -30, 120 }
        };
        for (int i = 0; i < 15; ++i) {
            float x = billboardPositions[i][0];
            float z = billboardPositions[i][1];
            float y = terreno->Superficie(x, z);
            ARBOL->Draw(camara->vista, camara->proyeccion, camara->posCam, x, z, y, 100);
        }

		TurnOffAlphaBlending();
		//model->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1);

        /*if (mostrarPantallaInicio) {
            pantallaInicio->Draw(-0.2f, 0.1f);
        }*/

		// En cada frame:
		auto ahora = std::chrono::steady_clock::now();
		float tiempoTotal = std::chrono::duration<float>(ahora - tiempoInicio).count();

		// Timer
		int hpActual = 100;
		if (tiempoTotal < 30.0f) {
			hpActual = 100;
			HP100->Draw(-0.7f, -0.45f);
		}
		else if (tiempoTotal < 60.0f) {
			hpActual = 75;
			HP75->Draw(-0.7f, -0.45f);
		}
		else if (tiempoTotal < 90.0f) {
			hpActual = 50;
			HP50->Draw(-0.7f, -0.45f);
		}
		else if (tiempoTotal < 120.0f) {
			hpActual = 25;
			HP25->Draw(-0.7f, -0.45f);
		}
		else {
			if (tiempoTotal >= 120.0f && tiempoTotal <=125) {
				hpActual = 0;
				HP0->Draw(-0.7f, -0.45f);
			}
			else if (tiempoTotal > 125.0f) {
				pantallaGameOver->Draw(-0.2f, 0.1f);
			}
		}
		if (hpActual != ultimoHP) {
			if (Música.m_pSoundBank) {
				XACTINDEX cueIndex = Música.m_pSoundBank->GetCueIndex("hit");
				if (cueIndex != XACTINDEX_INVALID) {
					Música.m_pSoundBank->Play(cueIndex, 0, 0, NULL);
				}
			}
			ultimoHP = hpActual;
		}

		//OBJETIVOS
		int objetivoActual = 0;
		if (!obj1) {
			OBJ1->Draw(0.2f, 0.78f);
		}
		else if (!obj2) {
			objetivoActual = 2;
			OBJ2->Draw(0.2, 0.78f);
		}
		else if (!obj3) {
			objetivoActual = 3;
			OBJ3->Draw(0.2f, 0.78f);
		}
		else if (!obj4) {
			objetivoActual = 4;
			OBJ4->Draw(0.2f, 0.78f);
		}
		else {
			objetivoActual = 5;
			objFinal = true;
		}
		if (objetivoActual != 0 && objetivoActual != ultimoObjetivo) {
			if (Música.m_pSoundBank) {
				XACTINDEX cueIndex = Música.m_pSoundBank->GetCueIndex("paper");
				if (cueIndex != XACTINDEX_INVALID) {
					Música.m_pSoundBank->Play(cueIndex, 0, 0, NULL);
				}
			}
			ultimoObjetivo = objetivoActual;
		}
		//FINAL
		if (objFinal) {
			pantallaFIN->Draw(-0.2f, 0.1f);
		}


				
		if (colisionTerreno->collider->isInside(camara->posCam) == false) {
			camara->posCam = camara->camaraPosAnterior;
		}

		horse->DrawTPS(camara->vista, camara->proyeccion, camara->posCam, caballo->posicion, 2.0f, 8, caballo->apunta, rotation);
		if (escondeEspada == false) {
			bronzeSword->DrawTPS(camara->vista, camara->proyeccion, camara->posCam, bronzeSwordCol->posicion, 2.0f, 5.0f, bronzeSwordCol->posicion, rotation);
		}
		/*fuente->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'a', 5);
		casa1->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'a', 5);
		casa2->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'a', 5);
		ruins->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 300), camara->posCam, 10.0f, 0, 'a', 5);
		tent->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'a', 1);
        marijuana->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 10);
        horse->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 10);
		bronzeSword->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'a', 10);
        woodenWatchTower->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'a', 10);
        tree->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'a', 7);*/


		DrawObjectOnTerrain(fuente, 5.0f, 0, 'a', 10);
		DrawObjectOnTerrain(casa1, 5.0f, -45, 'a', 10);
		DrawObjectOnTerrain(casa2, 5.0f, 0, 'a', 10);
		DrawObjectOnTerrain(ruins, 5.0f, 0, 'a', 10);
		DrawObjectOnTerrain(tent, 5.0f, 90, 'a',10);
		if (escondePlanta == false) {
			DrawObjectOnTerrain(marijuana, 6.0f, 0, 'A', 10);
		}
		//DrawObjectOnTerrain(horse, 10.0f, 0, 'A', 10);

		DrawObjectOnTerrain(woodenWatchTower, 5.0f, 0, 'a', 10);
		DrawObjectOnTerrain(tree, 5.0f, 0, 'a', 10);
		DrawObjectOnTerrain(tree2, 5.0f, 0, 'a', 10);
		DrawObjectOnTerrain(tree3, 5.0f, 0, 'a', 10);

		// Detectar colisión con la fuente antes de mover la cámara
		bool colFuente = fuenteCol->collider->isInside(camara->posCam);

		// Colisión física SIEMPRE
		if (colFuente) {
			camara->posCam = camara->camaraPosAnterior;
		}

		// Lógica de objetivo 3: solo si obj1 y obj2 ya son true
		if (obj1 && obj2 && colFuente) {
			obj3 = true;
		}


		if (casa1Col->collider->isInside(camara->posCam)) {
			camara->posCam = camara->camaraPosAnterior;
		}
		if (casa2Col->collider->isInside(camara->posCam)) {
			camara->posCam = camara->camaraPosAnterior;
		}
		
		/*if (tentCol->collider->isInside(camara->posCam)) {
			camara->posCam = camara->camaraPosAnterior;
		}*/
		// --- Colisiones y lógica de objetivos para marijuana y horse ---

// Detectar colisión antes de mover la cámara
		bool colMarijuana = marijuanaCol->collider->isInside(camara->posCam);
		bool colHorse = caballo->collider->isInside(camara->posCam);

		// Colisión física SIEMPRE
		if (colMarijuana) {
			camara->posCam = camara->camaraPosAnterior;
		}
		if (colHorse) {
			camara->posCam = camara->camaraPosAnterior;
		}

		// Lógica de objetivos SOLO si obj1 está cumplido
		if (obj1) {
			// Recoger la planta
			if (!escondePlanta && colMarijuana) {
				escondePlanta = true;
				dibujaPlanta = true;
			}

			if (dibujaPlanta) {
				Planta->Draw(-0.25f, -0.45f);
			}

			// Cumplir objetivo 2 al colisionar con el caballo
			if (dibujaPlanta && colHorse) {
				obj2 = true;
			}
		}

		if (escondeEspada == false) {
			if (bronzeSwordCol->collider->isInside(camara->posCam)) {
				escondeEspada = true;
				dibujaEspada = true;
			}
		}
		if (dibujaEspada == true) {
			objP1 = true;
			Espada->Draw(-0.38f, -0.45f);
		}
		bool colRuins = ruinsCol->collider->isInside(camara->posCam);
		if (colRuins) {
			camara->posCam = camara->camaraPosAnterior;
		}
		if (objP1 == true && colRuins) {
			obj1 = true;
		}


		if (woodenWatchTowerCol->collider->isInside(camara->posCam)) {
			camara->posCam = camara->camaraPosAnterior;
		}
		if (treeCol->collider->isInside(camara->posCam)) {
			camara->posCam = camara->camaraPosAnterior;
		}
		if (tree2Col->collider->isInside(camara->posCam)) {
			camara->posCam = camara->camaraPosAnterior;
		}
		if (tree3Col->collider->isInside(camara->posCam)) {
			camara->posCam = camara->camaraPosAnterior;
		}
		

		rotation = rotation + 0.1f;
		swapChain->Present( 1, 0 );

		//if (caballo->collider->isInside(camara->posCam) == true) {
		//	RestaurarPosicionCamara();
		//}

	}


	bool isPointInsideSphere(float* point, float* sphere) {
		bool collition = false;

		float distance = sqrt((point[0] - sphere[0]) * (point[0] - sphere[0]) +
			(point[1] - sphere[1]) * (point[1] - sphere[1]));

		if (distance < sphere[2])
			collition = true;
		return collition;
	}

	//Activa el alpha blend para dibujar con transparencias
	void TurnOnAlphaBlending()
	{
		float blendFactor[4];
		blendFactor[0] = 0.0f;
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[3] = 0.0f;
		HRESULT result;

		D3D11_BLEND_DESC descABSD;
		ZeroMemory(&descABSD, sizeof(D3D11_BLEND_DESC));
		descABSD.RenderTarget[0].BlendEnable = TRUE;
		descABSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descABSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descABSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		result = d3dDevice->CreateBlendState(&descABSD, &alphaBlendState);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(alphaBlendState, blendFactor, 0xffffffff);
	}

	//Regresa al blend normal(solido)
	void TurnOffAlphaBlending()
	{
		D3D11_BLEND_DESC descCBSD;
		ZeroMemory(&descCBSD, sizeof(D3D11_BLEND_DESC));
		descCBSD.RenderTarget[0].BlendEnable = FALSE;
		descCBSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descCBSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descCBSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;
		HRESULT result;

		result = d3dDevice->CreateBlendState(&descCBSD, &commonBlendState);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(commonBlendState, NULL, 0xffffffff);
	}

	void TurnOnDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDSD;
		ZeroMemory(&descDSD, sizeof(descDSD));
		descDSD.DepthEnable = true;
		descDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDSD.StencilEnable=true;
		descDSD.StencilReadMask = 0xFF;
		descDSD.StencilWriteMask = 0xFF;
		descDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDSD, &depthStencilState);
		
		d3dContext->OMSetDepthStencilState(depthStencilState, 1);
	}

	bool Timer(int sec) {
		Sleep(sec * 1000);
		return true;
	}

	void TurnOffDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDDSD;
		ZeroMemory(&descDDSD, sizeof(descDDSD));
		descDDSD.DepthEnable = false;
		descDDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDDSD.StencilEnable=true;
		descDDSD.StencilReadMask = 0xFF;
		descDDSD.StencilWriteMask = 0xFF;
		descDDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDDSD, &depthStencilDisabledState);
		d3dContext->OMSetDepthStencilState(depthStencilDisabledState, 1);
	}

	void billCargaFuego()
	{
		uv1[0].u = .125;
		uv2[0].u = .125;
		uv3[0].u = 0;
		uv4[0].u = 0;

		uv1[0].v = .25;
		uv2[0].v = 0;
		uv3[0].v = 0;
		uv4[0].v = .25;


		for (int j = 0; j < 8; j++) {
			uv1[j].u = uv1[0].u + (j * .125);
			uv2[j].u = uv2[0].u + (j * .125);
			uv3[j].u = uv3[0].u + (j * .125);
			uv4[j].u = uv4[0].u + (j * .125);

			uv1[j].v = .25;
			uv2[j].v = 0;
			uv3[j].v = 0;
			uv4[j].v = .25;
		}
		for (int j = 0; j < 8; j++) {
			uv1[j + 8].u = uv1[0].u + (j * .125);
			uv2[j + 8].u = uv2[0].u + (j * .125);
			uv3[j + 8].u = uv3[0].u + (j * .125);
			uv4[j + 8].u = uv4[0].u + (j * .125);

			uv1[j + 8].v = .5;
			uv2[j + 8].v = .25;
			uv3[j + 8].v = .25;
			uv4[j + 8].v = .5;
		}

		for (int j = 0; j < 8; j++) {
			uv1[j + 16].u = uv1[0].u + (j * .125);
			uv2[j + 16].u = uv2[0].u + (j * .125);
			uv3[j + 16].u = uv3[0].u + (j * .125);
			uv4[j + 16].u = uv4[0].u + (j * .125);

			uv1[j + 16].v = .75;
			uv2[j + 16].v = .5;
			uv3[j + 16].v = .5;
			uv4[j + 16].v = .75;
		}

		for (int j = 0; j < 8; j++) {
			uv1[j + 24].u = uv1[0].u + (j * .125);
			uv2[j + 24].u = uv2[0].u + (j * .125);
			uv3[j + 24].u = uv3[0].u + (j * .125);
			uv4[j + 24].u = uv4[0].u + (j * .125);

			uv1[j + 24].v = 1;
			uv2[j + 24].v = .75;
			uv3[j + 24].v = .75;
			uv4[j + 24].v = 1;
		}
	}

	// Fuera de la declaración de la clase en tu .cpp o después del .h
	

};
#endif