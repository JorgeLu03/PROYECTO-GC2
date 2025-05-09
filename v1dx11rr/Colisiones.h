#ifndef _COLISIONES
#define _COLISIONES
#pragma once
#include <d3d11.h>
#include <D3DX11.h>
#include <D3DX10math.h>
//#define NULL 0
//#include "VectorRR.h"
//#include "Camera.h"
//#include <stdlib.h>
//#include <gl\glut.h>
//#include "Flecha.h"
//#include "Camera.h"
//#include "Objeto.h"

class Colisiones
{
private:
	struct Sphere {
		float x = 0, y = 0, z = 0;
		float radio = 0;
		int sl = 0, st = 0;
		//unsigned int esferaTextura;
	}*e;

	struct Cube {
		float xMax, xMin;
		float yMax, yMin;
		float zMax, zMin;
	}*c;

	//void primitivo();
	//void caja();

public:
	//Colisiones(float _x, float _y, float _z, float _radio, int _sl, int _st);
	Colisiones(D3DXVECTOR3 _posicion, float _radio);
	//Colisiones(float _x, float _y, float _z, float _lado);
	Colisiones(D3DXVECTOR3 _posicion, float _x, float _y, float _z);
	Colisiones(float _xMin, float _xMax, float _yMin, float _yMax, float _zMin, float _zMax);
	~Colisiones();
	//void dibuja();
	bool isInside(D3DXVECTOR3 _posicion);
	bool isInsideXZ(D3DXVECTOR3 _posicion);
	bool colSpheres(Sphere* _local, Sphere* _extern);
	//bool isOutside(D3DXVECTOR3 _posicion);
	//bool isOutsideXZ(D3DXVECTOR3 _posicion);
	/*float GetDistancia(Objeto *&p);*/
	void Update(D3DXVECTOR3 _posicion);
	//int Puntos(Proyectil *p);
	//Para el target
	//RADIO
	//0.7f - 100
	//1.3f - 50
	//1.9f - 20
	//2.6f - 10
};
#endif // !_COLISIONES