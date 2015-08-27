
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

#include "vtkSmartPointer.h"
#include "vtkImageImport.h"
#include "vtkImageExport.h"
#include "vtkImageReader2.h"
#include "vtkMatrix4x4.h"
#include "vtkImageReslice.h"
#include "vtkLookupTable.h"
#include "vtkImageMapToColors.h"
#include "vtkImageResliceMapper.h"
#include "vtkImageActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkCommand.h"
#include "vtkImageData.h"
#include "vtkCamera.h"
#include "vtkStructuredPointsReader.h"
#include "vtkMarchingCubes.h"
#include "vtkConeSource.h"
#include "vtkPlane.h"
#include "vtkImageProperty.h"
#include "vtkImageSlice.h"
#include "vtkLinearTransform.h"
#include "vtkTransform.h"
#include "vtkHomogeneousTransform.h"

//For the triangle
#include <vtkTriangle.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
//For the triangle

#include "ImageViewer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

#define M_PI 3.1415926
#define Deg_to_Rad(X) (X*M_PI/180.0) 
#define Rad_to_Deg(X) (X*180.0/M_PI) 

class vtkImageInteractionCallback : public vtkCommand
{
public:

	static vtkImageInteractionCallback *New() {
		return new vtkImageInteractionCallback; };

	vtkImageInteractionCallback() {
		this->Slicing = 0; 
		this->ImageReslice = 0;
		this->Interactor = 0; 
		this->last_X = this->last_Y = this->last_Z = 0.0;
		this->counter = this->counter_data = 0;
		this->azimuth = this->elevation = this->roll = 0;
		this->x = 0/*848.343*/,y=612.007,z=174.787;//0, y = 0, z = 0;//-742.182, this->y = 770.512, this->z = 26.4033;
		this->startX = true, this->startY = this->startZ = false;
		//this->normal[0] = 0.0047638, this->normal[1] = 0.999794, this->normal[2] = -0.0197307;
		//this->normal[0] = 0.76858, this->normal[1] = 0.63943, this->normal[2] = -0.0203343;//for pitch 
		
		this->normal[0] = -1, this->normal[1] = -0.0263223, this->normal[2] = -0.00834897;
		
		this->distfromcam = 897;

	};

	void SetImageReslice(vtkImageReslice *reslice) {
			this->ImageReslice = reslice; };

	vtkImageReslice* GetImageReslice() {
				return this->ImageReslice; };

	void SetRenderer(vtkRenderer *ren) {
			this->renderer = ren; };

	vtkRenderer* GetRenderer() {
		return this->renderer; };
						
	void SetInteractor(vtkRenderWindowInteractor *interactor) {
					this->Interactor = interactor; };

	vtkRenderWindowInteractor *GetInteractor() {
						return this->Interactor; };

