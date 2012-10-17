#ifndef __VECTOR_3D_H__
#define __VECTOR_3D_H__

#include "math.h"

/*
ABOUT: 3D Vector and rotation functions. Rotations about X, Y, Z, and any arbitruary Vector
AUTHOR: Kenny Cason
WEBSITE: Ken-Soft.com
EMAIL: kenneth.cason@gmail.com
DATE: 11-19-2009
*/



// -----------------------------------------------------------------------------

//Vector3D rotateX(Vector3D vector, double theta);
//Vector3D rotateY(Vector3D vector, double theta);
//Vector3D rotateZ(Vector3D vector, double theta);


// -----------------------------------------------------------------------------

/**
  * [(1      ,0      ,0),
  *  (0      ,cos(a) ,-sin(a)),
  *  (0      ,sin(a) ,cos(a))]
  * @param Vector3D
  * @param theta
  */
Vector3D rotateX(vector3D vector, double theta) {
    Vector3D newv;
    newv.x = vector.x;
    newv.y = vector.y * cos(theta) + vector.z * -sin(theta);
    newv.z = vector.y * sin(theta) + vector.z * cos(theta);
    return newv;
}

/**
  * [(cos(a) ,0      ,sin(a)),
  *  (0      ,1      ,0),
  *  (-sin(a),0      ,cos(a))]
  * @param Vector3D
  * @param theta
  */
Vector3D rotateY(Vector3D vector, double theta) {
    Vector3D newv;
    newv.x = vector.x * cos(theta) + vector.z * sin(theta);
    newv.y = vector.y;
    newv.z = vector.x * -sin(theta) + vector.z * cos(theta);
    return newv;
}

/**
  * [(cos(a),-sin(a),0),
  *  (sin(a),cos(a) ,0),
  *  (0     ,0      ,1)]
  * @param Vector3D
  * @param theta
  */
Vector3D rotateZ(Vector3D vector, double theta) {
    Vector3D newv;
    newv.x = vector.x * cos(theta) + vector.y * -sin(theta);
    newv.y = vector.x * sin(theta) + vector.y * cos(theta);
    newv.z = vector.z;
    return newv;
}


#endif
