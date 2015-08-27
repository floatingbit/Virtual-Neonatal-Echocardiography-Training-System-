
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
#include "vtkPlaneSource.h"
#include "vtkPlaneWidget.h"
#include "vtkProbeFilter.h"
#include "vtkStructuredGridOutlineFilter.h"
#include "vtkPointData.h"
#include "vtkImageGaussianSmooth.h"

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

enum Cardiac_Windows
{
	Parasternal_1,
	Subcostal_1,
	Apical_1,
	Suprasternal_1,
	None
};

class Range
{
public:
	double min_azimuth, max_azimuth;
	double min_elevation, max_elevation;
	double min_roll, max_roll;

	double min_x, max_x;
	double min_y, max_y;
	double min_z, max_z;

public: 
	Range(double min_azimuth, double max_azimuth, double min_elevation, double max_elevation, double min_roll, double max_roll,
		double min_x, double max_x, double min_y, double max_y, double min_z, double max_z)
	{
		this->min_azimuth = min_azimuth;
		this->max_azimuth = max_azimuth;

		this->min_elevation = min_elevation;
		this->max_elevation = max_elevation;

		this->min_roll = min_roll;
		this->max_roll = max_roll;

		this->min_x = min_x;
		this->max_x = max_x;
		this->min_y = min_y;
		this->max_y = max_y;
		this->min_z = min_z;
		this->max_z = max_z;
	}
};
	
class vtkImageInteractionCallback : public vtkCommand
{
public:

	static vtkImageInteractionCallback *New() {
		return new vtkImageInteractionCallback; };

	vtkImageInteractionCallback() {
			this->last_X = this->last_Y = this->last_Z = 0.0;
			this->azimuth = this->elevation = this->roll = 0;
			this->distfromcam = 897;
			this->current_volume_index = 0;
			this->adjust_elevation = 0;
			this->adjust_roll = 0;
			this->adjust_azimuth = 0;
			this->current_cardiac_window = Cardiac_Windows::None;

			this->parasternal_range_1 = new Range(0, 100, 0, 100, 0, 100, 4.2, 4.84, 0.14, 0.99, -3.8, -2.6);
			this->apical_range_1 = new Range(0, 100, 0, 100, 0, 100, 3.5, 4.2, 1.0, 1.15, -3, -1);
			this->suprasternal_range_1 = new Range(0, 100, 0, 100, 0, 100, 5.7, 6.2, -0.4, 0.4, -3, -1);
			this->subcostal_range_1 = new Range(0, 100, 0, 100, 0, 100, 2.5, 3.0, -0.18, 0.36, -3.8, -2.6);

			this->par_angle_adjust[0] = 0;
			this->par_angle_adjust[1] = 0;
			this->par_angle_adjust[2] = 0;

			
			this->sub_angle_adjust[0] = 0;
			this->sub_angle_adjust[1] = 0;
			this->sub_angle_adjust[2] = 0;

			
			this->sup_angle_adjust[0] = 0;
			this->sup_angle_adjust[1] = 0;
			this->sup_angle_adjust[2] = 0;

			
			this->api_angle_adjust[0] = 0;
			this->api_angle_adjust[1] = 0;
			this->api_angle_adjust[2] = 0;

			this->par_image_alignment[0] = 0;
			this->par_image_alignment[1] = 0;
			this->par_image_alignment[2] = 0;

			this->x_view = 0;
			this->y_view = 0;
			this->z_view = 1;

			this->default_norm[0] = 0;
			this->default_norm[1] = 0;
			this->default_norm[2] = 1;

			this->mc_i = 10;
			this->mc_threshold = 50;
		};				

	void SetRenderer(vtkRenderer *ren) {
					this->renderer = ren; };

	void SetRenderWindow(vtkRenderWindow *renwin)
	{
		this->renWin = renwin;
	}

	vtkRenderer* GetRenderer() {
						return this->renderer; };

	void SetInteractor(vtkRenderWindowInteractor *interactor) {
							this->Interactor = interactor; };

	vtkRenderWindowInteractor *GetInteractor() {
								return this->Interactor; };

	Cardiac_Windows GetCurrentWindow(double x, double y, double z)
	{		
		return Cardiac_Windows::Parasternal_1;

		//Parasternal
		if(x >= this->parasternal_range_1->min_x && x <= this->parasternal_range_1->max_x && 
			y >= this->parasternal_range_1->min_y && y <= this->parasternal_range_1->max_y && 
			z >= this->parasternal_range_1->min_z && z <= this->parasternal_range_1->max_z)
		{
			//cout<<"Parasternal detected"<<endl;
			return Cardiac_Windows::Parasternal_1;
		}

		//Subcostal
		else if(x >= this->subcostal_range_1->min_x && x <= this->subcostal_range_1->max_x && 
			y >= this->subcostal_range_1->min_y && y <= this->subcostal_range_1->max_y && 
			z >= this->subcostal_range_1->min_z && z <= this->subcostal_range_1->max_z)
		{
			cout<<"Subcostal detected"<<endl;
			return Cardiac_Windows::Subcostal_1;
		}

		//Apical
		else if(x >= this->apical_range_1->min_x && x <= this->apical_range_1->max_x && 
			y >= this->apical_range_1->min_y && y <= this->apical_range_1->max_y && 
			z >= this->apical_range_1->min_z && z <= this->apical_range_1->max_z)
		{
			cout<<"Apical detected"<<endl;
			return Cardiac_Windows::Apical_1;
		}

		//Suprasternal
		else if(x >= this->suprasternal_range_1->min_x && x <= this->suprasternal_range_1->max_x && 
			y >= this->suprasternal_range_1->min_y && y <= this->suprasternal_range_1->max_y && 
			z >= this->suprasternal_range_1->min_z && z <= this->suprasternal_range_1->max_z)
		{
			cout<<"Suprasternal detected"<<endl;
			return Cardiac_Windows::Suprasternal_1;			
		}

		else
		{
			return Cardiac_Windows::None;
		}
	}

