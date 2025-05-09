#include "Colisiones.h"

//Colisiones::Colisiones(float _x, float _y, float _z, float _radio, int _sl, int _st)
//{
//	e = new Sphere;
//	e->x = _x, e->y = _y, e->z = _z;
//	e->radio = _radio;
//	e->sl = _sl, e->st = _st;
//}

Colisiones::Colisiones(D3DXVECTOR3 _posicion, float _radio) {
	c = NULL;
	e = new Sphere;
	e->x = _posicion.x;
	e->y = _posicion.y;
	e->z = _posicion.z;

	e->radio = _radio;
}

//Colisiones::Colisiones(float _x, float _y, float _z, float _lado)
//{
//	/*c = new Cube;
//	c->x = _x, c->y = _y, c->z = _z;
//	c->lado = _lado;
//
//	c->xMin = _x - (_lado / 2);
//	c->xMax = _x + (_lado / 2);
//
//	c->yMin = _y - (_lado / 2);
//	c->yMax = _y + (_lado / 2);
//
//	c->zMin = _z - (_lado / 2);
//	c->zMax = _z + (_lado / 2);*/
//}

Colisiones::Colisiones(D3DXVECTOR3 _posicion, float _x, float _y, float _z) {
	e = NULL;
	c = new Cube();
	c->xMin = _posicion.x - (_x / 2);
	c->xMax = _posicion.x + (_x / 2);

	c->yMin = _posicion.y;
	c->yMax = _posicion.y + _y;

	c->zMin = _posicion.z - (_z / 2);
	c->zMax = _posicion.z + (_z / 2);
}

Colisiones::Colisiones(float _xMin, float _xMax, float _yMin, float _yMax, float _zMin, float _zMax) {
	e = NULL;
	c = new Cube;
	//c->lado = 0;
	c->xMin = _xMin;
	c->xMax = _xMax;

	c->yMin = _yMin;
	c->yMax = _yMax;

	c->zMin = _zMin;
	c->zMax = _zMax;
}

Colisiones::~Colisiones()
{
	//Elimina la esfera o el cubo dependiendo cual se instanció
	if (e != NULL) {
		delete e;
		e = NULL;
	}
	else if(c != NULL){
		delete c;
		c = NULL;
	}
}

//void Colisiones::primitivo() {
//	if (e != NULL) {
//		glTranslatef(e->x, e->y, e->z);
//		//glScaled(1, 1, 1);
//		glutSolidSphere(e->radio, e->sl, e->st);
//	}
//	else {
//		caja();
//		/*if (c->lado != 0) {
//			glTranslatef(c->x, c->y, c->z);
//			glutSolidCube(c->lado);
//		}
//		else {
//			caja();
//		}*/
//	}
//}

//void Colisiones::caja() {
//	//glColor4d(0.0f, 1.0f, 0.0f, 0.2f);
//	glBegin(GL_QUADS);
//	//FRONTAL
//	glVertex3d(c->xMin, c->yMin, c->zMin); //0,0,0
//	glVertex3d(c->xMin, c->yMax, c->zMin); //0,1,0
//	glVertex3d(c->xMax, c->yMax, c->zMin); //1,1,0
//	glVertex3d(c->xMax, c->yMin, c->zMin); //1,0,0
//	//DERECHA
//	glVertex3d(c->xMax, c->yMin, c->zMin); //1,0,0
//	glVertex3d(c->xMax, c->yMax, c->zMin); //1,1,0
//	glVertex3d(c->xMax, c->yMax, c->zMax); //1,1,1
//	glVertex3d(c->xMax, c->yMin, c->zMax); //1,0,1
//	//TRASERA
//	glVertex3d(c->xMax, c->yMin, c->zMax); //1,0,1
//	glVertex3d(c->xMax, c->yMax, c->zMax); //1,1,1
//	glVertex3d(c->xMin, c->yMax, c->zMax); //0,1,1
//	glVertex3d(c->xMin, c->yMin, c->zMax); //0,0,1
//	//IZQUIERDA
//	glVertex3d(c->xMin, c->yMin, c->zMax); //0,0,1
//	glVertex3d(c->xMin, c->yMax, c->zMax); //0,1,1
//	glVertex3d(c->xMin, c->yMax, c->zMin); //0,1,0
//	glVertex3d(c->xMin, c->yMin, c->zMin); //0,0,0
//	//ARRIBA
//	glVertex3d(c->xMin, c->yMax, c->zMin); //0,1,0
//	glVertex3d(c->xMin, c->yMax, c->zMax); //0,1,1
//	glVertex3d(c->xMax, c->yMax, c->zMax); //1,1,1
//	glVertex3d(c->xMax, c->yMax, c->zMin); //1,1,0
//	//ABAJO
//	glVertex3d(c->xMin, c->yMin, c->zMin); //0,0,0
//	glVertex3d(c->xMin, c->yMin, c->zMax); //0,0,1
//	glVertex3d(c->xMax, c->yMin, c->zMax); //1,0,1
//	glVertex3d(c->xMax, c->yMin, c->zMin); //1,0,0
//	glEnd();
//}

//void Colisiones::dibuja() {
//	glPushMatrix();
//	//glTranslatef(e->x, e->y, e->z);
//	primitivo();
//	//Draw();
//	glPopMatrix();
//}


