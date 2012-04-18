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
MIL_OrderManager_ABC::MIL_OrderManager_ABC()
    : bNewMissionStarted_( false )
    , pController_       ( 0 )
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

    if( pNewMission_ )
    {
        try
        {
            pCurrentMission_->Start( pCurrentMission_ );
            if( pController_ )
                pController_->Start( pCurrentMission_ );
            bNewMissionStarted_ = true;
        }
        catch( std::exception& e )
        {
            if( pController_ && pCurrentMission_.get() )
                pController_->Stop( pCurrentMission_ );
            pNextMission_.reset();
            pCurrentMission_.reset();
            bNewMissionStarted_ = false;
            throw e;
        }
        pNewMission_.reset();
    }

    if( pNextMission_ != pCurrentMission_ )
    {
        boost::shared_ptr< MIL_Mission_ABC > pNewMission = pNextMission_;
        pNextMission_.reset();

        StopAllMissions();
        assert( !pCurrentMission_ );

        pNextMission_ = pNewMission;
        pCurrentMission_ = pNewMission;
        pNewMission_ = pNewMission;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::ReplaceMission
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_OrderManager_ABC::ReplaceMission( boost::shared_ptr< MIL_Mission_ABC > pNewMission )
{
    pNextMission_ = pNewMission;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::CancelMission
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_OrderManager_ABC::CancelMission()
{
    if( pController_ && pCurrentMission_.get() )
        pController_->Stop( pCurrentMission_ );
    pNextMission_.reset();
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::StopAllMissions
// Created: NLD 2003-01-10
//-----------------------------------------------------------------------------
void MIL_OrderManager_ABC::StopAllMissions()
{
    if( pNextMission_.get() )
        pNextMission_->Stop( pNextMission_ );
    if( pCurrentMission_.get() )
        pCurrentMission_->Stop( pCurrentMission_ );

    pNextMission_.reset();
    pCurrentMission_.reset();
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::GetMissionName
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
const std::string& MIL_OrderManager_ABC::GetMissionName() const
{
    if( pCurrentMission_ )
        return pCurrentMission_->GetName();
    static std::string none( "None" );
    return none;
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::GetDirDanger
// Created: JDY 03-05-07
//-----------------------------------------------------------------------------
const MT_Vector2D& MIL_OrderManager_ABC::GetDirDanger() const
{
    if( pCurrentMission_ )
        return pCurrentMission_->GetDirDanger();
    static const MT_Vector2D vDefaultDirDanger( 0, 1 );
    return vDefaultDirDanger;
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::GetLimas
// Created: NLD 2002-10-09
//-----------------------------------------------------------------------------
const T_LimaVector& MIL_OrderManager_ABC::GetLimas() const
{
    if( pCurrentMission_ )
        return pCurrentMission_->GetLimas();
    static const T_LimaVector emptyLimaVector;
    return emptyLimaVector;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::FindLima
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
MIL_LimaOrder* MIL_OrderManager_ABC::FindLima( const MIL_LimaFunction& function ) const
{
    if( !pCurrentMission_ )
        return 0;
    return pCurrentMission_->FindLima( function );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::FindLima
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
MIL_LimaOrder* MIL_OrderManager_ABC::FindLima( unsigned int nID ) const
{
    if( !pCurrentMission_ )
        return 0;
    return pCurrentMission_->FindLima( nID );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::FindNextScheduledLima
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
MIL_LimaOrder* MIL_OrderManager_ABC::FindNextScheduledLima() const
{
    if( !pCurrentMission_ )
        return 0;
    return pCurrentMission_->FindNextScheduledLima();
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::GetCurrentMissionType
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
const MIL_MissionType_ABC* MIL_OrderManager_ABC::GetCurrentMissionType() const
{
    if( !pCurrentMission_ )
        return 0;
    return &pCurrentMission_->GetType();
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
    return pCurrentMission_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::Register
// Created: LGY 2011-06-15
// -----------------------------------------------------------------------------
void MIL_OrderManager_ABC::Register( MissionController_ABC& pController )
{
    pController_ = &pController;
}
