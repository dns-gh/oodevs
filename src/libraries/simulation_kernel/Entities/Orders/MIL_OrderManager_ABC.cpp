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
#include "Checkpoints/SerializationTools.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_OrderManager_ABC )

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC constructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_OrderManager_ABC::MIL_OrderManager_ABC()
    : bNewMissionStarted_( false )
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

    if( pNextMission_ != pMission_ )
    {
        boost::shared_ptr< MIL_Mission_ABC > pNewMission = pNextMission_;
        pNextMission_.reset();

        StopAllMissions();
        assert( !pMission_ );

        pNextMission_ = pNewMission;
        pMission_ = pNewMission;
        if( pNewMission )
        {
            try
            {
                pMission_->Start( pMission_ );
                bNewMissionStarted_ = true;
            }
            catch( std::runtime_error& e )
            {
                pNextMission_.reset();
                pMission_.reset();
                bNewMissionStarted_ = false;
                throw e;
            }
        }
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
    if( pMission_.get() )
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
MIL_LimaOrder* MIL_OrderManager_ABC::FindLima( unsigned int nID ) const
{
    if( !pMission_ )
        return 0;
    return pMission_->FindLima( nID );
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
// Name: MIL_OrderManager_ABC::load
// Created: LGY 2011-05-30
// ----------------------------------------------------------------------------- 
void MIL_OrderManager_ABC::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    MIL_Mission_ABC* pMission = 0;
    file >> pMission
         >> bNewMissionStarted_;
    pNextMission_.reset( pMission );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::save
// Created: LGY 2011-05-30
// -----------------------------------------------------------------------------
void MIL_OrderManager_ABC::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    MIL_Mission_ABC* pMission = pMission_.get();
    file << pMission
         << bNewMissionStarted_;
}
