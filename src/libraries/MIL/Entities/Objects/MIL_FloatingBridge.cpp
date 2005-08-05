//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_FloatingBridge.cpp $
// $Author: Nld $
// $Modtime: 12/05/05 15:25 $
// $Revision: 11 $
// $Workfile: MIL_FloatingBridge.cpp $
//
//*****************************************************************************

#include "MIL_Pch.h"

#include "MIL_FloatingBridge.h"
#include "MIL_RealObjectType.h"
#include "Entities/RC/MIL_RC.h"
#include "TER/TER_PathFindManager.h"
#include "TER/TER_DynamicData.h"
#include "hla/Deserializer.h"

BOOST_CLASS_EXPORT_GUID( MIL_FloatingBridge, "MIL_FloatingBridge" )

//-----------------------------------------------------------------------------
// Name: MIL_FloatingBridge constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_FloatingBridge::MIL_FloatingBridge()
    : MIL_RealObject_ABC( MIL_RealObjectType::pontFlottant_ )
    , pPathfindData_( 0 )
{
    nFullNbrDotationForConstruction_    = 10; // Nbr mines
}

//-----------------------------------------------------------------------------
// Name: MIL_FloatingBridge destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_FloatingBridge::~MIL_FloatingBridge()
{
    UnlinkFromPathFinder();	
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_FloatingBridge::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_FloatingBridge::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< MIL_RealObject_ABC >( *this );
    LinkToPathFinder();
}

// -----------------------------------------------------------------------------
// Name: MIL_FloatingBridge::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_FloatingBridge::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}


//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_FloatingBridge::Initialize
// Created: JVT 02-10-22
//-----------------------------------------------------------------------------
bool MIL_FloatingBridge::Initialize( MIL_Army& army, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    if( !MIL_RealObject_ABC::Initialize( army, diaParameters, nCurrentParamIdx ) )
        return false;

    if( diaParameters.GetParameters().size() > nCurrentParamIdx )
        nFullNbrDotationForConstruction_ = (uint)diaParameters[ nCurrentParamIdx ].ToFloat();  // Nbr de ponts ...

    LinkToPathFinder();
    return true;
}


//-----------------------------------------------------------------------------
// Name: MIL_FloatingBridge::Initialize
// Created: NLD 2003-07-21
//-----------------------------------------------------------------------------
void MIL_FloatingBridge::Initialize( uint nID, MIL_InputArchive& archive )
{
    MIL_RealObject_ABC::Initialize( nID, archive );
    LinkToPathFinder();
}

// -----------------------------------------------------------------------------
// Name: MIL_FloatingBridge::Initialize
// Created: NLD 2003-08-04
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_FloatingBridge::Initialize( uint nID, const ASN1T_MagicActionCreateObject& asnCreateObject )
{
    ASN1T_EnumObjectErrorCode nErrorCode = MIL_RealObject_ABC::Initialize( nID, asnCreateObject );
    if( nErrorCode != EnumObjectErrorCode::no_error )
        return nErrorCode;
    LinkToPathFinder();
    return EnumObjectErrorCode::no_error;
}

//=============================================================================
// $$$$$ TMP
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_FloatingBridge::LinkToPathFinder
// Created: NLD 2003-01-17
//-----------------------------------------------------------------------------
void MIL_FloatingBridge::LinkToPathFinder()
{
    const T_PointVector points = GetLocalisation().GetPoints();
    assert( !points.empty() );
    delete pPathfindData_;
    pPathfindData_ = & TER_PathFindManager::GetPathFindManager().CreateDynamicData();

    if( points.size() == 1 )
        TER_PathFindManager::GetPathFindManager().CreateLineTree( points.front(), points.front(), *pPathfindData_, TerrainData::Bridge() );
    else
    {
        CIT_PointVector itPoint = points.begin();
        const MT_Vector2D* pLastPoint = &*itPoint;
        for( ++itPoint; itPoint != points.end(); ++itPoint )
        {
            const MT_Vector2D* pCurPoint = &*itPoint;
            MT_Vector2D direction( *pCurPoint - *pLastPoint );
            direction.Normalize() *= 150;
            TER_PathFindManager::GetPathFindManager().CreateLineTree( *pLastPoint - direction, *pCurPoint + direction, *pPathfindData_, TerrainData::Bridge() );
            pLastPoint = pCurPoint;
        }
    }
}

//-----------------------------------------------------------------------------
// Name: MIL_FloatingBridge::UnlinkFromPathFinder
// Created: NLD 2003-01-17
//-----------------------------------------------------------------------------
void MIL_FloatingBridge::UnlinkFromPathFinder()
{
    delete pPathfindData_;
    pPathfindData_ = 0;
}

// =============================================================================
// EVENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_FloatingBridge::ProcessAgentEntering
// Created: NLD 2004-06-24
// -----------------------------------------------------------------------------
void MIL_FloatingBridge::ProcessAgentEntering( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentEntering( agent );
    // assert( false );
    //$$$ TODO MIL_AgentServer::GetWorkspace().GetRCManager().DispatchRC( agent, eRC_EnCoursDeFranchissement );
}

// -----------------------------------------------------------------------------
// Name: MIL_FloatingBridge::Initialize
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
bool MIL_FloatingBridge::Initialize( const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass )
{
    if( MIL_RealObject_ABC::Initialize( strOption, strExtra, rCompletion, rMining, rBypass ) )
    {
        LinkToPathFinder();
        return true;
    }
    return false;
}
