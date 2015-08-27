/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    $RCSfile: ImageViewer.cxx,v $
Language:  C++
Date:      $Date: 2008-05-10 19:56:09 $
Version:   $Revision: 1.4 $

Copyright (c) 2002 Insight Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <iostream>

//Begin: Includes for VTK Viewer
#include "ImageSlicing.h"
#include "ImageViewer.h"
#include "itkCommand.h"
#include "itkImage.h"
#include "itkVTKImageExport.h"
#include "itkVTKImageImport.h"
#include "itkCurvatureFlowImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkRGBPixel.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkGDCMImageIO.h"
#include "vtkImageData.h"

#include "vtkImageImport.h"
#include "vtkImageExport.h"
#include "vtkImageActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h" 
#include "vtkImageReader2.h"
#include "vtkMatrix4x4.h"
#include "vtkImageReslice.h"
#include "vtkImageViewer2.h"
#include "vtkSmartPointer.h"
//End include for VTK Image Viewer

#include <itkImage.h>
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkExtractImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkNearestNeighborInterpolateImageFunction.h"

#include "..\GetSynchronousRecord Sample\GetSynchronousRecordSample.h"

#include "..\GetSynchronousRecord Sample\StdAfx.h"

using namespace std;

typedef itk::Image< float, 3 > ImageType;
typedef itk::ImageFileReader< ImageType > VolumeReaderType;
VolumeReaderType::Pointer vol_reader;

const char * inputFilename  = "C:\\VETS Source Code\\VETS 3D\\DICOM\\Diastole.dcm";
const char * outputFilename = "C:\\VETS Source Code\\VETS 3D\\DICOM\\Diastole_single_plane.dcm";

int index = 0;
int sliceNumber = 0;
int option = 0;

//Start ResampleImageFilter code
const     unsigned int   Dimension = 3;
typedef   unsigned char  InputPixelType;
typedef   unsigned char  OutputPixelType;
typedef itk::Image< InputPixelType,  Dimension >   InputImageType;
typedef itk::Image< OutputPixelType, Dimension >   OutputImageType;

typedef itk::ImageFileReader< InputImageType  >  ReaderType;
typedef itk::ImageFileWriter< OutputImageType >  WriterType;

typedef itk::ResampleImageFilter<InputImageType,OutputImageType> FilterType;
typedef itk::NearestNeighborInterpolateImageFunction< InputImageType, double >  InterpolatorType;

ReaderType::Pointer reader;
WriterType::Pointer writer;
FilterType::Pointer filter;
InterpolatorType::Pointer interpolator;

int errorCode;

CSensor			*pSensor;
CXmtr			*pXmtr;
int				i;
int				sensorID;
int				numberBytes;
clock_t			goal;
clock_t			wait;

double x;
double y;
double z;

double azimuth;
double elevation;
double roll;


void errorHandler(int error)
{
	char			buffer[1024];
	char			*pBuffer = &buffer[0];
	int				numberBytes;

	while(error!=BIRD_ERROR_SUCCESS)
	{
		error = GetErrorText(error, pBuffer, sizeof(buffer), SIMPLE_MESSAGE);
		numberBytes = strlen(buffer);
		buffer[numberBytes] = '\n';		// append a newline to buffer
		printf("%s", buffer);
	}
}
// modulu - similar to matlab's mod()
// result is always possitive. not similar to fmod()
// Mod(-3,4)= 1   fmod(-3,4)= -3
double inline __fastcall Mod(double x, double y)
{
    if (0 == y)
        return x;

    return x - y * floor(x/y);
}

// wrap [rad] angle to [-PI..PI)
//inline double WrapPosNegPI(double fAng)
//{
//	return Mod(fAng + Math::pi, 2*Math::pi) - Math::pi;
//}

// wrap [rad] angle to [0..Math::pi)
inline double WrapPI(double fAng)
{
    return Mod(fAng, 180);
}

// wrap [rad] angle to [0..TWOMath::pi)
inline double WrapTwoPI(double fAng)
{
    return Mod(fAng, 360);
}

// wrap [deg] angle to [-180..180)
inline double WrapPosNeg180(double fAng)
{
    return Mod(fAng + 180., 360.) - 180.;
}

// wrap [deg] angle to [0..360)
inline double Wrap360(double fAng)
{
    return Mod(fAng ,360.);
}