	virtual void Execute(vtkObject * obj, unsigned long event, void *)
	{		
		vtkRenderWindowInteractor *interactor = this->GetInteractor();
		vtkRenderer* ren = this->GetRenderer();
				
		if (event == vtkCommand::TimerEvent)
		{
			double x, y, z;
			double angle_adjust[3];
			vtkSmartPointer<vtkPlane> sliceplane = vtkSmartPointer<vtkPlane>::New();
			angle_adjust[0] = this->adjust_azimuth;
			angle_adjust[1] = this->adjust_elevation;
			angle_adjust[2] = this->adjust_roll;
						
			x = y = z = 0;

			this->current_cardiac_window = this->GetCurrentWindow(x, y, z);

			switch(this->current_cardiac_window)
			{
			case Cardiac_Windows::Parasternal_1:
				SetAlignment(sub_angle_adjust);
				break;

			case Cardiac_Windows::Subcostal_1:
				SetAlignment(sub_angle_adjust);
				break;

			case Cardiac_Windows::Suprasternal_1:
				SetAlignment(sup_angle_adjust);
				break;

			case Cardiac_Windows::Apical_1:
				SetAlignment(api_angle_adjust);
				break;

			case Cardiac_Windows::None:
				break;
			}

			vtkSmartPointer<vtkMatrix3x3> matrix = vtkSmartPointer<vtkMatrix3x3>::New();
			GetSensorOrientation(matrix);
			
			double new_norm[3];
			matrix->MultiplyPoint(default_norm, new_norm);
			this->planeWidget->SetNormal(new_norm);
			
			if(this->planeWidget->GetEnabled())
			{
				if(!this->guid_contour_actor->GetVisibility())
					this->guid_contour_actor->VisibilityOn();
				this->planeWidget->UpdatePlacement();
				this->planeWidget->GetPolyData(this->guid_plane_polydata);				
			}

			else 
				this->guid_contour_actor->VisibilityOff();
			
			switch(this->previous_cardiac_window)
			{
			case Cardiac_Windows::Parasternal_1:
				imageslice_par_1[this->current_volume_index]->SetVisibility(0);				
				break;

			case Cardiac_Windows::Apical_1:
				imageslice_api_1[this->current_volume_index]->SetVisibility(0);
				break;

			case Cardiac_Windows::Subcostal_1:
				imageslice_sub_1[this->current_volume_index]->SetVisibility(0);
				break;

			case Cardiac_Windows::Suprasternal_1:
				imageslice_sup_1[this->current_volume_index]->SetVisibility(0);
				break;

			default:
				//imageslice_noise_tipup[this->current_volume_index]->SetVisibility(0);
				break;
			}

			this->current_volume_index++;
			if(this->current_volume_index == 13)
				this->current_volume_index = 0;
						
			switch(this->current_cardiac_window)
			{
			case Cardiac_Windows::Parasternal_1:				
				imageslice_par_1[this->current_volume_index]->SetVisibility(1);
				
				sliceplane = resliceMapper_par_1[this->current_volume_index]->GetSlicePlane();
				break;

			case Cardiac_Windows::Apical_1:
				imageslice_api_1[this->current_volume_index]->SetVisibility(1);
				sliceplane = resliceMapper_api_1[this->current_volume_index]->GetSlicePlane();
				break;

			case Cardiac_Windows::Subcostal_1:
				imageslice_sub_1[this->current_volume_index]->SetVisibility(1);
				sliceplane = resliceMapper_sub_1[this->current_volume_index]->GetSlicePlane();
				break;

			case Cardiac_Windows::Suprasternal_1:
				imageslice_sup_1[this->current_volume_index]->SetVisibility(1);
				sliceplane = resliceMapper_sup_1[this->current_volume_index]->GetSlicePlane();
				break;

			default:
				imageslice_noise_tipup[this->current_volume_index]->SetVisibility(1);
				sliceplane = resliceMapper_noise_tipup[this->current_volume_index]->GetSlicePlane();
				break; 
			}

			double position[3];

			vtkSmartPointer<vtkCamera> cam = renderer->GetActiveCamera();

			if(this->current_cardiac_window != Cardiac_Windows::None)
			{
				matrix->MultiplyPoint(this->default_norm, this->normal);

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

				cam->SetPosition(position);		//Orientation of the slice plane		
				cam->SetFocalPoint(focalPt);	//Point at which my plane is fixed
				cam->SetViewUp(0,0,-1);	

				/*cam->Yaw(this->sub_angle_adjust[0]);
				cam->Elevation(this->sub_angle_adjust[1]);
				cam->Roll(this->sub_angle_adjust[2]);
				this->renderer->ResetCamera();*/
				
			}

			else
			{
				this->normal[0] = -0.07, this->normal[1] = 1, this->normal[2] = 0.02;

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
				
				cam->SetPosition(position);		
				cam->SetViewUp(0, 0, -1);
				cam->SetFocalPoint(focalPt);
			}

			interactor->Render();
			ren->ResetCameraClippingRange();

			this->previous_cardiac_window = this->current_cardiac_window;
		}

		if(event == vtkCommand::KeyPressEvent)
		{
			std::string key = interactor->GetKeySym();			

			if(key == "a" && this->par_angle_adjust[0] < 180)
			{
				for(int i = 0; i < 13; i++)
					imageslice_par_1[i]->RotateX(1);

				this->guidance_volume->RotateX(1);
				/*this->par_angle_adjust[0] += 1;
				cout<<"Azimuth :"<<this->par_angle_adjust[0]<<" Elevation :"<<this->par_angle_adjust[1]<<" Roll : "<<this->par_angle_adjust[2]<<endl;*/
			}

			else if(key == "z" && this->par_angle_adjust[0] > -180)
			{
				for(int i = 0; i < 13; i++)
					imageslice_par_1[i]->RotateX(-1);

				this->guidance_volume->RotateX(-1);
				/*this->par_angle_adjust[0] -= 1;
				cout<<"Azimuth :"<<this->par_angle_adjust[0]<<" Elevation :"<<this->par_angle_adjust[1]<<" Roll : "<<this->par_angle_adjust[2]<<endl;*/
			}

			else if(key == "s" && this->par_angle_adjust[1] < 180)
			{
				for(int i = 0; i < 13; i++)
					imageslice_par_1[i]->RotateY(1);

				this->guidance_volume->RotateY(1);
				/*this->par_angle_adjust[1] += 1;
				cout<<"Azimuth :"<<this->par_angle_adjust[0]<<" Elevation :"<<this->par_angle_adjust[1]<<" Roll : "<<this->par_angle_adjust[2]<<endl;*/
			}

			else if(key == "x" && this->par_angle_adjust[1] > -180)
			{
				for(int i = 0; i < 13; i++)
					imageslice_par_1[i]->RotateY(-1);

				this->guidance_volume->RotateY(-1);
				/*this->par_angle_adjust[1] -= 1;
				cout<<"Azimuth :"<<this->par_angle_adjust[0]<<" Elevation :"<<this->par_angle_adjust[1]<<" Roll : "<<this->par_angle_adjust[2]<<endl;*/
			}

			else if(key == "d" && this->par_angle_adjust[2] < 180)
			{
				for(int i = 0; i < 13; i++)
					imageslice_par_1[i]->RotateZ(1);

				this->guidance_volume->RotateZ(1);
				/*this->par_angle_adjust[2] += 1;
				cout<<"Azimuth :"<<this->par_angle_adjust[0]<<" Elevation :"<<this->par_angle_adjust[1]<<" Roll : "<<this->par_angle_adjust[2]<<endl;*/
			}

			else if(key == "c" && this->par_angle_adjust[2] > -180)
			{
				for(int i = 0; i < 13; i++)
					imageslice_par_1[i]->RotateZ(-1);

				this->guidance_volume->RotateZ(-1);
				/*this->par_angle_adjust[2] -= 1;
				cout<<"Azimuth :"<<this->par_angle_adjust[0]<<" Elevation :"<<this->par_angle_adjust[1]<<" Roll : "<<this->par_angle_adjust[2]<<endl;*/
			}

			else if(key == "m"){
				this->par_angle_adjust[0] = this->par_angle_adjust[1] = this->par_angle_adjust[2] = 0;
			}

			if(key == "g" && this->sub_angle_adjust[0] < 180)
			{
				this->sub_angle_adjust[0] += 1;
				cout<<"Azimuth :"<<this->sub_angle_adjust[0]<<" Elevation :"<<this->sub_angle_adjust[1]<<" Roll : "<<this->sub_angle_adjust[2]<<endl;
			}

			else if(key == "b" && this->sub_angle_adjust[0] > -180)
			{
				this->sub_angle_adjust[0] -= 1;
				cout<<"Azimuth :"<<this->sub_angle_adjust[0]<<" Elevation :"<<this->sub_angle_adjust[1]<<" Roll : "<<this->sub_angle_adjust[2]<<endl;
			}

			else if(key == "h" && this->sub_angle_adjust[1] < 180)
			{
				this->sub_angle_adjust[1] += 1;
				cout<<"Azimuth :"<<this->sub_angle_adjust[0]<<" Elevation :"<<this->sub_angle_adjust[1]<<" Roll : "<<this->sub_angle_adjust[2]<<endl;
			}

			else if(key == "n" && this->sub_angle_adjust[1] > -180)
			{
				this->sub_angle_adjust[1] -= 1;
				cout<<"Azimuth :"<<this->sub_angle_adjust[0]<<" Elevation :"<<this->sub_angle_adjust[1]<<" Roll : "<<this->sub_angle_adjust[2]<<endl;
			}

			else if(key == "j" && this->sub_angle_adjust[2] < 180)
			{
				this->sub_angle_adjust[2] += 1;
				cout<<"Azimuth :"<<this->sub_angle_adjust[0]<<" Elevation :"<<this->sub_angle_adjust[1]<<" Roll : "<<this->sub_angle_adjust[2]<<endl;
			}

			else if(key == "m" && this->sub_angle_adjust[2] > -180)
			{
				this->sub_angle_adjust[2] -= 1;
				cout<<"Azimuth :"<<this->sub_angle_adjust[0]<<" Elevation :"<<this->sub_angle_adjust[1]<<" Roll : "<<this->sub_angle_adjust[2]<<endl;
			}
		}
			
		if(event == vtkCommand::StartInteractionEvent)
		{	
			vtkPlaneWidget::SafeDownCast(obj)->GetPolyData(this->guid_plane_polydata);
			this->guid_contour_actor->VisibilityOn();
		}

		if(event == vtkCommand::InteractionEvent)
		{
			vtkPlaneWidget::SafeDownCast(obj)->GetPolyData(this->guid_plane_polydata);
		}
};

public:

