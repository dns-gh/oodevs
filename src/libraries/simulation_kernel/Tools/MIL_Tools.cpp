//*****************************************************************************
//
// $Created: NLD 2002-08-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Tools/MIL_Tools.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 11:03 $
// $Revision: 15 $
// $Workfile: MIL_Tools.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentServer.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "protocol/Protocol.h"
#include "simulation_terrain/TER_World.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/FileWrapper.h"
#include "Tools/MIL_Tools.h"
#include <sys/timeb.h>

MIL_Tools::converter< PHY_RawVisionData::E_VisionObject > MIL_Tools::environnementConverter_[] =
{
    converter< PHY_RawVisionData::E_VisionObject >( "Vide",   PHY_RawVisionData::eVisionEmpty ),
    converter< PHY_RawVisionData::E_VisionObject >( "Foret",  PHY_RawVisionData::eVisionForest ),
    converter< PHY_RawVisionData::E_VisionObject >( "Urbain", PHY_RawVisionData::eVisionUrban ),
    converter< PHY_RawVisionData::E_VisionObject >( "Sol",    PHY_RawVisionData::eVisionGround ),
    converter< PHY_RawVisionData::E_VisionObject >( "",      (PHY_RawVisionData::E_VisionObject)-1 )
};

//-----------------------------------------------------------------------------
// Name: MIL_Tools::GetAltitude
// Created: JVT 03-03-17
//-----------------------------------------------------------------------------
double MIL_Tools::GetAltitude( const MT_Vector2D& vPos )
{
    return MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetAltitude( vPos );
}

// -----------------------------------------------------------------------------
// Name: MIL_Tools::GetWind
// Created: JVT 2004-10-29
// -----------------------------------------------------------------------------
const weather::WindData& MIL_Tools::GetWind( const MT_Vector2D& vPos )
{
    return MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetWind( vPos );
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertCoordMosToSim
// Created: NLD 2002-08-06
//-----------------------------------------------------------------------------
void MIL_Tools::ConvertCoordMosToSim( const std::string& strMosPos, MT_Vector2D& vSimPos )
{
    TER_World::GetWorld().MosToSimMgrsCoord( strMosPos, vSimPos );
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertCoordMosToSim
// Created: NLD 2003-03-31
//-----------------------------------------------------------------------------
void MIL_Tools::ConvertCoordMosToSim( const sword::CoordLatLong& asn, MT_Vector2D& vSimPos )
{
    TER_World::GetWorld().MosToSimMgrsCoord( asn.latitude(), asn.longitude(), vSimPos );
}

//-----------------------------------------------------------------------------
// Name: std::string
// Created: NLD 2002-08-06
// Last modified: JVT 02-08-08
//-----------------------------------------------------------------------------
std::string MIL_Tools::ConvertCoordSimToMos( const MT_Vector2D& vSimPos )
{
    std::string strMosPos;
    TER_World::GetWorld().SimToMosMgrsCoord( vSimPos, strMosPos );
    return strMosPos;
}

// -----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertCoordSimToMos
// Created: AGE 2008-04-02
// -----------------------------------------------------------------------------
void MIL_Tools::ConvertCoordSimToMos( const MT_Vector2D& vSimPos, sword::CoordLatLong& asn )
{
    double x, y;
    TER_World::GetWorld().SimToMosMgrsCoord( vSimPos, x, y );
    asn.set_latitude( x );
    asn.set_longitude( y );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#include "MIL_Time_ABC.h"
#include <tools/Exception.h>

//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertSpeedMosToSim
// Created: JVT 02-08-08
//-----------------------------------------------------------------------------
double MIL_Tools::ConvertSpeedMosToSim( double v )
{
    static double rConvertionFactor = 1000. * MIL_Time_ABC::GetTime().GetTickDuration() / 3600.;
    // conversion des vitesses de Km/H en pixel/deltaT
    return v * rConvertionFactor;
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertSpeedSimToMos
// Created: JVT 02-08-08
//-----------------------------------------------------------------------------
unsigned int MIL_Tools::ConvertSpeedSimToMos( double v )
{
    static double rConvertionFactor = 3600. / MIL_Time_ABC::GetTime().GetTickDuration() / 1000.;
    // conversion des vitesses de pixel/deltaT en Km/H
    return static_cast< unsigned int >( std::floor( v * rConvertionFactor + 0.5f ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Tools::MIL_Tools::ConvertSecondsToSim
// Created: JCR 2010-10-26
// -----------------------------------------------------------------------------
double MIL_Tools::ConvertSecondsToSim( double v, const MIL_Time_ABC& time )
{
    return v / time.GetTickDuration();
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertSecondsToSim
// Created: JVT 03-01-08
//-----------------------------------------------------------------------------
double MIL_Tools::ConvertSecondsToSim( double v )  // s    -> DeltaT
{
    return ConvertSecondsToSim( v, MIL_Time_ABC::GetTime() );
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertMinutesToSim
// Created: JVT 03-01-08
//-----------------------------------------------------------------------------
double MIL_Tools::ConvertMinutesToSim( double v )  // min  -> DeltaT
{
    return ConvertSecondsToSim( 60. * v );
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertHoursToSim
// Created: JVT 03-06-10
//-----------------------------------------------------------------------------
double MIL_Tools::ConvertHoursToSim( double v ) // h -> DeltaT
{
    return ConvertSecondsToSim( 3600. * v );
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertSimToMinutes
// Created: JVT 04-02-02
//-----------------------------------------------------------------------------
double MIL_Tools::ConvertSimToMinutes( double v ) // deltaT -> min
{
    return v * MIL_Time_ABC::GetTime().GetTickDuration() / 60.;
}

// -----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertMeterSquareToSim
// Created: NLD 2004-11-18
// -----------------------------------------------------------------------------
double MIL_Tools::ConvertMeterSquareToSim( double rValue ) // m² => px²
{
    return rValue;
}

// -----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertSimToMeterSquare
// Created: NLD 2004-11-18
// -----------------------------------------------------------------------------
float MIL_Tools::ConvertSimToMeterSquare( double rValue ) // px² => m²
{
    return (float)( rValue );
}

// -----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertMeterToSim
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
double MIL_Tools::ConvertMeterToSim( double rValue )
{
    return rValue;
}

// -----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertSimToMeter
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
float MIL_Tools::ConvertSimToMeter( double rValue )
{
    return (float)( rValue  );
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::GetEnvironnementTypeName
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
const std::string& MIL_Tools::GetEnvironnementTypeName( PHY_RawVisionData::E_VisionObject val )
{
    return InverseFindInConverter( environnementConverter_, val );
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertLandType
// Created: AGN 03-07-30
//-----------------------------------------------------------------------------
E_KeyPoint MIL_Tools::ConvertLandType( const std::string& sName )
{
    if( sName == "forest" )
        return eKeyPointForest;
    if( sName == "urban area" )
        return eKeyPointUrban;
    if( sName == "crossroad" )
        return eKeyPointCrossroads;
    if( sName == "bridge" )
        return eKeyPointBridge;
    if( sName == "lima" )
        return eKeyPointLima;
    throw MASA_EXCEPTION( "Unknown landtype." );
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::GetLandTypeName
// Created: JVT 03-10-22
//-----------------------------------------------------------------------------
std::string MIL_Tools::GetLandTypeName( const TerrainData& data )
{
    return data.ToString();
}