void SetAlignment(double *angle_adjust)
{	
	if(option == 0){
	DOUBLE_ANGLES_RECORD align_angles, *p_record = &align_angles;
	align_angles.a = angle_adjust[0];
	align_angles.e = angle_adjust[1];
	align_angles.r = angle_adjust[2];
		
	errorCode = SetSensorParameter(0, ANGLE_ALIGN, p_record, sizeof(align_angles));
	if(errorCode!=BIRD_ERROR_SUCCESS) errorHandler(errorCode);
	}

	else {
	DOUBLE_ANGLES_RECORD align_angles, *p_record = &align_angles;
	align_angles.a = angle_adjust[0];
	align_angles.e = angle_adjust[1];
	align_angles.r = angle_adjust[2];
		
	errorCode = SetTransmitterParameter(0, REFERENCE_FRAME, p_record, sizeof(align_angles));
	if(errorCode!=BIRD_ERROR_SUCCESS) errorHandler(errorCode);
	}
}

void InitializeTrakStar()
{

	short id;
	CSystem	ATC3DG;

	wait = 1; 	// 10 ms delay

	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//
	// Initialize the ATC3DG driver and DLL
	//
	// It is always necessary to first initialize the ATC3DG "system". By
	// "system" we mean the set of ATC3DG cards installed in the PC. All cards
	// will be initialized by a single call to InitializeBIRDSystem(). This
	// call will first invoke a hardware reset of each board. If at any time 
	// during operation of the system an unrecoverable error occurs then the 
	// first course of action should be to attempt to Recall InitializeBIRDSystem()
	// if this doesn't restore normal operating conditions there is probably a
	// permanent failure - contact tech support.
	// A call to InitializeBIRDSystem() does not return any information.
	//
	errorCode = InitializeBIRDSystem();
	if(errorCode!=BIRD_ERROR_SUCCESS) errorHandler(errorCode);

	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//
	// GET SYSTEM CONFIGURATION
	//
	// In order to get information about the system we have to make a call to
	// GetBIRDSystemConfiguration(). This call will fill a fixed size structure
	// containing amongst other things the number of boards detected and the
	// number of sensors and transmitters the system can support (Note: This
	// does not mean that all sensors and transmitters that can be supported
	// are physically attached)
	//
	errorCode = GetBIRDSystemConfiguration(&ATC3DG.m_config);
	if(errorCode!=BIRD_ERROR_SUCCESS) errorHandler(errorCode);

	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//
	// GET SENSOR CONFIGURATION
	//
	// Having determined how many sensors can be supported we can dynamically
	// allocate storage for the information about each sensor.
	// This information is acquired through a call to GetSensorConfiguration()
	// This call will fill a fixed size structure containing amongst other things
	// a status which indicates whether a physical sensor is attached to this
	// sensor port or not.
	//
	pSensor = new CSensor[ATC3DG.m_config.numberSensors];
	for(i=0;i<ATC3DG.m_config.numberSensors;i++)
	{
		errorCode = GetSensorConfiguration(i, &(pSensor+i)->m_config);
		if(errorCode!=BIRD_ERROR_SUCCESS) errorHandler(errorCode);
	}

	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//
	// GET TRANSMITTER CONFIGURATION
	//
	// The call to GetTransmitterConfiguration() performs a similar task to the 
	// GetSensorConfiguration() call. It also returns a status in the filled
	// structure which indicates whether a transmitter is attached to this
	// port or not. In a single transmitter system it is only necessary to 
	// find where that transmitter is in order to turn it on and use it.
	//
	pXmtr = new CXmtr[ATC3DG.m_config.numberTransmitters];
	for(i=0;i<ATC3DG.m_config.numberTransmitters;i++)
	{
		errorCode = GetTransmitterConfiguration(i, &(pXmtr+i)->m_config);
		if(errorCode!=BIRD_ERROR_SUCCESS) errorHandler(errorCode);
	}

	//
	// MEASUREMENT_RATE
	//
	{
		double buffer = 40.0;			// only values between 20.0 and 120.0 are legal
		double *pBuffer = &buffer;
		errorCode = SetSystemParameter(MEASUREMENT_RATE, pBuffer, sizeof(buffer));
		if(errorCode!=BIRD_ERROR_SUCCESS) errorHandler(errorCode);
	}

	/*DOUBLE_POSITION_RECORD offset = {1, 0, 0};

	SET_SENSOR_PARAMETER(sensorID, DATA_FORMAT, DOUBLE_POSITION_ANGLES);
	SET_SENSOR_PARAMETER(0, SENSOR_OFFSET, DOUBLE_POSITION_RECORD);*/

	//
	// SENSOR_OFFSET
	//
	{
		//double buffer = 25.0;			// only values between 20.0 and 120.0 are legal
		DOUBLE_POSITION_RECORD buffer = {1.2, 0, 0};
		DOUBLE_POSITION_RECORD *pBuffer = &buffer;
		errorCode = SetSensorParameter(0, SENSOR_OFFSET, pBuffer, sizeof(buffer));
		if(errorCode!=BIRD_ERROR_SUCCESS) errorHandler(errorCode);
	}

	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//
	// Search for the first attached transmitter and turn it on
	//
	for(id=0;id<ATC3DG.m_config.numberTransmitters;id++)
	{
		if((pXmtr+id)->m_config.attached)
		{
			// Transmitter selection is a system function.
			// Using the SELECT_TRANSMITTER parameter we send the id of the
			// transmitter that we want to run with the SetSystemParameter() call
			errorCode = SetSystemParameter(SELECT_TRANSMITTER, &id, sizeof(id));
			if(errorCode!=BIRD_ERROR_SUCCESS) errorHandler(errorCode);
			break;
		}
	}




}