	vtkSmartPointer<vtkImageSlice> imageslice_par_1[13];
	vtkSmartPointer<vtkImageSlice> imageslice_par_2[13];
	vtkSmartPointer<vtkImageSlice> imageslice_api_1[13];
	vtkSmartPointer<vtkImageSlice> imageslice_api_2[13];
	vtkSmartPointer<vtkImageSlice> imageslice_sub_1[13];
	vtkSmartPointer<vtkImageSlice> imageslice_sub_2[13];
	vtkSmartPointer<vtkImageSlice> imageslice_sup_1[13];
	vtkSmartPointer<vtkImageSlice> imageslice_sup_2[13];
	vtkSmartPointer<vtkImageSlice> imageslice_noise_tipup[13];
	vtkSmartPointer<vtkImageSlice> imageslice_noise_tipdown[13];

	vtkSmartPointer<vtkImageResliceMapper> resliceMapper_par_1[13];
	vtkSmartPointer<vtkImageResliceMapper> resliceMapper_par_2[13];
	vtkSmartPointer<vtkImageResliceMapper> resliceMapper_api_1[13];
	vtkSmartPointer<vtkImageResliceMapper> resliceMapper_api_2[13];
	vtkSmartPointer<vtkImageResliceMapper> resliceMapper_sup_1[13];
	vtkSmartPointer<vtkImageResliceMapper> resliceMapper_sup_2[13];
	vtkSmartPointer<vtkImageResliceMapper> resliceMapper_sub_1[13];
	vtkSmartPointer<vtkImageResliceMapper> resliceMapper_sub_2[13];
	vtkSmartPointer<vtkImageResliceMapper> resliceMapper_noise_tipup[13];
	vtkSmartPointer<vtkImageResliceMapper> resliceMapper_noise_tipdown[13];

	vtkSmartPointer<vtkActor> guidance_volume;
	vtkSmartPointer<vtkActor> guidance_cutplane;
		
	vtkSmartPointer<vtkPolyData> guid_plane_polydata;
	vtkSmartPointer<vtkActor> guid_contour_actor;
	vtkSmartPointer<vtkPlaneWidget> planeWidget;

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

	int current_volume_index;
	Cardiac_Windows current_cardiac_window, previous_cardiac_window;
	
	Range *parasternal_range_1;
	Range *apical_range_1;
	Range *subcostal_range_1;
	Range *suprasternal_range_1;

	int x_view;
	int y_view;
	int z_view;

	double par_image_alignment[3];

	bool temp_turnOn;
	vtkRenderer* guid_renderer;
	vtkSmartPointer<vtkRenderer> throw_plane_renderer;
	vtkSmartPointer<vtkActor> throw_plane_actor;
	vtkSmartPointer<vtkProbeFilter> probe;
	vtkSmartPointer<vtkMarchingCubes> mc;
	vtkSmartPointer<vtkPolyDataMapper> contourMapper;

	int mc_i, mc_threshold;
	vtkRenderWindowInteractor *Interactor;

private: 

	int Slicing;
	
	vtkRenderer* renderer;
	
	vtkRenderWindow* renWin;

	double adjust_roll;
	double adjust_elevation;
	double adjust_azimuth;

