#include "StellarObject.h"
#include <array>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>


float pi = (float)M_PI;


StellarObject::StellarObject(char* name,int model_width, int model_height)
	:m_name(name)
{
	
	float *positions_planet = new float[model_width * model_height *5]; // Equirectangular projection^-1 (left to right, bottom to top)
	float inc_m = 1.0f / (float)model_height;
	float inc_n = 2.0f / (float)model_width;
	for (int j = 0; j < model_height; j++) {
		for (int i = 0; i < model_width; i++) {
			positions_planet[(j* model_width +i)*5  ] = cos(pi*(-0.5f+j*inc_m))*cos(pi*(-1.0f+i*inc_n));	//x pos
			positions_planet[(j* model_width +i)*5+1] = cos(pi*(-0.5f+j*inc_m))*sin(pi*(-1.0f+i*inc_n));	//y pos
			positions_planet[(j* model_width +i)*5+2] = sin(pi*(-0.5f+j*inc_m));								//z pos
			positions_planet[(j* model_width +i)*5+3] =  0.0f + i*1.0f / (float)model_width;				//x tex 
			positions_planet[(j* model_width +i)*5+4] =  0.0f + j*1.0f / (float)model_height;				//y tex
			
		}
	}

	unsigned int* indices_planet = new unsigned int[(model_width -1)*(model_height -1)*2*3];
	for (int j = 0; j < model_height -1; j++) {
		for (int i = 0; i < model_width -1; i++) {
			indices_planet[(j* model_width +i)*6+0] = j* model_width +i;			//1
			indices_planet[(j* model_width +i)*6+1] = j* model_width +i+1;			//2
			indices_planet[(j* model_width +i)*6+2] = (j+1)* model_width +i;		//3
			indices_planet[(j* model_width +i)*6+3] = (j+1)* model_width +i;		//1
			indices_planet[(j* model_width +i)*6+4] = (j+1)* model_width +i+1;		//2
			indices_planet[(j* model_width +i)*6+5] = j* model_width +i+1;			//3
		}
	}


	delete[] positions_planet;
	delete[] indices_planet;

}