void GetSensorData()//double ^x, double ^y, double ^z, double ^azimuth, double ^elevation, double ^roll)
{
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//
	// Collect data from all birds
	// Loop through all sensors and get a data record if the sensor is attached.
	// Print result to screen
	// Note: The default data format is DOUBLE_POSITION_ANGLES. We can use this
	// format without first setting it.
	// 
	//
	DOUBLE_POSITION_ANGLES_RECORD record, *pRecord = &record;

	// Set up time delay for first loop
	// It only makes sense to request a data record from the sensor once per
	// measurement cycle. Therefore we set up a 10ms loop and request a record
	// only after at least 10ms have elapsed.
	//
	goal = wait + clock();

	// collect as many records as specified in the command line
	//for(i=0;i<10000;i++)
	//{
	// delay 10ms between collecting data
	// wait till time delay expires
	while(goal>clock());
	// set up time delay for next loop
	goal = wait + clock();

	// scan the sensors and request a record
	//for(sensorID=0;sensorID<ATC3DG.m_config.numberSensors;sensorID++)
	//{
	// sensor attached so get record
	errorCode = GetAsynchronousRecord(/*sensorID*/ 0, pRecord, sizeof(record));
	if(errorCode!=BIRD_ERROR_SUCCESS) {errorHandler(errorCode);}

	// get the status of the last data record
	// only report the data if everything is okay
	unsigned int status = GetSensorStatus(/*sensorID*/ 0);

	if( status == VALID_STATUS)
	{
		x = record.x;
		y = record.y;
		z = record.z;

		azimuth = record.a;
		elevation = record.e;
		roll = record.r;

		std::cout<<"(X, Y, Z, Azimuth, Elevation, Roll) = "<<x<<", "<<y<<", "<<z<<","<<azimuth<< ", "<<elevation<<", "<<roll<<")"<<std::endl;
	}
}

void GetSensorData(double *trakStar_x, double *trakStar_y, double *trakStar_z, double *trakStar_azimuth, double *trakStar_elevation, double* trakStar_roll)
{
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//
	// Collect data from all birds
	// Loop through all sensors and get a data record if the sensor is attached.
	// Print result to screen
	// Note: The default data format is DOUBLE_POSITION_ANGLES. We can use this
	// format without first setting it.
	// 
	//
	DOUBLE_POSITION_ANGLES_RECORD record, *pRecord = &record;

	// Set up time delay for first loop
	// It only makes sense to request a data record from the sensor once per
	// measurement cycle. Therefore we set up a 10ms loop and request a record
	// only after at least 10ms have elapsed.
	//
	goal = wait + clock();

	// collect as many records as specified in the command line
	//for(i=0;i<10000;i++)
	//{
	// delay 10ms between collecting data
	// wait till time delay expires
	while(goal>clock());
	// set up time delay for next loop
	goal = wait + clock();

	// scan the sensors and request a record
	//for(sensorID=0;sensorID<ATC3DG.m_config.numberSensors;sensorID++)
	//{
	// sensor attached so get record
	errorCode = GetAsynchronousRecord(/*sensorID*/ 0, pRecord, sizeof(record));
	if(errorCode!=BIRD_ERROR_SUCCESS) {errorHandler(errorCode);}

	// get the status of the last data record
	// only report the data if everything is okay
	unsigned int status = GetSensorStatus(/*sensorID*/ 0);

	if( status == VALID_STATUS)
	{
		*trakStar_x = record.x;
		*trakStar_y = record.y;
		*trakStar_z = record.z;
				
		*trakStar_azimuth = (record.a);//(-180:180)
		*trakStar_elevation = (record.e);//(-90:90)
		*trakStar_roll = (record.r);//(-180:180)
	}
}

