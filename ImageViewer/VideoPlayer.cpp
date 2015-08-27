#include "VideoPlayer.h"

vector<string> Tokenize(string line, string delim) {
  vector<string> tokens;
  string::size_type start = line.find_first_not_of(delim);
  string::size_type end = line.find_first_of(delim);

  while( string::npos != start || string::npos != end) {
  	/*if(start == (end - start)){ //so if we have 2,,2 we return the vector [2, "", 2] HAVEN'T HANDLED THE CASE DUE TO LACK OF TIME
  		tokens.push_back("");std::cout<<"empty string"<<endl;}
  		
    else */tokens.push_back(line.substr(start, end - start));

    start = line.find_first_not_of(delim, end);
    end = line.find_first_of(delim, start);
  }

  return tokens;
}

//Tokenize based on whitespace
vector<string> Tokenize (string line) {
  vector<string> tokens;
  stringstream s_stream(line);
  string token;
  
  while(s_stream >> token ) {
    tokens.push_back(token);
  }

  return tokens;
}

VideoPlayer::VideoPlayer(void)
{
	ReadRanges();
	/*BuildVideoSet();*/
}

void VideoPlayer::ReadRanges(void)
{
	ifstream ranges_file;
	ranges_file.open ("C:\\VETS Source Code\\VETS 3D\\Video Frames\\Ranges\\Parasternal.txt");
	string line;

	if(ranges_file.good())
	{
		getline(ranges_file, line);
		vector<string> tokens = Tokenize(line);

		range_Par_LAX = new DataRange(atof(tokens[0].c_str()), atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()), atof(tokens[4].c_str()), atof(tokens[5].c_str()));
		
		getline(ranges_file, line);
		vector<string> tokens_2 = Tokenize(line);

		range_Par_RVOT = new DataRange(atof(tokens_2[0].c_str()), atof(tokens_2[1].c_str()), atof(tokens_2[2].c_str()), atof(tokens_2[3].c_str()), atof(tokens_2[4].c_str()), atof(tokens_2[5].c_str()));

		getline(ranges_file, line);
		vector<string> tokens_5 = Tokenize(line);

		range_Par_RVIN = new DataRange(atof(tokens_5[0].c_str()), atof(tokens_5[1].c_str()), atof(tokens_5[2].c_str()), atof(tokens_5[3].c_str()), atof(tokens_5[4].c_str()), atof(tokens_5[5].c_str()));

		getline(ranges_file, line);
		vector<string> tokens_3 = Tokenize(line);

		range_Par_SAX_Base = new DataRange(atof(tokens_3[0].c_str()), atof(tokens_3[1].c_str()), atof(tokens_3[2].c_str()), atof(tokens_3[3].c_str()), atof(tokens_3[4].c_str()), atof(tokens_3[5].c_str()));
		
		getline(ranges_file, line);
		vector<string> tokens_4 = Tokenize(line);

		range_Par_SAX_Mitral = new DataRange(atof(tokens_4[0].c_str()), atof(tokens_4[1].c_str()), atof(tokens_4[2].c_str()), atof(tokens_4[3].c_str()), atof(tokens_4[4].c_str()), atof(tokens_4[5].c_str()));
	}

	ranges_file.close();
		
	ranges_file.open ("C:\\VETS Source Code\\VETS 3D\\Video Frames\\Ranges\\Apical.txt");

	if(ranges_file.good())
	{
		getline(ranges_file, line);
		vector<string> tokens = Tokenize(line);

		range_Api_4_Chamber_1 = new DataRange(atof(tokens[0].c_str()), atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()), atof(tokens[4].c_str()), atof(tokens[5].c_str()));
		
		getline(ranges_file, line);
		vector<string> tokens_2 = Tokenize(line);

		range_Api_4_Chamber_2 = new DataRange(atof(tokens_2[0].c_str()), atof(tokens_2[1].c_str()), atof(tokens_2[2].c_str()), atof(tokens_2[3].c_str()), atof(tokens_2[4].c_str()), atof(tokens_2[5].c_str()));

		getline(ranges_file, line);
		vector<string> tokens_3 = Tokenize(line);

		range_Api_5_Chamber = new DataRange(atof(tokens_3[0].c_str()), atof(tokens_3[1].c_str()), atof(tokens_3[2].c_str()), atof(tokens_3[3].c_str()), atof(tokens_3[4].c_str()), atof(tokens_3[5].c_str()));
	}

	ranges_file.close();
		
	ranges_file.open ("C:\\VETS Source Code\\VETS 3D\\Video Frames\\Ranges\\Subcostal.txt");

	if(ranges_file.good())
	{		
		getline(ranges_file, line);
		vector<string> tokens_2 = Tokenize(line);

		range_Sub_Coronal_Anterior = new DataRange(atof(tokens_2[0].c_str()), atof(tokens_2[1].c_str()), atof(tokens_2[2].c_str()), atof(tokens_2[3].c_str()), atof(tokens_2[4].c_str()), atof(tokens_2[5].c_str()));

		getline(ranges_file, line);
		vector<string> tokens = Tokenize(line);

		range_Sub_Coronal_Posterior = new DataRange(atof(tokens[0].c_str()), atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()), atof(tokens[4].c_str()), atof(tokens[5].c_str()));
		
		getline(ranges_file, line);
		vector<string> tokens_8 = Tokenize(line);

		range_Sub_Coronal_Aorta = new DataRange(atof(tokens_8[0].c_str()), atof(tokens_8[1].c_str()), atof(tokens_8[2].c_str()), atof(tokens_8[3].c_str()), atof(tokens_8[4].c_str()), atof(tokens_8[5].c_str()));
		
		getline(ranges_file, line);
		vector<string> tokens_5 = Tokenize(line);

		range_Sub_Transverse = new DataRange(atof(tokens_5[0].c_str()), atof(tokens_5[1].c_str()), atof(tokens_5[2].c_str()), atof(tokens_5[3].c_str()), atof(tokens_5[4].c_str()), atof(tokens_5[5].c_str()));

		getline(ranges_file, line);
		vector<string> tokens_6 = Tokenize(line);

		range_Sub_Longitudanal_Aorta = new DataRange(atof(tokens_6[0].c_str()), atof(tokens_6[1].c_str()), atof(tokens_6[2].c_str()), atof(tokens_6[3].c_str()), atof(tokens_6[4].c_str()), atof(tokens_6[5].c_str()));

		getline(ranges_file, line);
		vector<string> tokens_7 = Tokenize(line);

		range_Sub_Longitudanal_IVC = new DataRange(atof(tokens_7[0].c_str()), atof(tokens_7[1].c_str()), atof(tokens_7[2].c_str()), atof(tokens_7[3].c_str()), atof(tokens_7[4].c_str()), atof(tokens_7[5].c_str()));
	
		getline(ranges_file, line);
		vector<string> tokens_3 = Tokenize(line);

		range_Sub_Sagital_Bicaval = new DataRange(atof(tokens_3[0].c_str()), atof(tokens_3[1].c_str()), atof(tokens_3[2].c_str()), atof(tokens_3[3].c_str()), atof(tokens_3[4].c_str()), atof(tokens_3[5].c_str()));
		
		getline(ranges_file, line);
		vector<string> tokens_9 = Tokenize(line);

		range_Sub_Sagital_RVOT = new DataRange(atof(tokens_9[0].c_str()), atof(tokens_9[1].c_str()), atof(tokens_9[2].c_str()), atof(tokens_9[3].c_str()), atof(tokens_9[4].c_str()), atof(tokens_9[5].c_str()));
		
	}

	ranges_file.close();
		
	ranges_file.open ("C:\\VETS Source Code\\VETS 3D\\Video Frames\\Ranges\\Suprasternal.txt");

	if(ranges_file.good())
	{
		getline(ranges_file, line);
		vector<string> tokens = Tokenize(line);

		range_Sup_Aortic_Arch = new DataRange(atof(tokens[0].c_str()), atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()), atof(tokens[4].c_str()), atof(tokens[5].c_str()));
		
		getline(ranges_file, line);
		vector<string> tokens_2 = Tokenize(line);

		range_Sup_Ductal_Cut = new DataRange(atof(tokens_2[0].c_str()), atof(tokens_2[1].c_str()), atof(tokens_2[2].c_str()), atof(tokens_2[3].c_str()), atof(tokens_2[4].c_str()), atof(tokens_2[5].c_str()));

		getline(ranges_file, line);
		vector<string> tokens_3 = Tokenize(line);

		range_Sup_Crab_View = new DataRange(atof(tokens_3[0].c_str()), atof(tokens_3[1].c_str()), atof(tokens_3[2].c_str()), atof(tokens_3[3].c_str()), atof(tokens_3[4].c_str()), atof(tokens_3[5].c_str()));
	}
}

