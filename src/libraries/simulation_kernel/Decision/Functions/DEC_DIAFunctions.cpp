//*****************************************************************************
//
// $Created: NLD 2002-12-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_DIAFunctions.cpp $
// $Author: Jvt $
// $Modtime: 18/05/05 14:49 $
// $Revision: 12 $
// $Workfile: DEC_DIAFunctions.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "DEC_DIAFunctions.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/PHY_Ephemeride.h"
#include "Tools/MIL_Tools.h"
#include "Decision/DEC_Tools.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint.h"
#include "Entities/Orders/MIL_Mission_ABC.h"
#include "Entities/Orders/MIL_MissionParameterFactory.h"
#include "simulation_terrain/TER_Localisation.h"
#include <iostream>

class DEC_Path_ABC;

// =============================================================================
// DEBUG
// =============================================================================

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::PointToString
// Created: JDY 03-01-21
//-----------------------------------------------------------------------------
std::string DEC_DIAFunctions::PointToString( MT_Vector2D* p1 )
{
    std::stringstream strTmp;
    strTmp << *p1;
    return strTmp.str();
}

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::DirectionToString
// Created: JDY 03-01-21
//-----------------------------------------------------------------------------
std::string DEC_DIAFunctions::DirectionToString( MT_Vector2D* p1 )
{
    std::stringstream strTmp;
    strTmp << *p1;
    return strTmp.str();
}

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::PathToString
// Created: AGN 03-01-09
//-----------------------------------------------------------------------------
std::string DEC_DIAFunctions::PathToString( DEC_Path_ABC* pPath )
{
    assert( pPath != 0 );
    std::stringstream strTmp;
    strTmp << "0x" << pPath;
    return strTmp.str();
}

//=============================================================================
// PARAMETERS COPY
//=============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyLocalisationToLocationListMission
// Created: NLD 2005-08-17
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::CopyLocalisationToLocationListMission( boost::shared_ptr< TER_Localisation > pLocSource, MIL_Mission_ABC* pMission, const std::string& parameter )
{
    assert( pLocSource.get() );
    pMission->AppendToParameter( parameter, pLocSource );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyPathMission
// Created: LDC 2009-07-08
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::CopyPathMission( std::vector< boost::shared_ptr< MT_Vector2D > >pointList, MIL_Mission_ABC* pMission, const std::string& parameter )
{
    pMission->SetParameter( parameter, MIL_MissionParameterFactory::CreatePath( pointList ) );
}    

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyPoint
// Created: NLD 2003-04-15
//-----------------------------------------------------------------------------
// static
void DEC_DIAFunctions::CopyPoint( MT_Vector2D* pPosSource, boost::shared_ptr< MT_Vector2D > pPosDest )
{
    assert( pPosSource );
    if( pPosDest )
        (*pPosDest) = (*pPosSource);
    else
    {
        throw std::runtime_error( "Can't assign a point into a nil variable of lua" );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyPointMission
// Created: LDC 2009-07-24
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::CopyPointMission( MT_Vector2D* pPosSource, MIL_Mission_ABC* pMission, const std::string& parameter )
{
    assert( pPosSource );
    pMission->SetParameter( parameter, MIL_MissionParameterFactory::Create( *pPosSource ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyPointToListPointMission
// Created: LDC 2009-07-10
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::CopyPointToListPointMission( boost::shared_ptr< MT_Vector2D > pPosSource, MIL_Mission_ABC* pMission, const std::string& parameter )
{
    assert( pPosSource );
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter = MIL_MissionParameterFactory::CreatePath( pPosSource );
    // $$$$ LDC: The list is cleared before adding the point inside...
    pMission->SetParameter( parameter, pParameter );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyLocalisation
// Created: NLD 2003-08-19
// -----------------------------------------------------------------------------
// static
void DEC_DIAFunctions::CopyLocalisation( const TER_Localisation* pLocSource, TER_Localisation* pLocDest )
{
    assert( pLocSource );
    pLocDest->Reset( *pLocSource );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyLocalisationMission
// Created: LDC 2009-07-24
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::CopyLocalisationMission( boost::shared_ptr< TER_Localisation > pLocation, MIL_Mission_ABC* pMission, const std::string& parameter )
{
    assert( pLocation );
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter = MIL_MissionParameterFactory::Create( pLocation );
    pMission->SetParameter( parameter, pParameter );
}    

// =============================================================================
// TIME MANAGEMENT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::IsNight
// Created: NLD 2004-05-25
// -----------------------------------------------------------------------------
bool DEC_DIAFunctions::IsNight()
{
    return MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetEphemeride().IsNight();
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::GetSimTime
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
float DEC_DIAFunctions::GetSimTime()
{
    return float( MIL_AgentServer::GetWorkspace().GetSimTime() );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::GetRealTime
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
float DEC_DIAFunctions::GetRealTime()
{
    return float( MIL_AgentServer::GetWorkspace().GetRealTime() );
}

// =============================================================================
// DIA USER TYPES LIST MANIPULATION
// =============================================================================

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::ListPoint_Size
// Created: AGN 03-04-09
//-----------------------------------------------------------------------------
// static
int DEC_DIAFunctions::ListPoint_Size( std::vector< MT_Vector2D* > list )
{
    return list.size();
}

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::ListPoint_GetAt
// Created: AGN 03-04-04
//-----------------------------------------------------------------------------
// static
boost::shared_ptr< MT_Vector2D > DEC_DIAFunctions::ListPoint_GetAt( std::vector< MT_Vector2D* > list, int nId )
{
    boost::shared_ptr< MT_Vector2D > position;
    if( nId < list.size() )
        position.reset( new MT_Vector2D( *list[ nId ] ) );
    return position;
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::BreakForDebug
// Created: LDC 2009-07-22
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::BreakForDebug( uint id, const std::string& message )
{
    std::cerr << id << ": " << message << std::endl;
}
