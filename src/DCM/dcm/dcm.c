/*
 * dcm.c
 *
 *  Created on: 25-Jun-2012
 *      Author: riya
 */

#include <math.h>

#include "pluto.h"

#if PLUTO_USE_DCM
/*
Output variables are:
  DcmEst[0..2] which are the direction cosine of the X,Y,Z axis
*/

/* maximum accelerometer weight in accelerometer-gyro fusion
 * formula this value is tuned-up experimentally:
 * if you get too much noise - decrease it if you get a delayed
 * response of the filtered values - increase it starting with
 * a value of  0.01 .. 0.05 will work for most sensors */

/*
 ******************************************************************************
 * DEFINES
 ******************************************************************************
 */

/* maximum accelerometer error relative to 1g ,
 * when error exceeds this value accelerometer weight becomes 0
 * this helps reject external accelerations (non-gravitational
 * inertial forces caused by device acceleration) */
#define ACC_ERR_MAX 0.3f

/* maximum magnetometer error relative to normal value of 1
 * (corresponding to earth's magnetic field) when error exceeds this
 * value magnetometer weight becomes 0 this helps reject magnetic
 * forces that are not attributed to earth's magnetic field */
#define MAG_ERR_MAX 0.2f

/*
 ******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************
 */

/* modulus of magnetic flux */
static float mag_modulus = 0;

/* accelerometer data weight relative to gyro's weight of 1 */
static float accweight = 0;

/* magnetometer data weight relative to gyro's weight of 1. */
static float magweight = 0;

static uint32_t imu_step = 0;

/*
 *******************************************************************************
 *******************************************************************************
 * LOCAL FUNCTIONS
 *******************************************************************************
 *******************************************************************************
 */

/* bring dcm matrix in order - adjust values to make
 * orthonormal (or at least closer to orthonormal) */
void dcm_orthonormalize(float dcm[3][3]){
  //err = X . Y ,  X = X - err/2 * Y , Y = Y - err/2 * X  (DCMDraft2 Eqn.19)
  float err = vector3d_dot((float*)(dcm[0]),(float*)(dcm[1]));
  float delta[2][3];
  vector3d_scale(-err/2,(float*)(dcm[1]),(float*)(delta[0]));
  vector3d_scale(-err/2,(float*)(dcm[0]),(float*)(delta[1]));
  vector3d_add((float*)(dcm[0]),(float*)(delta[0]),(float*)(dcm[0]));
  vector3d_add((float*)(dcm[1]),(float*)(delta[1]),(float*)(dcm[1]));

  //Z = X x Y  (DCMDraft2 Eqn. 20) ,
  vector3d_cross((float*)(dcm[0]),(float*)(dcm[1]),(float*)(dcm[2]));
  //re-nomralization
  vector3d_normalize((float*)(dcm[0]));
  vector3d_normalize((float*)(dcm[1]));
  vector3d_normalize((float*)(dcm[2]));
}


//rotate DCM matrix by a small rotation given by angular rotation vector w
//see http://gentlenav.googlecode.com/files/DCMDraft2.pdf
void dcm_rotate(float dcm[3][3], float w[3]){
  //float W[3][3];
  //creates equivalent skew symetric matrix plus identity matrix
  //vector3d_skew_plus_identity((float*)w,(float*)W);
  //float dcmTmp[3][3];
  //matrix_multiply(3,3,3,(float*)W,(float*)dcm,(float*)dcmTmp);

  uint32_t i;
  float dR[3];
  //update matrix using formula R(t+1)= R(t) + dR(t) = R(t) + w x R(t)
  for(i=0;i<3;i++){
    vector3d_cross(w, dcm[i], dR);
    vector3d_add(dcm[i], dR, dcm[i]);
  }

  //make matrix orthonormal again
  dcm_orthonormalize(dcm);
}

/*
 *******************************************************************************
 * EXPORTED FUNCTIONS
 *******************************************************************************
 */

/* accelerations in g (scale does not matter because values will be normalized),
 * angular rates in rad/s (scale is MATTER),
 * magnetic flux in uT (scale does not matter because values will be normalized),
 * time in s */