	double par_angle_adjust[3];
	double sub_angle_adjust[3];
	double sup_angle_adjust[3];
	double api_angle_adjust[3];
	double default_norm[3];
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
	vtkSmartPointer<vtkRenderWindow> window =
		vtkSmartPointer<vtkRenderWindow>::New();	
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();	
	vtkSmartPointer<vtkInteractorStyleImage> imageStyle =
		vtkSmartPointer<vtkInteractorStyleImage>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();

	// Define viewport ranges
	double guidance_viewport[4] = {0.0, 0.0, 0.5, 1.0};
	double slice_viewport[4] = {0.5, 0.0, 1.0, 1.0};

	renderer->SetViewport(slice_viewport);	
	window->AddRenderer(renderer);
	
	//window->FullScreenOn();
	renderer->SetBackground(0.03, 0.03, 0.03);
	


#pragma region File Names

	char par_vol_1[13][98] = {
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\1\\Par_1_0.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\1\\Par_1_1.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\1\\Par_1_2.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\1\\Par_1_3.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\1\\Par_1_4.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\1\\Par_1_5.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\1\\Par_1_6.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\1\\Par_1_7.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\1\\Par_1_8.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\1\\Par_1_9.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\1\\Par_1_10.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\1\\Par_1_11.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\1\\Par_1_12.dcm"
	};

	char par_vol_2[13][98] = {
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\2\\Par_2_0.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\2\\Par_2_1.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\2\\Par_2_2.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\2\\Par_2_3.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\2\\Par_2_4.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\2\\Par_2_5.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\2\\Par_2_6.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\2\\Par_2_7.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\2\\Par_2_8.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\2\\Par_2_9.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\2\\Par_2_10.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\2\\Par_2_11.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\2\\Par_2_12.dcm"
	};

	char api_vol_1[13][98] = {
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\1\\Api_1_0.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\1\\Api_1_1.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\1\\Api_1_2.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\1\\Api_1_3.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\1\\Api_1_4.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\1\\Api_1_5.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\1\\Api_1_6.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\1\\Api_1_7.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\1\\Api_1_8.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\1\\Api_1_9.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\1\\Api_1_10.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\1\\Api_1_11.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\1\\Api_1_12.dcm"
	};

	char api_vol_2[13][98] = {
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\2\\Api_2_0.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\2\\Api_2_1.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\2\\Api_2_2.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\2\\Api_2_3.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\2\\Api_2_4.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\2\\Api_2_5.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\2\\Api_2_6.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\2\\Api_2_7.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\2\\Api_2_8.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\2\\Api_2_9.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\2\\Api_2_10.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\2\\Api_2_11.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\2\\Api_2_12.dcm"
	};

	char sub_vol_1[13][98] = {
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\1\\Sub_1_0.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\1\\Sub_1_1.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\1\\Sub_1_2.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\1\\Sub_1_3.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\1\\Sub_1_4.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\1\\Sub_1_5.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\1\\Sub_1_6.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\1\\Sub_1_7.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\1\\Sub_1_8.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\1\\Sub_1_9.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\1\\Sub_1_10.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\1\\Sub_1_11.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\1\\Sub_1_12.dcm"
	};

	char sub_vol_2[13][98] = {
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\2\\Sub_2_0.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\2\\Sub_2_1.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\2\\Sub_2_2.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\2\\Sub_2_3.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\2\\Sub_2_4.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\2\\Sub_2_5.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\2\\Sub_2_6.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\2\\Sub_2_7.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\2\\Sub_2_8.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\2\\Sub_2_9.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\2\\Sub_2_10.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\2\\Sub_2_11.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\2\\Sub_2_12.dcm"
	};

	char sup_vol_1[13][98] = {
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\1\\Sup_1_0.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\1\\Sup_1_1.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\1\\Sup_1_2.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\1\\Sup_1_3.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\1\\Sup_1_4.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\1\\Sup_1_5.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\1\\Sup_1_6.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\1\\Sup_1_7.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\1\\Sup_1_8.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\1\\Sup_1_9.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\1\\Sup_1_10.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\1\\Sup_1_11.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\1\\Sup_1_12.dcm"
	};

	char sup_vol_2[13][98] = {
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\2\\Sup_2_0.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\2\\Sup_2_1.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\2\\Sup_2_2.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\2\\Sup_2_3.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\2\\Sup_2_4.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\2\\Sup_2_5.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\2\\Sup_2_6.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\2\\Sup_2_7.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\2\\Sup_2_8.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\2\\Sup_2_9.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\2\\Sup_2_10.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\2\\Sup_2_11.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\2\\Sup_2_12.dcm"
	};

	char noise_tip_down[13][98] = 
	{
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipDown_0.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipDown_1.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipDown_2.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipDown_3.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipDown_4.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipDown_5.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipDown_6.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipDown_7.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipDown_8.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipDown_9.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipDown_0.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipDown_1.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipDown_2.dcm",
	};

	char noise_tip_up[13][98] = 
	{
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipUp_0.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipUp_1.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipUp_2.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipUp_3.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipUp_4.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipUp_5.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipUp_6.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipUp_7.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipUp_8.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipUp_9.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipUp_0.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipUp_1.dcm",
		"C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\Noise_TipUp_2.dcm",
	};
#pragma endregion

	typedef short InputPixelType;
	typedef itk::Image<InputPixelType, 3> InputImageType;
	typedef itk::ImageFileReader<InputImageType> ReaderType;
	typedef itk::GDCMImageIO ImageIOType;
	typedef itk::VTKImageExport<InputImageType> ExportFilterType;
	ImageIOType::Pointer gdcmImageIO = ImageIOType::New();

	vtkSmartPointer<vtkImageProperty> image_property = vtkSmartPointer<vtkImageProperty>::New();
	image_property->SetColorWindow(214);
	image_property->SetColorLevel(100);
	image_property->SetAmbient(0.0);
	image_property->SetDiffuse(1.0);
	image_property->SetOpacity(1.0);
	image_property->SetInterpolationTypeToLinear();
	
	ReaderType::Pointer reader_par_1[13];	
	ReaderType::Pointer reader_par_2[13];
	ReaderType::Pointer reader_api_1[13];
	ReaderType::Pointer reader_api_2[13];
	ReaderType::Pointer reader_sub_1[13];
	ReaderType::Pointer reader_sub_2[13];
	ReaderType::Pointer reader_sup_1[13];
	ReaderType::Pointer reader_sup_2[13];
	
	ExportFilterType::Pointer itkExporter_par_1[13];
	ExportFilterType::Pointer itkExporter_par_2[13];
	ExportFilterType::Pointer itkExporter_api_1[13];
	ExportFilterType::Pointer itkExporter_api_2[13];
	ExportFilterType::Pointer itkExporter_sub_1[13];
	ExportFilterType::Pointer itkExporter_sub_2[13];
	ExportFilterType::Pointer itkExporter_sup_1[13];
	ExportFilterType::Pointer itkExporter_sup_2[13];
	
