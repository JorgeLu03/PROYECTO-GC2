#ifndef _objeto
#define _objeto

//Clase camara hecha por Rafael Rosas para los UltraLMADs
//Videojuegos
#pragma once
#include <d3d11.h>
#include <D3DX11.h>
#include <D3DX10math.h>
#include "Colisiones.h"

#include <string>

class Objeto {
public:
	D3DXVECTOR3 posicion;
	D3DXVECTOR3 apunta;
	//D3DXVECTOR3 direccion;
	D3DXVECTOR3 refUp;
	D3DXVECTOR3 refRight;
	D3DXVECTOR3 refFront;

	D3DXVECTOR3 vec_luz;

	Colisiones* collider = NULL;
	Colisiones* detection = NULL;

	int frame, frameLimit;
	bool explota;

	bool abduciendo;
	float velGravedad;
	bool inAir;

	//int time_disp;
	int vida;
	bool band_exp;

	string _state;
	D3DXVECTOR3 posBuffer;

	Objeto()
	{
		//posicion de la camara
		posicion = D3DXVECTOR3(0, 0,0);
		//a donde ve
		apunta = D3DXVECTOR3(0, 5, 0);
		refUp = D3DXVECTOR3(1, 0, 0);

		D3DXVec3Normalize(&refUp, &refUp);

		refFront = D3DXVECTOR3(apunta.x - posicion.x, apunta.y - posicion.y, apunta.z - posicion.z);
		D3DXVec3Normalize(&refFront, &refFront);

		D3DXVec3Cross(&refRight, &refFront, &refUp);
		D3DXVec3Normalize(&refRight, &refRight);

		//time_disp = 0;
	}

	Objeto(D3DXVECTOR3 eye, D3DXVECTOR3 target)
	{
		//posicion de la camara
		posicion = eye;
		//a donde ve
		apunta = target;
		refUp = D3DXVECTOR3(0, 1, 0);

		D3DXVec3Normalize(&refUp, &refUp);

		refFront = D3DXVECTOR3(target.x - eye.x, target.y - eye.y, target.z - eye.z);
		D3DXVec3Normalize(&refFront, &refFront);

		D3DXVec3Cross(&refRight, &refFront, &refUp);
		D3DXVec3Normalize(&refRight, &refRight);
	}

	Objeto(D3DXVECTOR3 eye, D3DXVECTOR3 target, float _radio)
	{
		//posicion de la camara
		posicion = eye;
		//a donde ve
		apunta = target;
		refUp = D3DXVECTOR3(0, 1, 0);

		D3DXVec3Normalize(&refUp, &refUp);

		refFront = D3DXVECTOR3(target.x - eye.x, target.y - eye.y, target.z - eye.z);
		D3DXVec3Normalize(&refFront, &refFront);

		D3DXVec3Cross(&refRight, &refFront, &refUp);
		D3DXVec3Normalize(&refRight, &refRight);

		collider = new Colisiones(posicion, _radio);
		detection = new Colisiones(posicion, 150);

		frameLimit = 9 * 9;
		frame = 0;
		explota = false;
		abduciendo = false;
		velGravedad = 0;

		inAir = true;

		//nuevo.
		_state = "suelo";

		vida = 5;
		band_exp = false;
	}

	Objeto(D3DXVECTOR3 eye, D3DXVECTOR3 target, float _x, float _y, float _z)
	{
		//posicion de la camara
		posicion = eye;
		//a donde ve
		apunta = target;
		refUp = D3DXVECTOR3(0, 1, 0);

		D3DXVec3Normalize(&refUp, &refUp);

		refFront = D3DXVECTOR3(target.x - eye.x, target.y - eye.y, target.z - eye.z);
		D3DXVec3Normalize(&refFront, &refFront);

		D3DXVec3Cross(&refRight, &refFront, &refUp);
		D3DXVec3Normalize(&refRight, &refRight);

		collider = new Colisiones(posicion, _x, _y, _z);
		detection = new Colisiones(posicion, 160);

		frameLimit = 9 * 9;
		frame = 0;
		explota = false;
		abduciendo = false;
		velGravedad = 0;

		inAir = true;

		//nuevo.
		_state = "suelo";

		vida = 3;
		band_exp = false;
	}

