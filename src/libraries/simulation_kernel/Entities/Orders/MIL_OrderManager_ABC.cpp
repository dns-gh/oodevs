//*****************************************************************************
//
// $Created: NLD 2003-01-10 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Pion/MIL_OrderManager_ABC.cpp $
// $Author: Nld $
// $Modtime: 4/04/05 18:30 $
// $Revision: 10 $
// $Workfile: MIL_OrderManager_ABC.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_OrderManager_ABC.h"

#include "MIL_Mission_ABC.h"
#include "Network/NET_AsnException.h"

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC constructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_OrderManager_ABC::MIL_OrderManager_ABC()
    : pMission_          ( 0 )
    , pNextMission_      ( 0 )
    , bNewMissionStarted_( false )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC destructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_OrderManager_ABC::~MIL_OrderManager_ABC()
{
    StopAllMissions();
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::Update
// Created: NLD 2003-09-02
// -----------------------------------------------------------------------------
void MIL_OrderManager_ABC::Update()
{
    bNewMissionStarted_ = false;

    if( pNextMission_ != pMission_ )
    {
        MIL_Mission_ABC* pNewMission = pNextMission_;
        pNextMission_ = 0;

        StopAllMissions();
        assert( !pMission_ );

        pNextMission_ = pNewMission;
        pMission_     = pNewMission;
        if( pNewMission )
        {
            pMission_->Start();
            bNewMissionStarted_ = true;   
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::ReplaceMission
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_OrderManager_ABC::ReplaceMission( MIL_Mission_ABC* pNewMission )
{
    if( pNextMission_ != pMission_ )
        delete pNextMission_;

    pNextMission_ = pNewMission;
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::StopAllMissions
// Created: NLD 2003-01-10
//-----------------------------------------------------------------------------
void MIL_OrderManager_ABC::StopAllMissions()
{
    if( pNextMission_ != pMission_ )
    {
        delete pNextMission_;
        pNextMission_ = 0;
    }
    if( pMission_ )
    {
        pMission_->Stop();
        delete pMission_;
        pMission_ = 0;
    }
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::GetMissionName
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
const std::string& MIL_OrderManager_ABC::GetMissionName() const
{
    if( pMission_ )
        return pMission_->GetName();
    static std::string none( "None" );
    return none;
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::GetDirDanger
// Created: JDY 03-05-07
//-----------------------------------------------------------------------------
const MT_Vector2D& MIL_OrderManager_ABC::GetDirDanger() const
{
    if( pMission_ )
        return pMission_->GetDirDanger();
    static const MT_Vector2D vDefaultDirDanger( 0, 1 );
    return vDefaultDirDanger;
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::GetLimas
// Created: NLD 2002-10-09
//-----------------------------------------------------------------------------
const T_LimaVector& MIL_OrderManager_ABC::GetLimas() const
{
    if( pMission_ )
        return pMission_->GetLimas();
    static const T_LimaVector emptyLimaVector;
    return emptyLimaVector;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::FindLima
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
MIL_LimaOrder* MIL_OrderManager_ABC::FindLima( const MIL_LimaFunction& function ) const
{
    if( !pMission_ )
        return 0;
    return pMission_->FindLima( function );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::FindLima
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
MIL_LimaOrder* MIL_OrderManager_ABC::FindLima( uint nID ) const
{
    if( !pMission_ )
        return 0;
    return pMission_->FindLima( nID );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::GetCurrentMissionType
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
const MIL_MissionType_ABC* MIL_OrderManager_ABC::GetCurrentMissionType() const
{
    if( !pMission_ )
        return 0;
    return &pMission_->GetType();
}