int VideoPlayer::GetSpecificCut(int cardiac_window, double azimuth, double elevation, double roll){
	int current_cut = No_Specific_Cut;

	switch(cardiac_window)
	{
	case Parasternal:
		if(azimuth >= range_Par_LAX->azStart && azimuth <= range_Par_LAX->azEnd &&
			elevation >= range_Par_LAX->elStart && elevation <= range_Par_LAX->elEnd &&
			roll >= range_Par_LAX->roStart && roll <= range_Par_LAX->roEnd)

			current_cut = Par_LAX;

		else 
			if(azimuth >= range_Par_RVOT->azStart && azimuth <= range_Par_RVOT->azEnd &&
				elevation >= range_Par_RVOT->elStart && elevation <= range_Par_RVOT->elEnd &&
				roll >= range_Par_RVOT->roStart && roll <= range_Par_RVOT->roEnd)

				current_cut = Par_RVOT;

			else 
				if(azimuth >= range_Par_RVIN->azStart && azimuth <= range_Par_RVIN->azEnd &&
					elevation >= range_Par_RVIN->elStart && elevation <= range_Par_RVIN->elEnd &&
					roll >= range_Par_RVIN->roStart && roll <= range_Par_RVIN->roEnd)

					current_cut = Par_RVIN;

				else 
					if(azimuth >= range_Par_SAX_Base->azStart && azimuth <= range_Par_SAX_Base->azEnd &&
						elevation >= range_Par_SAX_Base->elStart && elevation <= range_Par_SAX_Base->elEnd &&
						roll >= range_Par_SAX_Base->roStart && roll <= range_Par_SAX_Base->roEnd)

						current_cut = Par_SAX_Base;

					else 
						if(azimuth >= range_Par_SAX_Mitral->azStart && azimuth <= range_Par_SAX_Mitral->azEnd &&
							elevation >= range_Par_SAX_Mitral->elStart && elevation <= range_Par_SAX_Mitral->elEnd &&
							roll >= range_Par_SAX_Mitral->roStart && roll <= range_Par_SAX_Mitral->roEnd)

							current_cut = Par_SAX_Mitral;

		break;

	case Apical:
		if(azimuth >= range_Api_4_Chamber_1->azStart && azimuth <= range_Api_4_Chamber_1->azEnd &&
			elevation >= range_Api_4_Chamber_1->elStart && elevation <= range_Api_4_Chamber_1->elEnd &&
			roll >= range_Api_4_Chamber_1->roStart && roll <= range_Api_4_Chamber_1->roEnd)

			current_cut = Api_4_Chamber_1;

		else 
			if(azimuth >= range_Api_4_Chamber_2->azStart && azimuth <= range_Api_4_Chamber_2->azEnd &&
				elevation >= range_Api_4_Chamber_2->elStart && elevation <= range_Api_4_Chamber_2->elEnd &&
				roll >= range_Api_4_Chamber_2->roStart && roll <= range_Api_4_Chamber_2->roEnd)

				current_cut = Api_4_Chamber_2;
			else 
				if(azimuth >= range_Api_5_Chamber->azStart && azimuth <= range_Api_5_Chamber->azEnd &&
					elevation >= range_Api_5_Chamber->elStart && elevation <= range_Api_5_Chamber->elEnd &&
					roll >= range_Api_5_Chamber->roStart && roll <= range_Api_5_Chamber->roEnd)

					current_cut = Api_5_Chamber;
		break;


	case Suprasternal:
		if(azimuth >= range_Sup_Aortic_Arch->azStart && azimuth <= range_Sup_Aortic_Arch->azEnd &&
			elevation >= range_Sup_Aortic_Arch->elStart && elevation <= range_Sup_Aortic_Arch->elEnd &&
			roll >= range_Sup_Aortic_Arch->roStart && roll <= range_Sup_Aortic_Arch->roEnd)

			current_cut = Sup_Aortic_Arch;
		else 
			if(azimuth >= range_Sup_Ductal_Cut->azStart && azimuth <= range_Sup_Ductal_Cut->azEnd &&
				elevation >= range_Sup_Ductal_Cut->elStart && elevation <= range_Sup_Ductal_Cut->elEnd &&
				roll >= range_Sup_Ductal_Cut->roStart && roll <= range_Sup_Ductal_Cut->roEnd)

				current_cut = Sup_Ductal_Cut;
			else 
				if(azimuth >= range_Sup_Crab_View->azStart && azimuth <= range_Sup_Crab_View->azEnd &&
					elevation >= range_Sup_Crab_View->elStart && elevation <= range_Sup_Crab_View->elEnd &&
					roll >= range_Sup_Crab_View->roStart && roll <= range_Sup_Crab_View->roEnd)

					current_cut = Sup_Crab_View;
		break;
		
	case Subcostal_1:
		if(azimuth >= range_Sub_Coronal_Anterior->azStart && azimuth <= range_Sub_Coronal_Anterior->azEnd &&
			elevation >= range_Sub_Coronal_Anterior->elStart && elevation <= range_Sub_Coronal_Anterior->elEnd &&
			roll >= range_Sub_Coronal_Anterior->roStart && roll <= range_Sub_Coronal_Anterior->roEnd)

			current_cut = Sub_Coronal_Anterior;

		else 
			if(azimuth >= range_Sub_Coronal_Posterior->azStart && azimuth <= range_Sub_Coronal_Posterior->azEnd &&
				elevation >= range_Sub_Coronal_Posterior->elStart && elevation <= range_Sub_Coronal_Posterior->elEnd &&
				roll >= range_Sub_Coronal_Posterior->roStart && roll <= range_Sub_Coronal_Posterior->roEnd)

				current_cut = Sub_Coronal_Posterior;
			else 
				if(azimuth >= range_Sub_Coronal_Aorta->azStart && azimuth <= range_Sub_Coronal_Aorta->azEnd &&
					elevation >= range_Sub_Coronal_Aorta->elStart && elevation <= range_Sub_Coronal_Aorta->elEnd &&
					roll >= range_Sub_Coronal_Aorta->roStart && roll <= range_Sub_Coronal_Aorta->roEnd)

					current_cut = Sub_Coronal_Aorta;
			break;

		case Subcostal_2:
			if(azimuth >= range_Sub_Transverse->azStart && azimuth <= range_Sub_Transverse->azEnd &&
				elevation >= range_Sub_Transverse->elStart && elevation <= range_Sub_Transverse->elEnd &&
				roll >= range_Sub_Transverse->roStart && roll <= range_Sub_Transverse->roEnd)

				current_cut = Sub_Transverse;

			else 
				if(azimuth >= range_Sub_Longitudanal_Aorta->azStart && azimuth <= range_Sub_Longitudanal_Aorta->azEnd &&
					elevation >= range_Sub_Longitudanal_Aorta->elStart && elevation <= range_Sub_Longitudanal_Aorta->elEnd &&
					roll >= range_Sub_Longitudanal_Aorta->roStart && roll <= range_Sub_Longitudanal_Aorta->roEnd)

					current_cut = Sub_Longitudanal_Aorta;
				else 
					if(azimuth >= range_Sub_Longitudanal_IVC->azStart && azimuth <= range_Sub_Longitudanal_IVC->azEnd &&
						elevation >= range_Sub_Longitudanal_IVC->elStart && elevation <= range_Sub_Longitudanal_IVC->elEnd &&
						roll >= range_Sub_Longitudanal_IVC->roStart && roll <= range_Sub_Longitudanal_IVC->roEnd)

						current_cut = Sub_Longitudanal_IVC;
			break;

		case Subcostal_3:
			if(azimuth >= range_Sub_Sagital_Bicaval->azStart && azimuth <= range_Sub_Sagital_Bicaval->azEnd &&
					elevation >= range_Sub_Sagital_Bicaval->elStart && elevation <= range_Sub_Sagital_Bicaval->elEnd &&
					roll >= range_Sub_Sagital_Bicaval->roStart && roll <= range_Sub_Sagital_Bicaval->roEnd)

					current_cut = Sub_Sagital_Bicaval;

			else 
				if(azimuth >= range_Sub_Sagital_RVOT->azStart && azimuth <= range_Sub_Sagital_RVOT->azEnd &&
					elevation >= range_Sub_Sagital_RVOT->elStart && elevation <= range_Sub_Sagital_RVOT->elEnd &&
					roll >= range_Sub_Sagital_RVOT->roStart && roll <= range_Sub_Sagital_RVOT->roEnd)

					current_cut = Sub_Sagital_RVOT;

	}

	return current_cut;
}