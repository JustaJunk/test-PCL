//#############################################################################
//
//	pcd_xyz.cpp
//
//#############################################################################
#include <cstdlib>
#include <iostream>
#include "pcd_xyz.h"

namespace mypcl
{

//#############################################################################
//
//  loadXYZtoPCD(): read XYZ file and convert it to PCD object
//
//#############################################################################
int loadXYZtoPCD(	const std::string 						&xyz_filename,
					pcl::PointCloud<pcl::PointXYZ> 			&pcl_pc)
{
	std::ifstream 				xyz_fs;
	std::string 				line;
	std::vector<std::string> 	str_vec;

	//--- Check that file exists and open successfully
	xyz_fs.open(xyz_filename.c_str(), std::ios::binary);
	if (xyz_fs.fail() || !xyz_fs.is_open())
	{
	  std::cout << "ERROR: Could not find or open file " << xyz_filename.c_str() << std::endl;
	  xyz_fs.close();
	  return EXIT_FAILURE;
	}

	//--- Parse every line (correspond to every point)
	pcl_pc.clear();
	pcl_pc.reserve(1000000);
	while (!xyz_fs.eof())
	{
		std::getline(xyz_fs, line);

		//--- Ignore empty lines
		if (line == "")
		{
		  continue;
		}

		//--- Tokenize the line
		boost::trim(line);
		boost::split(str_vec, line, boost::is_any_of ("\t\r "), boost::token_compress_on);
		if (str_vec.size() != 3)
		{
		  continue;
		}
		pcl_pc.push_back(pcl::PointXYZ( (float)std::atof( str_vec.at(0).c_str() ),
										(float)std::atof( str_vec.at(1).c_str() ),
										(float)std::atof( str_vec.at(2).c_str() )));
	}
	xyz_fs.close();

	//--- Set pcd header
	pcl_pc.width 	= (uint32_t)pcl_pc.size();
	pcl_pc.height 	= 1; 
	pcl_pc.is_dense	= true;

	return EXIT_SUCCESS;
}

//#############################################################################
//
//  savePCDtoXYZ(): save PCD object into XYZ format
//
//#############################################################################
int savePCDtoXYZ(	const pcl::PointCloud<pcl::PointXYZ> 	&pcl_pc,
					const std::string 						&xyz_filename)
{
	std::ofstream 					xyz_file;
	pcl::PointXYZ 					point;
	size_t  						pc_size;

	//--- Open output XYZ file
	pc_size = pcl_pc.points.size();
	xyz_file.open(xyz_filename);
	if (!xyz_file.is_open())
	{
		std::cout << "ERROR: Saving XYZ file failure!" << std::endl;
		return EXIT_FAILURE;
	}

	//--- Write in data
	for (size_t i = 0; i < pc_size; ++i)
	{
		point = pcl_pc.points.at(i);
		xyz_file << point.x << " " << point.y << " " << point.z << "\n";
	}
	xyz_file.close();	
	return EXIT_SUCCESS;
}

} //--- namespace mypcl