void dcmUpdate(float dcmEst[3][3], float xacc, float yacc, float zacc, float xgyro, float ygyro, float zgyro, float xmag, float ymag, float zmag, float imu_interval)
{
  uint32_t i;
  float Kacc[3];  //K(b) vector according to accelerometer in body's coordinates
  float Imag[3];  //I(b) vector accordng to magnetometer in body's coordinates
  accweight = 0.01 ;
  magweight = 0.01 ;

  //---------------
  // I,J,K unity vectors of global coordinate system I-North,J-West,K-zenith
  // i,j,k unity vectors of body's coordiante system  i-"nose", j-"left wing", k-"top"
  //---------------
  //        [I.i , I.j, I.k]
  // DCM =  [J.i , J.j, J.k]
  //        [K.i , K.j, K.k]

  //---------------
  //Acelerometer
  //---------------
  /* Accelerometer measures gravity vector G in body coordinate system
  Gravity vector is the reverse of K unity vector of global system
  expressed in local coordinates K vector coincides with the z
  coordinate of body's i,j,k vectors expressed in global
  coordinates (K.i , K.j, K.k)

  Acc can estimate global K vector(zenith) measured in body's coordinate
  systems (the reverse of gravitation vector) */

  Kacc[0] = xacc;
  Kacc[1] = yacc;
  Kacc[2] = zacc;
  vector3d_normalize(Kacc);

  //calculate correction vector to bring dcmEst's K vector closer to Acc
  // vector (K vector according to accelerometer)
  float wA[3];
  // wA = Kgyro x  Kacc , rotation needed to bring Kacc to Kgyro
  vector3d_cross(dcmEst[2], Kacc, wA);

  //---------------
  //Magnetomer
  //---------------
  //calculate correction vector to bring dcmEst's I vector closer
  // to Mag vector (I vector according to magnetometer)
  float wM[3];
  //in the absence of magnetometer let's assume North vector (I) is
  // always in XZ plane of the device (y coordinate is 0)
  //    Imag[0] = sqrtf(1 - dcmEst[0][2] * dcmEst[0][2]);
  //    Imag[1] = 0;
  //    Imag[2] = dcmEst[0][2];

  Imag[0] = xmag;
  Imag[1] = ymag;
  Imag[2] = zmag;

  /* get length of vector if not yet present */
  if (mag_modulus == 0)
    mag_modulus = vector3d_modulus(Imag);

  /* ignore magnetometer readings if external magnetic field too strong */
  if(((vector3d_modulus(Imag) / mag_modulus) - 1) < MAG_ERR_MAX){
    float tmpM[3];
    vector3d_normalize(Imag);
    vector3d_cross(Kacc, Imag, tmpM);
    vector3d_normalize(tmpM);
    vector3d_cross(tmpM, Kacc, Imag);
    vector3d_normalize(Imag);
   // wM = Igyro x Imag, roation needed to bring Imag to Igyro
    vector3d_cross(dcmEst[0], Imag, wM);
  }
  else{
    wM[0] = 0.0;
    wM[1] = 0.0;
    wM[2] = 0.0;
  }

  //---------------
  //dcmEst
  //---------------
  //gyro rate direction is usually specified (in datasheets) as the device's(body's) rotation
  //about a fixed earth's (global) frame, if we look from the perspective of device then
  //the global vectors (I,K,J) rotation direction will be the inverse
  float w[3];     //gyro rates (angular velocity of a global vector in local coordinates)
  w[0] = -xgyro;  //rotation rate about accelerometer's X axis (GY output)
  w[1] = -ygyro;  //rotation rate about accelerometer's Y axis (GX output)
  w[2] = -zgyro;  //rotation rate about accelerometer's Z axis (GZ output)
  if (imu_step < 200){
    /* speedup magnetic course obtaining */
    for(i=0;i<3;i++){
      w[i] *= imu_interval;  //scale by elapsed time to get angle in radians
      w[i] = (float)(w[i] + 0.01*wA[i] + 0.1f*wM[i]) / (1.0f + 0.01 + 0.1f);
    }
  }
  else{
    /* normal mode */
    for(i=0;i<3;i++){
      w[i] *= imu_interval;  //scale by elapsed time to get angle in radians
      //compute weighted average with the accelerometer correction vector
      w[i] = (float)(w[i] + 0.01*wA[i] + 0.01*wM[i]) / (1.0f + 0.01 + 0.01);
    }
  }
  dcm_rotate(dcmEst, w);
  imu_step++;
}

//-------------------------------------------------------------------
// imu_init
//-------------------------------------------------------------------
void dcmInit(){
	magweight = 0.01 ;
	accweight = 0.01 ;
}
#endif /*PLUTO_USE_DCM */
