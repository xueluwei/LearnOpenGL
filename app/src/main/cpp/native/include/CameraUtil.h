#ifndef LEARNOPENGL_CAMERAUTIL_H
#define LEARNOPENGL_CAMERAUTIL_H

void matrixIdentityFunction(float* matrix);
void matrixMultiply(float* destination, float* operand1, float* operand2);
void matrixTranslate(float* matrix, float x, float y, float z);
void matrixRotateX(float* matrix, float angle);
void matrixRotateY(float* matrix, float angle);
void matrixRotateZ(float* matrix, float angle);
void matrixScale(float* matrix, float x, float y, float z);

void matrixPerspective(float* matrix, float fieldOfView, float aspectRatio, float zNear, float zFar);

#endif //LEARNOPENGL_CAMERAUTIL_H
