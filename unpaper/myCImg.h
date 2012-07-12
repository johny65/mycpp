/****************************************************************************
 * myCImg.h: Autor original Cristian Yones
 ***************************************************************************/

#ifndef MYCIMG_H
#define MYCIMG_H
//#include<CImg.h>

#include <algorithm>
#include <set>
#include <cassert>
#include <vector>
#include <deque>
#include <cmath>

using namespace std;
using namespace cimg_library;

#define forpixels( img ) \
for( int j=0; j<img.height(); j++ ) { \
	for( int i=0; i<img.width(); i++ )

#define endfor() }
#define byte unsigned char
#define intensidad(img,i,j) (img(i,j,0,0)+img(i,j,0,1)+img(i,j,0,2))/764
#define distancia( x1, y1, x2, y2) sqrt( pow(x1-x2,2) + pow(y1-y2,2) )

struct Pixel {
	int x;
	int y;
	CImg<double>* img;
	Pixel() : x(0), y(0), img(NULL) {};
	Pixel(int x, int y, CImg<double>* img) : x(x), y(y), img(img) {};
	bool operator<( const Pixel& p ) const {
		return this->value() < p.value();
	}
	double value() const {
		return img->operator()(x,y);
	}
	bool AdentroImagen( ) {
		return x>=0 && y>=0 && x<img->width() && y<img->height();
	}
};


void enfocarImagen( CImg<double> &img, double lim );
CImg<double> Graficar( CImg<double> &img, int h );
void SepararCanales( CImg<double>& img, CImg<double>& img1, CImg<double>& img2, CImg<double>& img3 );
CImg<double> PerfilY( CImg<double>& img );
CImg<double> PerfilX( CImg<double>& img );
Pixel Minimo( CImg<double>& img );
Pixel Maximo( CImg<double>& img );
CImg<bool> Inundar( CImg<double> img, double limite );
CImg<bool> InundarInverso( CImg<double> img, double limite );
CImg<bool> Crecimiento( CImg<double> img, double limite );
CImg<bool> CrecimientoInverso( CImg<double> img, double limite );
CImg<double> MedialAxis( CImg<double> img );
CImg<bool> MaximosLocales( CImg<double>& img, double limite=0 );
CImg<double> AlfaRecortado(CImg<double>& img, int mx, int my, int d);
CImg<double> TransformadaHough( CImg<bool>& img );
CImg<double> InversaHough(CImg<double> img, int nLineas);






using namespace std;

void enfocarImagen( CImg<double> &img, double lim ) {
	int xmin=img.width(), xmax=0, ymin=img.height(), ymax=0;
	for( int j=0; j<img.height(); j++ ) { 
		for( int i=0; i<img.width(); i++ ) {
			if( img(i,j,0,0)+img(i,j,0,1)+img(i,j,0,2) < lim*3 ) {
				if( xmin > i ) xmin=i;
				if( xmax < i ) xmax=i;
				if( ymin > j ) ymin=j;
				if( ymax < j ) ymax=j;
			}
		}
	}
	img.crop( xmin,ymin,xmax,ymax );
}

CImg<double> Graficar( CImg<double> &img, int h ) {
	static const byte tc[] = { 255 };
	CImg<double> grafico( img.width(), h );
	grafico.fill(0);
	grafico.draw_graph( img, tc );
	return grafico;
}

void SepararCanales( CImg<double>& img, CImg<double>& img1, CImg<double>& img2, CImg<double>& img3 ) {
	img1 = CImg<double>( img.width(), img.height() );
	img2 = CImg<double>( img.width(), img.height() );
	img3 = CImg<double>( img.width(), img.height() );
	forpixels(img) {
		img1(i,j) = img(i,j,0,0);
		img2(i,j) = img(i,j,0,1);
		img3(i,j) = img(i,j,0,2);
		endfor();
	}
}

CImg<double> PerfilY( CImg<double>& img ) {
	CImg<double> perfil( img.height(), 1 );
	perfil.fill(0);
	forpixels( img ) {
		perfil(j) += img(i,j);
		endfor();
	}
	return perfil;
}

CImg<double> PerfilX( CImg<double>& img ) {
	CImg<double> perfil = CImg<double> ( img.width(), 1 );
	perfil.fill(0);
	forpixels( img ) {
		perfil(i) += img(i,j);
		endfor();
	}
	return perfil;
}

Pixel Minimo( CImg<double>& img ) {
	int xmin=0, ymin=0;
	double minimo = img(0,0);
	forpixels(img) {
		if( img(i,j) < minimo ) {
			minimo = img(i,j);
			xmin = i;
			ymin = j;
		}
		endfor();
	}
	return Pixel(xmin, ymin, &img);
}

Pixel Maximo( CImg<double>& img ) {
	int xmax=0, ymax=0;
	double maximo = img(0,0);
	forpixels(img) {
		if( img(i,j) > maximo ) {
			maximo = img(i,j);
			xmax = i;
			ymax = j;
		}
		endfor();
	}
	return Pixel(xmax, ymax, &img);
}

CImg<bool> Inundar( CImg<double> img, double limite ) {
	double acumulador = 0;
	img.normalize(0.0, 1.0);
	limite *= ((double)img.width()*(double)img.height()) - img.sum();
	CImg<bool> mask(img.width(), img.height(), 1, 1, false );
	
	set<Pixel> cola;
	cola.insert( Minimo( img ) );
	
	while( acumulador < limite ) {
		//agrego el pixel de menor valor (el primero de la cola)
		Pixel p = (*cola.begin()); cola.erase( cola.begin() );
		acumulador += 1.0 - p.value();
		mask(p.x,p.y) = true;
		//Agrego los vecinos a la cola
		Pixel pix = p;
		pix.x++;
		if( pix.AdentroImagen() && !mask(pix.x, pix.y) ) cola.insert( pix );
		pix.x -= 2;
		if( pix.AdentroImagen() && !mask(pix.x, pix.y) ) cola.insert( pix );
		pix.x++; pix.y++;
		if( pix.AdentroImagen() && !mask(pix.x, pix.y) ) cola.insert( pix );
		pix.y -= 2;
		if( pix.AdentroImagen() && !mask(pix.x, pix.y) ) cola.insert( pix );
	}
	return mask;
}

CImg<bool> InundarInverso( CImg<double> img, double limite ) {
	return Inundar( img.max() - img, limite);
}

CImg<bool> Crecimiento( CImg<double> img, double limite ) {
	img.normalize(0.0, 1.0);
	CImg<bool> mask(img.width(), img.height(), 1, 1, false );
	CImg<bool> procesado(img.width(), img.height(), 1, 1, false );
	deque<Pixel> cola;
	cola.push_back( Minimo( img ) );
	procesado(cola.front().x, cola.front().y) = true;
	
	while( !cola.empty() ) {
		Pixel p = cola.front(); cola.pop_front();
		if( p.value() < limite ) {
			mask(p.x,p.y) = true;
			//agrego a los vecinos para procesarlos
			Pixel pix = p;
			pix.x++;
			if( pix.AdentroImagen() && !procesado(pix.x, pix.y) ) {
				cola.push_back( pix );
				procesado(pix.x, pix.y) = true;
			}
			pix.x -= 2;
			if( pix.AdentroImagen() && !procesado(pix.x, pix.y) ) {
				cola.push_back( pix );
				procesado(pix.x, pix.y) = true;
			}
			pix.x++; pix.y++;
			if( pix.AdentroImagen() && !procesado(pix.x, pix.y) ) {
				cola.push_back( pix );
				procesado(pix.x, pix.y) = true;
			}
			pix.y -= 2;
			if( pix.AdentroImagen() && !procesado(pix.x, pix.y) ) {
				cola.push_back( pix );
				procesado(pix.x, pix.y) = true;
			}
		}
	}
	return mask;
}

CImg<bool> CrecimientoInverso( CImg<double> img, double limite ) {
	return Crecimiento( img.max() - img, limite);
}

CImg<double> MedialAxis( CImg<double> img ) {
	bool hayCambios=true;
	CImg<double> copia = img;
	while( hayCambios ) {
		hayCambios = false;
		forpixels(img) {
			
			if(img(i,j) == 0) {
				double max = 0;
				for(int dx=-1; dx<=1; dx++) {
					for(int dy=-1; dy<=1; dy++) {
						if( abs(dx)+abs(dy)>1 ) continue;
						if(img(i+dx,j+dy) > max)
							max = img(i+dx,j+dy);
					}
				}
				if( max != 0 ) {
					hayCambios = true;
					copia(i,j) = max+1;
				}
			}
			endfor();
		}
		img = copia;
	}
	return img;
}

CImg<bool> MaximosLocales( CImg<double>& img, double limite ) {
	CImg<bool> maximo(img.width(), img.height(),1,1,true);
	forpixels(img) {
		for(int dx=-1; dx<=1 && maximo(i,j); dx++) {
			for(int dy=-1; dy<=1 && maximo(i,j); dy++) {
				if(dx==0 && dy==0) continue;
				if( i+dx < 0 || i+dx >= img.width() || j+dy < 0 || j+dy >= img.height() ) continue;
				maximo(i,j) = maximo(i,j) && ( img(i,j) >= img(i+dx,j+dy) ) && img(i,j) > limite; 
			}
		}
		endfor();
	}
	return maximo;
}


CImg<double> AlfaRecortado(CImg<double>& img, int mx, int my, int d) {
	int dx = (mx-1)/2;
	int dy = (my-1)/2;
	CImg<double> resultado( img.width(), img.height(), 1, 1, 0.0 );
	int upperLimit, lowerLimit, leftLimit, rightLimit;
	vector<double> vecino(mx*my);
	forpixels( img ) {
		leftLimit  = max( 0, i - dx );
		rightLimit = min( img.width()-1, i + dx );
		upperLimit = max( 0, j - dy );
		lowerLimit = min( img.height()-1, j + dy );
		vecino.clear();
		for(int x=leftLimit; x<=rightLimit; x++) {
			for(int y=upperLimit; y<=lowerLimit; y++) {
				vecino.push_back( img(x,y) );
			}
		}
		sort( vecino.begin(), vecino.end() );
		if( vecino.size() > 2*d ) {
			for( int k=d; k<vecino.size()-d; k++ )
				resultado(i,j) += vecino[k];
			resultado(i,j) /= double(vecino.size())-double(2*d);
		}
		else
			resultado(i,j) = img(i,j);
		endfor();
	}
	return resultado;
}


CImg<double> TransformadaHough(CImg<bool> img) {
	const unsigned M = img.width(),N = img.height();
	CImg<double> iHough(M, N, 1, 1, 0.0);
	double maxRho = sqrt(float(pow(N-1,2)+pow(M-1,2))), stepRho = 2.*maxRho/(N-1), stepTheta = M_PI/(M-1), rho, theta;
	int r;
	cimg_forXY(img,y,x){
		if ( img(y,x) ) {
			for (int t=0; t<M; t++) {
				theta = t*stepTheta - M_PI/2;
				rho = x*cos(theta) + y*sin(theta);
				r = floor((rho+maxRho)/stepRho+.5);
				iHough(t,r)+= 1;
			}
		}
	} 
	return iHough; 
}

CImg<double> InversaHough(CImg<double> img, int nLineas) {
	const unsigned M = img.width(),N = img.height();
	CImg<double> iHough(M, N, 1, 1, 0.0);
	double maxRho = sqrt(float(pow(N-1,2)+pow(M-1,2))), stepRho = 2.*maxRho/(N-1), stepTheta = M_PI/(M-1), rho, theta;
	const double blanco[1] = {255.f};
	float x0, x1, y0, y1;
	for( int nl=0; nl < nLineas; nl++ ) {
		Pixel p = Maximo( img );
		int t = p.x, r = p.y;
		theta=t*stepTheta-M_PI/2;
		rho=r*stepRho-maxRho;
		if (theta>-M_PI/2 && theta<M_PI/2){
			y0=0; y1=M-1;
			x0=rho/cos(theta);
			x1=rho/cos(theta)-(M-1)*tan(theta);
		}
		else {
			x0=0; x1=N-1;
			y0=rho/sin(theta);
			y1=rho/sin(theta)-(N-1)/tan(theta);
		}
		iHough.draw_line(y0,x0,y1,x1,blanco);
		img(t,r) = 0;
	}
	return iHough; 
}

#endif