void GetSensorPosition(double *trakStar_x, double *trakStar_y, double *trakStar_z)
{
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//
	// Collect data from all birds
	// Loop through all sensors and get a data record if the sensor is attached.
	// Print result to screen
	// Note: The default data format is DOUBLE_POSITION_ANGLES. We can use this
	// format without first setting it.
	// 
	//
	DOUBLE_POSITION_ANGLES_RECORD record, *pRecord = &record;

	// Set up time delay for first loop
	// It only makes sense to request a data record from the sensor once per
	// measurement cycle. Therefore we set up a 10ms loop and request a record
	// only after at least 10ms have elapsed.
	//
	goal = wait + clock();

	// collect as many records as specified in the command line
	//for(i=0;i<10000;i++)
	//{
	// delay 10ms between collecting data
	// wait till time delay expires
	while(goal>clock());
	// set up time delay for next loop
	goal = wait + clock();

	DATA_FORMAT_TYPE type = DOUBLE_POSITION_ANGLES;
	errorCode = SetSensorParameter(0,DATA_FORMAT,&type,sizeof(type));
	if(errorCode!=BIRD_ERROR_SUCCESS) 
		errorHandler(errorCode);

	// scan the sensors and request a record
	//for(sensorID=0;sensorID<ATC3DG.m_config.numberSensors;sensorID++)
	//{
	// sensor attached so get record
	errorCode = GetAsynchronousRecord(/*sensorID*/ 0, pRecord, sizeof(record));
	if(errorCode!=BIRD_ERROR_SUCCESS) {errorHandler(errorCode);}

	// get the status of the last data record
	// only report the data if everything is okay
	unsigned int status = GetSensorStatus(/*sensorID*/ 0);

	if( status == VALID_STATUS)
	{
		*trakStar_x = record.x;
		*trakStar_y = record.y;
		*trakStar_z = record.z;
	}
}

void GetSensorOrientation(double *trakStar_azimuth, double *trakStar_elevation, double* trakStar_roll)
{
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//
	// Collect data from all birds
	// Loop through all sensors and get a data record if the sensor is attached.
	// Print result to screen
	// Note: The default data format is DOUBLE_POSITION_ANGLES. We can use this
	// format without first setting it.
	// 
	//
	DOUBLE_POSITION_ANGLES_RECORD record, *pRecord = &record;

	// Set up time delay for first loop
	// It only makes sense to request a data record from the sensor once per
	// measurement cycle. Therefore we set up a 10ms loop and request a record
	// only after at least 10ms have elapsed.
	//
	goal = wait + clock();

	// collect as many records as specified in the command line
	//for(i=0;i<10000;i++)
	//{
	// delay 10ms between collecting data
	// wait till time delay expires
	while(goal>clock());
	// set up time delay for next loop
	goal = wait + clock();

	DATA_FORMAT_TYPE type = DOUBLE_POSITION_ANGLES;
	errorCode = SetSensorParameter(0,DATA_FORMAT,&type,sizeof(type));
	if(errorCode!=BIRD_ERROR_SUCCESS) 
		errorHandler(errorCode);

	// scan the sensors and request a record
	//for(sensorID=0;sensorID<ATC3DG.m_config.numberSensors;sensorID++)
	//{
	// sensor attached so get record
	errorCode = GetAsynchronousRecord(/*sensorID*/ 0, pRecord, sizeof(record));
	if(errorCode!=BIRD_ERROR_SUCCESS) {errorHandler(errorCode);}

	// get the status of the last data record
	// only report the data if everything is okay
	unsigned int status = GetSensorStatus(/*sensorID*/ 0);

	if( status == VALID_STATUS)
	{		
		*trakStar_azimuth = (record.a);//(-180:180)
		*trakStar_elevation = (record.e);//(-90:90)
		*trakStar_roll = (record.r);//(-180:180)
	}
}