	virtual void Execute(vtkObject *, unsigned long event, void *)
	{		
		vtkRenderWindowInteractor *interactor = this->GetInteractor();
		vtkRenderer* ren = this->GetRenderer();
		
		
		if (event == vtkCommand::TimerEvent)
		{
			double x, y, z, azimuth, elevation, roll;
			GetSensorData(&x, &y, &z, &azimuth, &elevation, &roll);
			elevation = floor(elevation * 100 + 0.5)/100;
			roll = 0;//floor(roll * 100 + 0.5)/100;


			double delta_X = (last_X - x);
			double delta_Y = (last_Y - y);
			double delta_Z = (last_Z - z);
			azimuth = 0;

			vtkSmartPointer<vtkCamera> cam = ren->GetActiveCamera();
			double* pos = cam->GetPosition();
			double* viewup = cam->GetViewUp();
			double* focalPoint = cam->GetFocalPoint();
			std::cout<<"Pos:("<<pos[0]<<","<<pos[1]<<","<<pos[2]<<")"<<endl; //ViewUp:("<<viewup[0]<<","<<viewup[1]<<","<<viewup[2]<<") focalpoint: ("<<focalPoint[0]<<","<<focalPoint[1]<<","<<focalPoint[2]<<")"<<endl;

			double distfromcam = 897;//cam->GetDistance();
			cout<<"Distance from camera: "<<distfromcam<<endl;

			
			//Calculate the position of the camera
			vtkSmartPointer<vtkPlane> sliceplane = vtkSmartPointer<vtkPlane>::New();
			sliceplane = resliceMapper->GetSlicePlane();

			double normal[3];
			double position[3];
			//double pitch = 1;
			//double roll = delta_X;

			double norm[3];
			norm[0] = -1;
			norm[1] = -0.0263223;
			norm[2] = -0.00834897;

			//Pitch
			/*this->normal[0] = cos(Deg_to_Rad(pitch)) * norm[0] + sin(Deg_to_Rad(pitch)) * norm[2];
			this->normal[1] = norm[1];
			this->normal[2] = -sin(Deg_to_Rad(pitch)) * norm[0] + cos(Deg_to_Rad(pitch)) * norm[2];*/

			//Roll
			/*this->normal[0] = norm[0];
			this->normal[1] = norm[1] * cos(Deg_to_Rad(roll)) - norm[2] * sin(Deg_to_Rad(roll));
			this->normal[2] = norm[1] * sin(Deg_to_Rad(roll)) + norm[2] * cos(Deg_to_Rad(roll));*/ 

			//Yaw
			/*this->normal[0] = cos(Deg_to_Rad(pitch)) * norm[0] - sin(Deg_to_Rad(pitch)) * norm[1];
			this->normal[1] = norm[0] * sin(Deg_to_Rad(pitch)) + norm[1] * cos(Deg_to_Rad(pitch));
			this->normal[2] = norm[2];*/

			//Roll, pitch, yaw combined
			
			this->normal[0] = cos(Deg_to_Rad(azimuth)) * cos(Deg_to_Rad(elevation)) * norm[0] +
								(cos(Deg_to_Rad(azimuth)) * sin(Deg_to_Rad(elevation)) * sin(Deg_to_Rad(roll))) - (sin(Deg_to_Rad(azimuth))*cos(Deg_to_Rad(roll))) * norm[1] +
								(cos(Deg_to_Rad(azimuth)) * sin(Deg_to_Rad(elevation)) * cos(Deg_to_Rad(roll))) + (sin(Deg_to_Rad(azimuth)) * sin(Deg_to_Rad(roll))) * norm[2];

			this->normal[1] = sin(Deg_to_Rad(azimuth)) * cos(Deg_to_Rad(elevation)) * norm[0] +
								(sin(Deg_to_Rad(azimuth)) * sin(Deg_to_Rad(elevation)) * sin(Deg_to_Rad(roll))) + (cos(Deg_to_Rad(azimuth))*cos(Deg_to_Rad(roll))) * norm[1] +
								(sin(Deg_to_Rad(azimuth)) * sin(Deg_to_Rad(elevation)) * cos(Deg_to_Rad(roll))) - (cos(Deg_to_Rad(azimuth)) * sin(Deg_to_Rad(roll))) * norm[2];

			this->normal[2] = -sin(Deg_to_Rad(elevation)) * norm[0] +
								cos(Deg_to_Rad(elevation)) * sin(Deg_to_Rad(roll)) * norm[1] +
								cos(Deg_to_Rad(elevation)) * cos(Deg_to_Rad(roll)) * norm[2];

			//Calculate the focal point
			double focalPt[3];
			double centerofvol[] = {dimX/2, dimY/2, dimZ/2};
			vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
			plane->SetOrigin(dimX/2, dimY/2, 0);
			plane->SetNormal(this->normal);
			plane->ProjectPoint( centerofvol, focalPt ); 

			position[0] = focalPt[0] + distfromcam * this->normal[0];
			position[1] = focalPt[1] + distfromcam * this->normal[1];
			position[2] = focalPt[2] + distfromcam * this->normal[2];		
			
			sliceplane->GetNormal(normal);
			std::cout<<"Normal: ("<<normal[0]<<","<<normal[1]<<","<<normal[2]<<")"<<endl;	

			cam->SetPosition(position);		
			cam->SetViewUp(0, 0, -1);
			cam->SetFocalPoint(focalPt);//dimX/2, dimY/2, dimZ/2);
			
			interactor->Render();

			last_X = x;
			last_Y = y;
			last_Z = z;
		}
	};

public:
	vtkSmartPointer<vtkImageActor> actor[10];
	vtkSmartPointer<vtkImageReslice> reslice[10];