	ReaderType::Pointer reader_noise_tipup[13];
	ReaderType::Pointer reader_noise_tipdown[13];
	ExportFilterType::Pointer itkExporter_noise_tipup[13];
	ExportFilterType::Pointer itkExporter_noise_tipdown[13];

	//Objects for guidance
	vtkSmartPointer<vtkMarchingCubes> mc = vtkSmartPointer<vtkMarchingCubes>::New();
	vtkSmartPointer<vtkPolyDataMapper> mapper =	vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkActor> isoActor = vtkSmartPointer<vtkActor>::New();
	//End
	vtkSmartPointer<vtkPlaneWidget> planeWidget = vtkSmartPointer<vtkPlaneWidget>::New();
	for(int i = 0; i < 13; i++)
	{		
		reader_par_1[i] = ReaderType::New();
		reader_par_1[i]->SetFileName(par_vol_1[i]);
		reader_par_1[i]->SetImageIO(gdcmImageIO);
		reader_par_1[i]->Update();

		callback->dimX = reader_par_1[i]->GetImageIO()->GetDimensions(0);
		callback->dimY = reader_par_1[i]->GetImageIO()->GetDimensions(1);
		callback->dimZ = reader_par_1[i]->GetImageIO()->GetDimensions(2);
		
		itkExporter_par_1[i] = ExportFilterType::New();
		vtkImageImport *vtkImporter = vtkImageImport::New();
		itkExporter_par_1[i]->SetInput(reader_par_1[i]->GetOutput());

		ConnectPipelines(itkExporter_par_1[i], vtkImporter);

		callback->resliceMapper_par_1[i] = vtkSmartPointer<vtkImageResliceMapper>::New();
		callback->resliceMapper_par_1[i]->SetInputConnection(vtkImporter->GetOutputPort());
		callback->resliceMapper_par_1[i]->SliceFacesCameraOn();
		callback->resliceMapper_par_1[i]->SliceAtFocalPointOn();
		callback->resliceMapper_par_1[i]->BorderOff();

		callback->imageslice_par_1[i] = vtkSmartPointer<vtkImageSlice>::New();
		callback->imageslice_par_1[i]->SetMapper(callback->resliceMapper_par_1[i]);
		callback->imageslice_par_1[i]->SetProperty(image_property);
		callback->imageslice_par_1[i]->SetVisibility(0);
		callback->imageslice_par_1[i]->SetOrigin(callback->dimX/2, callback->dimY/2, 0);
		renderer->AddViewProp(callback->imageslice_par_1[i]);

		//Adding the parasternal 3D image for guidance
		if(i == 0)
		{
			// Create a 3D model using marching cubes
			vtkSmartPointer<vtkImageGaussianSmooth> gaussianSmoothFilter = 
				vtkSmartPointer<vtkImageGaussianSmooth>::New(); 
			gaussianSmoothFilter->SetInputConnection(vtkImporter->GetOutputPort()); 
			gaussianSmoothFilter->SetRadiusFactor(200); 
			gaussianSmoothFilter->Update(); 

			mc->SetInputConnection(gaussianSmoothFilter->GetOutputPort());
			mc->ComputeNormalsOn();
			mc->ComputeGradientsOn();
			mc->SetNumberOfContours(10);
			mc->SetValue(0,1); 
			mc->SetValue(1, 50);
			mc->SetValue(2, 100);
			mc->SetValue(3, 150);
			mc->SetValue(4, 200);
			mc->SetValue(5, 255);
			//mc->SetValue(, 0);

			mapper->SetInputConnection(mc->GetOutputPort());
			mapper->ScalarVisibilityOff();
			isoActor->SetVisibility(1);
			isoActor->SetMapper(mapper);
			isoActor->GetProperty()->SetOpacity(0.1);
			isoActor->GetProperty()->SetColor(1, 0, 0);
			isoActor->SetOrigin(callback->dimX/2, callback->dimY/2, 0);
			callback->guidance_volume = isoActor;
			callback->mc = mc;
			//End



			//Taken from ProbingWithPlaneWidget.py
			
			planeWidget->SetInput(mc->GetOutput());	
			planeWidget->NormalToXAxisOn();
			planeWidget->SetResolution(20);
			planeWidget->SetRepresentationToOutline();
			planeWidget->GetPlaneProperty()->SetColor(0,0,0);
			planeWidget->PlaceWidget();
			//planeWidget->EnabledOn();
			double origin[] = {0,0,0};
			double point1[] = {350, 0, 0};
			double point2[] = {0, 350, 0};
			double center[] = {callback->dimX/2, callback->dimY/2, 0};

			planeWidget->SetOrigin(origin);
			planeWidget->SetPoint1(point1);
			planeWidget->SetPoint2(point2);
			planeWidget->SetCenter(center);
			callback->planeWidget = planeWidget;

			vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
			planeWidget->GetPolyData(polydata);
			callback->guid_plane_polydata = polydata;

			callback->probe = vtkSmartPointer<vtkProbeFilter>::New();
			callback->probe->SetInput(polydata);
			callback->probe->SetSource(vtkImporter->GetOutput());

			callback->contourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
			callback->contourMapper->SetInputConnection(callback->probe->GetOutputPort());
			callback->contourMapper->SetLookupTable(image_property->GetLookupTable());
		}
	}

	

	vtkSmartPointer<vtkActor> contourActor = vtkSmartPointer<vtkActor>::New();
	contourActor->SetMapper(callback->contourMapper);
	contourActor->VisibilityOff();
	callback->guid_contour_actor = contourActor;

	// An outline is shown for context
	/*vtkSmartPointer<vtkStructuredGridOutlineFilter> outline = vtkSmartPointer<vtkStructuredGridOutlineFilter>::New();
	outline->SetInputConnection(mc->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	outlineMapper->SetInputConnection(outline->GetOutputPort());

	vtkSmartPointer<vtkActor> outlineActor = vtkSmartPointer<vtkActor>::New();
	outlineActor->SetMapper(outlineMapper);*/
	

	// Associate the widget with the interactor
	planeWidget->SetInteractor(interactor);
	// Handle the events->
	planeWidget->AddObserver(vtkCommand::EnableEvent, callback);
	planeWidget->AddObserver(vtkCommand::StartInteractionEvent, callback);
	planeWidget->AddObserver(vtkCommand::InteractionEvent, callback);
	
	//Adding guidance actor
	vtkSmartPointer<vtkRenderer> guid_renderer = vtkSmartPointer<vtkRenderer>::New();

