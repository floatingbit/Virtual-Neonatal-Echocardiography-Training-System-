#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <vector>
#include <string>
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
#include "vtkLinearTransform.h"

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

using namespace std;

#define M_PI 3.1415926
#define Deg_to_Rad(X) (X*M_PI/180.0) 
#define Rad_to_Deg(X) (X*180.0/M_PI) 

float temp = 0.6;

enum Cardiac_Windows
{
	Parasternal,	
	Apical,
	Suprasternal,
	Subcostal_1,
	Subcostal_2,
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
			this->current_cardiac_window = this->previous_cardiac_window = Cardiac_Windows::None;

			this->parasternal_range_1 = new Range(0, 100, 0, 100, 0, 100, 4.2, 4.84, 0.14, 0.99, -3.8, -2.6);
			this->apical_range_1 = new Range(0, 100, 0, 100, 0, 100, 3.5, 4.2, 1.8, 2.1, -3, -1);
			this->suprasternal_range_1 = new Range(0, 100, 0, 100, 0, 100, 5.7, 6.2, -0.4, 0.4, -3, -1);
			this->subcostal_range_1 = new Range(0, 100, 0, 100, 0, 100, 2.5, 3.0, -0.18, 0.36, -3.8, -2.6);

			this->par_angle_adjust[0] = 180;
			this->par_angle_adjust[1] = -6;
			this->par_angle_adjust[2] = 152;

			this->sup_angle_adjust[0] = 17;
			this->sup_angle_adjust[1] = -123;
			this->sup_angle_adjust[2] = 37;
			
			this->api_angle_adjust[0] = 72;
			this->api_angle_adjust[1] = -14;
			this->api_angle_adjust[2] = -122;
						
			this->sub_angle_adjust_1[0] = 0;
			this->sub_angle_adjust_1[1] = 73;
			this->sub_angle_adjust_1[2] = 180;
			
			this->sub_angle_adjust_2[0] = 0;
			this->sub_angle_adjust_2[1] = 0;
			this->sub_angle_adjust_2[2] = 0;
			
			this->default_norm[0] = 0;
			this->default_norm[1] = 0;
			this->default_norm[2] = 1;

			this->default_viewup[0] = 0;
			this->default_viewup[1] = 0;
			this->default_viewup[2] = -1;
			this->default_viewup[3] = 0;

			this->view_up[0] = 0;
			this->view_up[1] = 0;
			this->view_up[2] = -1;
			this->view_up[3] = 0;
			
