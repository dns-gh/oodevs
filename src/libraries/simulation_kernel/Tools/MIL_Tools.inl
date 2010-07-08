// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#include "MIL_AgentServer.h"

//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertSpeedMosToSim
// Created: JVT 02-08-08
//-----------------------------------------------------------------------------
inline
MT_Float MIL_Tools::ConvertSpeedMosToSim( MT_Float v )
{
    static MT_Float rConvertionFactor = 1000. * MIL_AgentServer::GetWorkspace().GetTimeStepDuration() / 3600.;
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
    static MT_Float rConvertionFactor = 3600. / MIL_AgentServer::GetWorkspace().GetTimeStepDuration() / 1000.;

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
MT_Float MIL_Tools::ConvertMeterSquareToSim( MT_Float rValue ) // m� => px�
{
    return rValue;
}

// -----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertSimToMeterSquare
// Created: NLD 2004-11-18
// -----------------------------------------------------------------------------
inline
float MIL_Tools::ConvertSimToMeterSquare( MT_Float rValue ) // px� => m�
{
    return (float)( rValue );
}


// -----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertMeterToSim
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
MT_Float MIL_Tools::ConvertMeterToSim( MT_Float rValue )
{
    return rValue;
}

// -----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertSimToMeter
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
float MIL_Tools::ConvertSimToMeter( MT_Float rValue )
{
    return (float)( rValue  );
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
