#include "vtkMatrix3x3.h"

class SensorData
{
public:
	double x;
	double y;
	double z;

	double azimuth;
	double elevation;
	double roll;
};

void errorHandler(int error);
void InitializeTrakStar();
void SetAlignment(double *angle_adjust);
void GetSensorData();
void GetSensorData(double *trakStar_x, double *trakStar_y, double *trakStar_z, double *trakStar_azimuth, double *trakStar_elevation, double* trakStar_roll);
void GetSensorPosition(double *trakStar_x, double *trakStar_y, double *trakStar_z);
void GetSensorOrientation(double *trakStar_azimuth, double *trakStar_elevation, double* trakStar_roll);
void GetSensorOrientation(vtkMatrix3x3* matrix);
void GetSensorOrientation(double *trakStar_x, double *trakStar_y, double *trakStar_z, vtkMatrix3x3* matrix);
void DeinitializeTrakStar();

