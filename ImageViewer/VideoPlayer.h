#pragma once
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <vector>
#include <string>
#include <sstream>
#include <list>
#include <fstream>
#include <iostream>

//using namespace System;
//using namespace System::IO;
using namespace std;

struct DataRange
{
	double azStart;
	double azEnd;

	double elStart;
	double elEnd;

	double roStart;
	double roEnd;

	DataRange(void) {azStart = azEnd = elStart = elEnd = roStart = roEnd = 0;}
	DataRange(double azstart, double azend, double elstart, double elend, double rostart, double roend)
	{
		this->azStart = azstart;
		this->azEnd = azend;
		this->elStart = elstart;
		this->elEnd = elend;
		this->roStart = rostart;
		this->roEnd = roend;
	}

};

enum Cardiac_Windows
{
	Parasternal,	
	Apical,
	Suprasternal,
	Subcostal_1,
	Subcostal_2,
	Subcostal_3,
	None
};

enum Specific_cuts
{
	Par_LAX,
	Par_RVOT,
	Par_RVIN,
	Par_SAX_Base,
	Par_SAX_Mitral,

	Api_4_Chamber_1,
	Api_4_Chamber_2,
	Api_5_Chamber,

	Sub_Coronal_Posterior,
	Sub_Coronal_Anterior,
	Sub_Coronal_Aorta,

	Sub_Transverse,
	Sub_Longitudanal_Aorta,
	Sub_Longitudanal_IVC,

	Sub_Sagital_Bicaval,
	Sub_Sagital_RVOT,

	Sup_Aortic_Arch,
	Sup_Ductal_Cut,
	Sup_Crab_View,
	
	No_Specific_Cut
};

class VideoPlayer
{
public:

	DataRange *range_Par_LAX;
	DataRange *range_Par_RVOT;
	DataRange *range_Par_RVIN;//New
	DataRange *range_Par_SAX_Base;
	DataRange *range_Par_SAX_Mitral;

	DataRange *range_Api_4_Chamber_1;
	DataRange *range_Api_4_Chamber_2;
	DataRange *range_Api_5_Chamber;

	DataRange *range_Sub_Coronal_Posterior;
	DataRange *range_Sub_Coronal_Anterior;
	DataRange *range_Sub_Coronal_Aorta; //New
		
	DataRange *range_Sub_Transverse;
	DataRange *range_Sub_Longitudanal_Aorta;
	DataRange *range_Sub_Longitudanal_IVC;

	DataRange *range_Sub_Sagital_Bicaval;
	DataRange *range_Sub_Sagital_RVOT;//New

	DataRange *range_Sup_Aortic_Arch;
	DataRange *range_Sup_Ductal_Cut;
	DataRange *range_Sup_Crab_View;
	
public:
	VideoPlayer(void);
	void ReadRanges(void);
	int GetSpecificCut(int, double, double, double);
};