	vtkSmartPointer<vtkActor> left_triangle_Actor;
	vtkSmartPointer<vtkActor> right_triangle_Actor;
	vtkRenderer* triRenderer;
	vtkRenderer* guidanceRenderer;

	vtkSmartPointer<vtkImageResliceMapper> resliceMapper;
	vtkSmartPointer<vtkImageSlice> imageSlice;
	double x,y,z;
	double last_X;
	double last_Y;
	double last_Z;
	double normal[3];

	double azimuth;
	double elevation;
	double roll;

	int counter;
	int counter_data;	
	int dimX, dimY, dimZ;
	double distfromcam;

private: 
	
	int Slicing;
	vtkImageReslice *ImageReslice;
	vtkRenderWindowInteractor *Interactor;
	vtkRenderer* renderer;

	bool startX, startY, startZ;
	
};


/**
* This function will connect the given itk::VTKImageExport filter to
* the given vtkImageImport filter.
*/
template <typename ITK_Exporter, typename VTK_Importer>
void ConnectPipelines(ITK_Exporter exporter, VTK_Importer* importer)
{
	importer->SetUpdateInformationCallback(exporter->GetUpdateInformationCallback());
	importer->SetPipelineModifiedCallback(exporter->GetPipelineModifiedCallback());
	importer->SetWholeExtentCallback(exporter->GetWholeExtentCallback());
	importer->SetSpacingCallback(exporter->GetSpacingCallback());
	importer->SetOriginCallback(exporter->GetOriginCallback());
	importer->SetScalarTypeCallback(exporter->GetScalarTypeCallback());
	importer->SetNumberOfComponentsCallback(exporter->GetNumberOfComponentsCallback());
	importer->SetPropagateUpdateExtentCallback(exporter->GetPropagateUpdateExtentCallback());
	importer->SetUpdateDataCallback(exporter->GetUpdateDataCallback());
	importer->SetDataExtentCallback(exporter->GetDataExtentCallback());
	importer->SetBufferPointerCallback(exporter->GetBufferPointerCallback());
	importer->SetCallbackUserData(exporter->GetCallbackUserData());
}