			this->win_center_x = 0;
			this->win_center_y = -0.6;			
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
							this->interactor = interactor; };

	vtkRenderWindowInteractor *GetInteractor() {
								return this->interactor; };

	Cardiac_Windows GetCurrentWindow(double x, double y, double z)
	{		
		if(x >= this->parasternal_range_1->min_x && x <= this->parasternal_range_1->max_x && 
			y >= this->parasternal_range_1->min_y && y <= this->parasternal_range_1->max_y && 
			z >= this->parasternal_range_1->min_z && z <= this->parasternal_range_1->max_z)
		{
			//cout<<"Parasternal detected"<<endl;
			return Cardiac_Windows::Parasternal;
		}

		//Subcostal
		else if(x >= this->subcostal_range_1->min_x && x <= this->subcostal_range_1->max_x && 
			y >= this->subcostal_range_1->min_y && y <= this->subcostal_range_1->max_y && 
			z >= this->subcostal_range_1->min_z && z <= this->subcostal_range_1->max_z)
		{
			//cout<<"Subcostal detected"<<endl;
			return Cardiac_Windows::Subcostal_1;
		}

		//Apical
		else if(x >= this->apical_range_1->min_x && x <= this->apical_range_1->max_x && 
			y >= this->apical_range_1->min_y && y <= this->apical_range_1->max_y && 
			z >= this->apical_range_1->min_z && z <= this->apical_range_1->max_z)
		{
			//cout<<"Apical detected"<<endl;
			return Cardiac_Windows::Apical;
		}

		//Suprasternal
		else if(x >= this->suprasternal_range_1->min_x && x <= this->suprasternal_range_1->max_x && 
			y >= this->suprasternal_range_1->min_y && y <= this->suprasternal_range_1->max_y && 
			z >= this->suprasternal_range_1->min_z && z <= this->suprasternal_range_1->max_z)
		{
			//cout<<"Suprasternal detected"<<endl;
			return Cardiac_Windows::Suprasternal;			
		}

		else
		{
			return Cardiac_Windows::None;
		}
	}

	virtual void Execute(vtkObject * obj, unsigned long event, void *)
	{					
		if (event == vtkCommand::TimerEvent)
		{
			double x, y, z;
						
			GetSensorPosition(&x, &y, &z);

			this->current_cardiac_window = Parasternal;// this->GetCurrentWindow(x, y, z);			
			
			vtkSmartPointer<vtkMatrix3x3> matrix = vtkSmartPointer<vtkMatrix3x3>::New();
			GetSensorOrientation(matrix);
			
			double new_norm[3];
			matrix->MultiplyPoint(default_norm, new_norm);
			this->planeWidget->SetNormal(new_norm);			
			
			switch(this->previous_cardiac_window)
			{
			case Cardiac_Windows::Parasternal:
				imageslice_par[this->current_volume_index]->SetVisibility(0);				
				break;

			case Cardiac_Windows::Apical:
				imageslice_api[this->current_volume_index]->SetVisibility(0);
				break;

			case Cardiac_Windows::Subcostal_1:
				imageslice_sub_1[this->current_volume_index]->SetVisibility(0);
				break;

			case Cardiac_Windows::Subcostal_2:
				imageslice_sub_2[this->current_volume_index]->SetVisibility(0);
				break;

			case Cardiac_Windows::Suprasternal:
				imageslice_sup[this->current_volume_index]->SetVisibility(0);
				break;

			default:
				imageslice_noise_tipup[this->current_volume_index]->SetVisibility(0);
				break;
			}

			this->current_volume_index++;
			if(this->current_volume_index == this->total_no_frames[Parasternal])
				this->current_volume_index = 0;
			
			vtkSmartPointer<vtkMatrix4x4> matrix_;

			switch(this->current_cardiac_window)
			{
			case Cardiac_Windows::Parasternal:				
				imageslice_par[this->current_volume_index]->SetVisibility(1);				
				matrix_= imageslice_par[this->current_volume_index]->GetMatrix();
				matrix_->MultiplyPoint(default_viewup, this->view_up);
				break;

			case Cardiac_Windows::Apical:
				imageslice_api[this->current_volume_index]->SetVisibility(1);				
				matrix_= imageslice_api[this->current_volume_index]->GetMatrix();
				matrix_->MultiplyPoint(default_viewup, this->view_up);
				break;

			case Cardiac_Windows::Subcostal_1:
				imageslice_sub_1[this->current_volume_index]->SetVisibility(1);				
				matrix_= imageslice_sub_1[this->current_volume_index]->GetMatrix();
				matrix_->MultiplyPoint(default_viewup, this->view_up);
				break;

			case Cardiac_Windows::Subcostal_2:
				imageslice_sub_2[this->current_volume_index]->SetVisibility(1);				
				matrix_= imageslice_sub_2[this->current_volume_index]->GetMatrix();
				matrix_->MultiplyPoint(default_viewup, this->view_up);
				break;

			case Cardiac_Windows::Suprasternal:
				imageslice_sup[this->current_volume_index]->SetVisibility(1);				
				matrix_= imageslice_sup[this->current_volume_index]->GetMatrix();
				matrix_->MultiplyPoint(default_viewup, this->view_up);
				break;

			default:
				imageslice_noise_tipup[this->current_volume_index]->SetVisibility(1);
				break; 
			}			

			double position[3];

			vtkSmartPointer<vtkCamera> cam = renderer->GetActiveCamera();
			
			if(this->current_cardiac_window != Cardiac_Windows::None)
			{
				matrix->MultiplyPoint(this->default_norm, this->normal);
			
				double focalPt[] = {0,0,0};
				double centerofvol[] = {dimX/2, dimY/2, dimZ/2};
				double origin[] = {dimX/2, dimY/2, 0};

				vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
				plane->ProjectPoint( centerofvol, focalPt );
				
				position[0] = focalPt[0] + distfromcam * this->normal[0];
				position[1] = focalPt[1] + distfromcam * this->normal[1];
				position[2] = focalPt[2] + distfromcam * this->normal[2];
				
				cam->SetPosition(position);		//Orientation of the slice plane		
				cam->SetFocalPoint(focalPt);	//Point at which my plane is fixed	
				cam->SetViewUp(this->view_up);
				cam->SetWindowCenter(win_center_x, win_center_y);
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
				cam->SetViewUp(this->default_viewup);
				cam->SetFocalPoint(focalPt);
				cam->SetWindowCenter(0, 0);
			}

			interactor->Render();
			renderer->ResetCameraClippingRange();

			this->previous_cardiac_window = this->current_cardiac_window;
		}

		if(event == vtkCommand::KeyPressEvent)
		{
			std::string key = interactor->GetKeySym();			
			double *angle_adjust;		//This is to save the values
			double x = 0, y = 0, z = 0; //This is to be able to apply incremental/decremental values coz RotateX/Y/Z needs unit values

			switch(this->current_cardiac_window)
			{
			case Cardiac_Windows::Parasternal:
				angle_adjust = this->par_angle_adjust;
				break;

			case Cardiac_Windows::Apical:
				angle_adjust = this->api_angle_adjust;
				break;

			case Cardiac_Windows::Suprasternal:
				angle_adjust = this->sup_angle_adjust;
				break;

			case Cardiac_Windows::Subcostal_1:
				angle_adjust = this->sub_angle_adjust_1;
				break;

			case Cardiac_Windows::Subcostal_2:
				angle_adjust = this->sub_angle_adjust_2;
				break;
			}

			if(key == "Tab")
			{
				this->default_viewup[2] *= -1;
				this->win_center_y *= -1;
			}
						
			if(key == "Insert" && angle_adjust[0] < 180)
			{
				this->guidance_volume->RotateX(1);
				
				angle_adjust[0]++;
				x++;
			}

			else if(key == "Delete" && angle_adjust[0] > -180)
			{
				this->guidance_volume->RotateX(-1);
				
				angle_adjust[0]--;
				x--;
			}

			else if(key == "Home" && angle_adjust[1] < 180)
			{
				this->guidance_volume->RotateY(1);
				
				angle_adjust[1]++;
				y++;
			}

			else if(key == "End" && angle_adjust[1] > -180)
			{
				this->guidance_volume->RotateY(-1);
				
				angle_adjust[1]--;
				y--;
			}

			else if(key == "Prior" && angle_adjust[2] < 180)
			{
				this->guidance_volume->RotateZ(1);
				
				angle_adjust[2]++;
				z++;
			}

			else if(key == "Next" && angle_adjust[2] > -180)
			{
				this->guidance_volume->RotateZ(-1);

				angle_adjust[2]--;
				z--;
			}		

			switch(this->current_cardiac_window)
			{
			case Cardiac_Windows::Parasternal:				
				for(int i = 0; i < 13; i++){
					imageslice_par[i]->RotateX(x);
					imageslice_par[i]->RotateY(y);
					imageslice_par[i]->RotateZ(z);
				}
				break;

			case Cardiac_Windows::Apical:
				for(int i = 0; i < 13; i++){
					imageslice_api[i]->RotateX(x);
					imageslice_api[i]->RotateY(y);
					imageslice_api[i]->RotateZ(z);
				}
				break;

			case Cardiac_Windows::Suprasternal:
				for(int i = 0; i < 13; i++){
					imageslice_sup[i]->RotateX(x);
					imageslice_sup[i]->RotateY(y);
					imageslice_sup[i]->RotateZ(z);
				}
				break;

			case Cardiac_Windows::Subcostal_1:
				for(int i = 0; i < 13; i++){
					imageslice_sub_1[i]->RotateX(x);
					imageslice_sub_1[i]->RotateY(y);
					imageslice_sub_1[i]->RotateZ(z);
				}
				break;

			case Cardiac_Windows::Subcostal_2:
				for(int i = 0; i < 13; i++){
					imageslice_sub_2[i]->RotateX(x);
					imageslice_sub_2[i]->RotateY(y);
					imageslice_sub_2[i]->RotateZ(z);
				}
				break;
			}
		}
};