bool Colisiones::isInside(D3DXVECTOR3 _posicion) {
	//Si la colision es esfera.
	if (e != NULL) {
		double sumaX, sumaY, sumaZ;
		sumaX = (e->x - _posicion.x);
		sumaY = (e->y - _posicion.y);
		sumaZ = (e->z - _posicion.z);

		sumaX = pow(sumaX, 2);
		sumaY = pow(sumaY, 2);
		sumaZ = pow(sumaZ, 2);

		double sumatotal = sumaX + sumaY + sumaZ;
		double distancia = sqrt(sumatotal);

		if (distancia < e->radio) {
			return true;
		}
		return false;
	}
	//Si es un cubo
	if (c != NULL) {
		//Si esta dentro de "x", "y" y "z"
		if ((_posicion.x >= c->xMin && _posicion.x <= c->xMax) &&
			(_posicion.y >= c->yMin && _posicion.y <= c->yMax) &&
			(_posicion.z >= c->zMin && _posicion.z <= c->zMax)) {
			return true;
		}
		return false;
	}
}

bool Colisiones::colSpheres(Sphere* _local, Sphere* _extern) {
	//double _distance = _local->radio + _extern->radio;
	double sumaX, sumaY, sumaZ;
	sumaX = _local->x - _extern->x;
	sumaY = _local->y - _extern->y;
	sumaZ = _local->z - _extern->z;

	sumaX = pow(sumaX, 2);
	sumaY = pow(sumaY, 2);
	sumaZ = pow(sumaZ, 2);

	double _sumatotal = sumaX + sumaY + sumaZ;
	double _distancia = sqrt(_sumatotal);

	if (_distancia < (_local->radio + _extern->radio)) {
		return true;
	}
	return false;
}

bool Colisiones::isInsideXZ(D3DXVECTOR3 _posicion) {
	//Si la colision es esfera.
	if (e != NULL) {
		double sumaX, sumaZ;
		sumaX = (e->x - _posicion.x);
		//sumaY = (e->y - _posicion.y);
		sumaZ = (e->z - _posicion.z);

		sumaX = pow(sumaX, 2);
		//sumaY = pow(sumaY, 2);
		sumaZ = pow(sumaZ, 2);

		double sumatotal = sumaX + sumaZ;
		double distancia = sqrt(sumatotal);

		if (distancia < e->radio) {
			return true;
		}
		return false;
	}
	////Si es un cubo
	//if (c != NULL) {
	//	//Si esta dentro de "x", "y" y "z"
	//	if ((_posicion.x >= c->xMin && _posicion.x <= c->xMax) &&
	//		(_posicion.y >= c->yMin && _posicion.y <= c->yMax) &&
	//		(_posicion.z >= c->zMin && _posicion.z <= c->zMax)) {
	//		return true;
	//	}
	//	return false;
	//}
}

//bool Colisiones::isOutside(D3DXVECTOR3 _posicion) {
//	//Si la colision es esfera.
//	if (e != NULL) {
//		double sumaX, sumaY, sumaZ;
//		sumaX = (e->x - _posicion.x);
//		sumaY = (e->y - _posicion.y);
//		sumaZ = (e->z - _posicion.z);
//
//		sumaX = pow(sumaX, 2);
//		sumaY = pow(sumaY, 2);
//		sumaZ = pow(sumaZ, 2);
//
//		double sumatotal = sumaX + sumaY + sumaZ;
//		double distancia = sqrt(sumatotal);
//
//		if (distancia > e->radio) {
//			return true;
//		}
//		return false;
//	}
//}

//bool Colisiones::isOutsideXZ(D3DXVECTOR3 _posicion) {
//	//Si la colision es esfera.
//	if (e != NULL) {
//		double sumaX, sumaZ;
//		sumaX = (e->x - _posicion.x);
//		sumaZ = (e->z - _posicion.z);
//
//		sumaX = pow(sumaX, 2);
//		sumaZ = pow(sumaZ, 2);
//
//		double sumatotal = sumaX + sumaZ;
//		double distancia = sqrt(sumatotal);
//
//		if (distancia > e->radio) {
//			return true;
//		}
//		return false;
//	}
//}


//int Colisiones::Puntos(Proyectil *p) {
//	float distan = distancia(p);
//	
//	if (distan <= 0.7f) {
//		return  100;
//	}
//	else if (distan <= 1.3f) {
//		return 50;
//	}
//	else if (distan <= 1.9f) {
//		return 20;
//	}
//	else if (distan <= 2.6f) {
//		return 10;
//	}
//	return 0;
//}

//float Colisiones::GetDistancia(Objeto *&p) { 
//	double sumaX, sumaY;
//	sumaX = (e->x - p->posicion.x);
//	sumaY = (e->y - p->posicion.y);
//
//	sumaX = pow(sumaX, 2);
//	sumaY = pow(sumaY, 2);
//
//	double sumatotal = sumaX + sumaY;
//	double distancia = sqrt(sumatotal);
//
//	float distanciaF = (float)distancia;
//
//	return distanciaF;
//}

void Colisiones::Update(D3DXVECTOR3 _posicion) {
	e->x = _posicion.x;
	e->y = _posicion.y;
	e->z = _posicion.z;
}