	void Update(float dirFront, float arriaba, float izqder)
	{
		posBuffer = posicion;

		D3DXVECTOR4 tempo;
		D3DXQUATERNION quatern; //quaternion temporal para la camara
		D3DXMATRIX giraUp, giraRight; //matrices temporales para los giros

		//creamos al quaternion segun el vector up
		D3DXQuaternionRotationAxis(&quatern, &refUp, izqder); //izqder
		//lo normalizamos para que no acumule error
		D3DXQuaternionNormalize(&quatern, &quatern);
		//creamos la matriz de rotacion basados en el quaternion
		D3DXMatrixRotationQuaternion(&giraUp, &quatern);

		//transformamos a los vectores ded la camara
		D3DXVec3Transform(&tempo, &refFront, &giraUp);
		//como el resultado de la operacion anterior es Vec4 lo casteamos para hacerlo vec3
		refFront = (D3DXVECTOR3)tempo;
		//normalizamos para no acumular error
		D3DXVec3Normalize(&refFront, &refFront);
		//Con el vector de referencia y el nuevo front calculamos right de nuevo
		D3DXVec3Cross(&refRight, &refFront, &refUp);

		//una vez calculado right a partir de front y up ahora rotamos sobre right
		//repetimos el procedimiento anterior
		D3DXQuaternionRotationAxis(&quatern, &refRight, arriaba); //arriaba
		D3DXQuaternionNormalize(&quatern, &quatern);
		D3DXMatrixRotationQuaternion(&giraRight, &quatern);

		D3DXVec3Transform(&tempo, &refFront, &giraRight);
		refFront = (D3DXVECTOR3)tempo;
		D3DXVec3Normalize(&refFront, &refFront);


		//ajustamos la matriz de vista con lo obtenido
		posicion += refFront * dirFront / 10.0;
		apunta = posicion + refFront;
		//Para la luz
		vec_luz = posicion + (refFront * 1000);

		if(collider != NULL)
			collider->Update(posicion);

		if (detection != NULL)
			detection->Update(posicion);
	}

	void explosion() {
		if(this->explota)
			frame++;

		//Si el frame llega al limite, se reinicia la animacion;
		//if (frame == frameLimit)
			//frame = 0;
	}

	void gravedad(float ypos, float ydir) { //parametros basados en la posicion del terreno.
		//if (_state == "abduccion") {
		//	if (velGravedad < 0.25f) {
		//		velGravedad += 0.1f;
		//	}
		//	else {
		//		velGravedad = 0.25f;
		//	}
		//}
		//else if (_state == "aire") {
		//	if (velGravedad > -2.5f) {
		//		velGravedad -= 0.05f;
		//	}
		//	else {
		//		velGravedad = -2.5f;
		//	}

		//	posicion.y = posicion.y + velGravedad;
		//	apunta.y = posicion.y + velGravedad;
		//}
		//else if (_state == "suelo") {
		//	posicion.y = ypos;
		//	apunta.y = ydir;
		//	velGravedad = 0;
		//}


		if (abduciendo) {
			if (velGravedad < 0) {
				velGravedad = 0;
			}
			else if (velGravedad < 0.25f) {
				velGravedad += 0.1f;
			}
			else {
				velGravedad = 0.25f;
			}
		}
		else {
			if (velGravedad > -2.5f) {
				velGravedad -= 0.05f;
				//abduciendo = false;
			}
			else {
				velGravedad = -2.5f;
				//abduciendo = false;
			}
		}

		if (posicion.y > ypos) {
			posicion.y = posicion.y + velGravedad;
			apunta.y = posicion.y + velGravedad;
			inAir = true;
		}
		else if (posicion.y < ypos) {
			posicion.y = ypos;
			apunta.y = ydir;
			velGravedad = 0;
			inAir = false;
		}
	}

	~Objeto()
	{
		delete collider;
		collider = NULL;

		delete detection;
		detection = NULL;
	}
};
#endif