public:

	vtkSmartPointer<vtkImageSlice> imageslice_par[50];
	vtkSmartPointer<vtkImageSlice> imageslice_api[50];
	vtkSmartPointer<vtkImageSlice> imageslice_sub_1[50];
	vtkSmartPointer<vtkImageSlice> imageslice_sub_2[50];
	vtkSmartPointer<vtkImageSlice> imageslice_sup[50];
	vtkSmartPointer<vtkImageSlice> imageslice_noise_tipup[50];
	vtkSmartPointer<vtkImageSlice> imageslice_noise_tipdown[50];

	vtkSmartPointer<vtkImageResliceMapper> resliceMapper_par[50];
	vtkSmartPointer<vtkImageResliceMapper> resliceMapper_api[50];
	vtkSmartPointer<vtkImageResliceMapper> resliceMapper_sup[50];
	vtkSmartPointer<vtkImageResliceMapper> resliceMapper_sub_1[50];
	vtkSmartPointer<vtkImageResliceMapper> resliceMapper_sub_2[50];
	vtkSmartPointer<vtkImageResliceMapper> resliceMapper_noise_tipup[50];
	vtkSmartPointer<vtkImageResliceMapper> resliceMapper_noise_tipdown[50];

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
	int total_no_frames[6];
	Cardiac_Windows current_cardiac_window, previous_cardiac_window;
	
	Range *parasternal_range_1;
	Range *apical_range_1;
	Range *subcostal_range_1;
	Range *suprasternal_range_1;

	vtkRenderer* guid_renderer;
	vtkSmartPointer<vtkRenderer> throw_plane_renderer;
	vtkSmartPointer<vtkActor> throw_plane_actor;
	vtkSmartPointer<vtkProbeFilter> probe;
	vtkSmartPointer<vtkMarchingCubes> mc;
	vtkSmartPointer<vtkPolyDataMapper> contourMapper;
	
	vtkRenderer* renderer;	
	vtkRenderWindow* renWin;
	vtkRenderWindowInteractor *interactor;

	double par_angle_adjust[3];	
	double sup_angle_adjust[3];
	double api_angle_adjust[3];
	double sub_angle_adjust_1[3];
	double sub_angle_adjust_2[3];

