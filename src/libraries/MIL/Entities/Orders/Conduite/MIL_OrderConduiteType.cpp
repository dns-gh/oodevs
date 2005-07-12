// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Conduite/MIL_OrderConduiteType.cpp $
// $Author: Nld $
// $Modtime: 9/11/04 10:27 $
// $Revision: 2 $
// $Workfile: MIL_OrderConduiteType.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "MIL_OrderConduiteType.h"

#include "MIL_OrderConduite_ABC.h"
#include "MIL_OrderConduiteRequest_ABC.h"

MIL_OrderConduiteType::T_OrderConduiteFromIDVector MIL_OrderConduiteType::orderConduitesFromID_;
MIL_OrderConduiteType::T_OrderConduiteFromAsnIDMap MIL_OrderConduiteType::orderConduitesFromAsnID_;
MIL_OrderConduiteType::T_OrderConduiteFromNameMap  MIL_OrderConduiteType::orderConduitesFromName_;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// ============================================================================


// -----------------------------------------------------------------------------
// Name: MIL_OrderConduiteType::InitializeDIA
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void MIL_OrderConduiteType::InitializeDIA()
{
    MIL_OrderConduiteRequest_ABC::InitializeDIA();
    MIL_OrderConduite_ABC       ::InitializeDIA();
    for( CIT_OrderConduiteFromIDVector it = orderConduitesFromID_.begin(); it != orderConduitesFromID_.end(); ++it )
    {
        // $$$$ AGE 2004-09-23: Tmp while eOrdreConduite_Automate_AjouterObstacleAPlanObstacle keeps sucking
        if( *it )
        {
            const MIL_OrderConduiteType& type = **it;
            type.orderConduiteDiaInitializer_( type );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduiteType::Terminate
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void MIL_OrderConduiteType::Terminate()
{
    for( CIT_OrderConduiteFromIDVector it = orderConduitesFromID_.begin(); it != orderConduitesFromID_.end(); ++it )
        delete *it;
    orderConduitesFromID_   .clear();
    orderConduitesFromAsnID_.clear();
    orderConduitesFromName_ .clear();
}


// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduiteType constructor
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
MIL_OrderConduiteType::MIL_OrderConduiteType( const std::string& strName, uint nConduiteID, uint nAsnConduiteID, const std::string& strConduiteDIATypeName, T_OrderConduiteAllocator allocator, T_DiaInitializer diaInitializer, bool bAvailableForAllMissions, bool bAvailableWithoutMission, uint nAsnRequestID )
    : strName_                    ( strName )
    , nConduiteID_                ( nConduiteID )
    , nAsnConduiteID_             ( nAsnConduiteID )
    , strConduiteDIATypeName_     ( strConduiteDIATypeName )
    , orderConduiteAllocator_     ( allocator )
    , orderConduiteDiaInitializer_( diaInitializer )
    , bAvailableForAllMissions_   ( bAvailableForAllMissions )
    , bAvailableWithoutMission_   ( bAvailableWithoutMission )
    , nAsnRequestID_              ( nAsnRequestID )
{

}

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduiteType destructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
MIL_OrderConduiteType::~MIL_OrderConduiteType()
{

}

// =============================================================================
// OPERATIONS
// =============================================================================q

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduiteType::InstanciateOrderConduiteRequest
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
MIL_OrderConduiteRequest_ABC& MIL_OrderConduiteType::InstanciateOrderConduiteRequest() const
{
    return *new MIL_OrderConduiteRequest_ABC( *this );
}
