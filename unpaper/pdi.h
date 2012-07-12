/*
 * pdi.h: Funciones para PDI.
 * Autor: Juan Bertinetti
 * Procesamiento Digital de Imágenes - 2012
*/

#include <complex>

using namespace cimg_library;

/****************************************************************************
 * UTILIDADES:
 ***************************************************************************/

///Tipo de dato `unsigned char` (typedef).
typedef unsigned char uc;


///Dimensiones de la imagen (macro).
/**
 * Crea las variables con nombre W y H y les asigna el ancho y alto de la
 * imagen img.
*/
#define pdi_WH(img, W, H) int W = img.width(), H = img.height()


///Convierte una imagen a escala de grises.
/**
 * \param img Imagen.
 * \return La imagen en escala de grises (con 1 solo canal).
*/
template<class T>
CImg<T> greyscale(CImg<T> &img)
{
	CImg<T> gris(img.width(), img.height(), 1, 1);
	gris = (img.get_channel(0) + img.get_channel(1) + img.get_channel(2))/3;
	return gris;
}



/****************************************************************************
 * ALGUNOS COLORES PARA FUNCIONES DE DIBUJADO:
 ***************************************************************************/

const uc blanco[] = {255, 255, 255};
const uc negro[] = {0, 0, 0};
const uc rojo[] = {255, 0, 0};
const uc verde[] = {0, 255, 0};
const uc azul[] = {0, 0, 255};
const uc amarillo[] = {255, 255, 0};



/****************************************************************************
 * FILTROS DE SUAVIZADO Y ACENTUADO:
 ***************************************************************************/

///Filtro de suavizado (gaussiano).
/**
 * Suaviza la imagen aplicando un filtro de promediado gaussiano (espacial).
 *
 * \param img Imagen.
 * \param n Tamaño de la máscara (n x n). Por defecto 3.
 * \param sigma Sigma para la gaussiana. Por defecto 1.0.
 * \return La imagen suavizada.
*/
template<class T>
CImg<T> suavizar(CImg<T> &img, int n = 3, double sigma = 1.0)
{
	CImg<double> gauss(n, n, 1, 1, 0.0);
	gauss.draw_gaussian((n-1)/2.0, (n-1)/2.0, sigma, blanco);
	gauss /= gauss.sum();

	CImg<T> res = img.get_convolve(gauss);
	return res;
}


///Filtro de acentuado de alta-potencia.
/**
 * Filtro de acentuado usando la siguiente máscara de 3x3:
 * 
 *         | -1   -1    -1 |
 *         | -1  A + 8  -1 |
 *         | -1   -1    -1 |
 * 
 * \param img Imagen.
 * \return La imagen procesada.
*/
template<class T>
CImg<T> acentuar(CImg<T> &img, double A = 0.0)
{
	double data[] = {-1, -1, -1, -1, A + 8, -1, -1, -1, -1};
	CImg<double> m(data, 3, 3);
	CImg<T> res = img.get_convolve(m);
	return res;
}



/****************************************************************************
 * FILTROS DE MEDIAS:
 ***************************************************************************/

///Filtro de la media aritmética.
/**
 * Filtro de la media aritmética (promedio) para reducir ruido gaussiano.
 * Usa condición de borde Neumann (repite los píxeles extremos cuando la
 * máscara se sale de la imagen).
 * 
 * \param img Imagen.
 * \param ancho_filtro Tamaño de la máscara (ancho). Por defecto 3.
 * \param alto_filtro Tamaño de la máscara (alto). Por defecto 3.
 * \return La imagen filtrada.
*/
template<class T>
CImg<T> media_aritmetica(const CImg<T> &img, int ancho_filtro = 3, int alto_filtro = 3)
{
	int &M = ancho_filtro, &N = alto_filtro;
	CImg<double> masc(M, N, 1, 1, 1.0/(M*N));
	CImg<T> res = img.get_convolve(masc);
	return res;
}