	callback->throw_plane_renderer = vtkSmartPointer<vtkRenderer>::New();
	callback->throw_plane_renderer->SetViewport(slice_viewport);	
	callback->throw_plane_renderer->SetBackground(1,1,1);
	callback->throw_plane_actor = vtkSmartPointer<vtkActor>::New();
	callback->throw_plane_actor->SetMapper(callback->contourMapper);
	callback->throw_plane_renderer->AddActor(callback->throw_plane_actor);
	//window->AddRenderer(callback->throw_plane_renderer);
	
	guid_renderer->SetViewport(guidance_viewport);
	guid_renderer->AddActor(isoActor);
	// Add the actors to the renderer, set the background and size
	//guid_renderer->AddActor(outlineActor);
	guid_renderer->AddActor(contourActor);
	//guid_renderer->AddActor(plane_actor);
	guid_renderer->SetBackground(1,1,1);
	window->AddRenderer(guid_renderer);
	callback->guid_renderer = guid_renderer;
	
	/*vtkSmartPointer<vtkCamera> guid_cam = guid_renderer->GetActiveCamera();
	guid_cam->Elevation(90);
	guid_renderer->ResetCamera();
	guid_cam->Roll(180);
	guid_renderer->ResetCamera();*/

	
	//callback->guidance_cutplane = plane_actor;

#pragma region Read Volumes

	//for(int i = 0; i < 13; i++)
	//{		
	//	reader_par_2[i] = ReaderType::New();
	//	reader_par_2[i]->SetFileName(par_vol_2[i]);
	//	reader_par_2[i]->SetImageIO(gdcmImageIO);
	//	reader_par_2[i]->Update();

	//	/*callback->dimX = reader_par_2[i]->GetImageIO()->GetDimensions(0);
	//	callback->dimY = reader_par_2[i]->GetImageIO()->GetDimensions(1);
	//	callback->dimZ = reader_par_2[i]->GetImageIO()->GetDimensions(2);*/
	//	
	//	itkExporter_par_2[i] = ExportFilterType::New();
	//	vtkImageImport *vtkImporter = vtkImageImport::New();
	//	itkExporter_par_2[i]->SetInput(reader_par_2[i]->GetOutput());

	//	ConnectPipelines(itkExporter_par_2[i], vtkImporter);

	//	callback->resliceMapper_par_2[i] = vtkSmartPointer<vtkImageResliceMapper>::New();
	//	callback->resliceMapper_par_2[i]->SetInputConnection(vtkImporter->GetOutputPort());
	//	callback->resliceMapper_par_2[i]->SliceFacesCameraOn();
	//	callback->resliceMapper_par_2[i]->SliceAtFocalPointOn();
	//	callback->resliceMapper_par_2[i]->BorderOff();

	//	callback->imageslice_par_2[i] = vtkSmartPointer<vtkImageSlice>::New();
	//	callback->imageslice_par_2[i]->SetMapper(callback->resliceMapper_par_2[i]);
	//	callback->imageslice_par_2[i]->SetProperty(image_property);
	//	callback->imageslice_par_2[i]->SetVisibility(0);
	//	renderer->AddViewProp(callback->imageslice_par_2[i]);
	//}

	//for(int i = 0; i < 13; i++)
	//{		
	//	reader_api_1[i] = ReaderType::New();
	//	reader_api_1[i]->SetFileName(api_vol_1[i]);
	//	reader_api_1[i]->SetImageIO(gdcmImageIO);
	//	reader_api_1[i]->Update();

	//	/*callback->dimX = reader_api_1[i]->GetImageIO()->GetDimensions(0);
	//	callback->dimY = reader_api_1[i]->GetImageIO()->GetDimensions(1);
	//	callback->dimZ = reader_api_1[i]->GetImageIO()->GetDimensions(2);*/
	//	
	//	itkExporter_api_1[i] = ExportFilterType::New();
	//	vtkImageImport *vtkImporter = vtkImageImport::New();
	//	itkExporter_api_1[i]->SetInput(reader_api_1[i]->GetOutput());

	//	ConnectPipelines(itkExporter_api_1[i], vtkImporter);

	//	callback->resliceMapper_api_1[i] = vtkSmartPointer<vtkImageResliceMapper>::New();
	//	callback->resliceMapper_api_1[i]->SetInputConnection(vtkImporter->GetOutputPort());
	//	callback->resliceMapper_api_1[i]->SliceFacesCameraOn();
	//	callback->resliceMapper_api_1[i]->SliceAtFocalPointOn();
	//	callback->resliceMapper_api_1[i]->BorderOff();

	//	callback->imageslice_api_1[i] = vtkSmartPointer<vtkImageSlice>::New();
	//	callback->imageslice_api_1[i]->SetMapper(callback->resliceMapper_api_1[i]);
	//	callback->imageslice_api_1[i]->SetProperty(image_property);
	//	callback->imageslice_api_1[i]->SetVisibility(0);
	//	renderer->AddViewProp(callback->imageslice_api_1[i]);
	//}

	//for(int i = 0; i < 13; i++)
	//{		
	//	reader_api_2[i] = ReaderType::New();
	//	reader_api_2[i]->SetFileName(api_vol_2[i]);
	//	reader_api_2[i]->SetImageIO(gdcmImageIO);
	//	reader_api_2[i]->Update();

	//	/*callback->dimX = reader_api_2[i]->GetImageIO()->GetDimensions(0);
	//	callback->dimY = reader_api_2[i]->GetImageIO()->GetDimensions(1);
	//	callback->dimZ = reader_api_2[i]->GetImageIO()->GetDimensions(2);*/
	//	
	//	itkExporter_api_2[i] = ExportFilterType::New();
	//	vtkImageImport *vtkImporter = vtkImageImport::New();
	//	itkExporter_api_2[i]->SetInput(reader_api_2[i]->GetOutput());

	//	ConnectPipelines(itkExporter_api_2[i], vtkImporter);

	//	callback->resliceMapper_api_2[i] = vtkSmartPointer<vtkImageResliceMapper>::New();
	//	callback->resliceMapper_api_2[i]->SetInputConnection(vtkImporter->GetOutputPort());
	//	callback->resliceMapper_api_2[i]->SliceFacesCameraOn();
	//	callback->resliceMapper_api_2[i]->SliceAtFocalPointOn();
	//	callback->resliceMapper_api_2[i]->BorderOff();

	//	callback->imageslice_api_2[i] = vtkSmartPointer<vtkImageSlice>::New();
	//	callback->imageslice_api_2[i]->SetMapper(callback->resliceMapper_api_2[i]);
	//	callback->imageslice_api_2[i]->SetProperty(image_property);
	//	callback->imageslice_api_2[i]->SetVisibility(0);
	//	renderer->AddViewProp(callback->imageslice_api_2[i]);
	//}

