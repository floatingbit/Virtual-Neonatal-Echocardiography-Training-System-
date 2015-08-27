
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
		this->azimuth = this->elevation = this->roll = 10;
		this->x = -1500, this->y = -1500, this->z = 176;
		this->startX = true, this->startY = this->startZ = false;

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
		
		if(event == vtkCommand::KeyPressEvent)
		{
			std::string key = interactor->GetKeySym();
			
			if(key == "a")
			{
				this->azimuth += 1;
			}

			else if(key == "z")
			{
				this->azimuth -= 1;
			}

			else if(key == "s")
			{
				this->elevation += 1;
			}

			else if(key == "x")
			{
				this->elevation -= 1;
			}

			else if(key == "d")
			{
				this->roll += 1;
			}

			else if(key == "c")
			{
				this->roll -= 1;
			}			

			vtkSmartPointer<vtkCamera> cam = vtkSmartPointer<vtkCamera>::New();
			cam->Azimuth(this->azimuth);
			cam->Elevation(this->elevation);
			cam->Roll(this->roll);
			cam->OrthogonalizeViewUp();
			
			guidanceRenderer->SetActiveCamera(cam);
			guidanceRenderer->ResetCamera();
			interactor->Render();	
		}

		//if (event == vtkCommand::TimerEvent)
  //      {
		//	double x, y, z, azimuth, elevation, roll;
		//	GetSensorData(&x, &y, &z, &azimuth, &elevation, &roll);

		//	double delta_X = (last_X - x);
		//	double delta_Y = (last_Y - y);
		//	double delta_Z = (last_Z - z);

		//	/*if(!(z >= -3.6 && z <= -2.0))
		//	{
		//		std::cout<<"Outside any of the windows ("<<x<<", "<<y<<", "<<z<<","<<azimuth<< ", "<<elevation<<", "<<roll<<")"<<std::endl;
		//		interactor->Render();
		//		ren->ResetCamera(); 
		//		return;
		//	}

		//	if(!(x >= 4.0 && x <= 5.2 && y >= 0.0 && y <= 1.0) && 
		//		!(x >= 3.0 && x <= 3.9 && y >= 1.1 && y <= 1.6) && 
		//		!(x >= 2.0 && x <= 3.8 && y >= -0.7 && y <= 0.7) && 
		//		!(x >= 5.3 && x <= 6.2 && y >= -1.0 && y <= 0.5))
		//	{
		//		std::cout<<"Outside any of the windows ("<<x<<", "<<y<<", "<<z<<","<<azimuth<< ", "<<elevation<<", "<<roll<<")"<<std::endl;
		//		interactor->Render();
		//		ren->ResetCamera(); 
		//		return;
		//	}*/

		//	azimuth += this->azimuth;
		//	elevation += this->elevation;
		//	roll += this->roll;

		//	reslice[0]->GetOutput()->UpdateInformation();
		//	
		//	double sliceSpacing_0 = reslice[0]->GetOutput()->GetSpacing()[0];
		//	double sliceSpacing_1 = reslice[0]->GetOutput()->GetSpacing()[1];
		//	double sliceSpacing_2 = reslice[0]->GetOutput()->GetSpacing()[2];

		//	vtkMatrix4x4 *matrix = reslice[0]->GetResliceAxes();

		//	//Set the 1st col
		//	matrix->SetElement(0, 0, cos(Deg_to_Rad(azimuth)) * cos(Deg_to_Rad(elevation)));
		//	matrix->SetElement(1, 0, sin(Deg_to_Rad(azimuth)) * cos(Deg_to_Rad(elevation)));
		//	matrix->SetElement(2, 0, -sin(Deg_to_Rad(elevation)));
		//	
		//	//Set the 2nd col
		//	matrix->SetElement(0, 1, (cos(Deg_to_Rad(azimuth)) * sin(Deg_to_Rad(elevation)) * sin(Deg_to_Rad(roll))) - (sin(Deg_to_Rad(azimuth))*cos(Deg_to_Rad(roll))));
		//	matrix->SetElement(1, 1, (sin(Deg_to_Rad(azimuth)) * sin(Deg_to_Rad(elevation)) * sin(Deg_to_Rad(roll))) + (cos(Deg_to_Rad(azimuth))*cos(Deg_to_Rad(roll))));
		//	matrix->SetElement(2, 1, cos(Deg_to_Rad(elevation)) * sin(Deg_to_Rad(roll)));

		//	//Set the 3rd col
		//	matrix->SetElement(0, 2, (cos(Deg_to_Rad(azimuth)) * sin(Deg_to_Rad(elevation)) * cos(Deg_to_Rad(roll))) + (sin(Deg_to_Rad(azimuth)) * sin(Deg_to_Rad(roll))));
		//	matrix->SetElement(1, 2, (sin(Deg_to_Rad(azimuth)) * sin(Deg_to_Rad(elevation)) * cos(Deg_to_Rad(roll))) - (cos(Deg_to_Rad(azimuth)) * sin(Deg_to_Rad(roll))));
		//	matrix->SetElement(2, 2, cos(Deg_to_Rad(elevation)) * cos(Deg_to_Rad(roll)));

		//	//Set the 4th col
		//	matrix->SetElement(0, 3, delta_X * sliceSpacing_0);
		//	matrix->SetElement(1, 3, delta_Y * sliceSpacing_1);
		//	matrix->SetElement(2, 3, delta_Z * sliceSpacing_2);

		//	matrix->SetElement(3, 0, 0);
		//	matrix->SetElement(3, 1, 0);
		//	matrix->SetElement(3, 2, 0);
		//	matrix->SetElement(3, 3, 1);
		//	
		//	interactor->Render();			
		//	ren->ResetCamera(); 
		//	triRenderer->ResetCamera();

		//	last_X = x;
		//	last_Y = y;
		//	last_Z = z;
  //      }
		
		if (event == vtkCommand::TimerEvent)
		{

			vtkSmartPointer<vtkCamera> cam = ren->GetActiveCamera();
			double* pos = cam->GetPosition();
			double* viewup = cam->GetViewUp();
			double* focalpoint = cam->GetFocalPoint();
			std::cout<<"Pos:("<<pos[0]<<","<<pos[1]<<","<<pos[2]<<") ViewUp:("<<viewup[0]<<","<<viewup[1]<<","<<viewup[2]<<") focalpoint: ("<<focalpoint[0]<<","<<focalpoint[1]<<","<<focalpoint[2]<<")"<<endl;
			//cam->Print(std::cout);
			/*if(y == -1500)
			{
				startX = true;
				startY = false;
				y = 800;
			}*/
			if(x == -1500)
			{
				startY = true;
				startX = false;
				x = 800;//z+=100;				
			}
			/*if (startX)*/
				x-=10;

			/*if (startY)
				y-=10;*/

			z=dimZ/2;

			cam->SetPosition(x,y,z);
			cam->SetViewUp(0,0,-1);
			cam->SetFocalPoint(dimX/2, dimY/2, dimZ/2);
			renderer->ResetCameraClippingRange();

			//Normal = (-0.64, -0.76, -0.04)
			//Origin = (103.5, 111.5, 175.5)

			interactor->Render();

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

	double azimuth;
	double elevation;
	double roll;

	int counter;
	int counter_data;	
	int dimX, dimY, dimZ;

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

	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();	
	transform->RotateWXYZ(-20,0.0,-0.7,0.7);

	vtkSmartPointer<vtkImageSlice> ia = vtkSmartPointer<vtkImageSlice>::New();
	ia->SetMapper(resliceMapper);
	ia->SetProperty(ip);
	//ia->SetUserTransform(transform);

	vtkSmartPointer<vtkRenderWindow> window =
		vtkSmartPointer<vtkRenderWindow>::New();
	//window->FullScreenOn();
	window->SetSize(800,800);
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	window->AddRenderer(renderer);
	window->FullScreenOn();
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

	
	


	/*vtkSmartPointer<vtkMatrix4x4> resliceAxes =
		vtkSmartPointer<vtkMatrix4x4>::New();
	
	resliceAxes->DeepCopy(obliqueElements);
		
	callback->reslice[0] = vtkSmartPointer<vtkImageReslice>::New();
	callback->reslice[0]->SetInputConnection(vtkImporter->GetOutputPort());
	callback->reslice[0]->SetOutputDimensionality(2);
	callback->reslice[0]->SetResliceAxes(resliceAxes);
	callback->reslice[0]->SetInterpolationModeToLinear();

	vtkSmartPointer<vtkLookupTable> table =
		vtkSmartPointer<vtkLookupTable>::New();
	table->SetRange(0, 1000); // image intensity range
	table->SetValueRange(0.0, 1.0); // from black to white
	table->SetSaturationRange(0.0, 0.75); // no color saturation
	table->SetRampToLinear();
	table->Build();

	vtkSmartPointer<vtkImageMapToColors> color =
		vtkSmartPointer<vtkImageMapToColors>::New();
	color->SetLookupTable(table);
	color->SetInputConnection(callback->reslice[0]->GetOutputPort());

	callback->actor[0] = vtkSmartPointer<vtkImageActor>::New();
	callback->actor[0]->SetInput(color->GetOutput());
	//End adding new Image

#pragma endregion
	
	// Define viewport ranges
	double trianglesViewport[4] = {0.2, 0.0, 0.8, 1.0};
	double centerViewport[4] =    {0.0, 0.0, 1.0, 1.0};
	double guidanceViewport[4] =  {0.0, 0.7, 0.3, 1.0};
		
	vtkSmartPointer<vtkRenderWindow> window =
		vtkSmartPointer<vtkRenderWindow>::New();
	window->FullScreenOn();
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->SetViewport(centerViewport);

#pragma region Add Triangles

	// Create left triangle
	vtkSmartPointer<vtkPoints> points_left_triangle =
		vtkSmartPointer<vtkPoints>::New();
	points_left_triangle->InsertNextPoint ( 0, 0, 0 );//TOP LEFT
	points_left_triangle->InsertNextPoint ( 0.5, 1, 0 );//RIGHT TOP
	points_left_triangle->InsertNextPoint ( 0, 1, 0 );
 
	vtkSmartPointer<vtkTriangle> left_triangle =
		vtkSmartPointer<vtkTriangle>::New();
	left_triangle->GetPointIds()->SetId ( 0, 0 );
	left_triangle->GetPointIds()->SetId ( 1, 1 );
	left_triangle->GetPointIds()->SetId ( 2, 2 );
 
	vtkSmartPointer<vtkCellArray> left_triangle_cell_array =
		vtkSmartPointer<vtkCellArray>::New();
	left_triangle_cell_array->InsertNextCell ( left_triangle );
 
	// Create a polydata object
	vtkSmartPointer<vtkPolyData> left_triangle_PolyData =
		vtkSmartPointer<vtkPolyData>::New();
 
	// Add the geometry and topology to the polydata
	left_triangle_PolyData->SetPoints ( points_left_triangle );
	left_triangle_PolyData->SetPolys ( left_triangle_cell_array );
 
	// Create mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> left_triangle_mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();

	left_triangle_mapper->SetInput(left_triangle_PolyData);

	vtkSmartPointer<vtkActor> left_triangle_Actor =
		vtkSmartPointer<vtkActor>::New();
	left_triangle_Actor->SetMapper(left_triangle_mapper);
	left_triangle_Actor->GetProperty()->SetColor(0,0,0);
	
	//Right triangle

	// Create right triangle
	vtkSmartPointer<vtkPoints> points_right_triangle =
		vtkSmartPointer<vtkPoints>::New();
	points_right_triangle->InsertNextPoint ( 0.5, 1, 0 );
	points_right_triangle->InsertNextPoint ( 1, 1, 0 );
	points_right_triangle->InsertNextPoint ( 1, 0, 0 );
 
	vtkSmartPointer<vtkTriangle> right_triangle =
		vtkSmartPointer<vtkTriangle>::New();
	right_triangle->GetPointIds()->SetId ( 0, 0 );
	right_triangle->GetPointIds()->SetId ( 1, 1 );
	right_triangle->GetPointIds()->SetId ( 2, 2 );
 
	vtkSmartPointer<vtkCellArray> right_triangle_cell_array =
		vtkSmartPointer<vtkCellArray>::New();
	right_triangle_cell_array->InsertNextCell ( right_triangle );
 
	// Create a polydata object
	vtkSmartPointer<vtkPolyData> right_triangle_PolyData =
		vtkSmartPointer<vtkPolyData>::New();
 
	// Add the geometry and topology to the polydata
	right_triangle_PolyData->SetPoints ( points_right_triangle );
	right_triangle_PolyData->SetPolys ( right_triangle_cell_array );
 
	// Create mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper_right_triangle =
		vtkSmartPointer<vtkPolyDataMapper>::New();

	mapper_right_triangle->SetInput(right_triangle_PolyData);

	vtkSmartPointer<vtkActor> right_triangle_Actor =
		vtkSmartPointer<vtkActor>::New();
	
	right_triangle_Actor->SetMapper(mapper_right_triangle);
	right_triangle_Actor->GetProperty()->SetColor(0,0,0);

#pragma endregion

#pragma region Add heart for Positional Guidance

	// Load data
	vtkSmartPointer<vtkStructuredPointsReader> sp_reader =
	vtkSmartPointer<vtkStructuredPointsReader>::New();
	sp_reader->SetFileName("C:\\VETS Source Code\\VETS 3D\\4D_Fetal_Heart_GE\\3D_Fetal_Heart.vtk");
 
	// Create a 3D model using marching cubes
	vtkSmartPointer<vtkMarchingCubes> mc =
	vtkSmartPointer<vtkMarchingCubes>::New();
	mc->SetInputConnection(sp_reader->GetOutputPort());
	mc->ComputeNormalsOn();
	mc->ComputeGradientsOn();
	mc->SetValue(0, 120);  // second value acts as threshold	
	
	vtkSmartPointer<vtkPolyDataMapper> pd_mapper =
	vtkSmartPointer<vtkPolyDataMapper>::New();
	pd_mapper->SetInputConnection(mc->GetOutputPort());
	pd_mapper->ScalarVisibilityOff();    // utilize actor's property I set

	// Visualize
	vtkSmartPointer<vtkActor> guidance_Actor =
	vtkSmartPointer<vtkActor>::New();
	guidance_Actor->GetProperty()->SetColor(0.75 ,0.75 ,0.75 );
	guidance_Actor->SetMapper(pd_mapper);
  			
#pragma endregion

#pragma region Add Cone for Positional Guidance

	vtkConeSource *cone = vtkConeSource::New();
	cone->SetHeight( 3.0 );
	cone->SetRadius( 1.0 );
	cone->SetResolution( 512 );
    
	vtkPolyDataMapper *coneMapper = vtkPolyDataMapper::New();
	coneMapper->SetInputConnection( cone->GetOutputPort() );
	  
	vtkActor *coneActor = vtkActor::New();
	coneActor->SetMapper(coneMapper);
	
#pragma endregion

	renderer->InteractiveOff();
	window->AddRenderer(renderer);

	// Setup both renderers
	vtkSmartPointer<vtkRenderer> triangleRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	triangleRenderer->SetViewport(guidanceViewport);//trianglesViewport);
	triangleRenderer->EraseOff();
	triangleRenderer->InteractiveOff();
	triangleRenderer->AddActor(guidance_Actor);//left_triangle_Actor);
	//triangleRenderer->AddActor(right_triangle_Actor);
	triangleRenderer->SetBackground(0.1,0.1,0.1);

	// create a camera with the correct view up
	vtkCamera *aCamera = vtkCamera::New();
	aCamera->SetViewUp (0, 1, 0);
	aCamera->SetPosition (0.5,0.5,4.5);
	aCamera->SetFocalPoint (0.5, 0.5, 0);
	aCamera->ParallelProjectionOn();

	triangleRenderer->SetActiveCamera(aCamera);
	
	window->AddRenderer(triangleRenderer);
	
	callback->left_triangle_Actor = left_triangle_Actor;
	callback->right_triangle_Actor = right_triangle_Actor;
	callback->guidanceRenderer = triangleRenderer;

	// Set up the interaction
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> imageStyle =
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetInteractorStyle(imageStyle);
		
	window->SetInteractor(interactor);
	
	window->Render();

	renderer->AddActor(callback->actor[0]);

	callback->SetInteractor(interactor);
	callback->SetRenderer(renderer);
	callback->triRenderer = triangleRenderer;

	interactor->AddObserver(vtkCommand::TimerEvent, callback);
	interactor->AddObserver(vtkCommand::KeyPressEvent, callback);

	int timerId = interactor->CreateRepeatingTimer(90);
	
	interactor->Start();

	return EXIT_SUCCESS;*/
}

/*vtkSmartPointer<vtkImageReader2> reader1 =
		vtkSmartPointer<vtkImageReader2>::New();
	reader1->SetFilePrefix("C:/VETS Source Code/VETS 3D/VTK Data/Data/headsq/quarter");
	reader1->SetDataExtent(0, 63, 0, 63, 1, 93);
	reader1->SetDataSpacing(3.2, 3.2, 1.5);
	reader1->SetDataOrigin(0.0, 0.0, 0.0);
	reader1->SetDataScalarTypeToUnsignedShort();
	reader1->SetDataByteOrderToLittleEndian();
	reader1->UpdateWholeExtent();*/