///Filtro de la media geométrica.
/**
 * Filtro de la media geométrica para reducir ruido gaussiano.
 * Usa condición de borde Neumann (repite los píxeles extremos cuando la
 * máscara se sale de la imagen).
 * 
 * \param img Imagen.
 * \param ancho_filtro Tamaño de la máscara (ancho). Por defecto 3.
 * \param alto_filtro Tamaño de la máscara (alto). Por defecto 3.
 * \return La imagen filtrada.
*/
template<class T>
CImg<T> media_geometrica(const CImg<T> &img, int ancho_filtro = 3, int alto_filtro = 3)
{
	CImg<T> res(img);
	double aux, d = 1.0/(ancho_filtro * alto_filtro);
	int h = alto_filtro/2, w = ancho_filtro/2;
	cimg_forXY(img, x, y){
		CImg<T> vecinos = img.get_crop(x-w, y-h, x+w, y+h, true);
		aux = 1.0;
		cimg_forXY(vecinos, i, j){
			aux *= vecinos(i, j);
		}
		res(x, y) = pow(aux, d);
	}
	return res;
}


///Filtro de la media contra-armónica.
/**
 * Filtro de la media contra-armónica.
 * Usa condición de borde Neumann (repite los píxeles extremos cuando la
 * máscara se sale de la imagen).
 * 
 * \param img Imagen.
 * \param Q Orden del filtro.
 * \param ancho_filtro Tamaño de la máscara (ancho). Por defecto 3.
 * \param alto_filtro Tamaño de la máscara (alto). Por defecto 3.
 * \return La imagen filtrada.
*/
template<class T>
CImg<T> contra_armonica(const CImg<T> &img, int Q, int ancho_filtro = 3, int alto_filtro = 3)
{
	CImg<T> res(img);
	double aux1, aux2;
	int h = alto_filtro/2, w = ancho_filtro/2;
	cimg_forXY(img, x, y){
		CImg<T> vecinos = img.get_crop(x-w, y-h, x+w, y+h, true);
		aux1 = aux2 = 0.0;
		cimg_forXY(vecinos, i, j){
			aux1 += pow(vecinos(i, j), Q+1.0);
			aux2 += pow(vecinos(i, j), Q);
		}
		res(x, y) = aux1/aux2;
	}
	return res;
}



/****************************************************************************
 * FILTROS DE ORDEN:
 ***************************************************************************/
 
///Filtro de la mediana.
/**
 * Filtro de la mediana para reducir ruido impulsivo.
 * Usa condición de borde Neumann (repite los píxeles extremos cuando la
 * máscara se sale de la imagen).
 * 
 * \param img Imagen.
 * \param ancho_filtro Tamaño de la máscara (ancho). Por defecto 3.
 * \param alto_filtro Tamaño de la máscara (alto). Por defecto 3.
 * \return La imagen filtrada.
*/
template<class T>
CImg<T> mediana(const CImg<T> &img, int ancho_filtro = 3, int alto_filtro = 3)
{
	CImg<T> res(img);
	int h = alto_filtro/2, w = ancho_filtro/2;
	cimg_forXY(img, x, y){
		CImg<T> vecinos = img.get_crop(x-w, y-h, x+w, y+h, true);
		res(x, y) = vecinos.median();
	}
	return res;
}


///Filtro del punto medio.
/**
 * Filtro del punto medio ((max - min)/2).
 * Usa condición de borde Neumann (repite los píxeles extremos cuando la
 * máscara se sale de la imagen).
 * 
 * \param img Imagen.
 * \param ancho_filtro Tamaño de la máscara (ancho). Por defecto 3.
 * \param alto_filtro Tamaño de la máscara (alto). Por defecto 3.
 * \return La imagen filtrada.
*/
template<class T>
CImg<T> punto_medio(const CImg<T> &img, int ancho_filtro = 3, int alto_filtro = 3)
{
	CImg<T> res(img);
	int h = alto_filtro/2, w = ancho_filtro/2;
	T mx, mn;
	cimg_forXY(img, x, y){
		CImg<T> vecinos = img.get_crop(x-w, y-h, x+w, y+h, true);
		mx = vecinos.max_min(mn);
		res(x, y) = (mx + mn)/2.0;
	}
	return res;
}


///Filtro de la media alfa-recortado.
/**
 * Filtro de la media alfa-recortado.
 * Usa condición de borde Neumann (repite los píxeles extremos cuando la
 * máscara se sale de la imagen).
 * 
 * \param img Imagen.
 * \param d Parámetro del filtro.
 * \param ancho_filtro Tamaño de la máscara (ancho). Por defecto 3.
 * \param alto_filtro Tamaño de la máscara (alto). Por defecto 3.
 * \return La imagen filtrada.
*/
template<class T>
CImg<T> alfa_recortado(const CImg<T> &img, int d, int ancho_filtro = 3, int alto_filtro = 3)
{
	CImg<T> res(img);
	double aux;
	int h = alto_filtro/2, w = ancho_filtro/2;
	int s = alto_filtro*ancho_filtro - d, d2 = d/2;
	cimg_forXY(img, x, y){
		aux = 0.0;
		CImg<T> vecinos = img.get_crop(x-w, y-h, x+w, y+h, true).vector().sort();
		for (unsigned int i=d2; i<vecinos.size()-d2; ++i){
			aux += vecinos(i);
		}
		res(x, y) = aux/s;
	}
	return res;
}