	//for(int i = 0; i < 13; i++)
	//{		
	//	reader_sub_1[i] = ReaderType::New();
	//	reader_sub_1[i]->SetFileName(sub_vol_1[i]);
	//	reader_sub_1[i]->SetImageIO(gdcmImageIO);
	//	reader_sub_1[i]->Update();

	//	/*callback->dimX = reader_sub_1[i]->GetImageIO()->GetDimensions(0);
	//	callback->dimY = reader_sub_1[i]->GetImageIO()->GetDimensions(1);
	//	callback->dimZ = reader_sub_1[i]->GetImageIO()->GetDimensions(2);*/
	//	
	//	itkExporter_sub_1[i] = ExportFilterType::New();
	//	vtkImageImport *vtkImporter = vtkImageImport::New();
	//	itkExporter_sub_1[i]->SetInput(reader_sub_1[i]->GetOutput());

	//	ConnectPipelines(itkExporter_sub_1[i], vtkImporter);

	//	callback->resliceMapper_sub_1[i] = vtkSmartPointer<vtkImageResliceMapper>::New();
	//	callback->resliceMapper_sub_1[i]->SetInputConnection(vtkImporter->GetOutputPort());
	//	callback->resliceMapper_sub_1[i]->SliceFacesCameraOn();
	//	callback->resliceMapper_sub_1[i]->SliceAtFocalPointOn();
	//	callback->resliceMapper_sub_1[i]->BorderOff();

	//	callback->imageslice_sub_1[i] = vtkSmartPointer<vtkImageSlice>::New();
	//	callback->imageslice_sub_1[i]->SetMapper(callback->resliceMapper_sub_1[i]);
	//	callback->imageslice_sub_1[i]->SetProperty(image_property);
	//	callback->imageslice_sub_1[i]->SetVisibility(0);
	//	renderer->AddViewProp(callback->imageslice_sub_1[i]);
	//}

	//for(int i = 0; i < 13; i++)
	//{		
	//	reader_sub_2[i] = ReaderType::New();
	//	reader_sub_2[i]->SetFileName(sub_vol_2[i]);
	//	reader_sub_2[i]->SetImageIO(gdcmImageIO);
	//	reader_sub_2[i]->Update();

	//	/*callback->dimX = reader_sub_2[i]->GetImageIO()->GetDimensions(0);
	//	callback->dimY = reader_sub_2[i]->GetImageIO()->GetDimensions(1);
	//	callback->dimZ = reader_sub_2[i]->GetImageIO()->GetDimensions(2);*/
	//	
	//	itkExporter_sub_2[i] = ExportFilterType::New();
	//	vtkImageImport *vtkImporter = vtkImageImport::New();
	//	itkExporter_sub_2[i]->SetInput(reader_sub_2[i]->GetOutput());

	//	ConnectPipelines(itkExporter_sub_2[i], vtkImporter);

	//	callback->resliceMapper_sub_2[i] = vtkSmartPointer<vtkImageResliceMapper>::New();
	//	callback->resliceMapper_sub_2[i]->SetInputConnection(vtkImporter->GetOutputPort());
	//	callback->resliceMapper_sub_2[i]->SliceFacesCameraOn();
	//	callback->resliceMapper_sub_2[i]->SliceAtFocalPointOn();
	//	callback->resliceMapper_sub_2[i]->BorderOff();

	//	callback->imageslice_sub_2[i] = vtkSmartPointer<vtkImageSlice>::New();
	//	callback->imageslice_sub_2[i]->SetMapper(callback->resliceMapper_sub_2[i]);
	//	callback->imageslice_sub_2[i]->SetProperty(image_property);
	//	callback->imageslice_sub_2[i]->SetVisibility(0);
	//	renderer->AddViewProp(callback->imageslice_sub_2[i]);
	//}

	//for(int i = 0; i < 13; i++)
	//{		
	//	reader_sup_1[i] = ReaderType::New();
	//	reader_sup_1[i]->SetFileName(sup_vol_1[i]);
	//	reader_sup_1[i]->SetImageIO(gdcmImageIO);
	//	reader_sup_1[i]->Update();

	//	/*callback->dimX = reader_sup_1[i]->GetImageIO()->GetDimensions(0);
	//	callback->dimY = reader_sup_1[i]->GetImageIO()->GetDimensions(1);
	//	callback->dimZ = reader_sup_1[i]->GetImageIO()->GetDimensions(2);*/
	//	
	//	itkExporter_sup_1[i] = ExportFilterType::New();
	//	vtkImageImport *vtkImporter = vtkImageImport::New();
	//	itkExporter_sup_1[i]->SetInput(reader_sup_1[i]->GetOutput());

	//	ConnectPipelines(itkExporter_sup_1[i], vtkImporter);

	//	callback->resliceMapper_sup_1[i] = vtkSmartPointer<vtkImageResliceMapper>::New();
	//	callback->resliceMapper_sup_1[i]->SetInputConnection(vtkImporter->GetOutputPort());
	//	callback->resliceMapper_sup_1[i]->SliceFacesCameraOn();
	//	callback->resliceMapper_sup_1[i]->SliceAtFocalPointOn();
	//	callback->resliceMapper_sup_1[i]->BorderOff();

	//	callback->imageslice_sup_1[i] = vtkSmartPointer<vtkImageSlice>::New();
	//	callback->imageslice_sup_1[i]->SetMapper(callback->resliceMapper_sup_1[i]);
	//	callback->imageslice_sup_1[i]->SetProperty(image_property);
	//	callback->imageslice_sup_1[i]->SetVisibility(0);
	//	renderer->AddViewProp(callback->imageslice_sup_1[i]);
	//}

	//for(int i = 0; i < 13; i++)
	//{		
	//	reader_sup_2[i] = ReaderType::New();
	//	reader_sup_2[i]->SetFileName(sup_vol_2[i]);
	//	reader_sup_2[i]->SetImageIO(gdcmImageIO);
	//	reader_sup_2[i]->Update();

	//	/*callback->dimX = reader_sup_2[i]->GetImageIO()->GetDimensions(0);
	//	callback->dimY = reader_sup_2[i]->GetImageIO()->GetDimensions(1);
	//	callback->dimZ = reader_sup_2[i]->GetImageIO()->GetDimensions(2);*/
	//	
	//	itkExporter_sup_2[i] = ExportFilterType::New();
	//	vtkImageImport *vtkImporter = vtkImageImport::New();
	//	itkExporter_sup_2[i]->SetInput(reader_sup_2[i]->GetOutput());

	//	ConnectPipelines(itkExporter_sup_2[i], vtkImporter);

