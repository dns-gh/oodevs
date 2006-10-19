//*****************************************************************************
//
// $Created: NLD 2002-08-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Tools/MIL_Tools.inl $
// $Author: Nld $
// $Modtime: 23/03/05 10:16 $
// $Revision: 10 $
// $Workfile: MIL_Tools.inl $
//
//*****************************************************************************

#include "MIL_AgentServer.h"

#include "simulation_terrain/TER_World.h"
               
//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertCoordMosToSim
// Created: NLD 2002-08-06
//-----------------------------------------------------------------------------
inline
void MIL_Tools::ConvertCoordMosToSim( const std::string& strMosPos, MT_Vector2D& vSimPos )
{
    assert( strMosPos.size() == 15 );

    TER_World::GetWorld().MosToSimMgrsCoord( strMosPos, vSimPos );
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertCoordMosToSim
// Created: NLD 2003-03-31
//-----------------------------------------------------------------------------
inline
void MIL_Tools::ConvertCoordMosToSim( const ASN1T_CoordUTM& asnCoordUTM, MT_Vector2D& vSimPos )
{
    assert( asnCoordUTM.numocts == 15 );

    std::string strMgrs( (const char*)asnCoordUTM.data, 15 );
    TER_World::GetWorld().MosToSimMgrsCoord( strMgrs, vSimPos );   
}

//-----------------------------------------------------------------------------
// Name: std::string
// Created: NLD 2002-08-06
// Last modified: JVT 02-08-08
//-----------------------------------------------------------------------------
inline
std::string MIL_Tools::ConvertCoordSimToMos( const MT_Vector2D& vSimPos )
{
    std::string strMosPos;
    TER_World::GetWorld().SimToMosMgrsCoord( vSimPos, strMosPos );
    return strMosPos;
}


//-----------------------------------------------------------------------------
// Name: MIL_Tools::GetMeterByPixel
// Created: JVT 02-08-08
//-----------------------------------------------------------------------------
inline
MT_Float MIL_Tools::GetMeterPerPixel()
{
    return TER_World::GetWorld().GetMeterPerPixel();
}


// -----------------------------------------------------------------------------
// Name: MIL_Tools::GetMeterSquarePerPixelSquare
// Created: NLD 2003-10-17
// -----------------------------------------------------------------------------
inline
MT_Float MIL_Tools::GetMeterSquarePerPixelSquare()
{
    return TER_World::GetWorld().GetMeterSquarePerPixelSquare();
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertSpeedMosToSim
// Created: JVT 02-08-08
//-----------------------------------------------------------------------------
inline
MT_Float MIL_Tools::ConvertSpeedMosToSim( MT_Float v )
{
    static MT_Float rConvertionFactor = 1000. / GetMeterPerPixel() * MIL_AgentServer::GetWorkspace().GetTimeStepDuration() / 3600.;
    // conversion des vitesses de Km/H en pixel/deltaT
    return v * rConvertionFactor;
}


//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertSpeedSimToMos
// Created: JVT 02-08-08
//-----------------------------------------------------------------------------
inline
MT_Float MIL_Tools::ConvertSpeedSimToMos( MT_Float v )
{
    static MT_Float rConvertionFactor = 3600. / MIL_AgentServer::GetWorkspace().GetTimeStepDuration() * GetMeterPerPixel() / 1000.;

    // conversion des vitesses de pixel/deltaT en Km/H
    return v * rConvertionFactor;
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertSecondsToSim
// Created: JVT 03-01-08
//-----------------------------------------------------------------------------
inline
MT_Float MIL_Tools::ConvertSecondsToSim( MT_Float v )  // s    -> DeltaT
{
    return v / MIL_AgentServer::GetWorkspace().GetTimeStepDuration();
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertMinutesToSim
// Created: JVT 03-01-08
//-----------------------------------------------------------------------------
inline
MT_Float MIL_Tools::ConvertMinutesToSim( MT_Float v )  // min  -> DeltaT
{
    return ConvertSecondsToSim( 60. * v );
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertHoursToSim
// Created: JVT 03-06-10
//-----------------------------------------------------------------------------
inline
MT_Float MIL_Tools::ConvertHoursToSim( MT_Float v ) // h -> DeltaT
{
    return ConvertSecondsToSim( 3600. * v );
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertSimToMinutes
// Created: JVT 04-02-02
//-----------------------------------------------------------------------------
inline
MT_Float MIL_Tools::ConvertSimToMinutes( MT_Float v ) // deltaT -> m
{
    return v * MIL_AgentServer::GetWorkspace().GetTimeStepDuration() / 60.;
}

// -----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertMeterSquareToSim
// Created: NLD 2004-11-18
// -----------------------------------------------------------------------------
inline
MT_Float MIL_Tools::ConvertMeterSquareToSim( MT_Float rValue ) // m² => px²
{
    return rValue / GetMeterSquarePerPixelSquare();
}

// -----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertSimToMeterSquare
// Created: NLD 2004-11-18
// -----------------------------------------------------------------------------
inline
float MIL_Tools::ConvertSimToMeterSquare( MT_Float rValue ) // px² => m²
{
    return (float)( rValue * GetMeterSquarePerPixelSquare() );
}


// -----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertMeterToSim
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
MT_Float MIL_Tools::ConvertMeterToSim( MT_Float rValue )
{
    return rValue / GetMeterPerPixel();
}

// -----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertSimToMeter
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
float MIL_Tools::ConvertSimToMeter( MT_Float rValue )
{
    return (float)( rValue * GetMeterPerPixel() );
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::GetEnvironnementTypeName
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
inline
const std::string& MIL_Tools::GetEnvironnementTypeName( PHY_RawVisionData::E_VisionObject val )
{
    return InverseFindInConverter( environnementConverter_, val );
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertLandType
// Created: AGN 03-07-30
//-----------------------------------------------------------------------------
inline
TerrainData MIL_Tools::ConvertLandType( const std::string& sName )
{
    return TerrainData( sName );
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::GetLandTypeName
// Created: JVT 03-10-22
//-----------------------------------------------------------------------------
inline
std::string MIL_Tools::GetLandTypeName( const TerrainData& data )
{
    return data.ToString();
}