int Slicer ()
{	
	vtkSmartPointer<vtkImageInteractionCallback> callback =	
		vtkSmartPointer<vtkImageInteractionCallback>::New();	
	
	#pragma region Read Dicom Slices
		
	char cardiac_cycle_dcm_filename[10][98] = {
	"C:\\VETS Source Code\\VETS 3D\\My Heart\\my_heart_exported.dcm",
	"C:\\VETS Source Code\\VETS 3D\\4D_Fetal_Heart_GE\\ph.dcm",
	"C:\\VETS Source Code\\VETS 3D\\4D_Fetal_Heart_GE\\Series_0.dcm",
	"C:\\VETS Source Code\\VETS 3D\\4D_Fetal_Heart_GE\\Series_0.dcm",
	"C:\\VETS Source Code\\VETS 3D\\4D_Fetal_Heart_GE\\Series_0.dcm",
	"C:\\VETS Source Code\\VETS 3D\\4D_Fetal_Heart_GE\\Series_0.dcm",
	"C:\\VETS Source Code\\VETS 3D\\4D_Fetal_Heart_GE\\Series_0.dcm",
	"C:\\VETS Source Code\\VETS 3D\\4D_Fetal_Heart_GE\\Series_0.dcm",
	"C:\\VETS Source Code\\VETS 3D\\4D_Fetal_Heart_GE\\Series_0.dcm",
	"C:\\VETS Source Code\\VETS 3D\\4D_Fetal_Heart_GE\\Series_0.dcm"
};

	static double obliqueElements[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 };

	vtkSmartPointer<vtkMatrix4x4> resliceAxes = vtkSmartPointer<vtkMatrix4x4>::New();

	resliceAxes->DeepCopy(obliqueElements);
	typedef short InputPixelType;
	typedef itk::Image<InputPixelType, 3> InputImageType;
	typedef itk::ImageFileReader<InputImageType> ReaderType;
	typedef itk::GDCMImageIO ImageIOType;
	ImageIOType::Pointer gdcmImageIO = ImageIOType::New();

	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(cardiac_cycle_dcm_filename[0]);
	reader->SetImageIO(gdcmImageIO);
	reader->Update(); 

	typedef itk::VTKImageExport<InputImageType> ExportFilterType;
	ExportFilterType::Pointer itkExporter = ExportFilterType::New();
	itkExporter->SetInput(reader->GetOutput());

	vtkImageImport *vtkImporter = vtkImageImport::New();

	ConnectPipelines(itkExporter, vtkImporter);			
			
	vtkSmartPointer<vtkImageResliceMapper> resliceMapper =
		vtkSmartPointer<vtkImageResliceMapper>::New();
	resliceMapper->SetInputConnection(vtkImporter->GetOutputPort());

	callback->dimX = reader->GetImageIO()->GetDimensions(0);
	callback->dimY = reader->GetImageIO()->GetDimensions(1);
	callback->dimZ = reader->GetImageIO()->GetDimensions(2);

	vtkSmartPointer<vtkPlane> sliceplane = vtkSmartPointer<vtkPlane>::New();	
	resliceMapper->SliceFacesCameraOn();
	resliceMapper->SliceAtFocalPointOn();
	resliceMapper->BorderOff();

	vtkSmartPointer<vtkImageProperty> ip = vtkSmartPointer<vtkImageProperty>::New();
	ip->SetColorWindow(214);
	ip->SetColorLevel(100);
	ip->SetAmbient(0.0);
	ip->SetDiffuse(1.0);
	ip->SetOpacity(1.0);
	ip->SetInterpolationTypeToLinear();
		
	vtkSmartPointer<vtkImageSlice> ia = vtkSmartPointer<vtkImageSlice>::New();
	ia->SetMapper(resliceMapper);
	ia->SetProperty(ip);

	vtkSmartPointer<vtkRenderWindow> window =
		vtkSmartPointer<vtkRenderWindow>::New();
	//window->FullScreenOn();
	window->SetSize(800,800);
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	double centerViewport[4] =    {0.0, 0.0, 1.0, 1.0};
	renderer->SetViewport(centerViewport);
	window->AddRenderer(renderer);
	renderer->AddViewProp(ia);
	//renderer->SetBackground(0.1,0.2,0.4);
	

	vtkSmartPointer<vtkInteractorStyleImage> imageStyle =
		vtkSmartPointer<vtkInteractorStyleImage>::New();
	imageStyle->SetInteractionModeToImage3D();
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetInteractorStyle(imageStyle);
		
	callback->SetInteractor(interactor);
	callback->SetRenderer(renderer);	
	
	window->SetInteractor(interactor);
	
	window->Render();
	
	vtkSmartPointer<vtkCamera> cam = renderer->GetActiveCamera();
	cam->ParallelProjectionOn();	
	
	renderer->ResetCameraClippingRange();
	callback->SetInteractor(interactor);

	window->Render();
	interactor->AddObserver(vtkCommand::TimerEvent, callback);
	interactor->AddObserver(vtkCommand::MouseMoveEvent, callback);
	interactor->AddObserver(vtkCommand::LeftButtonReleaseEvent, callback);
		
	callback->resliceMapper = resliceMapper;
	callback->imageSlice = ia;

	int timerId = interactor->CreateRepeatingTimer(100);

	interactor->Start();

	return EXIT_SUCCESS;
}