	//	callback->resliceMapper_sup_2[i] = vtkSmartPointer<vtkImageResliceMapper>::New();
	//	callback->resliceMapper_sup_2[i]->SetInputConnection(vtkImporter->GetOutputPort());
	//	callback->resliceMapper_sup_2[i]->SliceFacesCameraOn();
	//	callback->resliceMapper_sup_2[i]->SliceAtFocalPointOn();
	//	callback->resliceMapper_sup_2[i]->BorderOff();

	//	callback->imageslice_sup_2[i] = vtkSmartPointer<vtkImageSlice>::New();
	//	callback->imageslice_sup_2[i]->SetMapper(callback->resliceMapper_sup_2[i]);
	//	callback->imageslice_sup_2[i]->SetProperty(image_property);
	//	callback->imageslice_sup_2[i]->SetVisibility(0);
	//	renderer->AddViewProp(callback->imageslice_sup_2[i]);
	//}

	//for(int i = 0; i < 13; i++)
	//{		
	//	reader_noise_tipup[i] = ReaderType::New();
	//	reader_noise_tipup[i]->SetFileName(noise_tip_up[i]);
	//	reader_noise_tipup[i]->SetImageIO(gdcmImageIO);
	//	reader_noise_tipup[i]->Update();
	//			
	//	itkExporter_noise_tipup[i] = ExportFilterType::New();
	//	vtkImageImport *vtkImporter = vtkImageImport::New();
	//	itkExporter_noise_tipup[i]->SetInput(reader_noise_tipup[i]->GetOutput());

	//	ConnectPipelines(itkExporter_noise_tipup[i], vtkImporter);

	//	callback->resliceMapper_noise_tipup[i] = vtkSmartPointer<vtkImageResliceMapper>::New();
	//	callback->resliceMapper_noise_tipup[i]->SetInputConnection(vtkImporter->GetOutputPort());
	//	callback->resliceMapper_noise_tipup[i]->SliceFacesCameraOn();
	//	callback->resliceMapper_noise_tipup[i]->SliceAtFocalPointOn();
	//	callback->resliceMapper_noise_tipup[i]->BorderOff();

	//	callback->imageslice_noise_tipup[i] = vtkSmartPointer<vtkImageSlice>::New();
	//	callback->imageslice_noise_tipup[i]->SetMapper(callback->resliceMapper_noise_tipup[i]);
	//	callback->imageslice_noise_tipup[i]->SetProperty(image_property);
	//	callback->imageslice_noise_tipup[i]->SetVisibility(0);
	//	renderer->AddViewProp(callback->imageslice_noise_tipup[i]);
	//}

	//for(int i = 0; i < 13; i++)
	//{		
	//	reader_noise_tipdown[i] = ReaderType::New();
	//	reader_noise_tipdown[i]->SetFileName(noise_tip_down[i]);
	//	reader_noise_tipdown[i]->SetImageIO(gdcmImageIO);
	//	reader_noise_tipdown[i]->Update();

	//	/*callback->dimX = reader_noise_tipdown[i]->GetImageIO()->GetDimensions(0);
	//	callback->dimY = reader_noise_tipdown[i]->GetImageIO()->GetDimensions(1);
	//	callback->dimZ = reader_noise_tipdown[i]->GetImageIO()->GetDimensions(2);*/
	//	
	//	itkExporter_noise_tipdown[i] = ExportFilterType::New();
	//	vtkImageImport *vtkImporter = vtkImageImport::New();
	//	itkExporter_noise_tipdown[i]->SetInput(reader_noise_tipdown[i]->GetOutput());

	//	ConnectPipelines(itkExporter_noise_tipdown[i], vtkImporter);

	//	callback->resliceMapper_noise_tipdown[i] = vtkSmartPointer<vtkImageResliceMapper>::New();
	//	callback->resliceMapper_noise_tipdown[i]->SetInputConnection(vtkImporter->GetOutputPort());
	//	callback->resliceMapper_noise_tipdown[i]->SliceFacesCameraOn();
	//	callback->resliceMapper_noise_tipdown[i]->SliceAtFocalPointOn();
	//	callback->resliceMapper_noise_tipdown[i]->BorderOff();

	//	callback->imageslice_noise_tipdown[i] = vtkSmartPointer<vtkImageSlice>::New();
	//	callback->imageslice_noise_tipdown[i]->SetMapper(callback->resliceMapper_noise_tipdown[i]);
	//	callback->imageslice_noise_tipdown[i]->SetProperty(image_property);
	//	callback->imageslice_noise_tipdown[i]->SetVisibility(0);
	//	renderer->AddViewProp(callback->imageslice_noise_tipdown[i]);
	//}
	
#pragma endregion

	callback->imageslice_par_1[0]->SetVisibility(1);
	
	imageStyle->SetInteractionModeToImage3D();
	
	interactor->SetInteractorStyle(imageStyle);

	callback->SetInteractor(interactor);
	callback->SetRenderer(renderer);	
	callback->SetRenderWindow(window);

	window->SetInteractor(interactor);
	//window->BordersOff();

	window->Render();

	vtkSmartPointer<vtkCamera> cam = renderer->GetActiveCamera();
	cam->ParallelProjectionOn();	
	cam->Zoom(1.5);

	renderer->ResetCameraClippingRange();	

	callback->imageslice_par_1[0]->SetVisibility(0);

	window->Render();
	interactor->AddObserver(vtkCommand::TimerEvent, callback);
	interactor->AddObserver(vtkCommand::KeyPressEvent, callback);

	interactor->CreateRepeatingTimer(1);

	interactor->Start();

	return EXIT_SUCCESS;
}

//Applying lookup tables
//vtkLookupTable* lut = vtkLookupTable::New();
//lut->SetNumberOfColors(25);
//double opacity = 1;
///*lut->SetTableValue(0, 0, 0, 1, opacity);
//lut->SetTableValue(1, 0, 1.0, 0, opacity);
//lut->SetTableValue(2, 0.6, 1.0, 0.0, opacity);
//lut->SetTableValue(3, 1.0, 1.0, 0.0, 0.7);
//lut->SetTableValue(4, 1.0, 0.8, 0.0, opacity);
//lut->SetTableValue(5, 1.0, 0.4, 0.0, opacity);
//lut->SetTableValue(6, 1.0, 0.0, 0.0, 1);
//lut->SetTableRange(0.0,6.0);*/

//for(int i = 0; i < 255; i+=10)
//	lut->SetTableValue(i, i, 0, 0, opacity);

//lut->Build();
//callback->contourMapper->SetInputConnection(callback->probe->GetOutputPort());
////callback->contourMapper->SetScalarRange(mc->GetOutput()->GetScalarRange());
//callback->contourMapper->SetLookupTable(lut);