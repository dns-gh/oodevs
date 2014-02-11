// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "PropagationManager.h"
#include "SimulationTimeManager_ABC.h"

#include "dispatcher/Logger_ABC.h"
#include "tools/ExerciseConfig.h"
#include "protocol/SimulationSenders.h"

#include <boost/lexical_cast.hpp>

using namespace plugins::hla;

namespace
{
    void writePropagationFile( const std::string& identifier, const tools::ExerciseConfig& config, const std::vector< std::string >& simTimes )
    {
        const tools::Path propFile( config.GetPropagationFile( identifier.c_str() ) );
        std::ofstream ofs( propFile.ToUnicode() );
        ofs << "<?xml version=\"1.0\"?>" << std::endl;
        ofs << "<config>" << std::endl;
        ofs << "\t<projection>latlon.prj</projection>" << std::endl;
        ofs << "\t<files>" << std::endl;
        std::size_t idx = 0;
        std::for_each( simTimes.begin(), simTimes.end(), [&]( const std::string& t) {
            ofs  << "\t\t<file time=\"" << t << "\">./hla_cbrn" << idx << ".asc</file>" << std::endl;
            ++idx;
                });
        ofs << "\t</files>" << std::endl;
        ofs << "</config>" << std::endl;
    }
    void writeDataFile( const std::string& identifier, const std::vector< ObjectListener_ABC::PropagationData >& data,
            int col, int lig, double xll, double yll, const tools::ExerciseConfig& config, std::size_t idx, double cellsize )
    {
        const tools::Path dataFile( config.BuildPropagationChildFile( identifier.c_str(), ("hla_cbrn" + boost::lexical_cast< std::string >( idx ) + ".asc").c_str() ) );
        std::ofstream ofs( dataFile.ToUnicode() );
        ofs << "ncols  " << col << std::endl;
        ofs << "nrows  " << lig << std::endl;
        ofs << "xllcorner  " << xll << std::endl;
        ofs << "yllcorner  " << yll << std::endl;
        ofs << "cellsize  " << cellsize << std::endl;
        ofs << "NODATA_value  " << -9999.00000 << std::endl;
        std::size_t p=0;
        for(int i=0; i<lig; ++i)
        {
            for(int j=0; j<col; ++j)
            {
                const ObjectListener_ABC::PropagationData& prop = data[p];
                ofs << prop.concentration << " ";
                ++p;
            }
            ofs << std::endl;
        }
    }
}

PropagationManager::PropagationManager( dispatcher::Logger_ABC& logger, const tools::ExerciseConfig& conf,
        const SimulationTimeManager_ABC& timeManager )
    : logger_( logger )
    , timeManager_( timeManager )
    , exerciseConfig_( conf )
{
    // NOTHING
}

PropagationManager::~PropagationManager()
{
    // NOTHING
}

void PropagationManager::saveProjectionFile( const std::string& identifier )
{
    const tools::Path projFile( exerciseConfig_.BuildPropagationChildFile( identifier.c_str(), "latlon.prj" ) );
    projFile.Parent().CreateDirectories();
    std::ofstream ofs(projFile.ToUnicode());
    ofs << "GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],TOWGS84[0,0,0,0,0,0,0],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.0174532925199433,AUTHORITY[\"EPSG\",\"9108\"]],AUTHORITY[\"EPSG\",\"4326\"]]";
}

void PropagationManager::saveDataFile( const std::string& identifier, const std::vector< ObjectListener_ABC::PropagationData >& data,
            int col, int lig, double xll, double yll, double cellsize )
{
    const std::string simTime = timeManager_.getSimulationTime();
    if( simTime.empty() )
        return;
    std::vector< std::string >& times = propagationTimes_[ identifier ];
    if( times.empty() )
        saveProjectionFile( identifier );
    writeDataFile( identifier, data, col, lig, xll, yll, exerciseConfig_, times.size(), cellsize );
    times.push_back( simTime );
    writePropagationFile( identifier, exerciseConfig_, times );
}