private: 
	double default_norm[3];
	double view_up[4];
	double default_viewup[4];

	double win_center_x;
	double win_center_y;
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

int ReadFolder(std::string folderpath, vector<string> &filenames)
{
   WIN32_FIND_DATA ffd;
   LARGE_INTEGER filesize;
   TCHAR szDir[MAX_PATH];
   HANDLE hFind = INVALID_HANDLE_VALUE;
   DWORD dwError=0;  

   StringCchCopy(szDir, MAX_PATH, folderpath.c_str());
   StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

   // Find the first file in the directory.

   hFind = FindFirstFile(szDir, &ffd);

   if (INVALID_HANDLE_VALUE == hFind) 
   {
      //DisplayErrorBox(TEXT("FindFirstFile"));
      return dwError;
   } 
   
   // List all the files in the directory with some info about them.

   do
   {
      if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
		  
         //_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
      }
      else
      {
         filesize.LowPart = ffd.nFileSizeLow;
         filesize.HighPart = ffd.nFileSizeHigh;
		 filenames.push_back(folderpath + string(ffd.cFileName));
         //_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
      }
   }
   while (FindNextFile(hFind, &ffd) != 0);
 
   dwError = GetLastError();
   if (dwError != ERROR_NO_MORE_FILES) 
   {
      //DisplayErrorBox(TEXT("FindFirstFile"));
   }

   FindClose(hFind);
   return dwError;
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
	double guidance_viewport[4] = {0.0, 0.8, 0.2, 1.0};
	double slice_viewport[4] = {0.0, 0.0, 1.0, 1.0};

	renderer->SetViewport(slice_viewport);	
	window->AddRenderer(renderer);
	
	window->FullScreenOn();
	renderer->SetBackground(0.03, 0.03, 0.03);

#pragma region File Names

	vector<string> par_vol;
	vector<string> api_vol;
	vector<string> sup_vol;
	vector<string> sub_1_vol;
	vector<string> sub_2_vol;
	vector<string> noise_vol;

	ReadFolder("C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Parasternal\\1\\", par_vol);
	ReadFolder("C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Apical\\1\\", api_vol);
	ReadFolder("C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Suprasternal\\1\\", sup_vol);
	ReadFolder("C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\1\\", sub_1_vol);
	ReadFolder("C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Subcostal\\2\\", sub_2_vol);
	ReadFolder("C:\\VETS Source Code\\VETS 3D\\Volumes for simulation\\Noise\\", noise_vol);

	//Initiliaze counters
	callback->total_no_frames[Parasternal] = par_vol.size();
	callback->total_no_frames[Apical] = api_vol.size();
	callback->total_no_frames[Suprasternal] = sup_vol.size();
	callback->total_no_frames[Subcostal_1] = sub_1_vol.size();
	callback->total_no_frames[Subcostal_2] = sub_2_vol.size();
	callback->total_no_frames[None] = noise_vol.size();

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

