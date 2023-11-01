#include <cstdlib>

// 用于定义恒等函数，恒等函数是为了初始化时所使用的矩阵与该矩阵相乘结果为其本身（不会平移、旋转或缩放）。
// 数学计算中，我们习惯设置矩阵是横向摆放数据，OpenGL中是竖向的所以下面的矩阵数学表示为：
//   1.0f, 0.0f, 0.0f, 0.0f # x = 0
//   0.0f, 1.0f, 0.0f, 0.0f # y = 0
//   0.0f, 0.0f, 1.0f, 0.0f # z = 0
//   0.0f, 0.0f, 0.0f, 1.0f # w = 1 //多出来的一维，为了上面三个纬度描述矩阵运动而凑整
// 如果不习惯OpenGL的摆放方式，可以自己做一下矩阵转换。
void matrixIdentityFunction(float* matrix)
{
    if(matrix == NULL)
    {
        return;
    }
    matrix[0] = 1.0f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = 1.0f;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;
    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = 1.0f;
    matrix[11] = 0.0f;
    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
}

// 4x4矩阵乘法的实现（这里为什么三维的矩阵也有第4列呢？是因为无值默认给0了）
// 两个for循环遍历1的每一行，然后遍历2的每一列，相乘后相加，并把结果放到结果矩阵相应位置中。
// 值得注意的是为什么我们新建了个缓存矩阵来存计算的结果，这是因为所有传入的参数都是指针，
// 如果传入的destination指针跟operand1或operand2指针相同时，我们直接修改其内容，会导致不可预期的结果。
void matrixMultiply(float* destination, float* operand1, float* operand2)
{
    float theResult[16];
    int row, column = 0;
    int i,j = 0;
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            theResult[4 * i + j] = operand1[j] * operand2[4 * i] + operand1[4 + j] * operand2[4 * i + 1] +
                                   operand1[8 + j] * operand2[4 * i + 2] + operand1[12 + j] * operand2[4 * i + 3];
        }
    }
    for(int i = 0; i < 16; i++)
    {
        destination[i] = theResult[i];
    }
}

// 矩阵移动，指的是使用最后的一列来处理x、y、z轴的移动，12、13、14分别代表x、y、z轴的移动。
// 需要注意的是我们为何先创建缓存矩阵并把它设置为定义的矩阵，我们这样做是为了避免污染已经传入方法的矩阵。
// 然后我们往缓存矩阵中添加新的转换值并用矩阵乘法转换成我们当前矩阵。
void matrixTranslate(float* matrix, float x, float y, float z)
{
    float temporaryMatrix[16];
    matrixIdentityFunction(temporaryMatrix);
    temporaryMatrix[12] = x;
    temporaryMatrix[13] = y;
    temporaryMatrix[14] = z;
    matrixMultiply(matrix,temporaryMatrix,matrix);
}

// 矩阵缩放，这是另一个简单的矩阵转换操作，0、5、10分别代表x、y、z轴的缩放。
void matrixScale(float* matrix, float x, float y, float z)
{
    float tempMatrix[16];
    matrixIdentityFunction(tempMatrix);
    tempMatrix[0] = x;
    tempMatrix[5] = y;
    tempMatrix[10] = z;
    matrixMultiply(matrix, tempMatrix, matrix);
}

// 矩阵旋转会显得更复杂一点，包含了三角学的知识，三角函数的使用取决于你想围绕哪个轴进行旋转。
// 以下是分别围绕x、y、z轴旋转的方法。
float matrixDegreesToRadians(float degrees) {
    return M_PI * degrees / 180.0f;
}
void matrixRotateX(float* matrix, float angle)
{
    float tempMatrix[16];
    matrixIdentityFunction(tempMatrix);
    tempMatrix[5] = cos(matrixDegreesToRadians(angle));
    tempMatrix[9] = -sin(matrixDegreesToRadians(angle));
    tempMatrix[6] = sin(matrixDegreesToRadians(angle));
    tempMatrix[10] = cos(matrixDegreesToRadians(angle));
    matrixMultiply(matrix, tempMatrix, matrix);
}
void matrixRotateY(float *matrix, float angle)
{
    float tempMatrix[16];
    matrixIdentityFunction(tempMatrix);
    tempMatrix[0] = cos(matrixDegreesToRadians(angle));
    tempMatrix[8] = sin(matrixDegreesToRadians(angle));
    tempMatrix[2] = -sin(matrixDegreesToRadians(angle));
    tempMatrix[10] = cos(matrixDegreesToRadians(angle));
    matrixMultiply(matrix, tempMatrix, matrix);
}
void matrixRotateZ(float *matrix, float angle)
{
    float tempMatrix[16];
    matrixIdentityFunction(tempMatrix);
    tempMatrix[0] = cos(matrixDegreesToRadians(angle));
    tempMatrix[4] = -sin(matrixDegreesToRadians(angle));
    tempMatrix[1] = sin(matrixDegreesToRadians(angle));
    tempMatrix[5] = cos(matrixDegreesToRadians(angle));
    matrixMultiply(matrix, tempMatrix, matrix);
}

// 矩阵视角投影的具体实现
void matrixFrustum(float* matrix, float left, float right, float bottom, float top, float zNear, float zFar)
{
    float temp, xDistance, yDistance, zDistance;
    temp = 2.0 * zNear;
    xDistance = right - left;
    yDistance = top - bottom;
    zDistance = zFar - zNear;
    matrixIdentityFunction(matrix);
    matrix[0] = temp / xDistance;
    matrix[5] = temp / yDistance;
    matrix[8] = (right + left) / xDistance;
    matrix[9] = (top + bottom) / yDistance;
    matrix[10] = (-zFar - zNear) / zDistance;
    matrix[11] = -1.0f;
    matrix[14] = (-temp * zFar) / zDistance;
    matrix[15] = 0.0f;
}

/**
 * 矩阵视角设置，做出近大远小的效果
 * @param matrix 要做投影的矩阵
 * @param fieldOfView 看视图的角度。
 * @param aspectRatio 宽高比，通常是你屏幕或显示表面的宽/高，为了适配各种设备而给的参数。
 * @param zNear 近平面，表示对象在消失和被裁剪前离相机有多近。
 * @param zFar 远平面，表示对象在不再绘制前离相机有多远。
 */
void matrixPerspective(float* matrix, float fieldOfView, float aspectRatio, float zNear, float zFar)
{
    float ymax, xmax;
    ymax = zNear * tanf(fieldOfView * M_PI / 360.0f);
    xmax = ymax * aspectRatio;
    matrixFrustum(matrix, -xmax, xmax, -ymax, ymax, zNear, zFar);
}
