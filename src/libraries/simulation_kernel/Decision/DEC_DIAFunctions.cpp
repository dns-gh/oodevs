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
#include "Meteo/PHY_Ephemeride.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Orders/MIL_Mission_ABC.h"
#include "Entities/Orders/MIL_MissionParameterFactory.h"
#include "simulation_terrain/TER_Localisation.h"
#include "MIL_AgentServer.h"
#include "MIL_Random.h"
#include <sstream>

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::PointToString
// Created: JDY 03-01-21
//-----------------------------------------------------------------------------
std::string DEC_DIAFunctions::PointToString( MT_Vector2D* p1 )
{    
    if( !p1 )
        throw MASA_EXCEPTION( "invalid parameter." );
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
    if( !p1 )
        throw MASA_EXCEPTION( "invalid parameter." );
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
    if( !pPath )
        throw MASA_EXCEPTION( "invalid parameter." );
    std::stringstream strTmp;
    strTmp << "0x" << pPath;
    return strTmp.str();
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyLocalisationToLocationListMission
// Created: NLD 2005-08-17
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::CopyLocalisationToLocationListMission( boost::shared_ptr< TER_Localisation > pLocSource, boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter )
{
    if( !pLocSource.get() )
        throw MASA_EXCEPTION( "invalid parameter." );
    pMission->AppendToParameter( parameter, pLocSource );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyLocalisationListMission
// Created: LDC 2009-12-04
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::CopyLocalisationListMission( const std::vector< boost::shared_ptr< TER_Localisation > >& locSource, boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter )
{
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > params;
    for( std::vector< boost::shared_ptr< TER_Localisation > >::const_iterator it = locSource.begin(); it != locSource.end(); ++it )
        params.push_back( MIL_MissionParameterFactory::CreateLocation( *it ) );
    MIL_MissionParameterFactory::AssignMissionListParameter( pMission, parameter, params );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyPathMission
// Created: LDC 2009-07-08
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::CopyPathMission( std::vector< boost::shared_ptr< MT_Vector2D > >pointList, boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter )
{
    if( !pMission.get() )
        throw MASA_EXCEPTION( "invalid parameter." );
    pMission->SetParameter( parameter, MIL_MissionParameterFactory::CreatePathConst( pointList ) );
}

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyPoint
// Created: NLD 2003-04-15
//-----------------------------------------------------------------------------
// static
void DEC_DIAFunctions::CopyPoint( MT_Vector2D* pPosSource, boost::shared_ptr< MT_Vector2D > pPosDest )
{
    if( !pPosSource )
        throw MASA_EXCEPTION( "invalid parameter." );
    if( pPosDest )
        (*pPosDest) = (*pPosSource);
    else
        throw MASA_EXCEPTION( "Can't assign a point into a nil variable of lua" );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyPointMission
// Created: LDC 2009-07-24
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::CopyPointMission( MT_Vector2D* pPosSource, boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter )
{
    if( !pPosSource ||!pMission.get() )
        throw MASA_EXCEPTION( "invalid parameter." );
    pMission->SetParameter( parameter, MIL_MissionParameterFactory::CreatePoint( *pPosSource ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyPointToListPointMission
// Created: LDC 2009-07-10
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::CopyPointToListPointMission( boost::shared_ptr< MT_Vector2D > pPosSource, boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter )
{
    if( !pPosSource || !pMission.get() )
        throw MASA_EXCEPTION( "invalid parameter." );
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
    if( !pLocSource || !pLocDest )
        throw MASA_EXCEPTION( "invalid parameter." );
    pLocDest->Reset( *pLocSource );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyLocalisationMission
// Created: LDC 2009-07-24
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::CopyLocalisationMission( boost::shared_ptr< TER_Localisation > pLocation, boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter )
{
    if( !pLocation || !pMission.get() )
        throw MASA_EXCEPTION( "invalid parameter." );
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter = MIL_MissionParameterFactory::CreateLocation( pLocation );
    pMission->SetParameter( parameter, pParameter );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyKnowledgeObjectToKnowledgeObjectListMission
// Created: JSR 2010-03-04
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::CopyKnowledgeObjectToKnowledgeObjectListMission( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr< DEC_Knowledge_Object > pKnowledgeObjectSource )
{
    if( !pKnowledgeObjectSource || !pMission.get() )
        throw MASA_EXCEPTION( "invalid parameter." );
    pMission->AppendToParameter( parameter, pKnowledgeObjectSource );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::CopyGenObjectToGenObjectListMission
// Created: LDC 2010-04-14
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::CopyGenObjectToGenObjectListMission( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr< DEC_Gen_Object > pGenObjectSource )
{
    if( !pGenObjectSource || !pMission.get() )
        throw MASA_EXCEPTION( "invalid parameter." );
    pMission->AppendToParameter( parameter, pGenObjectSource );
}

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
    return static_cast< float >( MIL_AgentServer::GetWorkspace().GetSimTime() );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::GetRealTime
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
float DEC_DIAFunctions::GetRealTime()
{
    return static_cast< float >( MIL_Time_ABC::GetTime().GetRealTime() );
}

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::ListPoint_Size
// Created: AGN 03-04-09
//-----------------------------------------------------------------------------
// static
int DEC_DIAFunctions::ListPoint_Size( std::vector< MT_Vector2D* > list )
{
    return static_cast< int >( list.size() );
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::GetRandomValue
// Created: LGY 2012-11-21
// -----------------------------------------------------------------------------
int DEC_DIAFunctions::GetRandomValue( int min, int max )
{
    return static_cast< int >( MIL_Random::rand32_ii( min, max ) );
}

//-----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::ListPoint_GetAt
// Created: AGN 03-04-04
//-----------------------------------------------------------------------------
// static
boost::shared_ptr< MT_Vector2D > DEC_DIAFunctions::ListPoint_GetAt( std::vector< MT_Vector2D* > list, int nId )
{
    boost::shared_ptr< MT_Vector2D > position;
    if( nId < static_cast< int >( list.size() ) )
        position.reset( new MT_Vector2D( *list[ nId ] ) );
    return position;
}

// -----------------------------------------------------------------------------
// Name: DEC_DIAFunctions::BreakForDebug
// Created: LDC 2009-07-22
// -----------------------------------------------------------------------------
void DEC_DIAFunctions::BreakForDebug( unsigned int id, const std::string& message )
{
    static int count = 0;
    std::cerr << "[" << ++count << "]: " << id << ": " << message << std::endl;
}