#pragma region Read Volumes

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
	
	ReaderType::Pointer reader_par_1[50];	
	ReaderType::Pointer reader_api_1[50];
	ReaderType::Pointer reader_sub_1[50];
	ReaderType::Pointer reader_sub_2[50];
	ReaderType::Pointer reader_sup_1[50];
	
	ExportFilterType::Pointer itkExporter_par_1[50];
	ExportFilterType::Pointer itkExporter_api_1[50];
	ExportFilterType::Pointer itkExporter_sub_1[50];
	ExportFilterType::Pointer itkExporter_sub_2[50];
	ExportFilterType::Pointer itkExporter_sup_1[50];
	
	ReaderType::Pointer reader_noise_tipup[50];
	ReaderType::Pointer reader_noise_tipdown[50];
	ExportFilterType::Pointer itkExporter_noise_tipup[50];
	ExportFilterType::Pointer itkExporter_noise_tipdown[50];

	//Objects for guidance
	vtkSmartPointer<vtkMarchingCubes> mc = vtkSmartPointer<vtkMarchingCubes>::New();
	vtkSmartPointer<vtkPolyDataMapper> mapper =	vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkActor> isoActor = vtkSmartPointer<vtkActor>::New();
	//End
	vtkSmartPointer<vtkPlaneWidget> planeWidget = vtkSmartPointer<vtkPlaneWidget>::New();
	for(int i = 0; i < callback->total_no_frames[Parasternal]; i++)
	{		
		reader_par_1[i] = ReaderType::New();
		reader_par_1[i]->SetFileName(par_vol[i]);
		reader_par_1[i]->SetImageIO(gdcmImageIO);
		reader_par_1[i]->Update();

		callback->dimX = reader_par_1[i]->GetImageIO()->GetDimensions(0);
		callback->dimY = reader_par_1[i]->GetImageIO()->GetDimensions(1);
		callback->dimZ = reader_par_1[i]->GetImageIO()->GetDimensions(2);
		
		itkExporter_par_1[i] = ExportFilterType::New();
		vtkImageImport *vtkImporter = vtkImageImport::New();
		itkExporter_par_1[i]->SetInput(reader_par_1[i]->GetOutput());

		ConnectPipelines(itkExporter_par_1[i], vtkImporter);

		callback->resliceMapper_par[i] = vtkSmartPointer<vtkImageResliceMapper>::New();
		callback->resliceMapper_par[i]->SetInputConnection(vtkImporter->GetOutputPort());
		callback->resliceMapper_par[i]->SliceFacesCameraOn();
		callback->resliceMapper_par[i]->SliceAtFocalPointOn();
		callback->resliceMapper_par[i]->BorderOff();

		callback->imageslice_par[i] = vtkSmartPointer<vtkImageSlice>::New();
		callback->imageslice_par[i]->SetMapper(callback->resliceMapper_par[i]);
		callback->imageslice_par[i]->SetProperty(image_property);
		callback->imageslice_par[i]->SetVisibility(0);
		callback->imageslice_par[i]->SetOrigin(callback->dimX/2, callback->dimY/2, 0);
		
		renderer->AddViewProp(callback->imageslice_par[i]);

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
	
	for(int i = 0; i < 13; i++)
	{		
		reader_api_1[i] = ReaderType::New();
		reader_api_1[i]->SetFileName(api_vol_1[i]);
		reader_api_1[i]->SetImageIO(gdcmImageIO);
		reader_api_1[i]->Update();
				
		itkExporter_api_1[i] = ExportFilterType::New();
		vtkImageImport *vtkImporter = vtkImageImport::New();
		itkExporter_api_1[i]->SetInput(reader_api_1[i]->GetOutput());

		ConnectPipelines(itkExporter_api_1[i], vtkImporter);

		callback->resliceMapper_api[i] = vtkSmartPointer<vtkImageResliceMapper>::New();
		callback->resliceMapper_api[i]->SetInputConnection(vtkImporter->GetOutputPort());
		callback->resliceMapper_api[i]->SliceFacesCameraOn();
		callback->resliceMapper_api[i]->SliceAtFocalPointOn();
		callback->resliceMapper_api[i]->BorderOff();

		callback->imageslice_api[i] = vtkSmartPointer<vtkImageSlice>::New();
		callback->imageslice_api[i]->SetMapper(callback->resliceMapper_api[i]);
		callback->imageslice_api[i]->SetProperty(image_property);
		callback->imageslice_api[i]->SetVisibility(0);
		callback->imageslice_api[i]->SetOrigin(callback->dimX/2, callback->dimY/2, 0);
		renderer->AddViewProp(callback->imageslice_api[i]);
	}

	for(int i = 0; i < 13; i++)
	{		
		reader_sub_1[i] = ReaderType::New();
		reader_sub_1[i]->SetFileName(sub_vol_1[i]);
		reader_sub_1[i]->SetImageIO(gdcmImageIO);
		reader_sub_1[i]->Update();
				
		itkExporter_sub_1[i] = ExportFilterType::New();
		vtkImageImport *vtkImporter = vtkImageImport::New();
		itkExporter_sub_1[i]->SetInput(reader_sub_1[i]->GetOutput());

		ConnectPipelines(itkExporter_sub_1[i], vtkImporter);

		callback->resliceMapper_sub_1[i] = vtkSmartPointer<vtkImageResliceMapper>::New();
		callback->resliceMapper_sub_1[i]->SetInputConnection(vtkImporter->GetOutputPort());
		callback->resliceMapper_sub_1[i]->SliceFacesCameraOn();
		callback->resliceMapper_sub_1[i]->SliceAtFocalPointOn();
		callback->resliceMapper_sub_1[i]->BorderOff();

		callback->imageslice_sub_1[i] = vtkSmartPointer<vtkImageSlice>::New();
		callback->imageslice_sub_1[i]->SetMapper(callback->resliceMapper_sub_1[i]);
		callback->imageslice_sub_1[i]->SetProperty(image_property);
		callback->imageslice_sub_1[i]->SetVisibility(0);
		callback->imageslice_sub_1[i]->SetOrigin(callback->dimX/2, callback->dimY/2, 0);
		renderer->AddViewProp(callback->imageslice_sub_1[i]);
	}

	for(int i = 0; i < 13; i++)
	{		
		reader_sub_2[i] = ReaderType::New();
		reader_sub_2[i]->SetFileName(sub_vol_2[i]);
		reader_sub_2[i]->SetImageIO(gdcmImageIO);
		reader_sub_2[i]->Update();
				
		itkExporter_sub_2[i] = ExportFilterType::New();
		vtkImageImport *vtkImporter = vtkImageImport::New();
		itkExporter_sub_2[i]->SetInput(reader_sub_2[i]->GetOutput());

		ConnectPipelines(itkExporter_sub_2[i], vtkImporter);

		callback->resliceMapper_sub_2[i] = vtkSmartPointer<vtkImageResliceMapper>::New();
		callback->resliceMapper_sub_2[i]->SetInputConnection(vtkImporter->GetOutputPort());
		callback->resliceMapper_sub_2[i]->SliceFacesCameraOn();
		callback->resliceMapper_sub_2[i]->SliceAtFocalPointOn();
		callback->resliceMapper_sub_2[i]->BorderOff();

		callback->imageslice_sub_2[i] = vtkSmartPointer<vtkImageSlice>::New();
		callback->imageslice_sub_2[i]->SetMapper(callback->resliceMapper_sub_2[i]);
		callback->imageslice_sub_2[i]->SetProperty(image_property);
		callback->imageslice_sub_2[i]->SetVisibility(0);
		callback->imageslice_sub_2[i]->SetOrigin(callback->dimX/2, callback->dimY/2, 0);
		renderer->AddViewProp(callback->imageslice_sub_2[i]);
	}

	for(int i = 0; i < 13; i++)
	{		
		reader_sup_1[i] = ReaderType::New();
		reader_sup_1[i]->SetFileName(sup_vol_1[i]);
		reader_sup_1[i]->SetImageIO(gdcmImageIO);
		reader_sup_1[i]->Update();
				
		itkExporter_sup_1[i] = ExportFilterType::New();
		vtkImageImport *vtkImporter = vtkImageImport::New();
		itkExporter_sup_1[i]->SetInput(reader_sup_1[i]->GetOutput());

		ConnectPipelines(itkExporter_sup_1[i], vtkImporter);

		callback->resliceMapper_sup[i] = vtkSmartPointer<vtkImageResliceMapper>::New();
		callback->resliceMapper_sup[i]->SetInputConnection(vtkImporter->GetOutputPort());
		callback->resliceMapper_sup[i]->SliceFacesCameraOn();
		callback->resliceMapper_sup[i]->SliceAtFocalPointOn();
		callback->resliceMapper_sup[i]->BorderOff();

		callback->imageslice_sup[i] = vtkSmartPointer<vtkImageSlice>::New();
		callback->imageslice_sup[i]->SetMapper(callback->resliceMapper_sup[i]);
		callback->imageslice_sup[i]->SetProperty(image_property);
		callback->imageslice_sup[i]->SetVisibility(0);
		callback->imageslice_sup[i]->SetOrigin(callback->dimX/2, callback->dimY/2, 0);
		renderer->AddViewProp(callback->imageslice_sup[i]);
	}

	for(int i = 0; i < 13; i++)
	{		
		reader_noise_tipup[i] = ReaderType::New();
		reader_noise_tipup[i]->SetFileName(noise_tip_up[i]);
		reader_noise_tipup[i]->SetImageIO(gdcmImageIO);
		reader_noise_tipup[i]->Update();
				
		itkExporter_noise_tipup[i] = ExportFilterType::New();
		vtkImageImport *vtkImporter = vtkImageImport::New();
		itkExporter_noise_tipup[i]->SetInput(reader_noise_tipup[i]->GetOutput());

		ConnectPipelines(itkExporter_noise_tipup[i], vtkImporter);

		callback->resliceMapper_noise_tipup[i] = vtkSmartPointer<vtkImageResliceMapper>::New();
		callback->resliceMapper_noise_tipup[i]->SetInputConnection(vtkImporter->GetOutputPort());
		callback->resliceMapper_noise_tipup[i]->SliceFacesCameraOn();
		callback->resliceMapper_noise_tipup[i]->SliceAtFocalPointOn();
		callback->resliceMapper_noise_tipup[i]->BorderOff();

		callback->imageslice_noise_tipup[i] = vtkSmartPointer<vtkImageSlice>::New();
		callback->imageslice_noise_tipup[i]->SetMapper(callback->resliceMapper_noise_tipup[i]);
		callback->imageslice_noise_tipup[i]->SetProperty(image_property);
		callback->imageslice_noise_tipup[i]->SetVisibility(0);
		renderer->AddViewProp(callback->imageslice_noise_tipup[i]);
	}

	for(int i = 0; i < 13; i++)
	{		
		reader_noise_tipdown[i] = ReaderType::New();
		reader_noise_tipdown[i]->SetFileName(noise_tip_down[i]);
		reader_noise_tipdown[i]->SetImageIO(gdcmImageIO);
		reader_noise_tipdown[i]->Update();
				
		itkExporter_noise_tipdown[i] = ExportFilterType::New();
		vtkImageImport *vtkImporter = vtkImageImport::New();
		itkExporter_noise_tipdown[i]->SetInput(reader_noise_tipdown[i]->GetOutput());

		ConnectPipelines(itkExporter_noise_tipdown[i], vtkImporter);

		callback->resliceMapper_noise_tipdown[i] = vtkSmartPointer<vtkImageResliceMapper>::New();
		callback->resliceMapper_noise_tipdown[i]->SetInputConnection(vtkImporter->GetOutputPort());
		callback->resliceMapper_noise_tipdown[i]->SliceFacesCameraOn();
		callback->resliceMapper_noise_tipdown[i]->SliceAtFocalPointOn();
		callback->resliceMapper_noise_tipdown[i]->BorderOff();

		callback->imageslice_noise_tipdown[i] = vtkSmartPointer<vtkImageSlice>::New();
		callback->imageslice_noise_tipdown[i]->SetMapper(callback->resliceMapper_noise_tipdown[i]);
		callback->imageslice_noise_tipdown[i]->SetProperty(image_property);
		callback->imageslice_noise_tipdown[i]->SetVisibility(0);
		renderer->AddViewProp(callback->imageslice_noise_tipdown[i]);
	}
	
#pragma endregion

	vtkSmartPointer<vtkActor> contourActor = vtkSmartPointer<vtkActor>::New();
	contourActor->SetMapper(callback->contourMapper);
	contourActor->VisibilityOff();
	callback->guid_contour_actor = contourActor;

	// Associate the widget with the interactor
	planeWidget->SetInteractor(interactor);
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
	
	guid_renderer->SetViewport(guidance_viewport);
	guid_renderer->AddActor(isoActor);
	guid_renderer->AddActor(contourActor);
	guid_renderer->SetBackground(1,1,1);
	window->AddRenderer(guid_renderer);
	callback->guid_renderer = guid_renderer;
	
	/*vtkSmartPointer<vtkCamera> guid_cam = guid_renderer->GetActiveCamera();
	guid_cam->Elevation(90);
	guid_renderer->ResetCamera();
	guid_cam->Roll(180);
	guid_renderer->ResetCamera();*/

	
	//Set initial orientations of volumes
	//Parasternal
	for(int i = 0; i < 13; i++)
		callback->imageslice_par[i]->RotateX(callback->par_angle_adjust[0]);

	for(int i = 0; i < 13; i++)
		callback->imageslice_par[i]->RotateY(callback->par_angle_adjust[1]);
	
	for(int i = 0; i < 13; i++)
		callback->imageslice_par[i]->RotateZ(callback->par_angle_adjust[2]);

	//Apical
	for(int i = 0; i < 13; i++)
		callback->imageslice_api[i]->RotateX(callback->api_angle_adjust[0]);

	for(int i = 0; i < 13; i++)
		callback->imageslice_api[i]->RotateY(callback->api_angle_adjust[1]);
	
	for(int i = 0; i < 13; i++)
		callback->imageslice_api[i]->RotateZ(callback->api_angle_adjust[2]);
	
	//Suprasternal
	for(int i = 0; i < 13; i++)
		callback->imageslice_sup[i]->RotateX(callback->sup_angle_adjust[0]);

	for(int i = 0; i < 13; i++)
		callback->imageslice_sup[i]->RotateY(callback->sup_angle_adjust[1]);

	for(int i = 0; i < 13; i++)
		callback->imageslice_sup[i]->RotateZ(callback->sup_angle_adjust[2]);

	//Subcostal
	//Pos #1
	for(int i = 0; i < 13; i++)
		callback->imageslice_sub_1[i]->RotateX(callback->sub_angle_adjust_1[0]);

	for(int i = 0; i < 13; i++)
		callback->imageslice_sub_1[i]->RotateY(callback->sub_angle_adjust_1[1]);

	for(int i = 0; i < 13; i++)
		callback->imageslice_sub_1[i]->RotateZ(callback->sub_angle_adjust_1[2]);

	//Pos #2
	for(int i = 0; i < 13; i++)
		callback->imageslice_sub_2[i]->RotateX(callback->sub_angle_adjust_2[0]);

	for(int i = 0; i < 13; i++)
		callback->imageslice_sub_2[i]->RotateY(callback->sub_angle_adjust_2[1]);

	for(int i = 0; i < 13; i++)
		callback->imageslice_sub_2[i]->RotateZ(callback->sub_angle_adjust_2[2]);

	callback->guidance_volume->RotateX(callback->par_angle_adjust[0]);
	callback->guidance_volume->RotateY(callback->par_angle_adjust[1]);
	callback->guidance_volume->RotateZ(callback->par_angle_adjust[2]);

	//End setting initial orientation of volumes
	callback->imageslice_par[0]->SetVisibility(1);
	
	imageStyle->SetInteractionModeToImage3D();
	
	interactor->SetInteractorStyle(imageStyle);

	callback->interactor = interactor;
	callback->renderer = renderer;	
	callback->renWin = window;

	window->SetInteractor(interactor);

	window->Render();

	vtkSmartPointer<vtkCamera> cam = renderer->GetActiveCamera();
	cam->ParallelProjectionOn();	
	//cam->Zoom(1.5);

	renderer->ResetCameraClippingRange();	

	callback->imageslice_par[0]->SetVisibility(0);

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