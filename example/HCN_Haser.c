#include "lime.h"

double Q=1.000000e+29;
double abund = 0.001136;
double tkin = 80.000000;
double vexp = 696.617304;
double rnuc = 650.000000;

double betamain = 1.048126e-05;
double betamol = 1.100533e-05;

double pi = 3.14159265358979323846;

/******************************************************************************/

void
input(inputPars *par, image *img){
  par->Qwater       = Q;
  par->useEP = 0;
  par->rHelio       = 1.070000;
  par->radius		= 2e8;
  par->minScale	   	= rnuc;
  par->pIntensity    	= 50000;
  par->sinkPoints    	= 500;
  par->moldatfile[0] 	= "hcn.dat";
  par->girdatfile[0] 	= "g_hcn_1au_new4.dat";
  par->girScale      = 0.873439;
  par->colliScale = 1.000000;
  par->nSolveIters  = 1;
  par->outputfile 	= "HCN.pop";
  par->gridfile		= "grid.vtk";
  par->collPartIds[0]   = 1;
  par->nMolWeights[0]   = 1.0;
  par->xne = 1.0;

  img[0].velres		= 50.000000;   // Channel resolution in m/s
  img[0].nchan		= 50;	  // Number of channels
  img[0].trans		= 3;      // zero-indexed J quantum number
  img[0].pxls		= 769;	  // Pixels per dimension (odd number allows true central peak)
  img[0].imgres		= 0.1;	  // Resolution in arc seconds
  img[0].distance	= 1.000 * AU; // source distance in m
  img[0].unit		= 0;	  // 0:Kelvin 1:Jansky/pixel 2:SI 3:Lsun/pixel 4:tau
  img[0].filename	= "HCN_3_SUBLIMED.fits"; // Output filename

}

/******************************************************************************/


void
density(double x, double y, double z, double *density){

  double r;

  r=sqrt(x*x+y*y+z*z);

  if(r<rnuc)
    density[0] = 1e-20;
  else
    density[0] = Q/(4.*pi*pow(r, 2)*vexp)*exp(-r*betamain/vexp);
}

/******************************************************************************/

void
temperature(double x, double y, double z, double *temperature){
  temperature[0] = tkin;
}

/******************************************************************************/

void
molNumDensity(double x, double y, double z, double *nmol){

  double r;

  r=sqrt(x*x+y*y+z*z);

  if(r<rnuc)
    nmol[0] = 0.;
  else
    nmol[0] = abund * Q / (4.*pi*pow(r, 2)*vexp)*exp(-r*betamol/vexp);
}

/******************************************************************************/

void
doppler(double x, double y, double z, double *doppler){
  *doppler = 50.000000;
}

/******************************************************************************/

void
velocity(double x, double y, double z, double *vel){

  double phi, theta;

  theta=atan2(sqrt(x*x+y*y),z);
  phi=atan2(y,x);

  vel[0]=vexp*sin(theta)*cos(phi);
  vel[1]=vexp*sin(theta)*sin(phi);
  vel[2]=vexp*cos(theta);
} 
   