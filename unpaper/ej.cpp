#include <iostream>
#include <limits>
#include "CImg.h"
#include "PDI_functions.h"
#include "pdi.h"

using namespace cimg_library;
using namespace std;

bool mostrar = true;

#define pdi_WH(img, W, H) int W = img.width(), H = img.height()
double neg[] = {0.0};
//double rojo[] = {255.0, 0.0, 0.0};

///Convierte una imagen a escala de grises.
CImg<double> greyscale(CImg<double> &img)
{
	CImg<double> gris(img.width(), img.height(), 1, 1);
	gris = (img.get_channel(0) + img.get_channel(1) + img.get_channel(2))/3;
	return gris;
}

///Procesa la magnitud de la FFT para que sea visualizable.
CImg<double> get_show_fft2(CImgList<double> &fft)
{
	CImg<double> mag = get_magnitud_fft(fft);
	pdi_WH(mag, M, N);
	mag.shift(M/2, N/2, 0, 0, 2);
	//le "tapo" el centro para que sus valores tan altos no molesten después
	int mm = min(M, N);
	mag.draw_circle(M/2, N/2, mm/2, neg);
	//mag += numeric_limits<double>::epsilon();
	mag += 0.01;
	mag.log();
	//mag.normalize(0, 255);
	return mag;
}


///Obtiene una recta representando la inclinación de la imagen.
CImg<double> get_inclinacion(CImg<double> &img, float &a)
{
	CImg<double> g = img;//.erode(5).dilate(2).erode(5).dilate(10).get_gradient()[1].abs().normalize(0, 255).display();
	CImgList<double> fft = g.get_FFT();
	CImg<double> mag = get_show_fft2(fft);

	if (mostrar)
		mag.display("FFT");
	
	//aplicando un umbral, trato de dejar puntos que representen la recta mayor
	//mag.threshold(mag.max()*0.94);
	mag.threshold(mag.max()*0.90);
	if (mostrar)
		mag.display("FFT umbralizada");

	double t;
		
	//dejo bien la recta con la transformada de Hough
	CImg<double> h = hough(mag, false, t, 0);
	h.threshold(h.max());
	//h.display("Hough umbralizado");
	int v[4];
	CImg<double> h2 = hough(h, true, t, v);
	h2.display("Hough inverso");

	a = t;
	/*
	//------pruebas----------
	float ang = t*180.0/M_PI;
	cout<<2*t<<endl;
	//------pruebas----------
	a = 2*t;
	*/
	
	CImg<double> color(mag.width(), mag.height(), 1, 3);
	mag = get_show_fft2(fft).normalize(0, 255);
	color.get_shared_channel(0) = mag;
	color.get_shared_channel(1) = mag;
	color.get_shared_channel(2) = mag;
	color.draw_line(v[0], v[1], v[2], v[3], rojo);
	color.display("Línea encontrada");
	//CImg<double> color(mag.width(), mag.height(), 1, 3);
	//mag = get_show_fft2(fft).normalize(0, 255);
	color.get_shared_channel(0) = g;
	color.get_shared_channel(1) = g;
	color.get_shared_channel(2) = g;
	color.draw_line(v[0], v[1], v[2], v[3], rojo);
	color.display("Línea encontrada");
	return h2;
}


//main:
int main(int argc, char *argv[]){

	cimg_usage("Parcial de práctica");
	const char *file = cimg_option("-i", "", "Imagen");
	
	if (!strcmp(file, ""))
		return -1;
	
	CImg<double> img_orig(file), img = greyscale(img_orig);
	pdi_WH(img, W, H);
	
	float ang;
	CImg<double> h2 = get_inclinacion(img, ang);
	//return 1;

	
	if (ang > 0){
		ang = (M_PI/2.0 - ang)*180.0/M_PI + 0.5;
		img_orig.get_rotate(-ang, 1, 2).display();
		img_orig.get_rotate(-ang, W/2, H/2, 1, 1, 2).display();
		//img_orig.get_rotate(-ang, 0, H-1, 1, 1, 2).display();
		//img_orig.get_rotate(-ang, W/2, H-1, 1, 1, 2).display();
		
	}
	else if (ang < 0){
		ang = (M_PI/2.0 + ang)*180.0/M_PI;
		img_orig.rotate(ang);
	}
	cout<<"Imagen rotada: "<<ang<<"º"<<endl;

	//if (mostrar)
		//h2.display();

	//img_orig.rotate(-ang, 0, H/2 - 1, 1, 1);
	
	//img_orig.display();
	

	
	
	//ver para qué lado está inclinada la imagen:
	//miro el cuadrante superior derecho, si está vacío es porque la recta está
	//inclinada para el otro lado. Antes "tapo" el centro para que no moleste.
/*	
	CImg<double> cuadrante = h2.get_crop(W/2, 0, W, H/2);
	cuadrante.draw_circle(0, cuadrante.height()-1, cuadrante.height()/2, neg);
	if (mostrar)
		cuadrante.display();


	if (cuadrante.max() == 0){ //rotar hacia la derecha
	
	
		//voy rotando hasta encontrar un ángulo que sirva:
		cuadrante = h2.get_crop(0, 0, W/2-1, H/2-1);
		float ang = 0.0;
		CImg<double> aux(cuadrante);
		while (aux.get_column(aux.width()-3).max() != 0){
			ang += 0.2;
			aux = cuadrante.get_rotate(ang, 0, cuadrante.height()-1, 1, 0);
			if (mostrar)
				aux.display();
		}

		cout<<"angle: "<<ang<<endl;
	
		img_orig.rotate(ang+1.0, 0, H/2 - 1, 1, 1);
		img_orig.display();

	}
	else { //rotar hacia la izquierda
	
		//voy rotando hasta encontrar un ángulo que sirva:
		cuadrante = h2.get_crop(W/2, 0, W-1, H/2-1);
		float ang = 0.0;
		CImg<double> aux(cuadrante), aux2(img);
		while (aux.get_column(3).max() != 0){
			ang -= 0.2;
			aux = cuadrante.get_rotate(ang, 0, cuadrante.height()-1, 1, 0);
			aux2 = img.get_rotate(ang, 0, H/2 - 1, 1, 1);
			if (mostrar)
				aux.display();
		}

		cout<<"angle: "<<ang<<endl;
	
		img_orig.rotate(ang-1.0, 0, H/2 - 1, 1, 1);

//		img_orig.rotate(-ang, 0, H/2 - 1, 1, 1);
		img_orig.display();	
	
	}
*/
	return 0;
	
}

