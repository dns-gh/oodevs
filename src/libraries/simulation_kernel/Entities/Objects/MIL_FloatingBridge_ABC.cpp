//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_FloatingBridge_ABC.cpp $
// $Author: Nld $
// $Modtime: 12/05/05 15:25 $
// $Revision: 11 $
// $Workfile: MIL_FloatingBridge_ABC.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_FloatingBridge_ABC.h"
#include "MIL_RealObjectType.h"
#include "Entities/RC/MIL_RC.h"
#include "simulation_terrain/TER_PathFindManager.h"
#include "simulation_terrain/TER_DynamicData.h"
#include "HLA/Deserializer.h"

//-----------------------------------------------------------------------------
// Name: MIL_FloatingBridge_ABC constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_FloatingBridge_ABC::MIL_FloatingBridge_ABC( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_RealObject_ABC( type, nID, army )
    , pPathfindData_    ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_FloatingBridge_ABC constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_FloatingBridge_ABC::MIL_FloatingBridge_ABC()
    : MIL_RealObject_ABC()
    , pPathfindData_    ( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_FloatingBridge_ABC destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_FloatingBridge_ABC::~MIL_FloatingBridge_ABC()
{
    UnlinkFromPathFinder();	
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_FloatingBridge_ABC::Initialize
// Created: JVT 02-10-22
//-----------------------------------------------------------------------------
bool MIL_FloatingBridge_ABC::Initialize( DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    if( !MIL_RealObject_ABC::Initialize( diaParameters, nCurrentParamIdx ) )
        return false;

//    if( diaParameters.GetParameters().size() > nCurrentParamIdx )
//        nFullNbrDotationForConstruction_ = (uint)diaParameters[ nCurrentParamIdx ].ToFloat();  // Nbr de ponts ...

    LinkToPathFinder();
    return true;
}


//-----------------------------------------------------------------------------
// Name: MIL_FloatingBridge_ABC::Initialize
// Created: NLD 2003-07-21
//-----------------------------------------------------------------------------
void MIL_FloatingBridge_ABC::Initialize( MIL_InputArchive& archive )
{
    MIL_RealObject_ABC::Initialize( archive );
    LinkToPathFinder();
}

// -----------------------------------------------------------------------------
// Name: MIL_FloatingBridge_ABC::Initialize
// Created: NLD 2003-08-04
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_FloatingBridge_ABC::Initialize( const ASN1T_MagicActionCreateObject& asnCreateObject )
{
    ASN1T_EnumObjectErrorCode nErrorCode = MIL_RealObject_ABC::Initialize( asnCreateObject );
    if( nErrorCode != EnumObjectErrorCode::no_error )
        return nErrorCode;
    LinkToPathFinder();
    return EnumObjectErrorCode::no_error;
}

//=============================================================================
// $$$$$ TMP
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_FloatingBridge_ABC::LinkToPathFinder
// Created: NLD 2003-01-17
//-----------------------------------------------------------------------------
void MIL_FloatingBridge_ABC::LinkToPathFinder()
{
    const T_PointVector points = GetLocalisation().GetPoints();
    assert( !points.empty() );

    if( pPathfindData_ )
        TER_PathFindManager::GetPathFindManager().RemoveDynamicData( *pPathfindData_ );
    
    T_PointVector tmpPoints; tmpPoints.reserve( points.size() * 2 );
    if( points.size() == 1 )
    {
        tmpPoints.push_back( points.front() );
        tmpPoints.push_back( points.front() );
    }
    else
    {
        CIT_PointVector itPoint = points.begin();
        const MT_Vector2D* pLastPoint = &*itPoint;
        for( ++itPoint; itPoint != points.end(); ++itPoint )
        {
            const MT_Vector2D* pCurPoint = &*itPoint;
            MT_Vector2D direction( *pCurPoint - *pLastPoint );
            direction.Normalize() *= 150;
            tmpPoints.push_back( *pLastPoint - direction );
            tmpPoints.push_back( *pCurPoint  + direction );
            pLastPoint = pCurPoint;
        }
    }

    pPathfindData_ = new TER_DynamicData( tmpPoints, TerrainData::Bridge() );
    TER_PathFindManager::GetPathFindManager().AddDynamicData( *pPathfindData_ );
}

//-----------------------------------------------------------------------------
// Name: MIL_FloatingBridge_ABC::UnlinkFromPathFinder
// Created: NLD 2003-01-17
//-----------------------------------------------------------------------------
void MIL_FloatingBridge_ABC::UnlinkFromPathFinder()
{
    if( pPathfindData_ )
    {
        TER_PathFindManager::GetPathFindManager().RemoveDynamicData( *pPathfindData_ );
        pPathfindData_ = 0;
    }
}

// =============================================================================
// EVENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_FloatingBridge_ABC::ProcessAgentEntering
// Created: NLD 2004-06-24
// -----------------------------------------------------------------------------
void MIL_FloatingBridge_ABC::ProcessAgentEntering( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentEntering( agent );
    // assert( false );
    //$$$ TODO MIL_AgentServer::GetWorkspace().GetRCManager().DispatchRC( agent, eRC_EnCoursDeFranchissement );
}

// -----------------------------------------------------------------------------
// Name: MIL_FloatingBridge_ABC::Initialize
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
bool MIL_FloatingBridge_ABC::Initialize( const TER_Localisation& localisation, const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass )
{
    if( MIL_RealObject_ABC::Initialize( localisation, strOption, strExtra, rCompletion, rMining, rBypass ) )
    {
        LinkToPathFinder();
        return true;
    }
    return false;
}