/****************************************************************************
 * UTILIDADES PARA TRANSFORMADA DE FOURIER:
 ***************************************************************************/

///FFT Inversa (macro).
#define get_IFFT() get_FFT(true)


///Magnitud de la transformada de Fourier.
/**
 * Calcula y devuelve la magnitud de la transformada de Fourier.
 * |F| = sqrt(real(F)^2 + imag(F)^2)
 *
 * \param fft La transformada ya calculada (como CImgList).
 * \return La magnitud de fft.
*/
CImg<double> get_magnitud_fft(CImgList<double> &fft)
{
	CImg<double> mag(fft[0].width(), fft[0].height(), 1, 1);
	CImg<double> &R = fft[0];
	CImg<double> &I = fft[1];
	mag = R.get_pow(2) + I.get_pow(2);
	mag.sqrt();
	return mag;
}


///Fase de la transformada de Fourier.
/**
 * Calcula y devuelve la fase de la transformada de Fourier.
 *
 * \param fft La transformada ya calculada (como CImgList).
 * \return La fase de fft.
*/
CImg<double> get_fase_fft(CImgList<double> &fft)
{
	CImg<double> &R = fft[0];
	CImg<double> &I = fft[1];
	CImg<double> fase(R);

	cimg_forXY(fase, x, y){
		std::complex<double> c(R(x, y), I(x, y));
		fase(x, y) = std::arg(c);
	}
	return fase;
}


///FFT linda para mostrar.
/**
 * Devuelve la magnitud de la transformada de Fourier acomodada de una forma
 * para poder mostrarla (centrada, con logaritmo y normalizada).
 *
 * \param fft La transformada ya calculada (como CImgList).
 * \return La magnitud de fft linda para mostrar.
*/
CImg<double> get_show_fft(CImgList<double> &fft)
{
	CImg<double> mag = get_magnitud_fft(fft);
	mag.shift(mag.width()/2, mag.height()/2, 0, 0, 2);
	mag += 0.000001;
	mag.log();
	mag.normalize(0, 255);
	return mag;
}


///Visualiza la FFT.
/**
 * Visualiza la magnitud de la transformada de Fourier de una manera
 * adecuada (centrada, con logaritmo y normalizada).
 *
 * \param fft La transformada ya calculada (como CImgList).
 * \param title Título para la ventana.
*/
void show_fft(CImgList<double> &fft, const char *title = 0)
{
	get_show_fft(fft).display(title);
}


///Filtro Butterworth pasa-bajos.
/**
 * Crea el H(u,v) de un filtro Butterworth pasa-bajos. Está definido de la
 * siguiente manera:
 * 
 *    H(u,v) =          1
 *             ----------------------
 *              1 + [D(u,v) / D0]^2n
 *
 * H sólo contiene valores reales y ya se encuentra descentrado para aplicar
 * directamente sobre la transformada devuelta por get_FFT(). Para hacer el
 * filtrado hay que multiplicar cada componente por H:
 * G = F·H --> G[0] = F[0]*H, G[1] = F[1]*H
 *
 * \param P Ancho del filtro.
 * \param Q Alto del filtro.
 * \param d0 Frecuencia de corte.
 * \param n Orden del filtro.
 * \return H(u,v).
*/
CImg<double> butterworth(int P, int Q, double d0, int n)
{
	CImg<double> U(P,Q,1,1), V(P,Q,1,1), D(P,Q,1,1);
	CImg<double> H(P, Q, 1, 1, 0);
	
	cimg_forXY(H, i, j){
		U(i,j) = -1. + i*2./P;
		V(i,j) = -1. + j*2./Q;
		D(i,j) = sqrt(pow(U(i,j), 2) + pow(V(i,j), 2));
		H(i, j) = 1.0 / (1 + pow(D(i, j) / d0, 2*n));
	}
	//descentrar:
	H.shift(P/2, Q/2, 0, 0, 2);
	return H;
}