void GetSensorOrientation(vtkMatrix3x3* matrix)
{
	DOUBLE_MATRIX_RECORD record, *pRecord = &record;

	goal = wait + clock();
	while(goal>clock());
	goal = wait + clock();

	DATA_FORMAT_TYPE type = DOUBLE_MATRIX;
	errorCode = SetSensorParameter(0,DATA_FORMAT,&type,sizeof(type));
	if(errorCode!=BIRD_ERROR_SUCCESS) 
		errorHandler(errorCode);

	errorCode = GetAsynchronousRecord(0, pRecord, sizeof(record));
	if (errorCode != BIRD_ERROR_SUCCESS) 
		errorHandler(errorCode);

	unsigned int status = GetSensorStatus(0);

	if( status == VALID_STATUS)
	{

		matrix->SetElement(0, 0, record.s[0][0]); matrix->SetElement(0, 1, record.s[0][1]);	matrix->SetElement(0, 2, record.s[0][2]);

		matrix->SetElement(1, 0, record.s[2][0]); matrix->SetElement(1, 1, record.s[2][1]); matrix->SetElement(1, 2, record.s[2][2]);

		matrix->SetElement(2, 0, record.s[1][0]); matrix->SetElement(2, 1, record.s[1][1]);	matrix->SetElement(2, 2, record.s[1][2]);		

		matrix->Transpose();
	}
}

void GetSensorOrientation(double *trakStar_x, double *trakStar_y, double *trakStar_z, vtkMatrix3x3* matrix)
{
	DOUBLE_MATRIX_RECORD record, *pRecord = &record;

	goal = wait + clock();
	while(goal>clock());
	goal = wait + clock();

	DATA_FORMAT_TYPE type = DOUBLE_MATRIX;
	errorCode = SetSensorParameter(0,DATA_FORMAT,&type,sizeof(type));
	if(errorCode!=BIRD_ERROR_SUCCESS) 
		errorHandler(errorCode);

	errorCode = GetAsynchronousRecord(0, pRecord, sizeof(record));
	if (errorCode != BIRD_ERROR_SUCCESS) 
		errorHandler(errorCode);

	unsigned int status = GetSensorStatus(0);

	if( status == VALID_STATUS)
	{

		matrix->SetElement(0, 0, record.s[0][0]); matrix->SetElement(0, 1, record.s[0][1]);	matrix->SetElement(0, 2, record.s[0][2]);

		matrix->SetElement(1, 0, record.s[2][0]); matrix->SetElement(1, 1, record.s[2][1]); matrix->SetElement(1, 2, record.s[2][2]);

		matrix->SetElement(2, 0, record.s[1][0]); matrix->SetElement(2, 1, record.s[1][1]);	matrix->SetElement(2, 2, record.s[1][2]);		

		matrix->Transpose();
	}

	
	DOUBLE_POSITION_ANGLES_RECORD record_2, *pRecord_2 = &record_2;	

	DATA_FORMAT_TYPE type_2 = DOUBLE_POSITION_ANGLES;
	errorCode = SetSensorParameter(0,DATA_FORMAT,&type_2,sizeof(type_2));
	if(errorCode!=BIRD_ERROR_SUCCESS) 
		errorHandler(errorCode);
	
	errorCode = GetAsynchronousRecord(0, pRecord_2, sizeof(record_2));
	if(errorCode!=BIRD_ERROR_SUCCESS) {errorHandler(errorCode);}
	
	unsigned int status_2 = GetSensorStatus(0);

	if( status_2 == VALID_STATUS)
	{
		*trakStar_x = record_2.x;
		*trakStar_y = record_2.y;
		*trakStar_z = record_2.z;
	}
}

void DeinitializeTrakStar()
{
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//
	// Turn off the transmitter before exiting
	// We turn off the transmitter by "selecting" a transmitter with an id of "-1"
	//
	short id = -1;
	errorCode = SetSystemParameter(SELECT_TRANSMITTER, &id, sizeof(id));
	if(errorCode!=BIRD_ERROR_SUCCESS) errorHandler(errorCode);

	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//
	//  Free memory allocations before exiting
	//
	delete[] pSensor;
	delete[] pXmtr;
}

int main(int argc, char **argv)
{	
	InitializeTrakStar();
	
	Slicer();

	DeinitializeTrakStar();

}