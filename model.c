#include "lime.h"

double Q1=6.2e+28;
double Q2=3.5e+28;
double abund = 0.2;
double tkin1 = 35.000000;
double tkin2 = 35.000000;
double vexp1 = 760.000000;
double vexp2 = 560.000000;
double rnuc = 2500.000000;
double openAngle = 1.0;

double betamain = 3.5e-06;
double betamol = 2.2e-07;

double pi = 3.14159265358979323846;

/******************************************************************************/

void
input(inputPars *par, image *img){
  par->useEP       = 0;
  par->Q1 = Q1;
  par->Q2 = Q2;
  par->openAngle = openAngle;
  par->rHelio       = 1.85;
  par->radius		= 1e9;
  par->minScale	   	= rnuc;
  par->pIntensity    	= 10000;
  par->sinkPoints = 500;
  par->moldatfile[0] 	= "co.dat";
  par->girdatfile[0] 	= "g_co_1au_new4.dat";
  par->girScale      = 0.292;
  par->colliScale = 1.000000;
  par->lte_only 	= 0;
  par->outputfile 	= "CO_NLTE.pop";
  par->binoutputfile 	= "restart.pop";
  par->gridfile		= "grid.vtk";
  par->collPartIds[0]   = 2;
  par->nMolWeights[0]   = 1.0;
  par->xne = 1.0;
  par->traceRayAlgorithm = 0;
  par->fixRNG = 1;

  img[0].velres		= 50.000000;   // Channel resolution in m/s
  img[0].nchan		= 50;	  // Number of channels
  img[0].trans		= 2;      // zero-indexed J quantum number
  img[0].pxls		= 512;	  // Pixels per dimension (odd number allows true central peak)
  img[0].imgres		= 0.5;	  // Resolution in arc seconds
  img[0].distance	= 1.47 * AU; // source distance in m
  img[0].unit		= 0;	  // 0:Kelvin 1:Jansky/pixel 2:SI 3:Lsun/pixel 4:tau
  img[0].filename	= "CO_example.fits"; // Output filename

}

/******************************************************************************/

void
density(double x, double y, double z, double *density){

  double r,b,angle;

  b = sqrt(x*x+y*y);
  angle = atan2(b,-z);

  const double rMin = rnuc; 

  r=sqrt(x*x+y*y+z*z);

  if(r<rMin)
      density[0] = 1e-20;
  else if(angle<openAngle)
    density[0] = Q1 /(4*PI*pow(r, 2)*vexp1)*exp(-r*betamain/vexp1);
  else
    density[0] = Q2 /(4*PI*pow(r, 2)*vexp2)*exp(-r*betamain/vexp2);
  
}

/******************************************************************************/

void
temperature(double x, double y, double z, double *temperature){

  double b,angle;

  b = sqrt(x*x+y*y);
  angle = atan2(b,-z);

  if(angle<openAngle){
    temperature[0] = tkin1;
  }
  else
    temperature[0] = tkin2;
}

/******************************************************************************/

void
molNumDensity(double x, double y, double z, double *nmol){

  double r,b,angle;

  b = sqrt(x*x+y*y);
  angle = atan2(b,-z);

  const double rMin = rnuc;

  r=sqrt(x*x+y*y+z*z);

  if(r<rMin)
    nmol[0] = 0.;
  else if(angle<openAngle)
    nmol[0] = abund * Q1/(4*PI*pow(r, 2)*vexp1)*exp(-r*betamol/vexp1);
  else
    nmol[0] = abund * Q2/(4*PI*pow(r, 2)*vexp2)*exp(-r*betamol/vexp2);
}

/******************************************************************************/

void
doppler(double x, double y, double z, double *doppler){
  *doppler = 0.0;
}

/******************************************************************************/

void
velocity(double x, double y, double z, double *vel){

  double phi, theta,b,angle;

  b = sqrt(x*x+y*y);
  angle = atan2(b,-z);

  theta=atan2(sqrt(x*x+y*y),z);
  phi=atan2(y,x);

  if(angle<openAngle){
    vel[0]=vexp1*sin(theta)*cos(phi);
    vel[1]=vexp1*sin(theta)*sin(phi);
    vel[2]=vexp1*cos(theta);

  }
  else{
    vel[0]=vexp2*sin(theta)*cos(phi);
    vel[1]=vexp2*sin(theta)*sin(phi);
    vel[2]=vexp2*cos(theta);
  }
}
   