///Filtrar en frecuencia con H real.
/**
 * Filtra la imagen con un filtro en frecuencia H con sólo componentes reales.
 * 
 * G = F·H --> real(G) = real(F)·H
 *             imag(G) = imag(F)·H
 * 
 * \param img Imagen.
 * \param H Filtro.
 * \return La imagen filtrada.
*/
template<class T>
CImg<T> aplicar_H_real(CImg<T> &img, CImg<double> &H)
{
	CImgList<double> fft = img.get_FFT();
	fft[0].mul(H); fft[1].mul(H);
	CImg<double> r = fft.get_IFFT()[0];
	CImg<T> res(r);
	return res;
}



/****************************************************************************
 * DETECTORES DE BORDES:
 ***************************************************************************/

///Operador gradiente de Roberts.
/**
 * Aplica el operador gradiente de Roberts y devuelve Gx y Gy como lista.
 * Roberts:
 * Gx = | 0  0  0 |         Gy = | 0  0  0 |
 *      | 0 -1  0 |              | 0  0 -1 |
 *      | 0  0  1 |              | 0  1  0 |
 * 
 * \param img Imagen.
 * \return Gx y Gy como CImgList<double>.
*/
template<class T>
CImgList<double> roberts(CImg<T> &img)
{
	CImg<double> mx(3, 3, 1, 1, 0), my(mx);
	mx(1, 1) = -1; mx(2, 2) = 1;
	my(2, 1) = -1; my(1, 2) = 1;
	
	CImg<double> gx = img.get_convolve(mx);
	CImg<double> gy = img.get_convolve(my);
	return (gx, gy);
}


///Magnitud del gradiente de Roberts.
/**
 * Aplica el operador gradiente de Roberts y devuelve la magnitud.
 * |grad(f)| ~= |Gx| + |Gy|
 *
 * \param img Imagen.
 * \return Magnitud del gradiente.
*/
template<class T>
CImg<T> roberts_mag(CImg<T> &img)
{
	CImgList<double> g = roberts(img);
	g[0].abs(); g[1].abs();
	CImg<T> res = g[0] + g[1];
	return res;
}


