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
#include "MissionController_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC constructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_OrderManager_ABC::MIL_OrderManager_ABC( MissionController_ABC& controller )
    : controller_        ( controller )
    , bNewMissionStarted_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC destructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_OrderManager_ABC::~MIL_OrderManager_ABC()
{
    StopAllMissions();
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::Update
// Created: NLD 2003-09-02
// -----------------------------------------------------------------------------
void MIL_OrderManager_ABC::Update()
{
    bNewMissionStarted_ = false;
    if( pNextMission_ == pMission_ )
        return;

    boost::shared_ptr< MIL_Mission_ABC > pNewMission = pNextMission_;
    pNextMission_.reset();
    StopAllMissions();
    assert( !pMission_ );
    pNextMission_ = pNewMission;
    pMission_ = pNewMission;
    if( !pNewMission )
        return;

    try
    {
        controller_.Start( pMission_ );
        bNewMissionStarted_ = true;
    }
    catch( const std::exception& )
    {
        if( pMission_  )
            controller_.Stop( pMission_ );
        pNextMission_.reset();
        pMission_.reset();
        bNewMissionStarted_ = false;
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::ReplaceMission
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_OrderManager_ABC::ReplaceMission( const boost::shared_ptr< MIL_Mission_ABC >& pNewMission )
{
    pNextMission_ = pNewMission;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::CancelMission
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_OrderManager_ABC::CancelMission()
{
    if( pMission_ )
        controller_.Stop( pMission_ );
    pNextMission_.reset();
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::StopAllMissions
// Created: NLD 2003-01-10
//-----------------------------------------------------------------------------
void MIL_OrderManager_ABC::StopAllMissions()
{
    if( pNextMission_ )
        pNextMission_->Stop( pNextMission_ );
    if( pMission_ )
        pMission_->Stop( pMission_ );

    pNextMission_.reset();
    pMission_.reset();
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::GetMissionName
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
const std::string& MIL_OrderManager_ABC::GetMissionName() const
{
    if( pMission_ )
        return pMission_->GetName();
    static const std::string none( "None" );
    return none;
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::GetDirDanger
// Created: JDY 03-05-07
//-----------------------------------------------------------------------------
const MT_Vector2D& MIL_OrderManager_ABC::GetDirDanger() const
{
    static const MT_Vector2D defaultDirection = MT_Vector2D( 0, 1 );
    return pMission_ ? *pMission_->GetDirDanger() : defaultDirection;
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
MIL_LimaOrder* MIL_OrderManager_ABC::FindLima( unsigned int nID ) const
{
    if( !pMission_ )
        return 0;
    return pMission_->FindLima( nID );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::FindAllLimas
// Created: NMI 2013-04-30
// -----------------------------------------------------------------------------
std::vector< MIL_LimaOrder* > MIL_OrderManager_ABC::FindAllLimas( const MIL_LimaFunction& function ) const
{
    if( !pMission_ )
        return std::vector< MIL_LimaOrder* >();
    return pMission_->FindAllLimas( function );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::FindNextScheduledLima
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
MIL_LimaOrder* MIL_OrderManager_ABC::FindNextScheduledLima() const
{
    if( !pMission_ )
        return 0;
    return pMission_->FindNextScheduledLima();
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

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::IsNewMissionStarted
// Created: NLD 2005-09-19
// -----------------------------------------------------------------------------
bool MIL_OrderManager_ABC::IsNewMissionStarted() const
{
    return bNewMissionStarted_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::GetCurrentMission
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_Mission_ABC > MIL_OrderManager_ABC::GetCurrentMission() const
{
    return pMission_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::GetController
// Created: BAX 2013-08-30
// -----------------------------------------------------------------------------
const MissionController_ABC& MIL_OrderManager_ABC::GetController() const
{
    return controller_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::AcquireId
// Created: BAX 2013-08-27
// -----------------------------------------------------------------------------
uint32_t MIL_OrderManager_ABC::AcquireId() const
{
    return controller_.AcquireId();
}