///Operador gradiente de Prewitt.
/**
 * Aplica el operador gradiente de Prewitt y devuelve Gx y Gy como lista.
 * Prewitt:
 * Gx = |-1 -1 -1 |         Gy = |-1  0  1 |
 *      | 0  0  0 |              |-1  0  1 |
 *      | 1  1  1 |              |-1  0  1 |
 *
 * \param img Imagen.
 * \return Gx y Gy como CImgList<double>.
*/
template<class T>
CImgList<double> prewitt(CImg<T> &img)
{
	double datax[] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
	double datay[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
	CImg<double> mx(datax, 3, 3), my(datay, 3, 3);
	
	CImg<double> gx = img.get_convolve(mx);
	CImg<double> gy = img.get_convolve(my);
	return (gx, gy);
}


///Magnitud del gradiente de Prewitt.
/**
 * Aplica el operador gradiente de Prewitt y devuelve la magnitud.
 * |grad(f)| ~= |Gx| + |Gy|
 *
 * \param img Imagen.
 * \return Magnitud del gradiente.
*/
template<class T>
CImg<T> prewitt_mag(CImg<T> &img)
{
	CImgList<double> g = prewitt(img);
	g[0].abs(); g[1].abs();
	CImg<T> res = g[0] + g[1];
	return res;
}


///Operador gradiente de Sobel.
/**
 * Aplica el operador gradiente de Sobel y devuelve Gx y Gy como lista.
 * Sobel:
 * Gx = |-1 -2 -1 |         Gy = |-1  0  1 |
 *      | 0  0  0 |              |-2  0  2 |
 *      | 1  2  1 |              |-1  0  1 |
 *
 * \param img Imagen.
 * \return Gx y Gy como CImgList<double>.
*/
template<class T>
CImgList<double> sobel(CImg<T> &img)
{
	double datax[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
	double datay[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
	CImg<double> mx(datax, 3, 3), my(datay, 3, 3);
	
	CImg<double> gx = img.get_convolve(mx);
	CImg<double> gy = img.get_convolve(my);
	return (gx, gy);
}


///Magnitud del gradiente de Sobel.
/**
 * Aplica el operador gradiente de Sobel y devuelve la magnitud.
 * |grad(f)| ~= |Gx| + |Gy|
 *
 * \param img Imagen.
 * \return Magnitud del gradiente.
*/
template<class T>
CImg<T> sobel_mag(CImg<T> &img)
{
	CImgList<double> g = sobel(img);
	g[0].abs(); g[1].abs();
	CImg<T> res = g[0] + g[1];
	return res;
}


///Laplaciano.
/**
 * Aplica el laplaciano usando la máscara de 8-vecinos a la imagen:
 * ∇^2 f = | -1 -1 -1 |
 *         | -1  8 -1 |
 *         | -1 -1 -1 |
 *
 * \param img Imagen.
 * \return El laplaciano de la imagen.
*/
template<class T>
CImg<double> laplaciano(CImg<T> &img)
{
	double data[] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
	CImg<double> m(data, 3, 3);
	CImg<double> res = img.get_convolve(m);
	return res;
}


///Laplaciano del gaussiano (LoG).
/**
 * Aplica el laplaciano a una función gaussiana de media cero y usa esto
 * como máscara.
 *
 * \param img Imagen.
 * \param sigma Desvío estándar para la función gaussiana.
 * \param size Tamaño de la máscara.
 * \return La imagen convolucionada con la máscara del LoG.
*/
template<class T>
CImg<double> laplaciano_gaussiano(CImg<T> &img, double sigma, int size = 5)
{
	int ini, fin;
	double r2;
	CImg<double> mask(size, size);
	ini = (int)(-1)*(size-1)/2;
	fin = (int)(size-1)/2;
	for (int x=ini; x<fin+1; ++x){
		for (int y=ini; y<fin+1; ++y){
			r2=(double)(x*x+y*y);
			mask(x-ini, y-ini) = 1.0*(r2-2*pow(sigma,2))/pow(sigma,4)*exp(-1.0*r2/(2*pow(sigma,2)));
		}
	}
	mask /= mask.sum();

	CImg<double> res = img.get_convolve(mask);
	return res;
}



/****************************************************************************
 * OTRAS:
 ***************************************************************************/

///Transformación lineal de grises.
/**
 * Aplica la transformación lineal
 *
 *       s = a*r + c
 *
 * a la imagen en escala de grises. Para ello implementa una LUT (Look Up
 * Table) y luego procesa la imagen.
 *
 * \param img Imagen.
 * \param a Parámetro `a`.
 * \param c Parámetro `c` (offset).
 * \param show Si es true muestra un gráfico de la transformación.
 * \return La imagen procesada.
*/
template<class T>
CImg<T> LUT(CImg<T> &img, float a, float c, bool show = false)
{
	//generar LUT
	CImg<uc> lut(256, 1, 1, 1);
	for (int i=0; i<256; ++i){
		int aux = a*i + c;
		if (aux > 255) lut(i) = 255;
		else if (aux < 0) lut(i) = 0;
		else lut(i) = (uc)aux;
	}
	//procesar imagen:
	CImg<T> res(img);
	cimg_forXY(res, x, y){
		res(x, y) = lut(img(x, y));
	}
	if (show){
		CImg<uc> g(256, 256, 1, 1, 0);
		g.draw_graph(lut, blanco, 1, 1, 0, 255, 0);
		g.display();
	}
	return res;
}


///Realce usando estadísticas locales.
/**
 * Hace un procesado de la imagen para realce teniendo en cuenta estadísticas
 * locales (media y varianza en cada vecindad). Método propuesto en el libro
 * Digital Image Processing (Gonzalez & Woods), 2º Ed., pág. 103: "Use of
 * Histogram Statistics for Image Enhancement".
 * El método se define como:
 * 
 *            |  E * f(x,y) si (`ms` <= k0*Mg) y (k1*Dg <= `ds` <= k2*Dg)
 * g(x,y) = <-|
 *            |  f(x,y) en otro caso
 *
 * donde: `ms` media local
 *        `ds` desvío estándar local
 *         Mg  media global
 *         Dg  desvío estándar global
 * 
 * \param img Imagen a procesar.
 * \param MG Media global de la imagen.
 * \param DG Desvío estándar global de la imagen.
 * \param k0 Constante positiva menor que 1.
 * \param k1 Constante, debe cumplir k1 < k2.
 * \param k2 Constante, mayor que 1 si se desea realzar áreas claras, menor
 * 	          que 1 si se desea realzar áreas oscuras.
 * \param E Constante multiplicativa.
 * \param size Tamaño de la vecindad (size x size). Defecto 3.
 * \param show Si es true, se muestran gráficos con las medias locales y
 *              desvíos locales de cada vecindad de cada píxel.
 * \return La imagen procesada.
*/
template<class T>
CImg<T> local_enhancement(CImg<T> &img, double MG, double DG, double k0,
	double k1, double k2, double E, int size = 3, bool show = false)
{
	CImg<T> res(img), medias(img), stds(img);
	int h = size/2, w = size/2;
	double m, d;
	cimg_forXY(img, x, y){
		CImg<T> vecinos = img.get_crop(x-w, y-h, x+w, y+h, true);
		d = sqrt(vecinos.variance_mean(0, m));
		if (m <= k0*MG && k1*DG <= d && d <= k2*DG)
			res(x, y) = E * img(x, y);
		if (show){
			medias(x, y) = m;
			stds(x, y) = d;
		}
	}
	if (show){
		medias.display("Medias locales");
		stds.display("Desvíos estándar locales");
	}
	return res;
}


///Transformada de Hough para detectar círculos.
/**
 * Realiza la transformada de Hough especializada para detectar círculos de un
 * radio dado en la imagen. En el plano rho-theta que devuelve, los máximos
 * representan el centro de un círculo de radio r.
 *
 * \param img Imagen binaria (sólo bordes).
 * \param r Radio deseado para los círculos a detectar.
*/
CImg<double> hough_circ(CImg<double> &img, int r)
{
	CImg<double> iHough(img); iHough.fill(0.0);
	pdi_WH(img, M, N);

	cimg_forXY(img, i, j){
		if (img(i, j) > 0.5){
			for (int a=0; a<M; ++a){
				double det = r*r-(i-a)*(i-a);
				if (det>0){
					double b=round(j-sqrt(det));
					if (b>0 && b<N)
						iHough(a,b)++;
				}
			}
		}
	}
	//iHough /= iHough.sum();
	return iHough;
}


///Función de degradación de desenfoque por movimiento lineal uniforme.
/**
 * Calcula y devuelve la función de degradación H de desenfoque debido a
 * movimiento lineal uniforme, definida por:
 *
 * H(u,v) =      1                        -jπ(ua+vb)
 *           --------- · sin[π(ua+vb)] · e
 *            π(ua+vb)
 * 
 * H(u,v) contiene números complejos, por lo que es devuelta como una lista
 * CImgList<double> donde están separadas la parte real y la parte imaginaria.
 * 
 * \param a Parámentro `a` de la función.
 * \param b Parámentro `b` de la función.
 * \param imgW Ancho de la imagen.
 * \param imgH Alto de la imagen.
 * \return CImgList<double>(real(H), imag(H)).
*/
CImgList<double> H_mlu(double a, double b, int imgW, int imgH)
{
	int M = imgW, N = imgH;
	double u, v;
	std::complex<double> i(0.0, 1.0), c;
	CImg<double> Hr(M, N, 1, 1), Hi(Hr);
	
	cimg_forXY(Hr, x, y){
		u = -1. + x*2./M;
		v = -1. + y*2./N;
		double argg = M_PI*(u*a + v*b);
		if (argg == 0)
			c = std::complex<double>(1.0, 0.0);
		else
			c = 1.0 / argg * sin(argg) * exp(-i*argg);
		Hr(x, y) = real(c);
		Hi(x, y) = imag(c);
	}

	Hr.shift(M/2, N/2, 0, 0, 2);
	Hi.shift(M/2, N/2, 0, 0, 2);
	return (Hr, Hi);
}



/****************************************************************************
 * SEGMENTACIÓN:
 ***************************************************************************/

///Segmentación de color (color-slicing).
/**
 * Segmenta un color dado teniendo en cuenta una esfera de radio definido por
 * una tolerancia (todos los colores que caigan en esta esfera se mantienen,
 * el resto se hace 0).
 *
 * \param img Imagen.
 * \param R Valor de rojo.
 * \param G Valor de verde.
 * \param B Valor de azul.
 * \param tol Tolerancia.
 * \return Imagen con el color segmentado.
*/
template<class T>
CImg<T> segment_color(CImg<T> &img, uc R, uc G, uc B, double tol)
{
	CImg<T> res(img);
	cimg_forXY(img, x, y){
		if (pow(img(x, y, 0) - R, 2) + pow(img(x, y, 1) - G, 2) +
				pow(img(x, y, 2) - B, 2) > tol){
			res(x, y, 0) = 0;
			res(x, y, 1) = 0;
			res(x, y, 2) = 0;
		}
	}
	return res;
}
